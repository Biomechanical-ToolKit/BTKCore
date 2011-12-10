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

#ifndef TimeEventBarWidget_h
#define TimeEventBarWidget_h

#include "Acquisition.h"

#include <QFrame>
#include <QString>
#include <QBrush>
#include <QPen>
#include <QRubberBand>
#include <QFont>

class TimeEventControlerWidget;

class TimeEventBarWidget : public QFrame
{
  Q_OBJECT

public:
  TimeEventBarWidget(QWidget* parent = 0);
  void load(Acquisition* acq);
  void reset();
  void setEvent(int id, Event* e);
  void removeEvent(int id);
  void insertEvent(int id, Event* e);
  
public slots:
  void setSliderValue(int frame);
  
signals:
  void sliderPositionChanged(int frame);
  void leftBoundPositionChanged(int frame);
  void rightBoundPositionChanged(int frame);
  void boundSelected(int frame);
  void boundDeselected(int frame);
  void eventSelectionChanged(const QList<int>& selectedIndices);
  void eventAboutToBeMoved(int frame);
  void eventPositionChanged(int frame); // Mouse motion
  void eventMotionFinished(int id, int frame); // Mouse release
  
protected:
  bool event(QEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);
  
private:
  friend class TimeEventControlerWidget;
  
  struct EventItem
  {
    Event* ptr;
    int id;
    int contextId;
    QString symbol;
    QColor color;
    QRectF boundingRect;
  };
  
  enum {None, MoveSlider, MoveLeftBound, MoveRightBound, MoveEvent, Rubber};
  int eventItemAt(const QPoint& pos) const;
  void setEventItem(EventItem& item, int id, Event* e);
  void updateInternals();
  void updateSliderPostion();
  void updateLeftBoundPostion();
  void updateRightBoundPostion();
  void updateEventsPosition();
  void updateEventPosition(int id);
  void updateEventGeometry(int idx);
  void updateEventSelection();
  
  int m_FirstFrame;
  int m_LastFrame;
  int m_ROIFirstFrame;
  int m_ROILastFrame;
  QStringList m_EventContexts;
  int m_SliderPos;
  int m_LeftBoundPos;
  int m_RightBoundPos;
  qreal m_UnitStep;
  int m_TickDivider;
  QVector<int> m_Ticks;
  QVector<QString> m_TicksLabel;
  QFontMetrics m_Fm;
  QPoint mp_PointsSlider[4]; // 0: shape ; 1-3: shadow
  QPoint mp_PointsBoundLeft[10]; // 0-6: shape ; 7-10: shadow
  QPoint mp_PointsBoundRight[12]; // 0-6: shape ; 7-12: shadow
  int m_Mode;
  QVector<EventItem> m_EventItems;
  QVector<QString> m_EventSymbols;
  QList<int> m_SelectedEvents;
  int m_MovingEventIndex;
  QRubberBand* mp_Rubber;
  QPoint m_RubberOrigin;
};

#endif // TimeEventBarWidget_h