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

#ifndef TimeEventControlerWidget_h
#define TimeEventControlerWidget_h

#include "ui_TimeEventControlerWidget.h"

#include <QWidget>
#include <QTimer>
#include <QMenu>

class Acquisition;
class NewEventDialog;

class TimeEventControlerWidget : public QWidget, public Ui::TimeEventControlerWidget
{
  Q_OBJECT

public:
  TimeEventControlerWidget(QWidget* parent = 0);
  ~TimeEventControlerWidget();
  
  bool playbackStatus() const {return this->mp_Timer->isActive();};
  
  void setAcquisition(Acquisition* acq);
  void load();
  void reset();
  QMenu* playbackSpeedMenu() const {return this->mp_PlaybackSpeedMenu;};
  QMenu* insertEventMenu() const {return this->mp_InsertEventMenu;};
  int currentFrame() const {return this->timeEventBar->m_SliderPos;};
  int leftBound() const {return this->timeEventBar->m_LeftBoundPos;};
  int rightBound() const {return this->timeEventBar->m_RightBoundPos;};
  
  bool openEditorWhenInsertingEventFlag() const {return this->m_OpenEditorWhenInserting;};
  void setOpenEditorWhenInsertingEventFlag(bool isUsed) {this->m_OpenEditorWhenInserting = isUsed;};
  
  bool eventItemData(int index, int& typeId, int& frame, double rgb[3]);
  
public slots:
  void togglePlayback();
  void stopPlayback();
  void startPlayback();
  void setCurrentFrame(int frame);
  void nextStep();
  void nextFrame();
  void previousFrame();
  void reframeAcquisition(int ff);
  void toggleZoomRegionOfInterest();
  void cropRegionOfInterest();
  void setRegionOfInterest(int lb, int rb);
  void previousEvent();
  void nextEvent();
  void clearEvents();
  void editSelectedEvents();
  void removeSelectedEvents();
  void removeAllRightFootStrikeEvents();
  void removeAllRightFootOffEvents();
  void removeAllLeftFootStrikeEvents();
  void removeAllLeftFootOffEvents();
  void removeAllGeneralFootStrikeEvents();
  void removeAllGeneralFootOffEvents();
  void insertRightFootStrike();
  void insertRightFootOff();
  void insertRightOther();
  void insertLeftFootStrike();
  void insertLeftFootOff();
  void insertLeftOther();
  void insertGeneralFootStrike();
  void insertGeneralFootOff();
  void insertGeneralOther();
  void setEvents(const QList<int>& ids, const QList<Event*>& events);
  void removeEvents(const QList<int>& ids, const QList<Event*>& events);
  void insertEvents(const QList<int>& ids, const QList<Event*>& events);

signals:
  void playbackStarted();
  void playbackStopped();
  void currentFrameChanged(int frame);
  void acquisitionReframed(int firstFrame);
  void regionOfInterestChanged(int ff, int lf);
  void eventFrameModified(int id, int frame);
  void eventsModified(const QList<int>& ids, const QList<Event*>& events);
  void eventsRemoved(const QList<int>& ids);
  void eventInserted(Event* e);
  
private slots:
  void pressPlayButton();
  void releasePlayButton();
  void emitAcquisitionReframedFromOne();
  void pressPrevEventButton();
  void releasePrevEventButton();
  void pressNextEventButton();
  void releaseNextEventButton();
  void changePlaybackParameters();
  void toggleEventSelection(const QList<int>& selectedIndices);
  void updateROIAction(int frame);
  void checkEventFrameModification(int id, int frame);
  void setEventFrame(int id, int frame);
  
private:
  void setFrame(int f);
  void setROI(int ff, int lf);
  void updateEventActions();
  QList<int> removeEvent(const QString& context, const QString& label);
  void insertEvent(const QString& label, int context, int frame);
  void setEventIconId(Event* e);
  void setEventTime(Event* e);
  QPoint eventDialogGlobaPos(const NewEventDialog* ned) const;
  
  Acquisition* mp_Acquisition;
  QMenu* mp_PlaybackSpeedMenu;
  QMenu* mp_InsertEventMenu;
  QIcon* mp_PlayIcon;
  QIcon* mp_PlayActiveIcon;
  QIcon* mp_PauseIcon;
  QIcon* mp_PauseActiveIcon;
  QIcon* mp_PrevEventIcon;
  QIcon* mp_PrevEventActiveIcon;
  QIcon* mp_NextEventIcon;
  QIcon* mp_NextEventActiveIcon;
  QTimer* mp_Timer;
  int m_PlaybackStep;
  int m_PlaybackDelay;
  bool m_OpenEditorWhenInserting;
};

#endif // TimeEventControlerWidget_h