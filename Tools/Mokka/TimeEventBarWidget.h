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
  void setEvent(int id, const Event* e);
  void removeEvent(int id);
  void insertEvent(int id, const Event* e);
  
public slots:
  void setSliderValue(int frame);
  
signals:
  void sliderPositionChanged(int frame);
  void leftBoundPositionChanged(int frame);
  void rightBoundPositionChanged(int frame);
  void boundSelected(int frame);
  void boundDeselected(int frame);
  void eventSelectionChanged(const QList<int>& selectedIndices);
  
protected:
  bool event(QEvent *event);
  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);
  
private:
  void updateEventPos(int idx);

  friend class TimeEventControlerWidget;
  
  struct EventItem
  {
    int id;
    int contextId;
    int frame;
    QString symbol;
    QColor color;
    QString toolTip;
    QRectF boundingRect;
  };
  
  enum {None, MoveSlider, MoveLeftBound, MoveRightBound, Rubber};
  int eventItemAt(const QPoint& pos) const;
  void setEventItem(EventItem& item, int id, const Event* e);
  void updateInternals();
  void updateSliderPostion();
  void updateLeftBoundPostion();
  void updateRightBoundPostion();
  void updateEventsPosition();
  void updateEventPosition(int id);
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
  int m_WBound;
  int m_WSlider;
  int m_TopMargin;
  int m_BottomMargin;
  int m_LeftMargin;
  int m_RightMargin;
  int m_TickDivider;
  QVector<int> m_Ticks;
  QVector<QString> m_TicksLabel;
  QFontMetrics m_Fm;
  QFont m_EventFont;
  qreal m_EventSymbolSize;
  qreal m_YEventPosRelative;
  QPoint mp_PointsSlider[4]; // 0: shape ; 1-3: shadow
  QPoint mp_PointsBoundLeft[10]; // 0-6: shape ; 7-10: shadow
  QPoint mp_PointsBoundRight[12]; // 0-6: shape ; 7-12: shadow
  QColor m_MovBrushColor;
  QColor m_BoundBrushColor;
  QPen m_MovPen;
  QPen m_MovShadowPen;
  int m_Mode;
  QVector<EventItem> m_EventItems;
  QVector<QString> m_EventSymbols;
  QList<int> m_SelectedEvents;
  QRubberBand* mp_Rubber;
  QPoint m_RubberOrigin;
};

inline void TimeEventBarWidget::updateEventPos(int idx)
{
  qreal xPos = this->m_LeftMargin + ((this->m_EventItems[idx].frame - this->m_ROIFirstFrame)*this->m_UnitStep) - this->m_EventSymbolSize / 2.0;// + 0.5;
  qreal yPos = this->m_EventItems[idx].contextId * 7.0 * (this->height() - this->m_TopMargin - this->m_BottomMargin) / 22.0 + this->m_YEventPosRelative;
  this->m_EventItems[idx].boundingRect.setRect(xPos, yPos, this->m_EventSymbolSize, this->m_YEventPosRelative);
};

#endif // TimeEventBarWidget_h