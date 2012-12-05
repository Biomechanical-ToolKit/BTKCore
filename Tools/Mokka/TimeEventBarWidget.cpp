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

#include "TimeEventBarWidget.h"

#include <btkMacro.h>

#include <QtGui>
#include <QLinearGradient>
#include <QFontMetrics>

#include <cmath>

static int oldMouseX = 0;
static int oldEventFrame = 0;
static const int WBound = 10;
static const int WSlider = 10;
static const int TopMargin = 10;
static const int BottomMargin = 30;
static const int LeftMargin = 60;
static const int RightMargin = 30;
#ifdef Q_WS_WIN
static const qreal EventRelativePosY = 13.0;
static const qreal EventSymbolSize = 15.0;
static const QFont EventFont = QFont("Lucida Sans Unicode", 14);
#else
static const qreal EventRelativePosY = 16.0;
static const qreal EventSymbolSize = 12.0;
static const QFont EventFont = QFont("Arial", 12);
#endif
static const QColor MoveBrushColor = QColor(0, 127, 255, 128);
static const QColor BoundBrushColor = QColor(0, 127, 255, 64);
static const QPen MovePen = QColor(0, 127, 255);
static const QPen MoveShadowPen = QColor(100, 100, 100, 128);

TimeEventBarWidget::TimeEventBarWidget(QWidget* parent)
: QFrame(parent), m_EventContexts(), m_Ticks(), m_TicksLabel(), 
  m_Fm(this->font()), m_EventItems(), m_EventSymbols(8), m_SelectedEvents(),
  m_RubberOrigin()
{
  this->m_ROIFirstFrame = 0;
  this->m_ROILastFrame = 0;
  this->m_EventContexts << "Right" << "Left" << "General";
  this->m_SliderPos = 0;
  this->m_LeftBoundPos = 0;
  this->m_RightBoundPos = 0;
  this->m_UnitStep = 1.0;
  this->m_TickDivider = 1;
#ifdef Q_OS_MAC
  QFont f = this->font();
  f.setPointSize(10); this->setFont(f);
  this->m_Fm = f;
#endif
  this->m_Mode = None;
  this->m_MovingEventIndex = -1;
  this->mp_Rubber = new QRubberBand(QRubberBand::Rectangle, this);
  this->m_TimeDisplay = false;
  this->m_TimeScale = 1.0;
  
  ushort s;
  s = 0x25C9; this->m_EventSymbols[0].setUtf16(&s,1); // General: Fisheye
  s = 0x25C8; this->m_EventSymbols[1].setUtf16(&s,1); // Foot strike: White diamond containing black small diamond
  s = 0x25B2; this->m_EventSymbols[2].setUtf16(&s,1); // Foot off:    Black up-pointing triangle
  s = 0x25CF; this->m_EventSymbols[3].setUtf16(&s,1); // Black circle
  s = 0x25CE; this->m_EventSymbols[4].setUtf16(&s,1); // Bullseye
  s = 0x25D2; this->m_EventSymbols[5].setUtf16(&s,1); // Circle with lower half black
  s = 0x25D3; this->m_EventSymbols[6].setUtf16(&s,1); // Circle with upper half black
  s = 0x25BC; this->m_EventSymbols[7].setUtf16(&s,1); // Black down-pointing triangle
};

void TimeEventBarWidget::load(Acquisition* acq)
{
  this->m_SelectedEvents.clear();
  if (acq->pointFrameNumber() == 0)
  {
    this->m_ROIFirstFrame = 0;
    this->m_ROILastFrame = 0;
  }
  else
  {
    acq->regionOfInterest(this->m_ROIFirstFrame, this->m_ROILastFrame);
  }
  
  this->m_SliderPos = this->m_ROIFirstFrame;
  this->m_LeftBoundPos = this->m_ROIFirstFrame;
  this->m_RightBoundPos = this->m_ROILastFrame;
  this->m_TimeScale = 1.0 / acq->pointFrequency();
  
  this->m_EventItems.resize(acq->eventCount());
  int inc = 0;
  for (QMap<int,Event*>::const_iterator it = acq->events().begin() ; it != acq->events().end() ; ++it)
    this->setEventItem(this->m_EventItems[inc++], it.key(), it.value());
  
  this->setSliderValue(this->m_ROIFirstFrame);
  this->updateInternals();
  this->update();
};

void TimeEventBarWidget::reset()
{
  this->m_SelectedEvents.clear();
  this->m_EventItems.clear();
  this->m_ROIFirstFrame = 0;
  this->m_ROILastFrame = 0;
  this->m_SliderPos = 0;
  this->m_LeftBoundPos = 0;
  this->m_RightBoundPos = 0;
  this->updateInternals();
  this->update();
};

void TimeEventBarWidget::setEvent(int id, Event* e)
{
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
  {
    if (this->m_EventItems[i].id == id)
    {
      this->setEventItem(this->m_EventItems[i], id, e);
      this->updateEventGeometry(i);
      if (this->m_SelectedEvents.contains(this->m_EventItems[i].id))
      {
        if (this->m_EventItems[i].contextId == 0) // Right
          this->m_EventItems[i].color = Qt::green;
        else if (this->m_EventItems[i].contextId == 1) // Left
          this->m_EventItems[i].color = Qt::red;
        else // General
          this->m_EventItems[i].color = Qt::yellow;
      }
      this->update();
      break;
    }
  }
};

void TimeEventBarWidget::removeEvent(int id)
{
  for (int i = 0 ; i < this->m_SelectedEvents.count(); ++i)
  {
    if (this->m_SelectedEvents[i] == id)
    {
      this->m_SelectedEvents.removeAt(i);
      break;
    }
  }
  for (int i = 0 ; i < this->m_EventItems.count(); ++i)
  {
    if (this->m_EventItems[i].id == id)
    {
      this->m_EventItems.remove(i);
      break;
    }
  }
  this->update();
  emit eventSelectionChanged(this->m_SelectedEvents);
};

void TimeEventBarWidget::insertEvent(int id, Event* e)
{
  EventItem item;
  this->setEventItem(item, id, e);
  this->m_EventItems.push_back(item);
  this->updateEventPosition(id);
  this->update();
};

void TimeEventBarWidget::setSliderValue(int frame)
{
  this->m_SliderPos = frame;
  this->updateSliderPostion();
  emit sliderPositionChanged(this->m_SliderPos);
  this->repaint();
};

void TimeEventBarWidget::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  
  // --------------------- Timeline ---------------------
  int numFrames = this->m_ROILastFrame - this->m_ROIFirstFrame + 1;
  QPen axisPen = QPen();
  painter.setPen(axisPen);
  qreal h = this->height() - TopMargin - BottomMargin;
  qreal hStep = h / 22.0;
  int yStep = 7;
  int dd = 1;
  int subTicksStart = 0;
  if (this->m_TickDivider >  10)
    dd = this->m_TickDivider / 10;
  if (dd != 1)
    subTicksStart = ((this->m_ROIFirstFrame / dd) + 1) * dd - this->m_ROIFirstFrame;
  int xMax = LeftMargin + (numFrames - (numFrames>1?1:0)) * this->m_UnitStep;
  for (int i = 0 ; i < 3 ; ++i)
  {
    // Context
    painter.drawText(0, i * yStep * hStep + 2 + TopMargin, LeftMargin - 20, yStep * hStep, Qt::AlignLeft | Qt::AlignBottom, this->m_EventContexts[i]);
    // Base (time axis)
    qreal tStep = (21 - i * yStep) * hStep + TopMargin;
    painter.drawLine(LeftMargin, tStep, xMax, tStep);
    // Extremities
    painter.drawLine(LeftMargin, tStep, LeftMargin, tStep - 9);
    painter.drawLine(xMax, tStep, xMax, tStep - 9);
    // Ticks & SubTicks
    int incST = subTicksStart + (this->m_TimeDisplay ? 1 : 0);
    int tickInc = 1;
    while (incST < numFrames)
    {
      if ((tickInc < this->m_Ticks.size()) && (abs(this->m_Ticks[tickInc] - incST) <= dd / 2)) // Final adjustment for the ticks
      {
        painter.drawLine(LeftMargin + incST * this->m_UnitStep, tStep, LeftMargin + incST * this->m_UnitStep, tStep - 6);
        ++tickInc;
      }
      painter.drawLine(LeftMargin + incST * this->m_UnitStep, tStep, LeftMargin + incST * this->m_UnitStep, tStep - 3);
      incST += dd; 
    }
  }
  // Text for the ticks
  for (int i = 0 ; i < this->m_TicksLabel.count() ; ++i)
  {
    int wTextFrame = this->m_Fm.width(this->m_TicksLabel[i]);
    painter.drawText(LeftMargin + this->m_Ticks[i] * this->m_UnitStep - wTextFrame / 2, h + TopMargin + 10, wTextFrame, 20, Qt::AlignHCenter | Qt::AlignTop, this->m_TicksLabel[i]);
  }
  
  // --------------------- Events ---------------------
  painter.setFont(EventFont);
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
  {
    if ((this->m_EventItems[i].ptr->frame >= this->m_ROIFirstFrame) && (this->m_EventItems[i].ptr->frame <= this->m_ROILastFrame))
    {
      painter.setPen(this->m_EventItems[i].color);
      painter.drawText(this->m_EventItems[i].boundingRect, Qt::AlignCenter, this->m_EventItems[i].symbol);
      //painter.drawRect(this->m_EventItems[i].boundingRect);
    }
  }
  
  // --------------------- Moveable objects ---------------------
  if (this->isEnabled())
  { 
    int wLRB = (int)((this->m_LeftBoundPos-this->m_ROIFirstFrame)*this->m_UnitStep); // width of the rectangle between the first frame and the left bound
    int wRRB = (int)((this->m_ROILastFrame-this->m_RightBoundPos)*this->m_UnitStep); // width of the rectangle between the right bound and the last frame
    int xRB = xMax - wRRB + 1;
    // - left & right rectangle
    painter.setPen(Qt::NoPen);
    painter.setBrush(BoundBrushColor);
    painter.drawRect(LeftMargin, TopMargin, wLRB, h+5);
    painter.drawRect(xRB, TopMargin, wRRB+1, h+5);
    // - shadows
    painter.setPen(MoveShadowPen);
    painter.drawPolyline(this->mp_PointsSlider+1, 3);
    painter.drawPolyline(this->mp_PointsBoundRight+7, 5);
    painter.drawPolyline(this->mp_PointsBoundLeft+7, 3);
    // - left & right bounds
    painter.setPen(MovePen);
    painter.setBrush(MoveBrushColor);
    painter.drawPolygon(this->mp_PointsBoundRight, 7);
    painter.drawPolygon(this->mp_PointsBoundLeft, 7);
    // - slider
    painter.drawRect(this->mp_PointsSlider[0].x()-WSlider/2, this->mp_PointsSlider[0].y(), WSlider, h+5);
    painter.drawLine(this->mp_PointsSlider[0].x(), this->mp_PointsSlider[0].y(), this->mp_PointsSlider[0].x(), this->mp_PointsSlider[0].y()+h+5);
  }
  
  this->QWidget::paintEvent(event);
};

void TimeEventBarWidget::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event);
  this->updateInternals();
  this->QWidget::resizeEvent(event);
};

bool TimeEventBarWidget::event(QEvent *event)
{
  if (event->type() == QEvent::ToolTip)
  {
    QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
    int index = this->eventItemAt(helpEvent->pos());
    if (index != -1)
    {
      Event* e = this->m_EventItems[index].ptr;
      QString text = QString("<b>Event:</b> %1 %2<br/><b>Subject:</b> %3<br/><b>Frame:</b> %4<br/><b>Time:</b> %5 second(s)").arg(e->context, e->label, e->subject).arg(e->frame).arg(e->time);
      QToolTip::showText(helpEvent->globalPos(), text);
    }
    else
    {
      QToolTip::hideText();
      event->ignore();
    }
    return true;
  }
  return QWidget::event(event);
};

void TimeEventBarWidget::mouseMoveEvent(QMouseEvent* event)
{
  // Fine movement: +/- 1 frame
  int fineShift = 1;
  if ((event->pos().x() - oldMouseX) < 0)
    fineShift = -1;
    
  if ((event->buttons() & Qt::LeftButton) && (this->m_Mode != None))
  {
    int frame = (int)((qreal)(event->pos().x() - LeftMargin) / this->m_UnitStep) + this->m_ROIFirstFrame;
    if (this->m_Mode == MoveSlider)
    {
      if (event->modifiers() & Qt::ShiftModifier) 
        frame = this->m_SliderPos + fineShift;
      if (frame < this->m_LeftBoundPos)
        frame = this->m_LeftBoundPos;
      else if (frame > this->m_RightBoundPos)
        frame = this->m_RightBoundPos;
      this->setSliderValue(frame);
    }
    else if (this->m_Mode == MoveLeftBound)
    {
      if (event->modifiers() & Qt::ShiftModifier) 
        frame = this->m_LeftBoundPos + fineShift;
      if (frame < this->m_ROIFirstFrame)
        frame = this->m_ROIFirstFrame;
      else if (frame > this->m_RightBoundPos)
        frame = this->m_RightBoundPos;
      this->m_LeftBoundPos = frame;
      if (this->m_SliderPos < this->m_LeftBoundPos)
      {
        this->m_SliderPos = this->m_LeftBoundPos;
        this->updateSliderPostion();
      }
      this->updateLeftBoundPostion();
      emit leftBoundPositionChanged(this->m_LeftBoundPos);
      this->repaint();
    }
    else if (this->m_Mode == MoveRightBound)
    {
      if (event->modifiers() & Qt::ShiftModifier) 
        frame = this->m_RightBoundPos + fineShift;
      if (frame < this->m_LeftBoundPos)
        frame = this->m_LeftBoundPos;
      else if (frame > this->m_ROILastFrame)
        frame = this->m_ROILastFrame;
      this->m_RightBoundPos = frame;
      if (this->m_SliderPos > this->m_RightBoundPos)
      {
        this->m_SliderPos = this->m_RightBoundPos;
        this->updateSliderPostion();
      }
      this->updateRightBoundPostion();
      emit rightBoundPositionChanged(this->m_RightBoundPos);
      this->repaint();
    }
    else if (this->m_Mode == MoveEvent)
    {
      if (event->modifiers() & Qt::ShiftModifier) 
        frame = this->m_EventItems[this->m_MovingEventIndex].ptr->frame + fineShift;
      if (frame < this->m_LeftBoundPos)
          frame = this->m_LeftBoundPos;
      else if (frame > this->m_RightBoundPos)
        frame = this->m_RightBoundPos;
      this->m_EventItems[this->m_MovingEventIndex].ptr->frame = frame;
      this->updateEventGeometry(this->m_MovingEventIndex);
      emit eventPositionChanged(frame);
      this->repaint();
    }
    else if (this->m_Mode == Rubber)
    { 
      if (event->modifiers() & Qt::ShiftModifier)
        this->mp_Rubber->setGeometry(QRect(this->m_RubberOrigin, event->pos()).normalized());
    }
    oldMouseX = event->pos().x();
  }
};

void TimeEventBarWidget::mousePressEvent(QMouseEvent* event)
{
  if ((event->button() == Qt::LeftButton) && this->isEnabled())
  {
    // Event movement
    if (event->modifiers() & Qt::AltModifier)
    {
      this->m_RubberOrigin = event->pos();
      this->mp_Rubber->setGeometry(QRect(this->m_RubberOrigin, QSize(1,1)));
      QRectF rubberRect = QRectF(this->mp_Rubber->x(), this->mp_Rubber->y(), this->mp_Rubber->rect().width(), this->mp_Rubber->rect().height());
      for (int i = 0 ; i <  this->m_EventItems.count() ; ++i)
      {
        if (rubberRect.intersects(this->m_EventItems[i].boundingRect))
        {
          this->m_Mode = MoveEvent;
          emit eventAboutToBeMoved(this->m_EventItems[i].ptr->frame);
          this->m_MovingEventIndex = i;
          oldEventFrame = this->m_EventItems[this->m_MovingEventIndex].ptr->frame;
          this->m_EventItems[i].color = MoveBrushColor;
          this->update();
          emit sliderPositionChanged(this->m_SliderPos); // To update some widgets which can display event
          break;
        }
      }
      return;
    }
    // Slider
    QRect slider(this->mp_PointsSlider[0].x()-(WSlider+4)/2, this->mp_PointsSlider[0].y(), WSlider+4, this->height() - TopMargin - BottomMargin + 5);
    if (slider.contains(event->pos()))
    {
      this->m_Mode = MoveSlider;
      return;
    }
    // Left bound
    QRect leftBound(this->mp_PointsBoundLeft[1].x()-WBound, this->mp_PointsBoundLeft[1].y()-20, WBound, 20);
    if (leftBound.contains(event->pos()))
    {
      this->m_Mode = MoveLeftBound;
      emit boundSelected(this->m_LeftBoundPos);
      return;
    }
    // Right bound
    QRect rightBound(this->mp_PointsBoundRight[1].x(), this->mp_PointsBoundRight[1].y()-20, WBound, 20);
    if (rightBound.contains(event->pos()))
    {
      this->m_Mode = MoveRightBound;
      emit boundSelected(this->m_RightBoundPos);
      return;
    }
    // Selection (point or rectangle)
    bool noPreviousEventSelection = this->m_SelectedEvents.isEmpty();
    this->m_Mode = Rubber;
    this->m_RubberOrigin = event->pos();
    this->mp_Rubber->setGeometry(QRect(this->m_RubberOrigin, QSize(1,1)));
    if (!(event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier)))
      this->m_SelectedEvents.clear();
    else if (event->modifiers() & Qt::ShiftModifier)
      this->mp_Rubber->show();
    QList<int> currentSelection = this->m_SelectedEvents;
    QList<int> selectedEvents = this->extractSelectedEvents();
    for (int i = 0 ; i < selectedEvents.count() ; ++i)
    {
      int idx = currentSelection.indexOf(selectedEvents[i]);
      if (idx == -1)
        currentSelection.append(selectedEvents[i]);
      else
        currentSelection.removeAt(idx);
    }
    this->updateEventSelection(currentSelection);
    emit sliderPositionChanged(this->m_SliderPos); // To update some widgets which can display event
    // If there was not some events selected, then we move the slider to the position
    if (noPreviousEventSelection && selectedEvents.isEmpty() && ((event->modifiers() & Qt::ShiftModifier) != Qt::ShiftModifier))
    {
      this->m_Mode = MoveSlider;
      int frame = (int)((qreal)(event->pos().x() - LeftMargin) / this->m_UnitStep) + this->m_ROIFirstFrame;
      if ((frame >= this->m_LeftBoundPos) && (frame <= this->m_RightBoundPos))
        this->setSliderValue(frame);
    }
  }
  else
    this->m_Mode = None;
};

void TimeEventBarWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (this->m_Mode == MoveEvent)
  {
    int newEventFrame = this->m_EventItems[this->m_MovingEventIndex].ptr->frame;
    this->m_EventItems[this->m_MovingEventIndex].ptr->frame = oldEventFrame;
    emit eventMotionFinished(this->m_EventItems[this->m_MovingEventIndex].id, newEventFrame);
    this->m_MovingEventIndex = -1;
    this->updateEventSelection(this->m_SelectedEvents);
  }
  else if (this->m_Mode == Rubber)
  {
    if (!this->mp_Rubber->isVisible())
    {
      this->m_RubberOrigin = event->pos();
      this->mp_Rubber->setGeometry(QRect(this->m_RubberOrigin, QSize(1,1)));
    }
    QList<int> selectedEvents = this->extractSelectedEvents();
    for (int i = 0 ; i < selectedEvents.count() ; ++i)
    {
      int idx = this->m_SelectedEvents.indexOf(selectedEvents[i]);
      if (idx == -1)
        this->m_SelectedEvents.append(selectedEvents[i]);
      else
        this->m_SelectedEvents.removeAt(idx);
    }
    this->updateEventSelection(this->m_SelectedEvents);
    this->mp_Rubber->hide();
  }
  
  this->m_Mode = None;
  emit boundDeselected(this->m_SliderPos);
};

int TimeEventBarWidget::eventItemAt(const QPoint& pos) const
{
  for (int i = 0 ; i < this->m_EventItems.count(); ++i)
  {
    const EventItem& item = this->m_EventItems[i];
    if (item.boundingRect.contains(pos))
      return i;
  }
  return -1;
};

void TimeEventBarWidget::setEventItem(EventItem& item, int id, Event* e)
{
  item.ptr = e;
  item.id = id;
  if (e->context.compare("Right", Qt::CaseInsensitive) == 0)
  {
    item.contextId = 0;
    item.color = Qt::darkGreen;
  }
  else if (e->context.compare("Left", Qt::CaseInsensitive) == 0)
  {
    item.contextId = 1;
    item.color = Qt::darkRed;
  }
  else
  {
    item.contextId = 2;
    item.color = Qt::darkYellow;
  }
  if ((e->iconId >= 0) && (e->iconId < this->m_EventSymbols.count()))
    item.symbol = this->m_EventSymbols[e->iconId];
  else
    item.symbol = "?";
};

void TimeEventBarWidget::updateInternals()
{
  int xMax = this->width() - RightMargin;
  qreal timeLength = xMax - LeftMargin;
  int numFrames = this->m_ROILastFrame - this->m_ROIFirstFrame + 1;
  this->m_UnitStep = timeLength / (qreal)numFrames;
  
  int dTrig = 100;
  qreal uTrig = 0.4;
  if (this->m_UnitStep > 1.5)
  {
    dTrig = 1000;
    uTrig = 4.0;
  }
  
  this->m_TickDivider = 1;
  while (this->m_UnitStep * this->m_TickDivider < uTrig)
    this->m_TickDivider *= 2;
  if (numFrames < dTrig)
    this->m_TickDivider *= 10;
  else if (numFrames >= dTrig)
    this->m_TickDivider *= 100;
    
  int n = btkNumberOfDigits(this->m_ROIFirstFrame);
  int off = 0;
  if (n >= 3)
  {
    int dp = std::pow(10.0, n-2);
    off = this->m_ROIFirstFrame / dp * dp;
  }
  else
  {
    int dp = 10;
    off = this->m_ROIFirstFrame / dp * dp;
  }
  int numTicks = numFrames / this->m_TickDivider + 1;
  if (numTicks * this->m_TickDivider >= numFrames + this->m_ROIFirstFrame - off - 1)
    numTicks -= 1;
  numTicks += 2;
  this->m_Ticks.resize(numTicks); this->m_TicksLabel.resize(numTicks);
  
  if (this->m_TimeDisplay)
  {
    int ff = (this->m_ROIFirstFrame == 0) ? 1 : this->m_ROIFirstFrame; // special case if there is no loaded acquisition
    this->m_Ticks[0] = 0; this->m_TicksLabel[0] = QString::number((ff - 1) * this->m_TimeScale, 'g', 3);
    this->m_Ticks[numTicks - 1] = numFrames-1; this->m_TicksLabel[numTicks - 1] = QString::number((this->m_ROILastFrame - 1) * this->m_TimeScale, 'g', 3);
    for (int i = 1 ; i < numTicks - 1 ; ++i)
    {
      this->m_Ticks[i] = off + i * this->m_TickDivider - this->m_ROIFirstFrame + 1;
      this->m_TicksLabel[i] = QString::number(static_cast<double>(this->m_Ticks[i] + this->m_ROIFirstFrame - 1) * this->m_TimeScale, 'g', 3);
    }
  }
  else
  {
    this->m_Ticks[0] = 0; this->m_TicksLabel[0] = QString::number(this->m_ROIFirstFrame);
    this->m_Ticks[numTicks - 1] = numFrames-1; this->m_TicksLabel[numTicks - 1] = QString::number(this->m_ROILastFrame);
    for (int i = 1 ; i < numTicks - 1 ; ++i)
    {
      this->m_Ticks[i] = off + i * this->m_TickDivider - this->m_ROIFirstFrame;
      this->m_TicksLabel[i] = QString::number(this->m_Ticks[i] + this->m_ROIFirstFrame);
    }
  }
  
  int wTextFrame = this->m_Fm.width(m_TicksLabel[0]);
  int wTextFrameTemp = this->m_Fm.width(m_TicksLabel[1]);
  int xTemp = LeftMargin + this->m_Ticks[1] * this->m_UnitStep - wTextFrameTemp / 2;
  if (xTemp <= (LeftMargin + wTextFrame / 2 + 5))
    this->m_TicksLabel[1] = "";
  wTextFrameTemp = this->m_Fm.width(m_TicksLabel[numTicks-2]);
  wTextFrame = this->m_Fm.width(m_TicksLabel[numTicks-1]);
  xTemp = LeftMargin + this->m_Ticks[numTicks - 2] * this->m_UnitStep - wTextFrameTemp / 2;
  if (xTemp + wTextFrameTemp >= (xMax - wTextFrame / 2 - 15))
    this->m_TicksLabel[numTicks-2] = "";
    
  this->updateSliderPostion();
  this->updateLeftBoundPostion();
  this->updateRightBoundPostion();
  this->updateEventsPosition();
};

void TimeEventBarWidget::updateSliderPostion()
{
  int xSP = LeftMargin + (int)((this->m_SliderPos - this->m_ROIFirstFrame)*this->m_UnitStep);
  int h = this->height() - TopMargin - BottomMargin;
  // X Coordinates
  this->mp_PointsSlider[0].setX(xSP);
  this->mp_PointsSlider[1].setX(xSP+WSlider/2+1);
  this->mp_PointsSlider[2].setX(xSP+WSlider/2+1);
  this->mp_PointsSlider[3].setX(xSP-WSlider/2+2);
  // Y Coordinates
  this->mp_PointsSlider[0].setY(TopMargin);
  this->mp_PointsSlider[1].setY(TopMargin+2);
  this->mp_PointsSlider[2].setY(TopMargin+h+5+1);
  this->mp_PointsSlider[3].setY(TopMargin+h+5+1);
};

void TimeEventBarWidget::updateLeftBoundPostion()
{
  int xLB = LeftMargin + (int)((this->m_LeftBoundPos - this->m_ROIFirstFrame) * this->m_UnitStep) - 1 - 1;
  int h = this->height() - TopMargin - BottomMargin;
  // X Coordinates
  this->mp_PointsBoundLeft[0].setX(xLB);
  this->mp_PointsBoundLeft[1].setX(xLB);
  this->mp_PointsBoundLeft[2].setX(xLB-WBound);
  this->mp_PointsBoundLeft[3].setX(xLB-WBound);
  this->mp_PointsBoundLeft[4].setX(xLB-2);
  this->mp_PointsBoundLeft[5].setX(xLB-2);
  this->mp_PointsBoundLeft[6].setX(xLB);
  this->mp_PointsBoundLeft[7].setX(xLB+1);
  this->mp_PointsBoundLeft[8].setX(xLB+1);
  this->mp_PointsBoundLeft[9].setX(xLB-WBound+2);
  // Y Coordinates
  this->mp_PointsBoundLeft[0].setY(TopMargin);
  this->mp_PointsBoundLeft[1].setY(TopMargin+h+5);
  this->mp_PointsBoundLeft[2].setY(TopMargin+h+5);
  this->mp_PointsBoundLeft[3].setY(TopMargin+h-5);
  this->mp_PointsBoundLeft[4].setY(TopMargin+h-15);
  this->mp_PointsBoundLeft[5].setY(TopMargin+2);
  this->mp_PointsBoundLeft[6].setY(TopMargin);
  this->mp_PointsBoundLeft[7].setY(TopMargin+2);
  this->mp_PointsBoundLeft[8].setY(TopMargin+h+5+1);
  this->mp_PointsBoundLeft[9].setY(TopMargin+h+5+1);
};

void TimeEventBarWidget::updateRightBoundPostion()
{
  int xRB = LeftMargin + (int)((this->m_RightBoundPos - this->m_ROIFirstFrame + (this->m_ROIFirstFrame == this->m_ROILastFrame ? 1 : 0))*this->m_UnitStep) + 1;
  int h = this->height() - TopMargin - BottomMargin;
  // X Coordinates
  this->mp_PointsBoundRight[0].setX(xRB);
  this->mp_PointsBoundRight[1].setX(xRB);
  this->mp_PointsBoundRight[2].setX(xRB+WBound);
  this->mp_PointsBoundRight[3].setX(xRB+WBound);
  this->mp_PointsBoundRight[4].setX(xRB+2);
  this->mp_PointsBoundRight[5].setX(xRB+2);
  this->mp_PointsBoundRight[6].setX(xRB);
  this->mp_PointsBoundRight[7].setX(xRB+2+1);
  this->mp_PointsBoundRight[8].setX(xRB+2+1);
  this->mp_PointsBoundRight[9].setX(xRB+WBound+1);
  this->mp_PointsBoundRight[10].setX(xRB+WBound+1);
  this->mp_PointsBoundRight[11].setX(xRB+2);
  // Y Coordinates
  this->mp_PointsBoundRight[0].setY(TopMargin);
  this->mp_PointsBoundRight[1].setY(TopMargin+h+5);
  this->mp_PointsBoundRight[2].setY(TopMargin+h+5);
  this->mp_PointsBoundRight[3].setY(TopMargin+h-5);
  this->mp_PointsBoundRight[4].setY(TopMargin+h-15);
  this->mp_PointsBoundRight[5].setY(TopMargin+2);
  this->mp_PointsBoundRight[6].setY(TopMargin);
  this->mp_PointsBoundRight[7].setY(TopMargin+2+2);
  this->mp_PointsBoundRight[8].setY(TopMargin+h-15);
  this->mp_PointsBoundRight[9].setY(TopMargin+h-5);
  this->mp_PointsBoundRight[10].setY(TopMargin+h+5+1);
  this->mp_PointsBoundRight[11].setY(TopMargin+h+5+1);
};

void TimeEventBarWidget::updateEventsPosition()
{
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
    this->updateEventGeometry(i);
};

void TimeEventBarWidget::updateEventPosition(int id)
{
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
  {
    if (this->m_EventItems[i].id == id)
    {
      this->updateEventGeometry(i);
      break;
    }
  }
};

void TimeEventBarWidget::updateEventGeometry(int idx)
{
  qreal xPos = LeftMargin + ((this->m_EventItems[idx].ptr->frame - this->m_ROIFirstFrame)*this->m_UnitStep) - EventSymbolSize / 2.0;// + 0.5;
  qreal yPos = this->m_EventItems[idx].contextId * 7.0 * (this->height() - TopMargin - BottomMargin) / 22.0 + EventRelativePosY;
  this->m_EventItems[idx].boundingRect.setRect(xPos, yPos, EventSymbolSize, EventRelativePosY);
};

void TimeEventBarWidget::updateEventSelection(const QList<int>& selection)
{
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
  {
    if (selection.contains(this->m_EventItems[i].id))
    {
      if (this->m_EventItems[i].contextId == 0) // Right
        this->m_EventItems[i].color = Qt::green;
      else if (this->m_EventItems[i].contextId == 1) // Left
        this->m_EventItems[i].color = Qt::red;
      else // General
        this->m_EventItems[i].color = Qt::yellow;
    }
    else
    {
      if (this->m_EventItems[i].contextId == 0)
        this->m_EventItems[i].color = Qt::darkGreen;
      else if (this->m_EventItems[i].contextId == 1)
        this->m_EventItems[i].color = Qt::darkRed;
      else
        this->m_EventItems[i].color = Qt::darkYellow;
    }
  }
  this->update();
  emit eventSelectionChanged(selection);
};

QList<int> TimeEventBarWidget::extractSelectedEvents() const
{
  QList<int> selection;
  QRectF rubberRect;
  rubberRect = QRectF(this->mp_Rubber->x(), this->mp_Rubber->y(), this->mp_Rubber->rect().width(), this->mp_Rubber->rect().height());
  for (int i = 0 ; i <  this->m_EventItems.count() ; ++i)
  {
    if (rubberRect.intersects(this->m_EventItems[i].boundingRect))
    {
      selection.append(this->m_EventItems[i].id);
      // Point or rectangle selection?
      if (!this->mp_Rubber->isVisible())
        break;
    }
  }
  return selection;
};