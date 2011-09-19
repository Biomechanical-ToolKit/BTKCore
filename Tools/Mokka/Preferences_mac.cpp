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
 
#include "Preferences_mac.h"

#include <QMenuBar>
#include <QFocusEvent>
#include <QToolTip>
#include <QFileDialog>
#include <QColorDialog>

Preferences::Preferences(QMainWindow* parent)
: QMainWindow(parent, Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint), lastDirectory(".")
{
  this->m_CurrentIndex = -1;
  
  this->setupUi(this);
  this->setFocusPolicy(Qt::StrongFocus);

  this->m_Buttons = this->findChildren<QToolButton*>();
  // Remove qt_toolbar_ext_button
  for (QList<QToolButton*>::iterator it = this->m_Buttons.begin() ; it != this->m_Buttons.end() ; ++it)
  {
    if ((*it)->objectName() == "qt_toolbar_ext_button")
    {
      this->m_Buttons.erase(it);
      break;
    }
  }
  // Install event filter on the toolbuttons to disable tooltips
  // And add the defaut style
  for (QList<QToolButton*>::iterator it = this->m_Buttons.begin() ; it != this->m_Buttons.end() ; ++it)
  {
    (*it)->installEventFilter(this);
    this->unstylizeAction(*it);
  }
  // Install event filter for some possible glitch with the toolbutton style (focusout event from children not took into account)
  defaultConfigurationLineEdit->installEventFilter(this);
  defaultConfigurationButton->installEventFilter(this);
  defaultPlaneOrientationComboBox->installEventFilter(this);
  defaultMarkerRadiusSpinBox->installEventFilter(this);
  defaultMarkerTrajectoryLengthComboBox->installEventFilter(this);
  showForcePlatformAxesComboBox->installEventFilter(this);
  showForcePlatformIndexComboBox->installEventFilter(this);
  
  connect(this->actionGeneral, SIGNAL(triggered()), this, SLOT(showGeneralPreferences()));
  connect(this->actionVisualisation, SIGNAL(triggered()), this, SLOT(showVisualisationPreferences()));
  connect(this->actionLayouts, SIGNAL(triggered()), this, SLOT(showLayoutsPreferences()));
  connect(this->actionAdvanced, SIGNAL(triggered()), this, SLOT(showAdvancedPreferences()));
  connect(this->defaultConfigurationButton, SIGNAL(clicked()), this, SLOT(setDefaultConfiguration()));
  
  connect(this->defaultConfigurationCheckBox, SIGNAL(toggled(bool)), this, SLOT(useDefaultConfiguration(bool)));
  connect(this->defaultConfigurationLineEdit, SIGNAL(editingFinished()), this, SLOT(setDefaultConfigurationPath()));
  connect(this->openEventEditorCheckBox, SIGNAL(toggled(bool)), this, SLOT(useEventEditorWhenInserting(bool)));
  connect(this->defaultPlaneOrientationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDefaultOrientation(int)));
  connect(this->defaultSegmentColorButton, SIGNAL(clicked()), this, SLOT(setDefaultSegmentColor()));
  connect(this->defaultMarkerColorButton, SIGNAL(clicked()), this, SLOT(setDefaultMarkerColor()));
  connect(this->defaultMarkerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDefaultMarkerRadius(double)));
  connect(this->defaultMarkerTrajectoryLengthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDefaultTrajectoryLength(int)));
  connect(this->showForcePlatformAxesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showForcePlatformAxes(int)));
  connect(this->showForcePlatformIndexComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showForcePlatformIndex(int)));
  connect(this->defaultForcePlateColorButton, SIGNAL(clicked()), this, SLOT(setDefaultForcePlateColor()));
  connect(this->defaultForceVectorColorButton, SIGNAL(clicked()), this, SLOT(setDefaultForceVectorColor()));
  connect(this->automaticCheckUpdateCheckBox, SIGNAL(toggled(bool)), this, SLOT(setAutomaticCheckUpdate(bool)));
  connect(this->layoutTable, SIGNAL(userLayoutRemoved(int)), this, SIGNAL(userLayoutRemoved(int)));
  connect(this->layoutTable, SIGNAL(userLayoutLabelChanged(int, QString)), this, SIGNAL(userLayoutLabelChanged(int, QString)));
  connect(this->layoutTable, SIGNAL(userLayoutDropped(int, int)), this, SIGNAL(userLayoutDropped(int, int)));
  
  this->setCurrentIndex(General);
  this->setFocus();
};

void Preferences::setCurrentIndex(int index)
{
  this->unstylizeFocusedCurrentAction();
  if ((index < 0) || index >= this->m_Buttons.count())
  {
    qDebug("Cannot select an invalid index. Reset to index 0");
    this->m_CurrentIndex = 0;
  }
  else
    this->m_CurrentIndex = index;
  this->stylizeFocusedCurrentAction();
  this->stackedWidget->setCurrentIndex(index);
};

void Preferences::showGeneralPreferences()
{
  this->setCurrentIndex(General);
  this->setWindowTitle(tr("General"));
};

void Preferences::showVisualisationPreferences()
{
  this->setCurrentIndex(Visualisation);
  this->setWindowTitle(tr("Visualisation"));
};

void Preferences::showLayoutsPreferences()
{
  this->setCurrentIndex(Layouts);
  this->setWindowTitle(tr("Layouts"));
};

void Preferences::showAdvancedPreferences()
{
  this->setCurrentIndex(Advanced);
  this->setWindowTitle(tr("Advanced"));
};

void Preferences::setDefaultConfiguration()
{
  QString filename = this->defaultConfigurationLineEdit->text();
  filename = QFileDialog::getOpenFileName(this,
    tr("Open Model Visual Configuration"),
    filename.isEmpty() ? this->lastDirectory : filename,
    tr("Model Visual Configuration Files (*.mvc)"));
  if (!filename.isEmpty())
  {
    this->defaultConfigurationLineEdit->setText(filename);
    emit defaultConfigurationPathChanged(filename);
  }
};

void Preferences::useDefaultConfiguration(bool isUsed)
{
  emit useDefaultConfigurationStateChanged(isUsed);
};

void Preferences::setDefaultConfigurationPath()
{
  emit defaultConfigurationPathChanged(this->defaultConfigurationLineEdit->text());
};

void Preferences::useEventEditorWhenInserting(bool isUsed)
{
  emit useEventEditorWhenInsertingStateChanged(isUsed);
};

void Preferences::setDefaultOrientation(int index)
{
  emit defaultGroundOrientationChanged(index);
};

void Preferences::setDefaultSegmentColor()
{
  QColor color = QColorDialog::getColor(this->defaultSegmentColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
  {
    colorizeButton(this->defaultSegmentColorButton, color);
    emit defaultSegmentColorChanged(color);
  }
};

void Preferences::setDefaultMarkerColor()
{
  QColor color = QColorDialog::getColor(this->defaultMarkerColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
  {
    colorizeButton(this->defaultMarkerColorButton, color);
    emit defaultMarkerColorChanged(color);
  }
};

void Preferences::setDefaultMarkerRadius(double radius)
{
  emit defaultMarkerRadiusChanged(radius);
};

void Preferences::setDefaultTrajectoryLength(int index)
{
  emit defaultMarkerTrajectoryLengthChanged(index);
};

void Preferences::showForcePlatformAxes(int index)
{
  emit showForcePlatformAxesChanged(index);
};

void Preferences::showForcePlatformIndex(int index)
{
  emit showForcePlatformIndexChanged(index);
};

void Preferences::setDefaultForcePlateColor()
{
  QColor color = QColorDialog::getColor(this->defaultForcePlateColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
  {
    colorizeButton(this->defaultForcePlateColorButton, color);
    emit defaultForcePlateColorChanged(color);
  }
};

void Preferences::setDefaultForceVectorColor()
{
  QColor color = QColorDialog::getColor(this->defaultForceVectorColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
  {
    colorizeButton(this->defaultForceVectorColorButton, color);
    emit defaultForceVectorColorChanged(color);
  }
};

void Preferences::setAutomaticCheckUpdate(bool isChecked)
{
  emit automaticCheckUpdateStateChanged(isChecked);
};

bool Preferences::eventFilter(QObject* obj, QEvent* event)
{
  Q_UNUSED(obj)
  if (event->type() == QEvent::ToolTip)
  {
    QToolTip::hideText();
    event->ignore();
    return true;
  }
  else if (event->type() == QEvent::FocusOut)
  {
    if (!this->isAncestorOf(QApplication::focusWidget())
        && (static_cast<QFocusEvent*>(event)->reason() == Qt::ActiveWindowFocusReason))
      this->stylizeUnfocusedCurrentAction();
  }
  return false;
};

void Preferences::focusInEvent(QFocusEvent* event)
{
  this->stylizeFocusedCurrentAction();
  this->QMainWindow::focusInEvent(event);
};

void Preferences::focusOutEvent(QFocusEvent* event)
{
  if (!this->isAncestorOf(QApplication::focusWidget()))
    this->stylizeUnfocusedCurrentAction();
  this->QMainWindow::focusOutEvent(event);
};