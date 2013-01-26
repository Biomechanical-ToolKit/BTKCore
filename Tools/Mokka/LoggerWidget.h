/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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


#ifndef LoggerWidget_h
#define LoggerWidget_h

#include "LoggerMessage.h"

#include <QListView>
#include <QStandardItem>
#include <QItemDelegate>

#include <iostream>
#include <cstdio> // Required under Linux

enum {LoggerMsgType = Qt::UserRole + 1, LoggerMsgDateTime, LoggerMsgDetail, LoggerMsgError};

class QStandardItemModel;
class QAction;

class LoggerWidget : public QListView
{
  Q_OBJECT
public:
  static QStandardItemModel* model();
  static void redirectCout(const QString& id = LOGGER_EXTERNAL_MESSAGE_ID);
  static void redirectCerr(const QString& id = LOGGER_EXTERNAL_MESSAGE_ID);
  static void messageHandler(QtMsgType type, const char* msg);
  
  static const QIcon& infoIcon();
  static const QIcon& warningIcon();
  static const QIcon& errorIcon();
  
  LoggerWidget(QWidget* parent = 0);
  
public slots:
  void clear();
  void copySelectedItemsToClipboard();
  
protected:
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void resizeEvent(QResizeEvent* event);
  
private:
  QAction* mp_CopyAction;
};

// ----------------------------------------------------------------------------

class LoggerItemDelegate : public QAbstractItemDelegate
{
  Q_OBJECT
  
public:
  LoggerItemDelegate(QObject* parent = 0);
  virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

// ----------------------------------------------------------------------------

class LoggerTraceItem : public QStandardItem
{
public:
  LoggerTraceItem();
};

class LoggerWarningItem : public QStandardItem
{
public:
  LoggerWarningItem();
};

class LoggerCriticalItem : public QStandardItem
{
public:
  LoggerCriticalItem();
};

class LoggerFatalItem : public QStandardItem
{
public:
  LoggerFatalItem();
};

// ----------------------------------------------------------------------------

template <class T>
class LoggerOutputRedirection
{
public:
  LoggerOutputRedirection(std::ostream* output, const QString& id);
  virtual ~LoggerOutputRedirection();
private:
  std::ostream* mp_Stream;
  T* mp_NewBuffer;
  std::streambuf* mp_OldBuffer;
};

class LoggerCoutToDebug;
class LoggerCoutRediction : public LoggerOutputRedirection<LoggerCoutToDebug>
{
public:
  LoggerCoutRediction(const QString& id) : LoggerOutputRedirection<LoggerCoutToDebug>(&std::cout, id) {};
};

class LoggerCerrToWarn;
class LoggerCerrRediction : public LoggerOutputRedirection<LoggerCerrToWarn>
{
public:
  LoggerCerrRediction(const QString& id) : LoggerOutputRedirection<LoggerCerrToWarn>(&std::cerr, id) {};
};

class LoggerStreambuf : public std::streambuf
{
public:
  LoggerStreambuf(void (*output)(const char* msg, ...), const QString& id)
  : m_Id(id), m_Message()
  {
    this->mp_Output = output;
  };

protected:
  inline virtual std::streamsize xsputn(const char* s, std::streamsize n); 
  inline virtual int overflow(int c = EOF);

private:
  void (*mp_Output)(const char * msg, ...);
  QString m_Id;
  QString m_Message;
};

class LoggerCoutToDebug : public LoggerStreambuf
{
public:
  LoggerCoutToDebug(const QString& id) : LoggerStreambuf(&qDebug, id)
  {};
};

class LoggerCerrToWarn : public LoggerStreambuf
{
public:
  LoggerCerrToWarn(const QString& id) : LoggerStreambuf(&qWarning, id)
  {};
};

// ----------------------------------------------------------------------------

std::streamsize LoggerStreambuf::xsputn(const char* s, std::streamsize n) 
{
  this->m_Message.append(s);
  return n;
};

int LoggerStreambuf::overflow(int c) 
{
  if (c == 10) // New line
  {
    (*this->mp_Output)("%s%s",  qPrintable(this->m_Id), qPrintable(this->m_Message));
    this->m_Message.clear();
  }
  else
    this->m_Message.append(QChar(static_cast<char>(c)));
  return 1;
};




#endif // LoggerWidget_h