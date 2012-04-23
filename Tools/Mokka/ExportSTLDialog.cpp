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
 
#include "ExportSTLDialog.h"

#include <QFileDialog>

ExportSTLDialog::ExportSTLDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
  this->resize(this->width(), this->height()-1); // FIXME: Only the way to remove the size grip under MacOS X?
#endif
  
  connect(this->pathButton, SIGNAL(clicked()), this, SLOT(setExportPath()));
  connect(this->pathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(validExportInfo()));
  connect(this->filePrefixLineEdit, SIGNAL(textChanged(QString)), this, SLOT(validExportInfo()));
  
  this->validExportInfo();
};
  
void ExportSTLDialog::setExportPath()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"), this->pathLineEdit->text());
  if (!dir.isEmpty())
    this->pathLineEdit->setText(dir);
};

void ExportSTLDialog::validExportInfo()
{
  bool valid = false;
  if (!this->pathLineEdit->text().isEmpty() && !this->filePrefixLineEdit->text().isEmpty() && (this->segmentListWidget->currentRow() != -1))
    valid = true;
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
};