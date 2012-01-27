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

#ifndef VideoWidget_h
#define VideoWidget_h

#include "TimeEventFunctors.h"

#include <QMediaPlayer>
#include <QVideoWidget>

class Acquisition;

typedef QMap<int, qint64> VideoDelays;

class VideoWidget : public QWidget
{
  Q_OBJECT

public:
  VideoWidget(QWidget* parent = 0);
  // ~VideoWidget(); // Implicit.

  Acquisition* acquisition() {return this->mp_Acquisition;};
  void setAcquisition(Acquisition* acq) {this->mp_Acquisition = acq;};
  VideoDelays* delays() {return this->mp_Delays;}
  void setDelays(VideoDelays* delays) {this->mp_Delays = delays;};
  void setCurrentFrameFunctor(btk::VTKCurrentFrameFunctor::Pointer functor) {this->mp_CurrentFrameFunctor = functor;};
  
  void render();
  void show(bool s);
  
  void copy(VideoWidget* source);

protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dragLeaveEvent(QDragLeaveEvent* event);
  void dropEvent(QDropEvent* event);
  void paintEvent(QPaintEvent* event);
  
private:
  Acquisition* mp_Acquisition;
  VideoDelays* mp_Delays;
  int m_VideoId;
  QMediaPlayer* mp_MediaPlayer;
  QVideoWidget* mp_VideoWidget;
  btk::VTKCurrentFrameFunctor::Pointer mp_CurrentFrameFunctor;
};

#endif // VideoWidget_h