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
#include <QDir>
#include <QStackedLayout>

VideoWidget::VideoWidget(QWidget* parent)
: QWidget(parent), mp_CurrentFrameFunctor()
{
  // Member(s)
  this->mp_Acquisition = 0;
  this->mp_Delays = 0;
  this->m_VideoId = -1;
  this->mp_MediaObject = new Phonon::MediaObject(this);
  this->mp_Video = new Phonon::VideoWidget(this);
  this->mp_Overlay = new VideoOverlayWidget(this);
#ifdef Q_OS_WIN
  this->m_PlaybackStarted = false;
  this->m_PlaybackFrameCounter = 0;
#endif
  // Drag and drop
  this->setAcceptDrops(true);
  
  // UI
  // - Background color
  QPalette p(this->palette());
  p.setColor(QPalette::Background, Qt::black);
  this->setAutoFillBackground(true);
  this->setPalette(p);
  // - Layout
  QStackedLayout* videoStack = new QStackedLayout;
  videoStack->addWidget(this->mp_Video);
  videoStack->addWidget(this->mp_Overlay);
  videoStack->setStackingMode(QStackedLayout::StackAll);
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0,0,0,0);
  layout->addLayout(videoStack);
  this->setLayout(layout);
  this->setVideoVisible(false);
  
  // Connect the media nodes
  Phonon::createPath(this->mp_MediaObject, this->mp_Video);

  connect(this->mp_MediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(checkMediaStatus(Phonon::State, Phonon::State)));
};

void VideoWidget::start()
{
#ifdef Q_OS_WIN
  this->m_PlaybackStarted = true;
  this->m_PlaybackFrameCounter = 0;
#endif
};

void VideoWidget::stop()
{
#ifdef Q_OS_WIN
  this->mp_MediaObject->pause();
  this->m_PlaybackStarted = false;
  this->render();
#endif
};

void VideoWidget::render()
{
  if (this->mp_Video->isVisible())
  {
    qint64 pos = static_cast<qint64>(static_cast<double>((*this->mp_CurrentFrameFunctor)() - 1) / this->mp_Acquisition->pointFrequency() * 1000.0) - this->mp_Delays->operator[](this->m_VideoId);
    if (pos >= 0)
    {
#ifdef Q_OS_WIN
      this->mp_MediaObject->play();
      this->mp_MediaObject->seek(pos);
      // Because some videos are not able to start correctly the frame by frame rendering and are like frozen
      if (!this->m_PlaybackStarted || (this->m_PlaybackFrameCounter > 3))
        this->mp_MediaObject->pause();
      ++this->m_PlaybackFrameCounter;
#else
      this->mp_MediaObject->seek(pos);
#endif
    }
  }
};

void VideoWidget::show(bool s)
{
  if (!s) // Hidden
  {
    if (this->mp_MediaObject->currentSource().type() != Phonon::MediaSource::Empty)
      this->mp_MediaObject->clear();
    this->setVideoVisible(false);
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
  this->setVideoVisible(false);
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
  if (this->mp_MediaObject->currentSource().type() != Phonon::MediaSource::Empty)
    this->setVideoVisible(true);
  this->QWidget::dragLeaveEvent(event);
}

void VideoWidget::dropEvent(QDropEvent* event)
{
  QMessageBox error(QMessageBox::Warning, "Video player", "Error when loading the video.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif

  event->setDropAction(Qt::IgnoreAction); // Only to know which Video IDs were dropped.
  event->accept();
  QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(event->source());
  QTreeWidgetItem* video = treeWidget->selectedItems().at(0);
  int id = video->data(0,VideoId).toInt();
  QString videoFilePath = this->mp_Acquisition->videoPath(id) + "/" + this->mp_Acquisition->videoFilename(id);
  if (this->mp_MediaObject->currentSource().fileName().compare(videoFilePath) == 0) // same file?
    this->setVideoVisible(true);
  else if (this->mp_Acquisition->videoPath(id).isEmpty())
  {
    LOG_CRITICAL("Error when loading the video file: File not found.");
    error.setInformativeText("<nobr>Video(s) must be in the same folder than the acquisition.</nobr>");
    error.exec();
  }
  else
  {
    LOG_INFO("Loading video from file: " + this->mp_Acquisition->videoFilename(id));
    this->mp_MediaObject->clear(); // Reset the internal state of the player
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->mp_MediaObject->setCurrentSource(videoFilePath);
    QApplication::restoreOverrideCursor();
    this->m_VideoId = id;
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    // WARNING: Never tried under Linux
    // Under windows, the mechanism is different and use the status of the media
    // Force the loading and sync the video with the curent 3D frame
    this->mp_MediaObject->play();
    this->mp_MediaObject->pause();
#endif
  }
  this->QWidget::dropEvent(event);
};

void VideoWidget::paintEvent(QPaintEvent* event)
{
  if (!this->mp_Video->isVisible())
  {
    // Title
    QString str = "Drop video";
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::lightGray);
    QFont f = this->font();
    f.setPointSize(32);
    painter.setFont(f);
    QRect rect = painter.fontMetrics().boundingRect(str);
    QPoint center = this->geometry().center();
    painter.drawText(center - rect.center() - QPoint(0, rect.height()/2), str);
    // informative text
    str = "from acquisition explorer";
    f.setPointSize(14);
    painter.setFont(f);
    QRect rect2 = painter.fontMetrics().boundingRect(str);
    painter.drawText(center - rect2.center() + QPoint(0, rect2.height()), str);
    // Box
    painter.setPen(QPen(Qt::lightGray, 1.75, Qt::DashLine));
    qreal side = static_cast<qreal>(qMin(this->width() / 2, this->height() / 2));
    side = qMax(side, rect.width() + 30.0); // 15 px on each side
    painter.drawRoundedRect(QRectF(center - QPointF(side, side) / 2.0, QSizeF(side, side)), 25.0, 25.0);
  }
  else
    this->QWidget::paintEvent(event);
};

void VideoWidget::setVideoVisible(bool v)
{
  this->mp_Video->setVisible(v);
  this->mp_Overlay->setVisible(v);
};

void VideoWidget::checkMediaStatus(Phonon::State newState, Phonon::State oldState)
{
  Q_UNUSED(oldState)
  switch(newState)
  {
  case Phonon::StoppedState:
    if (this->mp_MediaObject->currentSource().type() != Phonon::MediaSource::Empty)
    {
      this->setVideoVisible(true);
      this->render();
    }
    break;
  case Phonon::ErrorState:
    {
    if (this->mp_MediaObject->currentSource().type() != Phonon::MediaSource::Empty)
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
      this->mp_MediaObject->clear(); // Reset the player
      this->setVideoVisible(false);
      this->update();
      }
    }
    break;
  default:
    break;
  }
};

// ----------------------------------------------------------------------------

VideoOverlayWidget::VideoOverlayWidget(QWidget* parent)
: QWidget(parent)
{
  this->setAttribute(Qt::WA_NoSystemBackground);
};