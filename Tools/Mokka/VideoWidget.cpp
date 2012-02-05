/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "VideoWidget.h"
#include "Acquisition.h"
#include "UserDefined.h"
#include "LoggerMessage.h"

#include <QTreeWidgetItem>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QApplication>

VideoWidget::VideoWidget(QWidget* parent)
: QWidget(parent), mp_CurrentFrameFunctor()
{
  // Member(s)
  this->mp_Acquisition = 0;
  this->mp_Delays = 0;
  this->m_VideoId = -1;
  this->mp_MediaPlayer = new QMediaPlayer(this);
  this->mp_VideoWidget = new QVideoWidget(this);
  this->mp_MediaPlayer->setVideoOutput(this->mp_VideoWidget);
  this->mp_MediaPlayer->setMuted(true);
  this->m_VideoLoading = false;
  // Drag and drop
  this->setAcceptDrops(true);
  
  // UI
  // - Background color
  QPalette p(this->palette());
  p.setColor(QPalette::Background, Qt::black);
  this->setAutoFillBackground(true);
  this->setPalette(p);
  // - Layout
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(this->mp_VideoWidget);
  layout->setContentsMargins(0,0,0,0);
  this->setLayout(layout);
  this->mp_VideoWidget->setVisible(false);
  
  connect(this->mp_MediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(checkMediaStatus(QMediaPlayer::MediaStatus)));
  connect(this->mp_MediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(finalizeVideoLoading(qint64)));
};

void VideoWidget::render()
{
  if (this->mp_VideoWidget->isVisible())
  {
    qint64 pos = this->referencePosition();
    if (pos >= 0)
    {
      this->mp_MediaPlayer->setPosition(pos);
    }
  }
};

void VideoWidget::show(bool s)
{
  if (!s) // Hidden
  {
    this->mp_MediaPlayer->setMedia(QMediaContent()); // Remove the current media
    this->mp_VideoWidget->setVisible(false);
  }
};

void VideoWidget::copy(VideoWidget* source)
{
  this->mp_Acquisition = source->mp_Acquisition;
  this->mp_Delays = source->mp_Delays;
  this->mp_CurrentFrameFunctor = source->mp_CurrentFrameFunctor;
};

void VideoWidget::dragEnterEvent(QDragEnterEvent* event)
{
  this->mp_VideoWidget->setVisible(false);
  event->ignore();
  if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
  {
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(event->source());
    if (treeWidget)
    {
      QList<QTreeWidgetItem*> selectedItems = treeWidget->selectedItems();
      for (QList<QTreeWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
      {
        if ((*it)->type() != VideoType)
          break;
      }
      event->setDropAction(Qt::CopyAction); // To have the cross (+) symbol
      event->accept();
    }
  }
  this->QWidget::dragEnterEvent(event);
};

void VideoWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
  if (this->mp_MediaPlayer->mediaStatus() != QMediaPlayer::NoMedia)
    this->mp_VideoWidget->setVisible(true);
  this->QWidget::dragLeaveEvent(event);
  this->render(); // Force to update the content of the video widget as it was previously hidden
}

void VideoWidget::dropEvent(QDropEvent* event)
{
  event->setDropAction(Qt::IgnoreAction); // Only to know which Video IDs were dropped.
  event->accept();
  QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(event->source());
  QTreeWidgetItem* video = treeWidget->selectedItems().at(0);
  int id = video->data(0,VideoId).toInt();
  QUrl videoFileUrl = QUrl::fromLocalFile(this->mp_Acquisition->videoPath(id) + "/" + this->mp_Acquisition->videoFilename(id));
  if (this->mp_MediaPlayer->media().canonicalUrl() == videoFileUrl) // same file?
  {
    this->mp_VideoWidget->setVisible(true);
    this->render(); // Force to update the content of the video widget as it was previously hidden
  }
  else if (this->mp_Acquisition->videoPath(id).isEmpty())
  {
    QMessageBox error(QMessageBox::Warning, "Video player", "Video file not found.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    LOG_CRITICAL("Error when loading the video file: File not found.");
    error.setInformativeText("<nobr>Video(s) must be in the same folder than the acquisition.</nobr>");
    error.exec();
  }
  else
  {
    LOG_INFO("Loading video from file: " + this->mp_Acquisition->videoFilename(id));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->mp_MediaPlayer->setMedia(QMediaContent()); // Reset the internal states of the media: Required to be sure that the video will start at the current "3D" frame
    this->m_VideoLoading = true;
    this->mp_MediaPlayer->setMedia(videoFileUrl);
    QApplication::restoreOverrideCursor();
    this->m_VideoId = id;
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    // Never tried under Linux.
    // Under windows, the mechanism is different and use the status of the media
    this->mp_MediaPlayer->pause(); // Force the loading and sync the video with the curent 3D frame
#endif
  }
  this->QWidget::dropEvent(event);
};

void VideoWidget::paintEvent(QPaintEvent* event)
{
  if (!this->mp_VideoWidget->isVisible())
  {
    QString str = "Drop a video";
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::lightGray);
    QFont f = this->font();
    f.setPointSize(32);
    painter.setFont(f);
    QFontMetrics fm = painter.fontMetrics();
    QRect rect = fm.boundingRect(str);
    QPoint center = this->geometry().center();
    painter.drawText(center - rect.center(), str);
    painter.setPen(QPen(Qt::lightGray, 1.75, Qt::DashLine));
    qreal side = static_cast<qreal>(qMin(this->width() / 2, this->height() / 2));
    side = qMax(side, rect.width() + 30.0); // 15 px on each side
    painter.drawRoundedRect(QRectF(center - QPointF(side, side) / 2.0, QSizeF(side, side)), 25.0, 25.0);
  }
  else
  {
    this->render(); // Force the rendering of the current frame if previously hidden
    this->QWidget::paintEvent(event);
  }
};

void VideoWidget::checkMediaStatus(QMediaPlayer::MediaStatus status)
{
  switch(status)
  {
  case QMediaPlayer::LoadedMedia:
    this->mp_VideoWidget->setVisible(true);
#ifdef Q_OS_WIN
    this->mp_MediaPlayer->play(); // Force the buffering of the media (which will be paused when buffered).
#endif
    break;
  case QMediaPlayer::BufferedMedia:
    this->render();
    break;
  case QMediaPlayer::InvalidMedia:
    {
    QMessageBox error(QMessageBox::Warning, "Video player", "Error when loading the video.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    LOG_CRITICAL("Error when loading the video file: Have you the right video codec installed?");
    error.setInformativeText("<nobr>Have you the right video codec installed?</nobr>");
    error.exec();
    this->m_VideoId = -1;
    this->mp_MediaPlayer->setMedia(QMediaContent()); // Reset the player
    this->mp_VideoWidget->setVisible(false);
    this->update();
    }
    break;
  default:
    break;
  }
};

void VideoWidget::finalizeVideoLoading(qint64 pos)
{
#ifdef Q_OS_WIN
  if (this->m_VideoLoading)
  {
    if ((pos - this->referencePosition()) >= 0)
    {
      this->mp_MediaPlayer->pause();
      this->m_VideoLoading = false;
    }
  }
#else
  Q_UNUSED(pos);
  this->m_VideoLoading = false;
#endif
};

inline qint64 VideoWidget::referencePosition() const
{
  return static_cast<qint64>(static_cast<double>((*this->mp_CurrentFrameFunctor)() - 1) / this->mp_Acquisition->pointFrequency() * 1000.0) - this->mp_Delays->operator[](this->m_VideoId);
};