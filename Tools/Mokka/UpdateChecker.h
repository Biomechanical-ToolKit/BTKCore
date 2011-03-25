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

#ifndef UpdateChecker_h
#define UpdateChecker_h

#include "ui_UpdateChecker.h"

#include <QDialog>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;
class QThread;
class UpdateCheckerDialog;
class UpdateParser;

class UpdateChecker: public QObject
{
  Q_OBJECT
  
public:
  UpdateChecker(const QString& appVer, const QString& url = "", const QString& image = "", QWidget* parent = 0);
  ~UpdateChecker();
  
  void setIcon(const QString& path);
  void setUrl(const QString& path);
  
public slots:
  void check(bool quietNoUpdate = false);

private slots:
  void notifyUpdate(const QString& appName, const QString& appCurVer, const QString& appNewVer, const QString& appNote, const QString& appUrl);
  void notifyNoUpdate();
  void resetThread();
  
private:
  QThread* mp_Thread;
  UpdateParser* mp_Parser;
  UpdateCheckerDialog* mp_Dialog;
  bool m_QuietNoUpdate;
};

// Parser

class UpdateParser: public QObject
{
  Q_OBJECT
  
public:
  UpdateParser(const QString& appVer, const QString& url = "");
  // ~UpdateParser(); // Implicit
  
  QString url;
  
public slots:
  void check();
  
private slots:
  void parseReply(QNetworkReply* reply);
  
signals:
  void updateFound(const QString& appName, const QString& appCurVer, const QString& appNewVer, const QString& appNote, const QString& appUrl);
  void updateNotFound();
  void parsingFinished();
  
private:
  bool isGreaterRelease(const QStringList& max, const QStringList& rel) const;
  bool isGreaterRelease(const QString& max, const QString& rel) const;
  bool isNewRelease(const QStringList& rel) const {return !this->isGreaterRelease(this->m_CurrentVersion, rel);};

  QNetworkAccessManager* mp_Manager;
  QStringList m_CurrentVersion;
};

// Dialog

class UpdateCheckerDialog : public QDialog, public Ui::UpdateChecker
{
  Q_OBJECT

public:
  UpdateCheckerDialog(const QString& image = "", QWidget* parent = 0);
  // ~UpdateCheckerDialog(); // Implicit
  
  QString appNewVer;
  QString url;
  
public slots:
  void download();
  void remind();
  void skip();

};

#endif // ProgressWidget_h