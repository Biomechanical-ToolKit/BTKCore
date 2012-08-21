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

#ifndef UpdateManager_p_h
#define UpdateManager_p_h

#include "unzip.h"

#include <QDialog>
#include <QBuffer>

class QNetworkAccessManager;
class QNetworkReply;
class QThread;

struct UpdateApplicationDownload
{
  UpdateApplicationDownload();
  ~UpdateApplicationDownload();
  QString url;
  QString hash;
  QByteArray data;
  QBuffer buffer;
  unzFile unz;
};

// ------------------------------------------------------------------------- //

// This class processes all the data during the retrieving of the data, the download of the update and its installation.
class UpdateController: public QObject
{
  Q_OBJECT
  
public:
  UpdateController(const QString& appVer, const QString& feedUrl = "");
  // ~UpdateController(); // Implicit
  
  void setFeedUrl(const QString& url);
  void setApplicationDownloadUrl(const QString& url);
  void setInstallationPath(const QString& path);
  const QString& installationPath() const;
  
public slots:
  void checkUpdate();
  void downloadUpdate();
  void installUpdate();
  void finalizeUpdate();
  
private slots:
  void parseFeed(QNetworkReply* reply);
  void abortDownload();
  void checkDownload(QNetworkReply* reply);
  
signals:
  // Feed
  void updateFound(const QString& appName, const QString& appCurVer, const QString& appNewVer, const QString& appNote);
  void updateNotFound(const QString& appName, const QString& appCurVer);
  void updateIncomplete();
  void parsingFinished();
  // Application download
  void downloadProgress(qint64, qint64);
  void downloadReadyToInstall();
  void downloadIncomplete();
  void downloadFinished();
  // Application installation
  void installationIncomplete();
  void installationComplete();
  void installationFinalized();
  
private:
  bool isNewRelease(const QStringList& rel) const {return (this->compareRelease(this->m_CurrentVersion, rel) < 0);};
  bool isGreaterRelease(const QStringList& max, const QStringList& rel) const {return (this->compareRelease(max, rel) > 0);};
  int compareRelease(const QStringList& max, const QStringList& rel) const;
  int compareRelease(const QString& max, const QString& rel) const;
  int extractReleaseNumber(const QString& str, QString& suffix) const;

  QStringList m_CurrentVersion;
  QString m_FeedUrl;
  UpdateApplicationDownload m_Download;
  QString m_InstallationPath;
  QNetworkAccessManager* mp_Manager;
  QNetworkReply* mp_DownloadReply;
};

// ------------------------------------------------------------------------- //

// Widget to show the update information
#include "ui_UpdateManagerNewVersionDialog.h"
class UpdateManagerNewVersionDialog : public QDialog, public Ui::UpdateManagerNewVersionDialog
{
  Q_OBJECT

public:
  UpdateManagerNewVersionDialog(QWidget* parent = 0);
  // ~UpdateManagerNewVersionDialog(); // Implicit
  
  void setApplicationIcon(const QPixmap& icon);
  
  QString appNewVer;
  
public slots:
  void install();
  void remind();
  void skip();

signals:
  void installSoftwareButtonClicked();
};

// ------------------------------------------------------------------------- //

// Widget to interact with the download of the data and start the installation
#include "ui_UpdateManagerInstallerDialog.h"
class UpdateManagerInstallerDialog : public QDialog, public Ui::UpdateManagerInstallerDialog
{
  Q_OBJECT

public:
  UpdateManagerInstallerDialog(QWidget* parent = 0);
  // ~UpdateManagerInstallerDialog(); // Implicit
  
  void setApplicationIcon(const QPixmap& icon);
  
signals:
  void abortInstallButtonClicked();
  void launchInstallButtonClicked();
};

// ------------------------------------------------------------------------- //

// Private
class UpdateManagerPrivate
{
public:
  UpdateManagerPrivate();
  virtual ~UpdateManagerPrivate();
  
  QString m_IconPath;
  QThread* mp_Thread;
  QNetworkAccessManager* mp_Manager;
  UpdateController* mp_Controller;
  UpdateManagerNewVersionDialog* mp_NewVersionDialog;
  UpdateManagerInstallerDialog* mp_InstallerDialog;
  bool m_QuietNoUpdate;
  bool m_SeparatedUpdaterWithPrivileges;
};

#endif // UpdateManager_p_h