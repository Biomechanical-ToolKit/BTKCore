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

#include "UpdateManager_p.h"

#include <QDialog>
#include <QSettings>

UpdateNewVersionDialog::UpdateNewVersionDialog(QWidget* parent)
: QDialog(parent), appNewVer()
{
  this->setupUi(this);
  this->iconFrame->hide();
#ifdef Q_OS_MAC
  this->layout()->setContentsMargins(12,12,12,12);
  QFont f = this->textBrowser->font(); // Bold by default
  f = this->textBrowser->font();
  f.setPointSize(12);
  this->textBrowser->setFont(f);
#endif
  
  connect(this->installButton, SIGNAL(clicked(bool)), this, SLOT(install()));
  connect(this->remindButton, SIGNAL(clicked(bool)), this, SLOT(remind()));
  connect(this->skipButton, SIGNAL(clicked(bool)), this, SLOT(skip()));
};

void UpdateNewVersionDialog::setApplicationIcon(const QPixmap& icon)
{
  if (!icon.isNull())
  {
    this->iconLabel->setPixmap(icon);
    this->iconFrame->show();
  }
  else
    this->iconFrame->hide();
};

void UpdateNewVersionDialog::install()
{
  QSettings settings;
  settings.setValue("Updater/skippedVersion", "");
  emit installSoftwareButtonClicked();
  this->accept();
};

void UpdateNewVersionDialog::remind()
{
  QSettings settings;
  settings.setValue("Updater/skippedVersion", "");
  this->reject();
};

void UpdateNewVersionDialog::skip()
{
  QSettings settings;
  settings.setValue("Updater/skippedVersion", this->appNewVer);
  this->reject();
};