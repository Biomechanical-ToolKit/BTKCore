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

#ifndef Preferences_h
#define Preferences_h

#if !defined(Q_OS_MAC)

  #include "ui_Preferences.h"

  #include <QDialog>
  #include <QMap>

  class Preferences : public QDialog, public Ui::Preferences
  {
    Q_OBJECT

  public:
    enum {DefaultConfigurationUse = 0, DefaultConfigurationPath, EventEditorWhenInserting, DefaultGroundOrientation, 
          DefaultMarkerColor, DefaultMarkerRadius, DefaultTrajectoryLength, ForcePlatformAxesDisplay,
          ForcePlatformIndexDisplay, DefaultForcePlateColor, DefaultForceVectorColor, AutomaticCheckUpdateUse};
  
    Preferences(QWidget* parent = 0);
    // ~Preferences(); // Implicit
  
    void setPreference(int preference, QVariant data) {this->m_Data[preference] = data;};
  
    void saveSettings();
  
    QString lastDirectory;
  
  public slots:
    void setDefaultConfiguration();
    void setDefaultMarkerColor();
    void setDefaultForcePlateColor();
    void setDefaultForceVectorColor();
  
  signals:
    void useDefaultConfigurationStateChanged(bool isUsed);
    void defaultConfigurationPathChanged(const QString& path);
    void defaultGroundOrientationChanged(int index);
    void useEventEditorWhenInsertingStateChanged(bool isChecked);
    void defaultMarkerColorChanged(const QColor& color);
    void defaultMarkerRadiusChanged(double radius);
    void defaultMarkerTrajectoryLengthChanged(int index);
    void showForcePlatformAxesChanged(int index);
    void showForcePlatformIndexChanged(int index);
    void defaultForcePlateColorChanged(const QColor& color);
    void defaultForceVectorColorChanged(const QColor& color);
    void automaticCheckUpdateStateChanged(bool isChecked);
  
  private:
    QMap<int, QVariant> m_Data;
  };

  inline void colorizeButton(QPushButton* button, const QColor& color)
  {
    button->setStyleSheet("QPushButton {border: 1px solid darkgray;background: " + color.name() + ";} QPushButton:pressed {border: 1px solid rgb(100,100,100);}");
    button->setProperty("backgroundColor", color);
  };
#else
  #include "Preferences_mac.h"
#endif

#endif // Preferences_h