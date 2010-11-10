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

#include "FileInfoDockWidget.h"
#include "Acquisition.h"

FileInfoDockWidget::FileInfoDockWidget(QWidget* parent)
: QDockWidget(parent)
{
  this->mp_Acquisition = 0;
  this->setupUi(this);
};

void FileInfoDockWidget::setAcquisition(Acquisition* acq)
{
  if (this->mp_Acquisition)
    disconnect(this->mp_Acquisition, 0, this, 0);
  this->mp_Acquisition = acq;
  connect(this->mp_Acquisition, SIGNAL(informationsChanged(QVector<QString>)), this, SLOT(fill(QVector<QString>)));
};

void FileInfoDockWidget::fill(const QVector<QString>& infos)
{
  Q_ASSERT(infos.count() >= 16);
  this->layout()->setSizeConstraint(QLayout::SetDefaultConstraint);
  this->fileNameValue->setText(infos[0]);
  this->documentTypeValue->setText(infos[1]);
  this->fileSizeValue->setText(infos[2]);
  this->creationDateValue->setText(infos[3]);
  this->modificationDateValue->setText(infos[4]);
  this->fileFormatValue->setText(infos[5]);
  this->byteOrderValue->setText(infos[6]);
  this->storageFormatValue->setText(infos[7]);
  this->firstFrameValue->setText(infos[8]);
  this->lastFrameValue->setText(infos[9]);
  this->frameNumberValue->setText(infos[10]);
  this->pointNumberValue->setText(infos[11]);
  this->pointFrequencyValue->setText(infos[12]);
  this->analogNumberValue->setText(infos[13]);
  this->analogFrequencyValue->setText(infos[14]);
  this->eventNumberValue->setText(infos[15]);
  this->resize(this->width(), this->eventNumberValue->y() + this->eventNumberValue->height() + this->layout()->contentsMargins().bottom());
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);
};

void FileInfoDockWidget::reset()
{
  this->layout()->setSizeConstraint(QLayout::SetDefaultConstraint);
  this->fileNameValue->setText("                            ");
  this->documentTypeValue->clear();
  this->fileSizeValue->clear();
  this->creationDateValue->clear();
  this->modificationDateValue->clear();
  this->fileFormatValue->clear();
  this->byteOrderValue->clear();
  this->storageFormatValue->clear();
  this->firstFrameValue->clear();
  this->lastFrameValue->clear();
  this->frameNumberValue->clear();
  this->pointNumberValue->clear();
  this->pointFrequencyValue->clear();
  this->analogNumberValue->clear();
  this->analogFrequencyValue->clear();
  this->eventNumberValue->clear();
  this->resize(this->width(), this->eventNumberValue->y() + this->eventNumberValue->height() + this->layout()->contentsMargins().bottom());
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);
};

