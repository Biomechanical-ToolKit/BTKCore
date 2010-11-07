/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "TimeEventControlerWidget.h"
#include "NewEventDialog.h"

#include <QMenu>
#include <QKeyEvent>

TimeEventControlerWidget::TimeEventControlerWidget(QWidget* parent)
: QWidget(parent)
{
  this->mp_Acquisition = 0;
  this->mp_PlayIcon = new QIcon(QString::fromUtf8(":/Resources/Images/play.png"));
  this->mp_PlayActiveIcon = new QIcon(QString::fromUtf8(":/Resources/Images/play_active.png"));
  this->mp_PauseIcon = new QIcon(QString::fromUtf8(":/Resources/Images/pause.png"));
  this->mp_PauseActiveIcon = new QIcon(QString::fromUtf8(":/Resources/Images/pause_active.png"));
  this->mp_PrevEventIcon = new QIcon(QString::fromUtf8(":/Resources/Images/skip_backward.png"));
  this->mp_PrevEventActiveIcon = new QIcon(QString::fromUtf8(":/Resources/Images/skip_backward_active.png"));
  this->mp_NextEventIcon = new QIcon(QString::fromUtf8(":/Resources/Images/skip_forward.png"));
  this->mp_NextEventActiveIcon = new QIcon(QString::fromUtf8(":/Resources/Images/skip_forward_active.png"));
  this->mp_Timer = new QTimer(this);
  this->m_PlaybackStep = 1;
  this->m_PlaybackDelay = 33; // msec
  
  this->setupUi(this);
  this->setFocusPolicy(Qt::StrongFocus);
  this->prevEventButton->setIcon(*this->mp_PrevEventIcon);
  this->playButton->setIcon(*this->mp_PlayIcon);
  this->nextEventButton->setIcon(*this->mp_NextEventIcon);
  // Display options menu
  QActionGroup* playbackSpeedActionGroup = new QActionGroup(this);
  playbackSpeedActionGroup->addAction(this->actionPlaybackSpeedRealtime);
  playbackSpeedActionGroup->addAction(this->actionPlaybackSpeed1_2);
  playbackSpeedActionGroup->addAction(this->actionPlaybackSpeed1_4);
  playbackSpeedActionGroup->addAction(this->actionPlaybackSpeed1_5);
  playbackSpeedActionGroup->addAction(this->actionPlaybackSpeed1_10);
  playbackSpeedActionGroup->addAction(this->actionPlaybackSpeedFullFrames);
  QMenu* displayOptionsMenu = new QMenu(this);
  this->mp_PlaybackSpeedMenu = new QMenu(tr("Playback speed"),this);
  actionPlaybackSpeedRealtime->setChecked(true);
  this->mp_PlaybackSpeedMenu->addAction(this->actionPlaybackSpeedRealtime);
  this->mp_PlaybackSpeedMenu->addAction(this->actionPlaybackSpeed1_2);
  this->mp_PlaybackSpeedMenu->addAction(this->actionPlaybackSpeed1_4);
  this->mp_PlaybackSpeedMenu->addAction(this->actionPlaybackSpeed1_5);
  this->mp_PlaybackSpeedMenu->addAction(this->actionPlaybackSpeed1_10);
  this->mp_PlaybackSpeedMenu->addAction(this->actionPlaybackSpeedFullFrames);
  displayOptionsMenu->addMenu(this->mp_PlaybackSpeedMenu);
  displayOptionsMenu->addSeparator();
  displayOptionsMenu->addAction(actionCropRegionOfInterest);
  this->acquisitionOptionsButtonMenu->setMenu(displayOptionsMenu);
  // Event options menu
  QMenu* eventOptionsMenu = new QMenu(this);
  this->actionRemoveSelectedEvents->setEnabled(false);
  eventOptionsMenu->addAction(this->actionRemoveSelectedEvents);
  eventOptionsMenu->addAction(this->actionClearEvents);
  this->eventOptionsButtonMenu->setMenu(eventOptionsMenu);
  // Right event options menu
  QMenu* eventRightOptionsMenu = new QMenu(this);
  eventRightOptionsMenu->addAction(this->actionInsertRightFootStrike);
  eventRightOptionsMenu->addAction(this->actionInsertRightFootOff);
  eventRightOptionsMenu->addAction(this->actionInsertRightOther);
  eventRightOptionsMenu->addSeparator();
  eventRightOptionsMenu->addAction(this->actionRemoveAllRightFootStrike);
  eventRightOptionsMenu->addAction(this->actionRemoveAllRightFootOff);
  this->eventRightOptionsButtonMenu->setMenu(eventRightOptionsMenu);
  // Left event options menu
  QMenu* eventLeftOptionsMenu = new QMenu(this);
  eventLeftOptionsMenu->addAction(this->actionInsertLeftFootStrike);
  eventLeftOptionsMenu->addAction(this->actionInsertLeftFootOff);
  eventLeftOptionsMenu->addAction(this->actionInsertLeftOther);
  eventLeftOptionsMenu->addSeparator();
  eventLeftOptionsMenu->addAction(this->actionRemoveAllLeftFootStrike);
  eventLeftOptionsMenu->addAction(this->actionRemoveAllLeftFootOff);
  this->eventLeftOptionsButtonMenu->setMenu(eventLeftOptionsMenu);
  // General event options menu
  QMenu* eventGeneralOptionsMenu = new QMenu(this);
  eventGeneralOptionsMenu->addAction(this->actionInsertGeneralFootStrike);
  eventGeneralOptionsMenu->addAction(this->actionInsertGeneralFootOff);
  eventGeneralOptionsMenu->addAction(this->actionInsertGeneralOther);
  eventGeneralOptionsMenu->addSeparator();
  eventGeneralOptionsMenu->addAction(this->actionRemoveAllGeneralFootStrike);
  eventGeneralOptionsMenu->addAction(this->actionRemoveAllGeneralFootOff);
  this->eventGeneralOptionsButtonMenu->setMenu(eventGeneralOptionsMenu);
  
  connect(this->playButton, SIGNAL(pressed()), this, SLOT(pressPlayButton()));
  connect(this->playButton, SIGNAL(clicked()), this, SLOT(togglePlayback()));
  connect(this->playButton, SIGNAL(released()), this, SLOT(releasePlayButton()));
  connect(this->prevEventButton, SIGNAL(pressed()), this, SLOT(pressPrevEventButton()));
  connect(this->prevEventButton, SIGNAL(clicked()), this, SLOT(previousEvent()));
  connect(this->prevEventButton, SIGNAL(released()), this, SLOT(releasePrevEventButton()));
  connect(this->nextEventButton, SIGNAL(pressed()), this, SLOT(pressNextEventButton()));
  connect(this->nextEventButton, SIGNAL(clicked()), this, SLOT(nextEvent()));
  connect(this->nextEventButton, SIGNAL(released()), this, SLOT(releaseNextEventButton()));
  connect(this->mp_Timer, SIGNAL(timeout()), this, SLOT(nextStep()));
  // Time Event Bar
  connect(this->timeEventBar, SIGNAL(sliderPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this, SLOT(updateROIAction(int)));
  connect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this, SLOT(updateROIAction(int)));
  connect(this->timeEventBar, SIGNAL(boundSelected(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(boundDeselected(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(sliderPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(eventSelectionChanged(QList<int>)), this, SLOT(toggleEventSelection(QList<int>)));
  // Actions
  connect(playbackSpeedActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changePlaybackParameters()));
  connect(this->actionCropRegionOfInterest, SIGNAL(triggered()), this, SLOT(cropRegionOfInterest()));
  connect(this->actionClearEvents, SIGNAL(triggered()), this, SLOT(clearEvents()));
  connect(this->actionRemoveSelectedEvents, SIGNAL(triggered()), this, SLOT(removeSelectedEvents()));
  connect(this->actionRemoveAllRightFootStrike, SIGNAL(triggered()), this, SLOT(removeAllRightFootStrikeEvents()));
  connect(this->actionRemoveAllRightFootOff, SIGNAL(triggered()), this, SLOT(removeAllRightFootOffEvents()));
  connect(this->actionRemoveAllLeftFootStrike, SIGNAL(triggered()), this, SLOT(removeAllLeftFootStrikeEvents()));
  connect(this->actionRemoveAllLeftFootOff, SIGNAL(triggered()), this, SLOT(removeAllLeftFootOffEvents()));
  connect(this->actionRemoveAllGeneralFootStrike, SIGNAL(triggered()), this, SLOT(removeAllGeneralFootStrikeEvents()));
  connect(this->actionRemoveAllGeneralFootOff, SIGNAL(triggered()), this, SLOT(removeAllGeneralFootOffEvents()));
  connect(this->actionInsertRightFootStrike, SIGNAL(triggered()), this, SLOT(insertRightFootStrike()));
  connect(this->actionInsertRightFootOff, SIGNAL(triggered()), this, SLOT(insertRightFootOff()));
  connect(this->actionInsertRightOther, SIGNAL(triggered()), this, SLOT(insertRightOther()));
  connect(this->actionInsertLeftFootStrike, SIGNAL(triggered()), this, SLOT(insertLeftFootStrike()));
  connect(this->actionInsertLeftFootOff, SIGNAL(triggered()), this, SLOT(insertLeftFootOff()));
  connect(this->actionInsertLeftOther, SIGNAL(triggered()), this, SLOT(insertLeftOther()));
  connect(this->actionInsertGeneralFootStrike, SIGNAL(triggered()), this, SLOT(insertGeneralFootStrike()));
  connect(this->actionInsertGeneralFootOff, SIGNAL(triggered()), this, SLOT(insertGeneralFootOff()));
  connect(this->actionInsertGeneralOther, SIGNAL(triggered()), this, SLOT(insertGeneralOther()));
};

TimeEventControlerWidget::~TimeEventControlerWidget()
{
  delete this->mp_PlayIcon;
  delete this->mp_PlayActiveIcon;
  delete this->mp_PauseIcon;
  delete this->mp_PauseActiveIcon;
  delete this->mp_PrevEventIcon;
  delete this->mp_PrevEventActiveIcon;
  delete this->mp_NextEventIcon;
  delete this->mp_NextEventActiveIcon;
};

void TimeEventControlerWidget::load(Acquisition* acq)
{
  this->mp_Acquisition = acq;
  this->timeEventBar->load(acq);
  this->changePlaybackParameters();
  this->updateEventActions();
  this->actionCropRegionOfInterest->setEnabled(false);
};

void TimeEventControlerWidget::reset()
{
  this->mp_Timer->stop();
  this->playButton->setIcon(*this->mp_PlayIcon);
  this->timeEventBar->reset();
  this->lcdNumber->display(0);
};

void TimeEventControlerWidget::togglePlayback()
{
  if (this->mp_Timer->isActive())
  {
    this->mp_Timer->stop();
    this->playButton->setIcon(*this->mp_PlayIcon);
  }
  else
  {
    this->mp_Timer->start(this->m_PlaybackDelay);
    this->playButton->setIcon(*this->mp_PauseIcon);
  }
};

void TimeEventControlerWidget::setCurrentFrame(int frame)
{
  this->setFrame(frame);
};

void TimeEventControlerWidget::nextStep()
{
  this->setFrame(this->timeEventBar->m_SliderPos + this->m_PlaybackStep);
};

void TimeEventControlerWidget::nextFrame()
{
  this->setFrame(this->timeEventBar->m_SliderPos + 1);
};

void TimeEventControlerWidget::previousFrame()
{
  this->setFrame(this->timeEventBar->m_SliderPos - 1);
};

void TimeEventControlerWidget::cropRegionOfInterest()
{
  emit regionOfInterestChanged(this->timeEventBar->m_LeftBoundPos, this->timeEventBar->m_RightBoundPos);
};

void TimeEventControlerWidget::setRegionOfInterest(int lb, int rb)
{
  this->timeEventBar->m_ROIFirstFrame = lb;
  this->timeEventBar->m_ROILastFrame = rb;
  this->timeEventBar->updateInternals();
  this->timeEventBar->update();
};

void TimeEventControlerWidget::previousEvent()
{
  // Find events included between the bounds
  int lastEventFrame = this->timeEventBar->m_LeftBoundPos;
  int prevEventFrame = this->timeEventBar->m_ROIFirstFrame - 1;
  for (int i = 0 ; i < this->timeEventBar->m_EventItems.count() ; ++i)
  {
    int f = this->timeEventBar->m_EventItems[i].frame;
    if ((f > lastEventFrame) && (f < this->timeEventBar->m_RightBoundPos))
      lastEventFrame = f;
    if ((f > prevEventFrame) && (f < this->timeEventBar->m_SliderPos))
      prevEventFrame = f;
  }
  if (prevEventFrame >= this->timeEventBar->m_LeftBoundPos)
    this->timeEventBar->setSliderValue(prevEventFrame);
  else // Cycling
    this->timeEventBar->setSliderValue(lastEventFrame);
};

void TimeEventControlerWidget::nextEvent()
{
  // Find first & last event included between the bounds
  int firstEventFrame = this->timeEventBar->m_RightBoundPos;
  int nextEventFrame = this->timeEventBar->m_ROILastFrame + 1;
  for (int i = 0 ; i < this->timeEventBar->m_EventItems.count() ; ++i)
  {
    int f = this->timeEventBar->m_EventItems[i].frame;
    if ((f  < firstEventFrame) && (f > this->timeEventBar->m_LeftBoundPos))
      firstEventFrame = f;
    if ((f < nextEventFrame) && (f > this->timeEventBar->m_SliderPos))
      nextEventFrame = f;
  }
  if (nextEventFrame <= this->timeEventBar->m_RightBoundPos)
    this->timeEventBar->setSliderValue(nextEventFrame);
  else // Cycling
    this->timeEventBar->setSliderValue(firstEventFrame);
};

void TimeEventControlerWidget::clearEvents()
{
  QList<int> eventsId;
  for (int i = 0 ; i < this->timeEventBar->m_EventItems.count() ; ++i)
    eventsId.push_back(this->timeEventBar->m_EventItems[i].id);
  emit eventsRemoved(eventsId);
  this->updateEventActions();
};

void TimeEventControlerWidget::removeSelectedEvents()
{
  if (!this->timeEventBar->m_SelectedEvents.isEmpty())
    emit eventsRemoved(this->timeEventBar->m_SelectedEvents);
};

void TimeEventControlerWidget::removeAllRightFootStrikeEvents()
{
  QList<int> eventsId = this->removeEvent("Right", "Foot Strike");
  if (!eventsId.isEmpty())
    emit eventsRemoved(eventsId);
};

void TimeEventControlerWidget::removeAllRightFootOffEvents()
{
  QList<int> eventsId = this->removeEvent("Right", "Foot Off") ;
  if (!eventsId.isEmpty())
    emit eventsRemoved(eventsId);
};

void TimeEventControlerWidget::removeAllLeftFootStrikeEvents()
{
  QList<int> eventsId = this->removeEvent("Left", "Foot Strike");
  if (!eventsId.isEmpty())
    emit eventsRemoved(eventsId);
};

void TimeEventControlerWidget::removeAllLeftFootOffEvents()
{
  QList<int> eventsId = this->removeEvent("Left", "Foot Off");
  if (!eventsId.isEmpty())
    emit eventsRemoved(eventsId);
};

void TimeEventControlerWidget::removeAllGeneralFootStrikeEvents()
{
  QList<int> eventsId = this->removeEvent("General", "Foot Strike");
  eventsId.append(this->removeEvent("", "Foot Strike"));
  if (!eventsId.isEmpty())
    emit eventsRemoved(eventsId);
};

void TimeEventControlerWidget::removeAllGeneralFootOffEvents()
{
  QList<int> eventsId = this->removeEvent("General", "Foot Off");
  eventsId.append(this->removeEvent("", "Foot Off"));
  if (!eventsId.isEmpty())
    emit eventsRemoved(eventsId);
};

void TimeEventControlerWidget::insertRightFootStrike()
{
  this->insertEvent("Foot Strike", 0, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertRightFootOff()
{
  this->insertEvent("Foot Off", 0, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertRightOther()
{
  this->insertEvent("", 0, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertLeftFootStrike()
{
  this->insertEvent("Foot Strike", 1, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertLeftFootOff()
{
  this->insertEvent("Foot Off", 1, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertLeftOther()
{
  this->insertEvent("", 1, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertGeneralFootStrike()
{
  this->insertEvent("Foot Strike", 2, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertGeneralFootOff()
{
  this->insertEvent("Foot Off", 2, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertGeneralOther()
{
  this->insertEvent("", 2, this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::removeEvents(const QList<int>& ids, const QList<Event*>& events)
{
  Q_UNUSED(events);
  for (int i = 0 ; i < ids.count() ; ++i)
    this->timeEventBar->removeEvent(ids[i]);
  this->updateEventActions();
};

void TimeEventControlerWidget::insertEvents(const QList<int>& ids, const QList<Event*>& events)
{
  for (int i = 0 ; i < ids.count() ; ++i)
    this->timeEventBar->insertEvent(ids[i], events[i]);
  this->updateEventActions();
};

void TimeEventControlerWidget::keyPressEvent(QKeyEvent* event)
{
  if ((event->key() == Qt::Key_Backspace) || (event->key() == Qt::Key_Delete))
  {
    this->removeSelectedEvents();
    return;
  }
  this->QWidget::keyPressEvent(event);
};

void TimeEventControlerWidget::pressPlayButton()
{
  if (this->mp_Timer->isActive())
    this->playButton->setIcon(*this->mp_PauseActiveIcon);
  else
    this->playButton->setIcon(*this->mp_PlayActiveIcon);
};

void TimeEventControlerWidget::releasePlayButton()
{
  if (this->mp_Timer->isActive())
    this->playButton->setIcon(*this->mp_PauseIcon);
  else
    this->playButton->setIcon(*this->mp_PlayIcon);
};

void TimeEventControlerWidget::pressPrevEventButton()
{
  this->prevEventButton->setIcon(*this->mp_PrevEventActiveIcon);
};

void TimeEventControlerWidget::releasePrevEventButton()
{
  this->prevEventButton->setIcon(*this->mp_PrevEventIcon);
};

void TimeEventControlerWidget::pressNextEventButton()
{
  this->nextEventButton->setIcon(*this->mp_NextEventActiveIcon);
};

void TimeEventControlerWidget::releaseNextEventButton()
{
  this->nextEventButton->setIcon(*this->mp_NextEventIcon);
};

void TimeEventControlerWidget::changePlaybackParameters()
{
  if (!this->mp_Acquisition)
    return;
  
  double pointFrequency = this->mp_Acquisition->pointFrequency();
  
  // Compute playback step and delay
  if ((pointFrequency == 0.0) || (this->actionPlaybackSpeedFullFrames->isChecked()))
  {
    this->m_PlaybackDelay = 40; // 25 Hz
    this->m_PlaybackStep = 1;
  }
  else
  {
    double divider = 1.0;
    if (this->actionPlaybackSpeedRealtime->isChecked())
      divider = 1.0;
    else if (this->actionPlaybackSpeed1_2->isChecked())
      divider = 0.5;
    else if (this->actionPlaybackSpeed1_4->isChecked())
      divider = 0.25;
    else if (this->actionPlaybackSpeed1_5->isChecked())
      divider = 0.2;
    else if (this->actionPlaybackSpeed1_10->isChecked())
      divider = 0.1;

    double freq = pointFrequency * divider;
    int finalFreq = 1;
    if (freq >= 24.0)
    {
      double r = 1.0;
      for (int i = 24 ; i <= 30 ; ++i)
      {
        double residual = freq / static_cast<double>(i) - static_cast<double>(static_cast<int>(freq) / i);
        if (residual < r)
        {
          finalFreq = i;
          r = residual;
        }
      }
      this->m_PlaybackDelay = 1000 / finalFreq;
      this->m_PlaybackStep = freq / finalFreq;
    }
    else
    {
      this->m_PlaybackDelay = static_cast<int>(1000.0 / freq);
      this->m_PlaybackStep = 1;
    }
  }
  // Relaunch playback if necessary
  if (this->mp_Timer->isActive())
  {
    this->mp_Timer->stop();
    this->mp_Timer->start(this->m_PlaybackDelay);
  }
};

void TimeEventControlerWidget::toggleEventSelection(const QList<int>& selectedIndices)
{
  if (!selectedIndices.isEmpty())
    this->actionRemoveSelectedEvents->setEnabled(true);
  else
    this->actionRemoveSelectedEvents->setEnabled(false);
};

void TimeEventControlerWidget::setFrame(int f)
{
  if (f > this->timeEventBar->m_RightBoundPos)
    this->timeEventBar->setSliderValue(this->timeEventBar->m_LeftBoundPos);
  else if (f < this->timeEventBar->m_LeftBoundPos)
    this->timeEventBar->setSliderValue(this->timeEventBar->m_RightBoundPos);
  else
    this->timeEventBar->setSliderValue(f);
};

void TimeEventControlerWidget::updateEventActions()
{
  if (this->mp_Acquisition->hasEvents())
  {
    this->actionClearEvents->setEnabled(true);
    this->prevEventButton->setEnabled(true);
    this->nextEventButton->setEnabled(true);
  }
  else
  {
    this->actionClearEvents->setEnabled(false);
    this->prevEventButton->setEnabled(false);
    this->nextEventButton->setEnabled(false);
  }
}

QList<int> TimeEventControlerWidget::removeEvent(const QString& context, const QString& label)
{
  QList<int> eventsId;
  for (int i = 0 ; i < this->timeEventBar->m_EventItems.count() ; ++i)
  {
    const Event* e = this->mp_Acquisition->eventAt(this->timeEventBar->m_EventItems[i].id);
    if (e && (e->context.compare(context) == 0) && (e->label.compare(label) == 0))
      eventsId.push_back(this->timeEventBar->m_EventItems[i].id);
  }
  return eventsId;
};

void TimeEventControlerWidget::insertEvent(const QString& label, int context, int frame)
{
  NewEventDialog ned(this);
  ned.labelEdit->setText(label);
  ned.contextComboBox->setCurrentIndex(context);
  ned.frameSpinBox->setRange(this->mp_Acquisition->firstFrame(), this->mp_Acquisition->lastFrame());
  ned.frameSpinBox->setValue(frame);
  ned.exec();
  if (ned.result() == QDialog::Accepted)
  {
    Event* e = new Event;
    e->label = ned.labelEdit->text().trimmed();
    e->description = ned.descriptionEdit->text().trimmed();
    e->context = ned.contextComboBox->currentText();
    e->subject = ned.subjectEdit->text().trimmed();
    e->frame = ned.frameSpinBox->value();
    e->time = (e->frame - this->mp_Acquisition->firstFrame() + 1) / this->mp_Acquisition->pointFrequency();
    e->iconId = -1;
    // Look for the symbol to assign (icon ID)
    if (e->label.compare("Foot Strike", Qt::CaseInsensitive) == 0)
      e->iconId = 1;
    else if (e->label.compare("Foot Strike", Qt::CaseInsensitive) == 0)
      e->iconId = 2;
    else
    {
      QList<int> iconsUsed;
      for (int i = 0 ; i < this->timeEventBar->m_EventItems.count() ; ++i)
      {
        const Event* ev = this->mp_Acquisition->eventAt(this->timeEventBar->m_EventItems[i].id);
        if (ev && (ev->label.compare(e->label) == 0))
        {
          e->iconId = ev->iconId;
          break;
        }
        else if (!iconsUsed.contains(ev->iconId))
          iconsUsed.push_back(ev->iconId);
      }
      if (e->iconId == -1)
      {
        for (int i = 2 ; this->timeEventBar->m_EventSymbols.count() ; ++i)
        {
          if (!iconsUsed.contains(i+1))
          {
            e->iconId = i + 1;
            break;
          }
        }
      }
    }
    emit eventInserted(e);
  }
};

void TimeEventControlerWidget::updateROIAction(int frame)
{
  if ((frame != this->timeEventBar->m_ROIFirstFrame) && (frame != this->timeEventBar->m_ROILastFrame))
    this->actionCropRegionOfInterest->setEnabled(true);
  else
    this->actionCropRegionOfInterest->setEnabled(false);
};