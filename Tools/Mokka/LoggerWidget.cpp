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

#include "LoggerWidget.h"

#include <QStandardItemModel>
#include <QDateTime>
#include <QPainter>
#include <QResizeEvent>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QClipboard>

#include <cstdio> // EOF under Linux

void LoggerWidget::redirectCout(const QString& id)
{
  static LoggerCoutRediction logCout(id);
};

void LoggerWidget::redirectCerr(const QString& id)
{
  static LoggerCerrRediction logCout(id);
};

void LoggerWidget::messageHandler(QtMsgType type, const char* msg)
{
  QString msgDetail(msg);
  bool internalMessage = true;
  size_t lenId = strlen(LOGGER_EXTERNAL_MESSAGE_ID);
  if (msgDetail.left(lenId).compare(LOGGER_EXTERNAL_MESSAGE_ID) == 0)
  {
    msgDetail = msgDetail.right(msgDetail.length() - lenId); // Remove the message ID.
    internalMessage = false;
  }
  
  QStandardItem* item = 0;
  QString msgTypeStr;
  QString msgError;
  switch (type)
  {
  case QtDebugMsg:
    msgTypeStr = "Trace";
    item = new LoggerTraceItem();
    break;
  case QtWarningMsg:
    msgTypeStr = "Warning";
    item = new LoggerWarningItem();
    break;
  case QtCriticalMsg:
    msgTypeStr = "Critical Error";
    item = new LoggerCriticalItem();
    break;
  case QtFatalMsg:
    msgTypeStr = "Fatal Error";
    item = new LoggerFatalItem();
    break;
  }
  if (internalMessage || (type == QtFatalMsg))
    item->setData(QString(internalMessage ? "Internal " : "") + msgTypeStr + ": Contact the developers and report this message:", LoggerMsgError);
  item->setData(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), LoggerMsgDateTime);
  item->setData(msgDetail, LoggerMsgDetail);
  
#if 1
  LoggerWidget::model()->insertRow(0, item); // Push front
#else
  LoggerWidget::model()->appendRow(item); // Push back
#endif
};
 
const QIcon& LoggerWidget::infoIcon()
{
  static const QIcon _infoIcon(QString::fromUtf8(":/Resources/Images/info.png"));
  return _infoIcon;
};

const QIcon& LoggerWidget::warningIcon()
{
  static const QIcon _infoIcon(QString::fromUtf8(":/Resources/Images/warning.png"));
  return _infoIcon;
};

const QIcon& LoggerWidget::errorIcon()
{
  static const QIcon _infoIcon(QString::fromUtf8(":/Resources/Images/error.png"));
  return _infoIcon;
};


LoggerWidget::LoggerWidget(QWidget* parent)
: QListView(parent)
{
  this->setFocusPolicy(Qt::ClickFocus);
  this->setItemDelegate(new LoggerItemDelegate(this));
  this->setFrameShape(QListView::NoFrame);
  this->setModel(LoggerWidget::model());
  this->setResizeMode(QListView::Adjust);
  this->setSelectionMode(QListView::ExtendedSelection);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  
#ifdef Q_OS_MAC
  QFont f = this->font();
  f.setPixelSize(11);
  this->setFont(f);
  this->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

  QList<QAction*> actions;
  QAction* copyAction = new QAction(tr("Copy"), this); actions.push_back(copyAction);
  copyAction->setShortcut(QKeySequence::Copy);
  copyAction->setShortcutContext(Qt::WidgetShortcut); // To not be in conflict with a possible copy action set with the same shortcut in the main menu.
  QAction* separator = new QAction(this); separator->setSeparator(true); actions.push_back(separator);
  QAction* selectAllAction = new QAction(tr("Select All"), this); actions.push_back(selectAllAction);
  selectAllAction->setShortcut(QKeySequence::SelectAll);
  selectAllAction->setShortcutContext(Qt::WidgetShortcut); // To not be in conflict with a possible copy action set with the same shortcut in the main menu.
  QAction* separator2 = new QAction(this); separator2->setSeparator(true); actions.push_back(separator2);
  QAction* clearAllAction = new QAction(tr("Clear All"), this); actions.push_back(clearAllAction);
  // QAction* separator = new QAction(this); separator->setSeparator(true); actions.push_back(separator);
  // QMenu* displayMenu = new QMenu(tr("Level displayed"), this);
  // QAction* allLevel = new QAction(tr("All Levels"), this); displayMenu->addAction(allLevel);
  // allLevel->setChecked(true);
  // QAction* separator2 = new QAction(this); separator2->setSeparator(true); displayMenu->addAction(separator2);
  // QAction* traceAction = new QAction(tr("Trace Level"), this); displayMenu->addAction(traceAction);
  // QAction* warningAction = new QAction(tr("Warning Level"), this); displayMenu->addAction(warningAction);
  // QAction* errorAction = new QAction(tr("Error Level"), this); displayMenu->addAction(errorAction);
  // actions.push_back(displayMenu->menuAction());
  this->addActions(actions); 
  this->setContextMenuPolicy(Qt::ActionsContextMenu);
  
  connect(copyAction, SIGNAL(triggered()), this, SLOT(copySelectedItemsToClipboard()));
  connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));
  connect(clearAllAction, SIGNAL(triggered()), this, SLOT(clear()));
};

void LoggerWidget::copySelectedItemsToClipboard()
{
  QModelIndexList indices = this->selectedIndexes();
  QString text;
  for (QModelIndexList::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
    text += it->data(LoggerMsgDateTime).toString() + " - " + it->data(LoggerMsgDetail).toString() + "\n";
  text = text.left(text.length() - 1); // Remove the last '\n' character.
  if (!text.isEmpty())
    QApplication::clipboard()->setText(text);
};

void LoggerWidget::clear()
{
  LoggerWidget::model()->clear();
};

void LoggerWidget::resizeEvent(QResizeEvent* event)
{
   if ((event->size() - event->oldSize()).isNull())
     return;
  this->scheduleDelayedItemsLayout();
};

// ----------------------------------------------------------------------------

LoggerItemDelegate::LoggerItemDelegate(QObject* parent)
: QAbstractItemDelegate(parent)
{};

void LoggerItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QVariant msgType = index.data(LoggerMsgType);
  QVariant msgDetail = index.data(LoggerMsgDetail);
  QVariant msgDateTime = index.data(LoggerMsgDateTime);
  QVariant msgError = index.data(LoggerMsgError);
  
  if (!msgType.isValid() || !msgDetail.isValid() || !msgDateTime.isValid())
    return; // No paint
  
  QString msgTypeStr;
  QColor bottomLineColor;
  QIcon icon;
  switch (msgType.toInt())
  {
  case QtDebugMsg:
    bottomLineColor = QColor(200,200,200);
    icon = LoggerWidget::infoIcon();
    break;
  case QtWarningMsg:
    bottomLineColor = QColor(255,167,75);
    icon = LoggerWidget::warningIcon();
    break;
  case QtCriticalMsg:
    bottomLineColor = QColor(255,64,64);
    icon = LoggerWidget::errorIcon();
    break;
  case QtFatalMsg:
    bottomLineColor = QColor(255,64,64);
    icon = LoggerWidget::errorIcon();
    break;
  }
  
  painter->save();
  
  // Draw background
  if (option.showDecorationSelected && (option.state & QStyle::State_Selected))
  {
    QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
      cg = QPalette::Inactive;
    painter->fillRect(option.rect, option.palette.brush(cg, QPalette::Highlight));
  }
  else
  {
    QVariant value = index.data(Qt::BackgroundRole);
    if (qVariantCanConvert<QBrush>(value))
    {
      QPointF oldBO = painter->brushOrigin();
      painter->setBrushOrigin(option.rect.topLeft());
      painter->fillRect(option.rect, qvariant_cast<QBrush>(value));
      painter->setBrushOrigin(oldBO);
    }
  }
  QPen pen(bottomLineColor);
  pen.setWidth(1);
  painter->setPen(pen);
  painter->drawLine(option.rect.x(), option.rect.y() + option.rect.height() - 1, option.rect.x() + option.rect.width(), option.rect.y() + option.rect.height() - 1);
  
  const int padding = 3;
  QRect rect = option.rect;
  rect.setX(rect.x() + padding);
  rect.setY(rect.y() + padding);
  rect.setWidth(rect.width() - 2 * padding);
  rect.setHeight(rect.height() - 2 * padding);
  
  // Draw icon
  icon.paint(painter, rect, Qt::AlignTop | Qt::AlignLeft);
  
  // Draw text
  // - Shift text right to make icon visible
  QSize iconSize = icon.actualSize(rect.size());
  painter->translate(iconSize.width() + 3*padding, 0); 
  rect.setWidth(rect.width() - iconSize.width() - 2*padding);
  painter->setPen(option.palette.color(QPalette::Normal, QPalette::Text));
  // - Draw the date
  if (option.state & QStyle::State_Selected)
    painter->setPen(option.palette.color(QPalette::Normal, QPalette::HighlightedText));
  else
    painter->setPen(option.palette.color(QPalette::Normal, QPalette::Text));
  QFont f = painter->font() ;
  f.setBold(true);
  painter->setFont(f);
  painter->drawText(rect, Qt::AlignLeft, msgDateTime.toString());
  QFontMetrics fm(painter->font());
  int offsetX = fm.width(msgDateTime.toString());
  painter->translate(offsetX + 3*padding, 0); rect.setWidth(rect.width() - offsetX - 3*padding);
  // Draw the possible error message
  if (msgError.isValid())
  {
    QRect bounding;
    painter->setPen(Qt::red);
    painter->drawText(rect, Qt::AlignLeft | Qt::TextWordWrap, msgError.toString(), &bounding);
    painter->translate(0, bounding.height()); rect.setHeight(rect.height() - bounding.height());
  }
  // Draw detailed message
  if (option.state & QStyle::State_Selected)
    painter->setPen(option.palette.color(QPalette::Normal, QPalette::HighlightedText));
  else
    painter->setPen(option.palette.color(QPalette::Normal, QPalette::Text));
  f.setBold(false);
  painter->setFont(f);
  painter->drawText(rect, Qt::AlignLeft | Qt::TextWordWrap, msgDetail.toString());
  
  painter->restore();
};

QSize LoggerItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(option)
  
  QVariant msgDetail = index.data(LoggerMsgDetail);
  QVariant msgError = index.data(LoggerMsgError);
  if (!msgDetail.isValid())
    return QSize(0,0);
  else
  {
    QListView* p = static_cast<QListView*>(this->parent());
    QFont f = p->font(); f.setBold(true);
    QFontMetrics fmBold(f);
    float rw = float(p->viewport()->size().width() - fmBold.width("yyyy-MM-dd hh:mm:ss") - 31);
    int height = 0;
    if (msgError.isValid())
      height = fmBold.boundingRect(QRect(0,0,rw,9999), Qt::AlignLeft | Qt::TextWordWrap, msgError.toString()).height();// qCeil(fmErr.width(msgError.toString()) / rw); // padding-bottom
    QFontMetrics fm(p->font());
    height += fm.boundingRect(QRect(0,0,rw,9999), Qt::AlignLeft | Qt::TextWordWrap, msgDetail.toString()).height();// qCeil(fmDetail.width(msgDetail.toString()) / rw) * fm.height() + 9; // padding-top: 3; padding-bottom: 3
    height += 9; // padding
    if (height < 25)
      height = 25; // Height of the icon + padding
    return QSize(p->viewport()->size().width(), height);
  }
}

// ----------------------------------------------------------------------------

LoggerTraceItem::LoggerTraceItem()
: QStandardItem()
{
  this->setBackground(Qt::white);
  this->setData(QtDebugMsg, LoggerMsgType);
};

LoggerWarningItem::LoggerWarningItem()
: QStandardItem()
{
  this->setBackground(QColor(255, 253, 208));
  this->setData(QtWarningMsg, LoggerMsgType);
};

LoggerCriticalItem::LoggerCriticalItem()
: QStandardItem()
{
  this->setBackground(QColor(255, 192, 203));
  this->setData(QtCriticalMsg, LoggerMsgType);
};

LoggerFatalItem::LoggerFatalItem()
: QStandardItem()
{
  this->setBackground(QColor(255, 192, 203));
  this->setData(QtFatalMsg, LoggerMsgType);
};

QStandardItemModel* LoggerWidget::model()
{
  static QStandardItemModel model;
  return &model;
};

// ----------------------------------------------------------------------------

template <class T>
LoggerOutputRedirection<T>::LoggerOutputRedirection(std::ostream* output, const QString& id)
{
  this->mp_Stream = output;
  this->mp_NewBuffer = new T(id);
  this->mp_OldBuffer = this->mp_Stream->rdbuf(this->mp_NewBuffer);
}

template <class T>
LoggerOutputRedirection<T>::~LoggerOutputRedirection()
{
  this->mp_Stream->rdbuf(this->mp_OldBuffer);
  delete this->mp_NewBuffer;
};