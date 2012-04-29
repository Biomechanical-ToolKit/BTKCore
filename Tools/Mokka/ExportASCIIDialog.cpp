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
 
#include "ExportASCIIDialog.h"

#include <QPushButton>
#include <QFileDialog>

ExportASCIIDialog::ExportASCIIDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->layout()->setContentsMargins(12,12,12,12);
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
  this->resize(this->width(), this->height()-1); // FIXME: Only the way to remove the size grip under MacOS X?

  this->pointsTreeWidget->setAttribute(Qt::WA_MacShowFocusRect, 0);
  this->forcePlatesTreeWidget->setAttribute(Qt::WA_MacShowFocusRect, 0);
  this->analogsTreeWidget->setAttribute(Qt::WA_MacShowFocusRect, 0);
  
  QFont f = this->font();
  f.setPointSize(10);
  this->pointsTreeWidget->setFont(f);
  this->forcePlatesTreeWidget->setFont(f);
  this->analogsTreeWidget->setFont(f);
#endif

  connect(this->otherRadioButton, SIGNAL(clicked(bool)), this, SLOT(uncheckOtherDelimitersButton(bool)));
  connect(this->tabRadioButton, SIGNAL(clicked(bool)), this, SLOT(uncheckOtherDelimiterButton(bool)));
  connect(this->semicolonRadioButton, SIGNAL(clicked(bool)), this, SLOT(uncheckOtherDelimiterButton(bool)));
  connect(this->commaRadioButton, SIGNAL(clicked(bool)), this, SLOT(uncheckOtherDelimiterButton(bool)));
  connect(this->spaceRadioButton, SIGNAL(clicked(bool)), this, SLOT(uncheckOtherDelimiterButton(bool)));
  connect(this->otherLineEdit, SIGNAL(textChanged(QString)), this, SLOT(validExportInfo()));
  
  connect(this->pointsTreeWidget, SIGNAL(expanded(QModelIndex)), this, SLOT(updatePointsTreeSectionWidth()));
  connect(this->pointsTreeWidget, SIGNAL(collapsed(QModelIndex)), this, SLOT(updatePointsTreeSectionWidth()));
  connect(this->forcePlatesTreeWidget, SIGNAL(expanded(QModelIndex)), this, SLOT(updateForcePlatesTreeSectionWidth()));
  connect(this->forcePlatesTreeWidget, SIGNAL(collapsed(QModelIndex)), this, SLOT(updateForcePlatesTreeSectionWidth()));
  connect(this->analogsTreeWidget, SIGNAL(expanded(QModelIndex)), this, SLOT(updateAnalogsTreeSectionWidth()));
  connect(this->analogsTreeWidget, SIGNAL(collapsed(QModelIndex)), this, SLOT(updateAnalogsTreeSectionWidth()));
  
  this->validExportInfo();
};

void ExportASCIIDialog::fillPoints(QTreeWidgetItem* markersRoot, QTreeWidgetItem* modelOutputsRoot)
{
  this->pointsTreeWidget->blockSignals(true);
  QTreeWidgetItem* points = new QTreeWidgetItem(QStringList() << "Points");
  QFont f = points->font(0); f.setBold(true); points->setFont(0,f);
  points->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
  points->setCheckState(0, Qt::Checked);
  this->pointsTreeWidget->addTopLevelItem(points);
  points->setExpanded(true);
  // Markers
  if (!markersRoot->isHidden())
  {
    QTreeWidgetItem* markers = markersRoot->clone();
    markers->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
    markers->setCheckState(0, Qt::Checked);
    points->addChild(markers);
    markers->setExpanded(true);
    for (int i = 0 ; i < markers->childCount() ; ++i)
    {
      markers->child(i)->setHidden(markersRoot->child(i)->isHidden());
      markers->child(i)->setCheckState(0, Qt::Checked);
      markers->child(i)->setForeground(0, Qt::black);
      markers->child(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
  }
  // Models Outputs
  if (!modelOutputsRoot->isHidden())
  {
    for (int i = 0 ; i < modelOutputsRoot->childCount() ; ++i)
    {
      QTreeWidgetItem* subRoot = modelOutputsRoot->child(i);
      if (!subRoot->isHidden())
      {
        QTreeWidgetItem* sub = subRoot->clone();
        sub->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
        sub->setCheckState(0, Qt::Checked);
        points->addChild(sub);
        sub->setExpanded(true);
        for (int j = 0 ; j < sub->childCount() ; ++j)
        {
          sub->child(j)->setHidden(modelOutputsRoot->child(i)->child(j)->isHidden());
          sub->child(j)->setCheckState(0, Qt::Checked);
          sub->child(j)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        }
      }
    }
  }
  this->pointsTreeWidget->blockSignals(false);
  this->updatePointsTreeSectionWidth();
};

void ExportASCIIDialog::fillForcePlates(QTreeWidgetItem* forcePlatesRoot)
{
  this->forcePlatesTreeWidget->blockSignals(true);
  if (!forcePlatesRoot->isHidden())
  {
    QTreeWidgetItem* forceplates = forcePlatesRoot->clone();
    forceplates->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
    forceplates->setCheckState(0, Qt::Checked);
    this->forcePlatesTreeWidget->addTopLevelItem(forceplates);
    forceplates->setExpanded(true);
    for (int i = 0 ; i < forceplates->childCount() ; ++i)
    {
      QTreeWidgetItem* child = forceplates->child(i);
      child->setCheckState(0, Qt::Checked);
      child->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
      child->setExpanded(true);
      for (int j = 0 ; j < child->childCount() ; ++j)
      {
        child->child(j)->setCheckState(0, Qt::Checked);
        child->child(j)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      }
    }
  }
  this->forcePlatesTreeWidget->blockSignals(false);
  this->updateForcePlatesTreeSectionWidth();
};

void ExportASCIIDialog::fillAnalogs(QTreeWidgetItem* analogsRoot)
{
  this->analogsTreeWidget->blockSignals(true);
  if (!analogsRoot->isHidden())
  {
    QTreeWidgetItem* analogs = analogsRoot->clone();
    analogs->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
    analogs->setCheckState(0, Qt::Checked);
    this->analogsTreeWidget->addTopLevelItem(analogs);
    analogs->setExpanded(true);
    for (int i = 0 ; i < analogs->childCount() ; ++i)
    {
      analogs->child(i)->setHidden(analogsRoot->child(i)->isHidden());
      analogs->child(i)->setCheckState(0, Qt::Checked);
      analogs->child(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
  }
  this->analogsTreeWidget->blockSignals(false);
  this->updateAnalogsTreeSectionWidth();
};

QString ExportASCIIDialog::choosedSeparator() const
{
  if (this->tabRadioButton->isChecked())
    return "\t";
  else if (this->semicolonRadioButton->isChecked())
    return ";";
  else if (this->commaRadioButton->isChecked())
    return ",";
  else if (this->spaceRadioButton->isChecked())
    return " ";
  else
    return this->otherLineEdit->text();
};

QString ExportASCIIDialog::suggestedExtension() const
{
  if (this->tabRadioButton->isChecked())
    return "txt";
  else if (this->semicolonRadioButton->isChecked())
    return "txt";
  else if (this->commaRadioButton->isChecked())
    return "csv";
  else if (this->spaceRadioButton->isChecked())
    return "txt";
  else
    return "txt";
};

void ExportASCIIDialog::uncheckOtherDelimitersButton(bool checked)
{
  if (checked)
  {
    this->tabRadioButton->setChecked(false);
    this->semicolonRadioButton->setChecked(false);
    this->commaRadioButton->setChecked(false);
    this->spaceRadioButton->setChecked(false);
    this->otherLineEdit->setFocus();
  }
};

void ExportASCIIDialog::uncheckOtherDelimiterButton(bool checked)
{
  if (checked)
    this->otherRadioButton->setChecked(false);
};

void ExportASCIIDialog::validExportInfo()
{
  bool valid = true;
  if (this->otherRadioButton->isChecked() && this->otherLineEdit->text().isEmpty())
    valid = false;
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
};

void ExportASCIIDialog::updatePointsTreeSectionWidth()
{
  this->pointsTreeWidget->resizeColumnToContents(0);
};

void ExportASCIIDialog::updateForcePlatesTreeSectionWidth()
{
  this->forcePlatesTreeWidget->resizeColumnToContents(0);
};

void ExportASCIIDialog::updateAnalogsTreeSectionWidth()
{
  this->analogsTreeWidget->resizeColumnToContents(0);
};