/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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
#include "Acquisition.h"
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
  this->m_OpenEditorWhenInserting = true;
  
  this->setupUi(this);
  this->setFocusPolicy(Qt::StrongFocus);
  this->prevEventButton->setIcon(*this->mp_PrevEventIcon);
  this->playButton->setIcon(*this->mp_PlayIcon);
  this->nextEventButton->setIcon(*this->mp_NextEventIcon);
  // Shortcuts
  this->actionEditSelectedEvents->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_E));
  this->actionRemoveSelectedEvents->setShortcut(QKeySequence::Delete);
  this->actionNextEvent->setShortcut(QKeySequence::MoveToNextWord);
  this->actionPreviousEvent->setShortcut(QKeySequence::MoveToPreviousWord);
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
  displayOptionsMenu->addAction(this->actionReframeFromOne);
  displayOptionsMenu->addSeparator();
  displayOptionsMenu->addAction(this->actionZoomUnzoomRegionOfInterest);
  displayOptionsMenu->addAction(this->actionCropRegionOfInterest);
  this->acquisitionOptionsButtonMenu->setMenu(displayOptionsMenu);
  // Event options menu
  QMenu* eventOptionsMenu = new QMenu(this);
  this->actionEditSelectedEvents->setEnabled(false);
  this->actionRemoveSelectedEvents->setEnabled(false);
  eventOptionsMenu->addAction(this->actionEditSelectedEvents);
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
  // Other extra menu
  this->mp_InsertEventMenu = new QMenu(tr("Insert Event"), this);
  this->mp_InsertEventMenu->addAction(this->actionInsertRightFootStrike);
  this->mp_InsertEventMenu->addAction(this->actionInsertRightFootOff);
  this->mp_InsertEventMenu->addAction(this->actionInsertRightOther);
  this->mp_InsertEventMenu->addSeparator();
  this->mp_InsertEventMenu->addAction(this->actionInsertLeftFootStrike);
  this->mp_InsertEventMenu->addAction(this->actionInsertLeftFootOff);
  this->mp_InsertEventMenu->addAction(this->actionInsertLeftOther);
  this->mp_InsertEventMenu->addSeparator();
  this->mp_InsertEventMenu->addAction(this->actionInsertGeneralFootStrike);
  this->mp_InsertEventMenu->addAction(this->actionInsertGeneralFootOff);
  this->mp_InsertEventMenu->addAction(this->actionInsertGeneralOther);
  
  // Add a contextual menu
  this->addAction(this->mp_InsertEventMenu->menuAction());
  QAction* sep1 = new QAction(this); sep1->setSeparator(true); this->addAction(sep1);
  this->addAction(this->actionEditSelectedEvents);
  this->addAction(this->actionRemoveSelectedEvents);
  this->addAction(this->actionClearEvents);
  QAction* sep2 = new QAction(this); sep2->setSeparator(true); this->addAction(sep2);
  this->addAction(this->actionNextEvent);
  this->addAction(this->actionPreviousEvent);
  QAction* sep3 = new QAction(this); sep3->setSeparator(true); this->addAction(sep3);
  this->addAction(actionZoomUnzoomRegionOfInterest);
  this->addAction(actionCropRegionOfInterest);
  this->setContextMenuPolicy(Qt::ActionsContextMenu);
  
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
  connect(this->timeEventBar, SIGNAL(sliderPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this, SLOT(updateROIAction(int)));
  connect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this, SLOT(updateROIAction(int)));
  connect(this->timeEventBar, SIGNAL(boundSelected(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(boundSelected(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(boundDeselected(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(boundDeselected(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(eventSelectionChanged(QList<int>)), this, SLOT(toggleEventSelection(QList<int>)));
  connect(this->timeEventBar, SIGNAL(eventAboutToBeMoved(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(eventAboutToBeMoved(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(eventPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(eventPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(eventMotionFinished(int, int)), this, SLOT(checkEventFrameModification(int, int)));
  // Actions
  connect(playbackSpeedActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changePlaybackParameters()));
  connect(this->actionZoomUnzoomRegionOfInterest, SIGNAL(triggered()), this, SLOT(toggleZoomRegionOfInterest()));
  connect(this->actionCropRegionOfInterest, SIGNAL(triggered()), this, SLOT(cropRegionOfInterest()));
  connect(this->actionPreviousEvent, SIGNAL(triggered()), this, SLOT(previousEvent()));
  connect(this->actionNextEvent, SIGNAL(triggered()), this, SLOT(nextEvent()));
  connect(this->actionClearEvents, SIGNAL(triggered()), this, SLOT(clearEvents()));
  connect(this->actionEditSelectedEvents, SIGNAL(triggered()), this, SLOT(editSelectedEvents()));
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
  connect(this->actionReframeFromOne, SIGNAL(triggered()), this, SLOT(emitAcquisitionReframedFromOne()));
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

void TimeEventControlerWidget::setAcquisition(Acquisition* acq)
{
  if (this->mp_Acquisition)
    disconnect(this->mp_Acquisition, 0, this, 0);
  this->mp_Acquisition = acq;
  connect(this->mp_Acquisition, SIGNAL(firstFrameChanged(int)), this, SLOT(reframeAcquisition(int)));
  connect(this->mp_Acquisition, SIGNAL(regionOfInterestChanged(int, int)), this, SLOT(setRegionOfInterest(int, int)));
  connect(this->mp_Acquisition, SIGNAL(eventFrameChanged(int,int)), this, SLOT(setEventFrame(int,int)));
  connect(this->mp_Acquisition, SIGNAL(eventsModified(QList<int>, QList<Event*>)), this, SLOT(setEvents(QList<int>, QList<Event*>)));
  connect(this->mp_Acquisition, SIGNAL(eventsRemoved(QList<int>, QList<Event*>)), this, SLOT(removeEvents(QList<int>, QList<Event*>)));
  connect(this->mp_Acquisition, SIGNAL(eventsInserted(QList<int>, QList<Event*>)), this, SLOT(insertEvents(QList<int>, QList<Event*>)));
}

void TimeEventControlerWidget::load()
{
  if (!this->mp_Acquisition)
    return;
  this->timeEventBar->load(this->mp_Acquisition);
  this->changePlaybackParameters();
  this->updateEventActions();
  this->actionCropRegionOfInterest->setEnabled(false);
  this->actionZoomUnzoomRegionOfInterest->setEnabled(false);
  this->actionNextEvent->setEnabled(this->timeEventBar->m_EventItems.isEmpty() ? false : true);
  this->actionPreviousEvent->setEnabled(this->timeEventBar->m_EventItems.isEmpty() ? false : true);
};

void TimeEventControlerWidget::reset()
{
  if (this->mp_Timer->isActive())
    this->stopPlayback();
  this->timeEventBar->reset();
  this->lcdNumber->display(0);
};

bool TimeEventControlerWidget::eventItemData(int index, int& typeId, int& frame, double rgb[3])
{
  if (index >= this->timeEventBar->m_EventItems.count())
    return false;
  const Event* evt = this->mp_Acquisition->eventAt(this->timeEventBar->m_EventItems[index].id);
  if (evt == 0)
    return false;
  typeId = evt->iconId;
  frame = this->timeEventBar->m_EventItems[index].ptr->frame;
  this->timeEventBar->m_EventItems[index].color.getRgbF(&rgb[0], &rgb[1], &rgb[2]);
  return true;
};

void TimeEventControlerWidget::togglePlayback()
{
  if (this->isEnabled())
  {
    if (this->mp_Timer->isActive())
      this->stopPlayback();
    else
      this->startPlayback();
  }
};

void TimeEventControlerWidget::startPlayback()
{
  this->mp_Timer->start(this->m_PlaybackDelay);
  this->playButton->setIcon(*this->mp_PauseIcon);
  disconnect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  disconnect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  disconnect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  disconnect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  disconnect(this->timeEventBar, SIGNAL(boundSelected(int)), this->lcdNumber, SLOT(display(int)));
  disconnect(this->timeEventBar, SIGNAL(boundSelected(int)), this, SIGNAL(currentFrameChanged(int)));
  disconnect(this->timeEventBar, SIGNAL(boundDeselected(int)), this->lcdNumber, SLOT(display(int)));
  disconnect(this->timeEventBar, SIGNAL(boundDeselected(int)), this, SIGNAL(currentFrameChanged(int)));
  disconnect(this->timeEventBar, SIGNAL(eventAboutToBeMoved(int)), this->lcdNumber, SLOT(display(int)));
  disconnect(this->timeEventBar, SIGNAL(eventAboutToBeMoved(int)), this, SIGNAL(currentFrameChanged(int)));
  disconnect(this->timeEventBar, SIGNAL(eventPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  disconnect(this->timeEventBar, SIGNAL(eventPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  emit playbackStarted();
}

void TimeEventControlerWidget::stopPlayback()
{
  this->mp_Timer->stop();
  this->playButton->setIcon(*this->mp_PlayIcon);
  connect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(leftBoundPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(rightBoundPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(boundSelected(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(boundSelected(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(boundDeselected(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(boundDeselected(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(eventAboutToBeMoved(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(eventAboutToBeMoved(int)), this, SIGNAL(currentFrameChanged(int)));
  connect(this->timeEventBar, SIGNAL(eventPositionChanged(int)), this->lcdNumber, SLOT(display(int)));
  connect(this->timeEventBar, SIGNAL(eventPositionChanged(int)), this, SIGNAL(currentFrameChanged(int)));
  emit playbackStopped();
}

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

void TimeEventControlerWidget::reframeAcquisition(int ff)
{
  int diff = this->timeEventBar->m_FirstFrame - ff;
  this->timeEventBar->m_FirstFrame = this->mp_Acquisition->firstFrame();
  this->timeEventBar->m_LastFrame = this->mp_Acquisition->lastFrame();
  this->timeEventBar->m_ROIFirstFrame -= diff;
  this->timeEventBar->m_ROILastFrame -= diff;
  this->timeEventBar->m_SliderPos -= diff;
  this->timeEventBar->m_LeftBoundPos -= diff;
  this->timeEventBar->m_RightBoundPos -= diff;
  this->timeEventBar->updateInternals();
  this->timeEventBar->update();
  this->lcdNumber->display(this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::toggleZoomRegionOfInterest()
{
  // Zoom mode
  if ((this->timeEventBar->m_FirstFrame == this->timeEventBar->m_ROIFirstFrame) && (this->timeEventBar->m_LastFrame == this->timeEventBar->m_ROILastFrame))
  {
    this->setRegionOfInterest(this->timeEventBar->m_LeftBoundPos, this->timeEventBar->m_RightBoundPos);
    this->actionZoomUnzoomRegionOfInterest->setText(tr("Unzoom Region of Interest"));
  }
  else
  {
    this->setRegionOfInterest(this->timeEventBar->m_FirstFrame, this->timeEventBar->m_LastFrame);
    this->actionZoomUnzoomRegionOfInterest->setText(tr("Zoom Region of Interest"));
  }
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
    int f = this->timeEventBar->m_EventItems[i].ptr->frame;
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
    int f = this->timeEventBar->m_EventItems[i].ptr->frame;
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

void TimeEventControlerWidget::editSelectedEvents()
{
  if (!this->timeEventBar->m_SelectedEvents.isEmpty())
  {
    QString label, description, context, subject;
    int frame;
    const Event* e = this->mp_Acquisition->eventAt(this->timeEventBar->m_SelectedEvents[0]);
    label = e->label;
    description = e->description;
    context = e->context;
    subject = e->subject;
    frame = e->frame;
    for (int i = 1 ; i < this->timeEventBar->m_SelectedEvents.count() ; ++i)
    {
      e = this->mp_Acquisition->eventAt(this->timeEventBar->m_SelectedEvents[i]);
      if (label.compare(e->label) != 0)
        label = "";
      if (description.compare(e->description) != 0)
        description = "";
      if (context.compare(e->context) != 0)
        context = "";
      if (subject.compare(e->subject) != 0)
        subject = "";
      if (frame != e->frame)
        frame = -1;
    }
    int oldFrame = this->currentFrame();
    NewEventDialog ned(NewEventDialog::Edit, this);
    int  frameAndROI[3] = {frame, this->timeEventBar->m_FirstFrame, this->timeEventBar->m_LastFrame};
    ned.setInformations(label, ned.contextComboBox->findText(context), frameAndROI, subject, description);
    ned.move(this->eventDialogGlobaPos(&ned));
    if (!this->mp_Timer->isActive())
    {
      connect(ned.frameSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setCurrentFrame(int)));
      if (frame != -1)
        this->setCurrentFrame(frame);
    }
    ned.exec();
    if (ned.result() == QDialog::Accepted)
    {
      // Check if there is some modifications
      if ((ned.labelCheckBox->checkState() == Qt::Checked)
       || (ned.contextCheckBox->checkState() == Qt::Checked)
       || (ned.frameCheckBox->checkState() == Qt::Checked)
       || (ned.subjectCheckBox->checkState() == Qt::Checked)
       || (ned.descriptionCheckBox->checkState() == Qt::Checked))
      {
        QList<Event*> events;
        // Create events with modified informations
        for (int i = 0 ; i < this->timeEventBar->m_SelectedEvents.count() ; ++i)
        {
          e = this->mp_Acquisition->eventAt(this->timeEventBar->m_SelectedEvents[i]);
          Event* ev = new Event;
          // Label modification?
          if (ned.labelCheckBox->checkState() == Qt::Checked)
            ev->label = ned.labelEdit->text();
          else
            ev->label = e->label;
          // Context modification?
          if (ned.contextCheckBox->checkState() == Qt::Checked)
            ev->context= ned.contextComboBox->currentText();
          else
            ev->context= e->context;
          // Frame modification?
          if (ned.frameCheckBox->checkState() == Qt::Checked)
            ev->frame = ned.frameSpinBox->value();
          else
            ev->frame = e->frame;
          // Subject modification?
          if (ned.subjectCheckBox->checkState() == Qt::Checked)
            ev->subject = ned.subjectEdit->text();
          else
            ev->subject = e->subject;
          // Description modification?
          if (ned.descriptionCheckBox->checkState() == Qt::Checked)
            ev->description = ned.descriptionEdit->text();
          else
            ev->description = e->description;
          // Select the icon id
          this->setEventIconId(ev);
          // Set the time
          ev->time = this->mp_Acquisition->timeFromFrame(ev->frame);
          
          events.push_back(ev);
        }
        emit eventsModified(this->timeEventBar->m_SelectedEvents, events);
      }
    }
    else if (!this->mp_Timer->isActive())
      this->setCurrentFrame(oldFrame);
  }
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

void TimeEventControlerWidget::setEvents(const QList<int>& ids, const QList<Event*>& events)
{
  for (int i = 0 ; i < ids.count() ; ++i)
    this->timeEventBar->setEvent(ids[i], events[i]);
  this->updateEventActions();
  emit currentFrameChanged(this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::removeEvents(const QList<int>& ids, const QList<Event*>& events)
{
  Q_UNUSED(events);
  for (int i = 0 ; i < ids.count() ; ++i)
    this->timeEventBar->removeEvent(ids[i]);
  this->updateEventActions();
  emit currentFrameChanged(this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::insertEvents(const QList<int>& ids, const QList<Event*>& events)
{
  for (int i = 0 ; i < ids.count() ; ++i)
    this->timeEventBar->insertEvent(ids[i], events[i]);
  this->updateEventActions();
  emit currentFrameChanged(this->timeEventBar->m_SliderPos);
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

void TimeEventControlerWidget::emitAcquisitionReframedFromOne()
{
  emit acquisitionReframed(1);
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
    if (freq >= 24.0)
    {
      int finalFreq = 1;
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
  {
    this->actionEditSelectedEvents->setEnabled(true);
    this->actionRemoveSelectedEvents->setEnabled(true);
  }
  else
  {
    this->actionEditSelectedEvents->setEnabled(false);
    this->actionRemoveSelectedEvents->setEnabled(false);
  }
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
    this->actionPreviousEvent->setEnabled(true);
    this->actionNextEvent->setEnabled(true);
    this->prevEventButton->setEnabled(true);
    this->nextEventButton->setEnabled(true);
  }
  else
  {
    this->actionClearEvents->setEnabled(false);
    this->actionPreviousEvent->setEnabled(false);
    this->actionNextEvent->setEnabled(false);
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
  int oldFrame = this->currentFrame();
  NewEventDialog ned(NewEventDialog::New, this);
  int  frameAndROI[3] = {frame, this->timeEventBar->m_FirstFrame, this->timeEventBar->m_LastFrame};
  ned.setInformations(label, context, frameAndROI, "", "");
  ned.move(this->eventDialogGlobaPos(&ned));
  if (!this->mp_Timer->isActive())
    connect(ned.frameSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setCurrentFrame(int)));
  if (this->m_OpenEditorWhenInserting)
    ned.exec();
  if ((ned.result() == QDialog::Accepted) || !this->m_OpenEditorWhenInserting)
  {
    Event* e = new Event;
    e->label = ned.labelEdit->text().trimmed();
    e->description = ned.descriptionEdit->text().trimmed();
    e->context = ned.contextComboBox->currentText();
    e->subject = ned.subjectEdit->text().trimmed();
    e->frame = ned.frameSpinBox->value();
    e->time = this->mp_Acquisition->timeFromFrame(e->frame);
    this->setEventIconId(e);
    emit eventInserted(e);
  }
  else if (!this->mp_Timer->isActive())
    this->setCurrentFrame(oldFrame);
};

void TimeEventControlerWidget::updateROIAction(int frame)
{
  Q_UNUSED(frame);
  //if ((this->timeEventBar->m_ROIFirstFrame != this->mp_Acquisition->firstFrame()) || (this->timeEventBar->m_ROILastFrame != this->mp_Acquisition->lastFrame()))
  //if ((this->timeEventBar->m_ROIFirstFrame != this->timeEventBar->m_FirstFrame) || (this->timeEventBar->m_ROILastFrame != this->timeEventBar->m_LastFrame))
  if ((this->timeEventBar->m_LeftBoundPos != this->timeEventBar->m_ROIFirstFrame) || (this->timeEventBar->m_RightBoundPos != this->timeEventBar->m_ROILastFrame))
  {
    this->actionCropRegionOfInterest->setEnabled(true);
    this->actionZoomUnzoomRegionOfInterest->setEnabled(true);
  }
  else
  {
    this->actionCropRegionOfInterest->setEnabled(false);
    this->actionZoomUnzoomRegionOfInterest->setEnabled(false);
  }
};

void TimeEventControlerWidget::checkEventFrameModification(int id, int frame)
{
  const Event* ev = this->mp_Acquisition->eventAt(id);
  if (ev && ev->frame != frame)
    emit eventFrameModified(id, frame);
};

void TimeEventControlerWidget::setEventFrame(int id, int frame)
{
  Q_UNUSED(frame);
  for (int i = 0 ; i < this->timeEventBar->m_EventItems.count() ; ++i)
  {
    if (this->timeEventBar->m_EventItems[i].id == id)
    {
      this->timeEventBar->updateEventGeometry(i);
      break;
    }
  }
  this->timeEventBar->update();
  emit currentFrameChanged(this->timeEventBar->m_SliderPos);
};

void TimeEventControlerWidget::setEventIconId(Event* e)
{
  e->iconId = -1;
  // Look for the symbol to assign (icon ID)
  if (e->label.compare("Foot Strike", Qt::CaseInsensitive) == 0)
    e->iconId = 1;
  else if (e->label.compare("Foot Off", Qt::CaseInsensitive) == 0)
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
};

QPoint TimeEventControlerWidget::eventDialogGlobaPos(const NewEventDialog* ned) const
{
  QStyle* wStyle = ned->style();
  QStyleOptionTitleBar so;
  so.titleBarState = 1;		// kThemeStateActive
  so.titleBarFlags = Qt::Window;
  int titleBarHeight = wStyle->pixelMetric(QStyle::PM_TitleBarHeight,&so,ned);
#if defined(Q_WS_MAC) // see http://bugreports.qt.nokia.com/browse/QTBUG-13475
  titleBarHeight -= 4;
#endif
  QRect thisRect = this->rect();
  QRect nedRect = ned->rect();
  QPoint p = this->mapToGlobal(thisRect.topRight());
  return p - QPoint(nedRect.width() + 15, nedRect.height() + titleBarHeight + 15);
};