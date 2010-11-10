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

#ifndef TimeEventControlerWidget_h
#define TimeEventControlerWidget_h

#include "ui_TimeEventControlerWidget.h"

#include <QWidget>
#include <QTimer>
#include <QMenu>

class Acquisition;

class TimeEventControlerWidget : public QWidget, public Ui::TimeEventControlerWidget
{
  Q_OBJECT

public:
  TimeEventControlerWidget(QWidget* parent = 0);
  ~TimeEventControlerWidget();
  
  void setAcquisition(Acquisition* acq);
  void load();
  void reset();
  QMenu* playbackSpeedMenu() {return this->mp_PlaybackSpeedMenu;};
  int currentFrame() {return this->timeEventBar->m_SliderPos;};
  
public slots:
  void togglePlayback();
  void setCurrentFrame(int frame);
  void nextStep();
  void nextFrame();
  void previousFrame();
  void cropRegionOfInterest();
  void setRegionOfInterest(int lb, int rb);
  void previousEvent();
  void nextEvent();
  void clearEvents();
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
  void removeEvents(const QList<int>& ids, const QList<Event*>& events);
  void insertEvents(const QList<int>& ids, const QList<Event*>& events);
  
protected:
  void keyPressEvent(QKeyEvent* event);

signals:
  void currentFrameChanged(int frame);
  void regionOfInterestChanged(int ff, int lf);
  void eventsRemoved(const QList<int>& ids);
  void eventInserted(Event* e);
  
private slots:
  void pressPlayButton();
  void releasePlayButton();
  void pressPrevEventButton();
  void releasePrevEventButton();
  void pressNextEventButton();
  void releaseNextEventButton();
  void changePlaybackParameters();
  void toggleEventSelection(const QList<int>& selectedIndices);
  void updateROIAction(int frame);
  
private:
  void setFrame(int f);
  void setROI(int ff, int lf);
  void updateEventActions();
  QList<int> removeEvent(const QString& context, const QString& label);
  void insertEvent(const QString& label, int context, int frame);
  
  Acquisition* mp_Acquisition;
  QMenu* mp_PlaybackSpeedMenu;
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
};

#endif // TimeEventControlerWidget_h