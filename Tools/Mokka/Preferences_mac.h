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

#ifndef Preferences_mac_h
#define Preferences_mac_h

#include "ui_Preferences_mac.h"

#include <QToolButton>
#include <QMainWindow>

class QFocusEvent;

class Preferences : public QMainWindow, public Ui::Preferences
{
  Q_OBJECT

public:
  Preferences(QMainWindow* parent = 0);
  // ~Preferences(); // Implicit

  void setUserLayouts(QList<QVariant>* layouts) {this->layoutTable->setUserLayouts(layouts);};
  void refreshUserLayouts() {this->layoutTable->refresh();};

  QString lastDirectory;

public slots:
  void setCurrentIndex(int index);
  void showGeneralPreferences();
  void showVisualisationPreferences();
  void showLayoutsPreferences();
  void showAdvancedPreferences();

  void setDefaultConfiguration();
  void useDefaultConfiguration(bool isUsed);
  void setDefaultConfigurationPath();
  void useEventEditorWhenInserting(bool isUsed);
  void setDefaultOrientation(int index);
  void setDefaultSegmentColor();
  void setDefaultMarkerColor();
  void setDefaultMarkerRadius(double radius);
  void setDefaultTrajectoryLength(int index);
  void showForcePlatformAxes(int index);
  void showForcePlatformIndex(int index);
  void setDefaultForcePlateColor();
  void setDefaultForceVectorColor();
  void setAutomaticCheckUpdate(bool isChecked);

signals:
  void useDefaultConfigurationStateChanged(bool isUsed);
  void defaultConfigurationPathChanged(const QString& path);
  void defaultGroundOrientationChanged(int index);
  void useEventEditorWhenInsertingStateChanged(bool isChecked);
  void defaultSegmentColorChanged(const QColor& color);
  void defaultMarkerColorChanged(const QColor& color);
  void defaultMarkerRadiusChanged(double radius);
  void defaultMarkerTrajectoryLengthChanged(int index);
  void showForcePlatformAxesChanged(int index);
  void showForcePlatformIndexChanged(int index);
  void defaultForcePlateColorChanged(const QColor& color);
  void defaultForceVectorColorChanged(const QColor& color);
  void automaticCheckUpdateStateChanged(bool isChecked);
  void userLayoutRemoved(int index);
  void userLayoutLabelChanged(int index, const QString& label);
  void userLayoutDropped(int newRow, int oldRow);

protected:
  bool eventFilter(QObject* obj, QEvent* event);
  void focusInEvent(QFocusEvent* event);
  void focusOutEvent(QFocusEvent* event);

private:
  enum {General = 0, Visualisation, Layouts, Advanced};

  void stylizeFocusedCurrentAction();
  void unstylizeFocusedCurrentAction();
  void stylizeUnfocusedCurrentAction();
  void unstylizeAction(QToolButton* tb);

  int m_CurrentIndex;
  QList<QToolButton*> m_Buttons;
};

inline void Preferences::stylizeFocusedCurrentAction()
{
  QToolButton* tb = this->m_Buttons[this->m_CurrentIndex];
  tb->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:0.5 rgba(139, 139, 139, 255), stop:1 rgba(0, 0, 0, 0)); \
                     border: 0px; \
                     border-left: 1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:0.5 rgba(89, 89, 89, 255), stop:1 rgba(0, 0, 0, 0)); \
                     border-right: 1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:0.5 rgba(89, 89, 89, 255), stop:1 rgba(0, 0, 0, 0));");
};

inline void Preferences::unstylizeFocusedCurrentAction()
{
  if (this->m_CurrentIndex == -1)
    return;
  this->unstylizeAction(this->m_Buttons[this->m_CurrentIndex]);
};

inline void Preferences::stylizeUnfocusedCurrentAction()
{
  QToolButton* tb = this->m_Buttons[this->m_CurrentIndex];
  tb->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:0.5 rgba(193, 193, 193, 255), stop:1 rgba(0, 0, 0, 0)); \
                     border: 0px; \
                     border-left: 1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:0.5 rgba(170, 170, 170, 255), stop:1 rgba(0, 0, 0, 0)); \
                     border-right: 1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 0, 0, 0), stop:0.5 rgba(170, 170, 170, 255), stop:1 rgba(0, 0, 0, 0));");
};

inline void Preferences::unstylizeAction(QToolButton* tb)
{
  tb->setStyleSheet("background-color: rgba(0, 0, 0, 0); \
                     border: 0px; \
                     border-left: 1px solid rgba(0, 0, 0, 0); \
                     border-right: 1px solid rgba(0, 0, 0, 0);");
};

inline void colorizeButton(QPushButton* button, const QColor& color)
{
  button->setStyleSheet("QPushButton {border: 1px solid darkgray;background: " + color.name() + ";} QPushButton:pressed {border: 1px solid rgb(100,100,100);}");
  button->setProperty("backgroundColor", color);
};

#endif // Preferences_mac_h