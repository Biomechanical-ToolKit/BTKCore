/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

#ifndef MetadataView_h
#define MetadataView_h

#include <QTreeView>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QHeaderView>

#include <btkMetaData.h>

static int checkState2 = Qt::UserRole + 100;
static int metadataInfoFirstValue = Qt::UserRole + 101;
static int metadataInfoValuesCount = Qt::UserRole + 102;

class MetadataView : public QTreeView
{
  Q_OBJECT

public:
  MetadataView(QWidget* parent = 0);
  ~MetadataView() { delete this->mp_Model; };
  
  void load(btk::MetaData::Pointer m);
  void reset();

public Q_SLOTS:
  void dispatchChangement(QStandardItem*);
  void expandItem(QStandardItem*);
  
private:
  void constructView(btk::MetaData::Pointer m, QStandardItem* parent);
  
  QStandardItemModel* mp_Model;
};

// --------------- MetdataViewItem ---------------

class DimensionsItem : public QStandardItem
{
public:
  DimensionsItem(btk::MetaDataInfo::Pointer info)
  : QStandardItem()
  {
    QString dims = "";
    if (info.get() != 0)
    {
      int size = info->GetDimensions().size();
      if (size != 0)
      {
        for (int j = 0 ; j < size ; ++j)
        {
          dims += QString::number(static_cast<int>(info->GetDimensions()[j]));
          if (j < (size - 1))
            dims += " x ";
        }
      }
    }
    this->setText(dims);
  };
};

class ValuesItem : public QStandardItem
{
public:
  ValuesItem(btk::MetaDataInfo::Pointer info)
  : QStandardItem()
  {
    QString txt = "";
    if ((info.get() != 0) && (info->GetValues().size() != 0))
    {
      int size = info->GetValues().size();
      switch(info->GetFormat())
      {
        case btk::MetaDataInfo::Char:
          txt = QString::fromStdString(info->ToString(0));
          break;
        case btk::MetaDataInfo::Byte:
          txt = QString::number(info->ToInt(0));
          break;
        case btk::MetaDataInfo::Integer:
          txt = QString::number(info->ToInt(0));
          break;
        case btk::MetaDataInfo::Real:
          txt = QString::number(info->ToDouble(0));
          break;
      }
      this->setData(QVariant(txt), metadataInfoFirstValue);
      this->setData(QVariant(size), metadataInfoValuesCount);
      if (size != 1)
      {
        txt = "<multiple lines>";
        QFont f = this->font();
        f.setItalic(true);
        this->setFont(f);
      }
    }
    this->setText(txt);
  };
};

// --------------- Specials QCheckBox ---------------

class Lock : public QCheckBox
{
public:
  Lock(bool unlocked)
  : QCheckBox()
  {
    this->setStyleSheet(
      "QCheckBox { spacing: 0px; margin-left:5px; }\
       QCheckBox::indicator { width: 13px; height: 13px; }\
       QCheckBox::indicator:unchecked { image: url(:/Resources/Images/unlock.png); }\
       QCheckBox::indicator:unchecked:hover { image: url(:/Resources/Images/unlock.png); }\
       QCheckBox::indicator:unchecked:pressed { image: url(:/Resources/Images/unlock.png); }\
       QCheckBox::indicator:checked { image: url(:/Resources/Images/lock.png); }\
       QCheckBox::indicator:checked:hover { image: url(:/Resources/Images/lock.png); }\
       QCheckBox::indicator:checked:pressed { image: url(:/Resources/Images/lock.png); }\
       QCheckBox::indicator:indeterminate:hover { image: url(:/Resources/Images/unlock.png); }\
       QCheckBox::indicator:indeterminate:pressed { image: url(:/Resources/Images/unlock.png); }"
     );
    if (unlocked)
      this->setCheckState(Qt::Unchecked);
    else
      this->setCheckState(Qt::Checked);
  };
};

class Expander : public QCheckBox
{
  Q_OBJECT
  
public:
  Expander(btk::MetaDataInfo::Pointer info, QStandardItem* item)
  : QCheckBox()
  {
    this->mp_Item = item;
    this->setUnvisible(true);
    if (info.get() != 0)
    {
      if (info->GetValues().size() > 1)
      {
        this->setCheckState(Qt::Unchecked);
        this->setUnvisible(false);
      }
    }
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(itemChanged(int)));
  };

private Q_SLOTS:
  void itemChanged(int s)
  {
    if (!this->m_Invisible)
      this->mp_Item->setData(s, checkState2);
  };

private:
  void setUnvisible(bool hide)
  {
    this->m_Invisible = hide;
    if (hide)
    {
      this->setStyleSheet(
        "QCheckBox { spacing: 0px; }\
         QCheckBox::indicator { width: 16px; height: 16px; }\
         QCheckBox::indicator:unchecked { image: url(:/Resources/Images/invisible.png); }\
         QCheckBox::indicator:unchecked:hover { image: url(:/Resources/Images/invisible.png); }\
         QCheckBox::indicator:unchecked:pressed { image: url(:/Resources/Images/invisible.png); }\
         QCheckBox::indicator:checked { image: url(:/Resources/Images/invisible.png); }\
         QCheckBox::indicator:checked:hover { image: url(:/Resources/Images/invisible.png); }\
         QCheckBox::indicator:checked:pressed { image: url(:/Resources/Images/invisible.png); }\
         QCheckBox::indicator:indeterminate:hover { image: url(:/Resources/Images/invisible.png); }\
         QCheckBox::indicator:indeterminate:pressed { image: url(:/Resources/Images/invisible.png); }"
      );
    }
    else
    {
      this->setStyleSheet(
        "QCheckBox { spacing: 0px; }\
         QCheckBox::indicator { width: 16px; height: 16px; }\
         QCheckBox::indicator:unchecked { image: url(:/Resources/Images/disclosureTriangleSmallRightBlack.png); }\
         QCheckBox::indicator:unchecked:hover { image: url(:/Resources/Images/disclosureTriangleSmallRightBlack.png); }\
         QCheckBox::indicator:unchecked:pressed { image: url(:/Resources/Images/disclosureTriangleSmallRightBlack.png); }\
         QCheckBox::indicator:checked { image: url(:/Resources/Images/disclosureTriangleSmallDownBlack.png); }\
         QCheckBox::indicator:checked:hover { image: url(:/Resources/Images/disclosureTriangleSmallDownBlack.png); }\
         QCheckBox::indicator:checked:pressed { image: url(:/Resources/Images/disclosureTriangleSmallDownBlack.png); }\
         QCheckBox::indicator:indeterminate:hover { image: url(:/Resources/Images/disclosureTriangleSmallDownBlack.png); }\
         QCheckBox::indicator:indeterminate:pressed { image: url(:/Resources/Images/disclosureTriangleSmallDownBlack.png); }"
      );
    }
  };
  
  QStandardItem* mp_Item;
  bool m_Invisible;
};

#endif // MetadataView_h