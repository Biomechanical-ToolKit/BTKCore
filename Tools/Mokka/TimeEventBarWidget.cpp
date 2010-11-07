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

#include "TimeEventBarWidget.h"

#include <QtGui>
#include <QLinearGradient>
#include <QFontMetrics>

#include <cmath>
#define NUMLENGTH(num) ((num==0)?1:(int)log10(std::fabs((float)num))+1)

TimeEventBarWidget::TimeEventBarWidget(QWidget* parent)
: QFrame(parent), m_EventContexts(), m_Ticks(), m_TicksLabel(), 
  m_Fm(this->font()), m_EventItems(), m_EventSymbols(8), m_SelectedEvents(),
  m_RubberOrigin()
{
  this->m_FirstFrame = 0;
  this->m_LastFrame = 0;
  this->m_ROIFirstFrame = this->m_FirstFrame;
  this->m_ROILastFrame = this->m_LastFrame;
  this->m_EventContexts << "Right" << "Left" << "General";
  this->m_SliderPos = this->m_FirstFrame;
  this->m_LeftBoundPos = this->m_FirstFrame;
  this->m_RightBoundPos = this->m_LastFrame;
  this->m_UnitStep = 1.0;
  this->m_WBound = 10;
  this->m_WSlider = 10;
  this->m_TopMargin = 10;
  this->m_BottomMargin = 30;
  this->m_LeftMargin = 60;
  this->m_RightMargin = 30;
  this->m_TickDivider = 1;
#ifdef Q_OS_MAC
  QFont f = this->font();
  f.setPointSize(10); this->setFont(f);
  this->m_Fm = f;
#endif 
#ifdef Q_WS_WIN
  this->m_YEventPosRelative = 13.0;
  this->m_EventSymbolSize = 14.0;
  this->m_EventFont = QFont("Lucida Sans Unicode", 14);
#else
  this->m_YEventPosRelative = 16.0;
  this->m_EventSymbolSize = 12.0;
  this->m_EventFont = QFont("Arial", 12);
#endif
  this->m_MovBrushColor = QColor(0, 127, 255); this->m_MovBrushColor.setAlpha(128);
  this->m_BoundBrushColor = QColor(0, 127, 255); this->m_BoundBrushColor.setAlpha(64);
  this->m_MovPen = QColor(0, 127, 255);
  QColor movShadowColor = QColor(100, 100, 100); movShadowColor.setAlpha(128);
  this->m_MovShadowPen = movShadowColor;
  this->mp_Rubber = new QRubberBand(QRubberBand::Rectangle, this);
  
  ushort s;
  s = 0x25C8; this->m_EventSymbols[0].setUtf16(&s,1); // Foot strike: White diamond containing black small diamond
  s = 0x25B2; this->m_EventSymbols[1].setUtf16(&s,1); // Foot off:    Black up-pointing triangle
  s = 0x25C9; this->m_EventSymbols[2].setUtf16(&s,1); // Fisheye
  s = 0x25CF; this->m_EventSymbols[3].setUtf16(&s,1); // Black circle
  s = 0x25CE; this->m_EventSymbols[4].setUtf16(&s,1); // Bullseye
  s = 0x25D2; this->m_EventSymbols[5].setUtf16(&s,1); // Circle with lower half black
  s = 0x25D3; this->m_EventSymbols[6].setUtf16(&s,1); // Circle with upper half black
  s = 0x25BC; this->m_EventSymbols[7].setUtf16(&s,1); // Black down-pointing triangle
};

void TimeEventBarWidget::load(Acquisition* acq)
{
  this->m_SelectedEvents.clear();
  this->m_FirstFrame = acq->firstFrame();
  this->m_LastFrame = acq->lastFrame();
  this->m_ROIFirstFrame = this->m_FirstFrame;
  this->m_ROILastFrame = this->m_LastFrame;
  this->m_SliderPos = this->m_FirstFrame;
  this->m_LeftBoundPos = this->m_FirstFrame;
  this->m_RightBoundPos = this->m_LastFrame;
  
  this->m_EventItems.resize(acq->eventCount());
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
    this->setEventItem(this->m_EventItems[i], i, acq->eventAt(i));
  
  this->setSliderValue(this->m_FirstFrame);
  this->updateInternals();
  this->update();
};

void TimeEventBarWidget::reset()
{
  this->m_SelectedEvents.clear();
  this->m_EventItems.clear();
  this->m_FirstFrame = 0;
  this->m_LastFrame = 0;
  this->m_ROIFirstFrame = this->m_FirstFrame;
  this->m_ROILastFrame = this->m_LastFrame;
  this->m_SliderPos = this->m_FirstFrame;
  this->m_LeftBoundPos = this->m_FirstFrame;
  this->m_RightBoundPos = this->m_LastFrame;
  this->updateInternals();
  this->update();
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

void TimeEventBarWidget::insertEvent(int id, const Event* e)
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
  this->update();
};

void TimeEventBarWidget::paintEvent(QPaintEvent* event)
{

  QPainter painter(this);
  
  // --------------------- Timeline ---------------------
  int numTicks = this->m_Ticks.size();
  int numFrames = this->m_ROILastFrame - this->m_ROIFirstFrame + 1;
  QPen axisPen = QPen();
  painter.setPen(axisPen);
  qreal h = this->height() - this->m_TopMargin - this->m_BottomMargin;
  qreal hStep = h / 22.0;
  int yStep = 7;
  int dd = 1;
  int subTicksStart = 0;
  if (this->m_TickDivider >  10)
    dd = this->m_TickDivider / 10;
  if (dd != 1)
    subTicksStart = ((this->m_ROIFirstFrame / dd) + 1) * dd - this->m_ROIFirstFrame;
  int xMax = this->m_LeftMargin + (numFrames - (numFrames>1?1:0)) * this->m_UnitStep;
  for (int i = 0 ; i < 3 ; ++i)
  {
    // Context
    painter.drawText(0, i * yStep * hStep + 2 + this->m_TopMargin, this->m_LeftMargin - 20, yStep * hStep, Qt::AlignLeft | Qt::AlignBottom, this->m_EventContexts[i]);
    // Base (time axis)
    qreal tStep = (21 - i * yStep) * hStep + this->m_TopMargin;
    painter.drawLine(this->m_LeftMargin, tStep, xMax, tStep);
    // Extremities
    painter.drawLine(this->m_LeftMargin, tStep, this->m_LeftMargin, tStep - 9);
    painter.drawLine(xMax, tStep, xMax, tStep - 9);
    // Ticks
    for (int j = 1 ; j < numTicks-1 ; ++j)
      painter.drawLine(this->m_LeftMargin + this->m_Ticks[j] * this->m_UnitStep, tStep, this->m_LeftMargin + this->m_Ticks[j] * this->m_UnitStep, tStep - 6);
    // SubTicks
    int incST = subTicksStart;
    while (incST < numFrames)
    {
      painter.drawLine(QPoint(this->m_LeftMargin + incST * this->m_UnitStep, tStep), QPoint(this->m_LeftMargin + incST * this->m_UnitStep, tStep - 3));
      incST += dd; 
    }
  }
  // Text for this->m_Ticks
  for (int i = 0 ; i < this->m_TicksLabel.count() ; ++i)
  {
    int wTextFrame = this->m_Fm.width(this->m_TicksLabel[i]);
    painter.drawText(this->m_LeftMargin + this->m_Ticks[i] * this->m_UnitStep - wTextFrame / 2, h + this->m_TopMargin + 10, wTextFrame, 20, Qt::AlignHCenter | Qt::AlignTop, this->m_TicksLabel[i]);
  }
  
  // --------------------- Events ---------------------
  painter.setFont(this->m_EventFont);
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
  {
    if ((this->m_EventItems[i].frame > this->m_ROIFirstFrame) && (this->m_EventItems[i].frame < this->m_ROILastFrame))
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
    painter.setBrush(this->m_BoundBrushColor);
    painter.drawRect(this->m_LeftMargin, this->m_TopMargin, wLRB, h+5);
    painter.drawRect(xRB, this->m_TopMargin, wRRB+1, h+5);
    // - shadows
    painter.setPen(this->m_MovShadowPen);
    painter.drawPolyline(this->mp_PointsSlider+1, 3);
    painter.drawPolyline(this->mp_PointsBoundRight+7, 5);
    painter.drawPolyline(this->mp_PointsBoundLeft+7, 3);
    // - left & right bounds
    painter.setPen(this->m_MovPen);
    painter.setBrush(this->m_MovBrushColor);
    painter.drawPolygon(this->mp_PointsBoundRight, 7);
    painter.drawPolygon(this->mp_PointsBoundLeft, 7);
    // - slider
    painter.drawRect(this->mp_PointsSlider[0].x()-this->m_WSlider/2, this->mp_PointsSlider[0].y(), this->m_WSlider, h+5);
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
      QToolTip::showText(helpEvent->globalPos(), this->m_EventItems[index].toolTip);
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
  if ((event->buttons() & Qt::LeftButton) && (this->m_Mode != None))
  {
    int frame = (int)((qreal)(event->pos().x() - this->m_LeftMargin) / this->m_UnitStep) + this->m_ROIFirstFrame;
    if (this->m_Mode == MoveSlider)
    {
      if (frame < this->m_LeftBoundPos)
        frame = this->m_LeftBoundPos;
      else if (frame > this->m_RightBoundPos)
        frame = this->m_RightBoundPos;
      this->setSliderValue(frame);
      this->repaint();
    }
    else if (this->m_Mode == MoveLeftBound)
    {
      if (frame > this->m_SliderPos)
        frame = this->m_SliderPos;
      else if (frame < this->m_ROIFirstFrame)
        frame = this->m_ROIFirstFrame;
      else if (frame > this->m_ROILastFrame)
        frame = this->m_ROILastFrame;
      this->m_LeftBoundPos = frame;
      this->updateLeftBoundPostion();
      emit leftBoundPositionChanged(this->m_LeftBoundPos);
      this->repaint();
    }
    else if (this->m_Mode == MoveRightBound)
    {
      if (frame < this->m_SliderPos)
        frame = this->m_SliderPos;
      else if (frame < this->m_ROIFirstFrame)
        frame = this->m_ROIFirstFrame;
      else if (frame > this->m_ROILastFrame)
        frame = this->m_ROILastFrame;
      this->m_RightBoundPos = frame;
      this->updateRightBoundPostion();
      emit rightBoundPositionChanged(this->m_RightBoundPos);
      this->repaint();
    }
    else if (this->m_Mode == Rubber)
      this->mp_Rubber->setGeometry(QRect(this->m_RubberOrigin, event->pos()).normalized());
  }
};

void TimeEventBarWidget::mousePressEvent(QMouseEvent* event)
{
  if ((event->button() == Qt::LeftButton) && this->isEnabled())
  {
    // Slider
    QRect slider(this->mp_PointsSlider[0].x()-(this->m_WSlider+4)/2, this->mp_PointsSlider[0].y(), this->m_WSlider+4, this->height() - this->m_TopMargin - this->m_BottomMargin + 5);
    if (slider.contains(event->pos()))
    {
      this->m_Mode = MoveSlider;
      return;
    }
    // Left bound
    QRect leftBound(this->mp_PointsBoundLeft[1].x()-this->m_WBound, this->mp_PointsBoundLeft[1].y()-20, this->m_WBound, 20);
    if (leftBound.contains(event->pos()))
    {
      this->m_Mode = MoveLeftBound;
      emit boundSelected(this->m_LeftBoundPos);
      return;
    }
    // Right bound
    QRect rightBound(this->mp_PointsBoundRight[1].x(), this->mp_PointsBoundRight[1].y()-20, this->m_WBound, 20);
    if (rightBound.contains(event->pos()))
    {
      this->m_Mode = MoveRightBound;
      emit boundSelected(this->m_RightBoundPos);
      return;
    }
    // Rubber
    this->m_Mode = Rubber;
    this->m_RubberOrigin = event->pos();
    this->mp_Rubber->setGeometry(QRect(this->m_RubberOrigin, QSize(1,1)));
    this->mp_Rubber->show();
    if (!(event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier)))
    {
      this->m_SelectedEvents.clear();
      this->updateEventSelection();
    }
  }
  else
    this->m_Mode = None;
};

void TimeEventBarWidget::mouseReleaseEvent(QMouseEvent* event)
{
  Q_UNUSED(event);
  
  if (this->m_Mode == Rubber)
  {
    QRectF rubberRect = QRectF(this->mp_Rubber->x(), this->mp_Rubber->y(), this->mp_Rubber->rect().width(), this->mp_Rubber->rect().height());
    for (int i = 0 ; i <  this->m_EventItems.count() ; ++i)
    {
      if (rubberRect.intersects(this->m_EventItems[i].boundingRect))
        this->m_SelectedEvents.push_back(this->m_EventItems[i].id);
    }
    this->updateEventSelection();
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

void TimeEventBarWidget::setEventItem(EventItem& item, int id, const Event* e)
{
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
  int iconId = e->iconId - 1; // Because Vicon use id 1 and 2 for the foot strike and foot off respectively
  if ((iconId >= 0) && (iconId < this->m_EventSymbols.count()))
    item.symbol = this->m_EventSymbols[iconId];
  else
    item.symbol = "?";
  item.frame = e->frame;
  item.toolTip = QString("<b>Event:</b> %1 %2<br/><b>Subject:</b> %3<br/><b>Frame:</b> %4<br/><b>Time:</b> %5 second(s)").arg(e->context, e->label, e->subject).arg(e->frame).arg(e->time);
};

void TimeEventBarWidget::updateInternals()
{
  int xMax = this->width() - this->m_RightMargin;
  qreal timeLength = xMax - this->m_LeftMargin;
  int numFrames = this->m_ROILastFrame - this->m_ROIFirstFrame + 1;
  this->m_UnitStep = timeLength / (qreal)numFrames;
  
  int dTrig = 100;
  qreal uTrig = 0.4;
  if (this->m_UnitStep > 1.5)
  {
    dTrig = 1000;
    uTrig = 4;
  }
  
  this->m_TickDivider = 1;
  while (this->m_UnitStep * this->m_TickDivider < uTrig)
    this->m_TickDivider *= 2;
  if (numFrames < dTrig)
    this->m_TickDivider *= 10;
  else if (numFrames >= dTrig)
    this->m_TickDivider *= 100;
    
  int n = NUMLENGTH(this->m_ROIFirstFrame);
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
  this->m_Ticks[0] = 0; this->m_TicksLabel[0] = QString::number(this->m_ROIFirstFrame);
  this->m_Ticks[numTicks - 1] = numFrames-1; this->m_TicksLabel[numTicks - 1] = QString::number(this->m_ROILastFrame);
  for (int i = 1 ; i < numTicks - 1 ; ++i)
  {
    this->m_Ticks[i] = off + i * this->m_TickDivider - this->m_ROIFirstFrame;
    this->m_TicksLabel[i] = QString::number(this->m_Ticks[i] + this->m_ROIFirstFrame);
  }
  
  int wTextFrame = this->m_Fm.width(m_TicksLabel[0]);
  int wTextFrameTemp = this->m_Fm.width(m_TicksLabel[1]);
  int xTemp = this->m_LeftMargin + this->m_Ticks[1] * this->m_UnitStep - wTextFrameTemp / 2;
  if (xTemp <= (this->m_LeftMargin + wTextFrame / 2 + 5))
    this->m_TicksLabel[1] = "";
  wTextFrameTemp = this->m_Fm.width(m_TicksLabel[numTicks-2]);
  wTextFrame = this->m_Fm.width(m_TicksLabel[numTicks-1]);
  xTemp = this->m_LeftMargin + this->m_Ticks[numTicks - 2] * this->m_UnitStep - wTextFrameTemp / 2;
  if (xTemp + wTextFrameTemp >= (xMax - wTextFrame / 2 - 5))
    this->m_TicksLabel[numTicks-2] = "";
    
  this->updateSliderPostion();
  this->updateLeftBoundPostion();
  this->updateRightBoundPostion();
  this->updateEventsPosition();
};

void TimeEventBarWidget::updateSliderPostion()
{
  int xSP = this->m_LeftMargin + (int)((this->m_SliderPos - this->m_ROIFirstFrame)*this->m_UnitStep);
  int h = this->height() - this->m_TopMargin - this->m_BottomMargin;
  // X Coordinates
  this->mp_PointsSlider[0].setX(xSP);
  this->mp_PointsSlider[1].setX(xSP+this->m_WSlider/2+1);
  this->mp_PointsSlider[2].setX(xSP+this->m_WSlider/2+1);
  this->mp_PointsSlider[3].setX(xSP-this->m_WSlider/2+2);
  // Y Coordinates
  this->mp_PointsSlider[0].setY(this->m_TopMargin);
  this->mp_PointsSlider[1].setY(this->m_TopMargin+2);
  this->mp_PointsSlider[2].setY(this->m_TopMargin+h+5+1);
  this->mp_PointsSlider[3].setY(this->m_TopMargin+h+5+1);
};

void TimeEventBarWidget::updateLeftBoundPostion()
{
  int xLB = this->m_LeftMargin + (int)((this->m_LeftBoundPos - this->m_ROIFirstFrame) * this->m_UnitStep) - 1 - 1;
  int h = this->height() - this->m_TopMargin - this->m_BottomMargin;
  // X Coordinates
  this->mp_PointsBoundLeft[0].setX(xLB);
  this->mp_PointsBoundLeft[1].setX(xLB);
  this->mp_PointsBoundLeft[2].setX(xLB-this->m_WBound);
  this->mp_PointsBoundLeft[3].setX(xLB-this->m_WBound);
  this->mp_PointsBoundLeft[4].setX(xLB-2);
  this->mp_PointsBoundLeft[5].setX(xLB-2);
  this->mp_PointsBoundLeft[6].setX(xLB);
  this->mp_PointsBoundLeft[7].setX(xLB+1);
  this->mp_PointsBoundLeft[8].setX(xLB+1);
  this->mp_PointsBoundLeft[9].setX(xLB-this->m_WBound+2);
  // Y Coordinates
  this->mp_PointsBoundLeft[0].setY(this->m_TopMargin);
  this->mp_PointsBoundLeft[1].setY(this->m_TopMargin+h+5);
  this->mp_PointsBoundLeft[2].setY(this->m_TopMargin+h+5);
  this->mp_PointsBoundLeft[3].setY(this->m_TopMargin+h-5);
  this->mp_PointsBoundLeft[4].setY(this->m_TopMargin+h-15);
  this->mp_PointsBoundLeft[5].setY(this->m_TopMargin+2);
  this->mp_PointsBoundLeft[6].setY(this->m_TopMargin);
  this->mp_PointsBoundLeft[7].setY(this->m_TopMargin+2);
  this->mp_PointsBoundLeft[8].setY(this->m_TopMargin+h+5+1);
  this->mp_PointsBoundLeft[9].setY(this->m_TopMargin+h+5+1);
};

void TimeEventBarWidget::updateRightBoundPostion()
{
  int xRB = this->m_LeftMargin + (int)((this->m_RightBoundPos - this->m_ROIFirstFrame + (this->m_ROIFirstFrame == this->m_ROILastFrame ? 1 : 0))*this->m_UnitStep) + 1;
  int h = this->height() - this->m_TopMargin - this->m_BottomMargin;
  // X Coordinates
  this->mp_PointsBoundRight[0].setX(xRB);
  this->mp_PointsBoundRight[1].setX(xRB);
  this->mp_PointsBoundRight[2].setX(xRB+this->m_WBound);
  this->mp_PointsBoundRight[3].setX(xRB+this->m_WBound);
  this->mp_PointsBoundRight[4].setX(xRB+2);
  this->mp_PointsBoundRight[5].setX(xRB+2);
  this->mp_PointsBoundRight[6].setX(xRB);
  this->mp_PointsBoundRight[7].setX(xRB+2+1);
  this->mp_PointsBoundRight[8].setX(xRB+2+1);
  this->mp_PointsBoundRight[9].setX(xRB+this->m_WBound+1);
  this->mp_PointsBoundRight[10].setX(xRB+this->m_WBound+1);
  this->mp_PointsBoundRight[11].setX(xRB+2);
  // Y Coordinates
  this->mp_PointsBoundRight[0].setY(this->m_TopMargin);
  this->mp_PointsBoundRight[1].setY(this->m_TopMargin+h+5);
  this->mp_PointsBoundRight[2].setY(this->m_TopMargin+h+5);
  this->mp_PointsBoundRight[3].setY(this->m_TopMargin+h-5);
  this->mp_PointsBoundRight[4].setY(this->m_TopMargin+h-15);
  this->mp_PointsBoundRight[5].setY(this->m_TopMargin+2);
  this->mp_PointsBoundRight[6].setY(this->m_TopMargin);
  this->mp_PointsBoundRight[7].setY(this->m_TopMargin+2+2);
  this->mp_PointsBoundRight[8].setY(this->m_TopMargin+h-15);
  this->mp_PointsBoundRight[9].setY(this->m_TopMargin+h-5);
  this->mp_PointsBoundRight[10].setY(this->m_TopMargin+h+5+1);
  this->mp_PointsBoundRight[11].setY(this->m_TopMargin+h+5+1);
};

void TimeEventBarWidget::updateEventsPosition()
{
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
    this->updateEventPos(i);
};

void TimeEventBarWidget::updateEventPosition(int id)
{
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
  {
    if (this->m_EventItems[i].id == id)
    {
      this->updateEventPos(i);
      break;
    }
  }
};

void TimeEventBarWidget::updateEventSelection()
{
  for (int i = 0 ; i < this->m_EventItems.count() ; ++i)
  {
    if (this->m_SelectedEvents.contains(this->m_EventItems[i].id))
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
  emit eventSelectionChanged(this->m_SelectedEvents);
};