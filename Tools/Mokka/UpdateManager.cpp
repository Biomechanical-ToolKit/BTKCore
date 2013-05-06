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

#include "UpdateManager.h"
#include "UpdateManager_p.h"

#include <QScrollBar>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QProcess>
#include <QDir>

#ifdef Q_OS_WIN
  // For the win32ShellExecute method
  #include "qt_windows.h"
  #include "qwindowdefs_win.h"
  #include <shellapi.h>
#endif

UpdateManagerPrivate::UpdateManagerPrivate()
: m_IconPath()
{
  this->mp_NewVersionDialog = NULL;
  this->mp_InstallerDialog = NULL;
  this->mp_Thread = NULL;
  this->mp_Controller = NULL;
  this->m_QuietNoUpdate = false;
  this->m_SeparatedUpdaterWithPrivileges = false;
}

UpdateManagerPrivate::~UpdateManagerPrivate()
{
  if (this->mp_Thread != NULL)
  {
    this->mp_Thread->quit();
    this->mp_Thread->wait();
    delete this->mp_Thread;
  }
  if (this->mp_Controller != NULL)
    delete this->mp_Controller;
};

// ------------------------------------------------------------------------- //

UpdateManager::UpdateManager(const QString& currentApplicationVersion, const QString& applicationUpdateURL, const QString& iconPath, QWidget* parent)
: QObject(parent), d_ptr(new UpdateManagerPrivate)
{
  Q_D(UpdateManager);
  d->mp_NewVersionDialog = new UpdateManagerNewVersionDialog(parent);
  d->mp_InstallerDialog = new UpdateManagerInstallerDialog(parent);
  d->mp_Thread = new QThread;
  d->mp_Controller = new UpdateController(currentApplicationVersion, applicationUpdateURL);
  d->mp_Controller->moveToThread(d->mp_Thread);
#ifdef Q_OS_WIN
  if (QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA)
    d->m_SeparatedUpdaterWithPrivileges = true;
#endif
  
  this->setIcon(iconPath);
  
  connect(d->mp_Controller, SIGNAL(updateFound(QString, QString, QString, QString)), this, SLOT(notifyUpdate(QString, QString, QString, QString)));
  connect(d->mp_Controller, SIGNAL(updateNotFound(QString, QString)), this, SLOT(notifyNoUpdate(QString, QString)));
  connect(d->mp_Controller, SIGNAL(updateIncomplete()), this, SLOT(notifyUpdateError()));
  connect(d->mp_Controller, SIGNAL(parsingFinished()), this, SLOT(resetThread()));
  connect(d->mp_Controller, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(setDownloadProgress(qint64,qint64)));
  connect(d->mp_Controller, SIGNAL(downloadReadyToInstall()), this, SLOT(notifyReadyToInstall()));
  connect(d->mp_Controller, SIGNAL(downloadIncomplete()), this, SLOT(notifyDownloadError()));
  connect(d->mp_Controller, SIGNAL(downloadFinished()), this, SLOT(resetThread()));
  connect(d->mp_Controller, SIGNAL(installationIncomplete()), this, SLOT(notifyInstallationError()));
  connect(d->mp_Controller, SIGNAL(installationComplete()), this, SLOT(restartApplication()));
  connect(d->mp_Controller, SIGNAL(installationFinalized()), this, SLOT(resetThread()));
  connect(d->mp_NewVersionDialog, SIGNAL(installSoftwareButtonClicked()), this, SLOT(downloadUpdate()));
  connect(d->mp_InstallerDialog, SIGNAL(abortInstallButtonClicked()), this, SLOT(abortDownload()));
  connect(d->mp_InstallerDialog, SIGNAL(launchInstallButtonClicked()), this, SLOT(installUpdate()));
}

UpdateManager::~UpdateManager()
{};

void UpdateManager::setIcon(const QString& path)
{
  Q_D(UpdateManager);
  
  QPixmap icon;
  if (!path.isEmpty() && !icon.load(path))
  {
    qDebug("Error durning the loading of the image used for the application icon in the update dialogs.");
    d->m_IconPath = "";
  }
  else
    d->m_IconPath = path;
  d->mp_NewVersionDialog->setApplicationIcon(icon);
  d->mp_InstallerDialog->setApplicationIcon(icon);
};

void UpdateManager::setFeedUrl(const QString& url)
{
  Q_D(UpdateManager);
  d->mp_Controller->setFeedUrl(url);
};

void UpdateManager::acceptDevelopmentUpdate(bool accepted)
{
  Q_D(UpdateManager);
  d->mp_Controller->acceptDevelopmentUpdate(accepted);
};

void UpdateManager::checkUpdate(bool quietNoUpdate)
{
  Q_D(UpdateManager);
  
  d->mp_InstallerDialog->setModal(false);
  d->mp_InstallerDialog->buttonBox->button(QDialogButtonBox::Cancel)->show();
  d->mp_InstallerDialog->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  d->mp_InstallerDialog->buttonBox->button(QDialogButtonBox::Ok)->hide();
  d->mp_InstallerDialog->progressBar->setRange(0,0);
  d->mp_InstallerDialog->progressBar->setValue(-1);
  d->mp_InstallerDialog->progressLabel->setText("0 KB of ???");
  d->mp_InstallerDialog->progressLabel->show();
  
  
  disconnect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, 0);
  connect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, SLOT(checkUpdate()));
  d->m_QuietNoUpdate = quietNoUpdate;
  d->mp_Thread->start();
};

void UpdateManager::finalizeUpdate()
{
  Q_D(UpdateManager);
  
  disconnect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, 0);
  connect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, SLOT(finalizeUpdate()));
  d->mp_Thread->start();
};

void UpdateManager::downloadUpdate()
{
  Q_D(UpdateManager);
  disconnect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, 0);
  connect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, SLOT(downloadUpdate()));
  d->mp_InstallerDialog->show();
  d->mp_Thread->start();
};

void UpdateManager::abortDownload()
{
  Q_D(UpdateManager);
  this->resetThread();
  d->mp_InstallerDialog->hide();
};

void UpdateManager::installUpdate()
{
  Q_D(UpdateManager);
  disconnect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, 0);
  connect(d->mp_Thread, SIGNAL(started()), d->mp_Controller, SLOT(installUpdate()));
  
  if (d->m_SeparatedUpdaterWithPrivileges)
  {
    d->mp_Controller->setInstallationPath(QDir::tempPath() + "/" + QCoreApplication::applicationName() + "Updater." + QString::number(QCoreApplication::applicationPid()));
  }
  d->mp_InstallerDialog->titleLabel->setText(tr("Installing update..."));
  d->mp_InstallerDialog->setWindowModality(Qt::ApplicationModal);
  d->mp_InstallerDialog->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  d->mp_InstallerDialog->progressBar->setRange(0,0);
  d->mp_InstallerDialog->progressBar->setValue(-1);
  d->mp_InstallerDialog->show();
  
  d->mp_Thread->start();
};

void UpdateManager::notifyUpdate(const QString& appName, const QString& appCurVer, const QString& appNewVer, const QString& appNote)
{
  Q_D(UpdateManager);
  
  QSettings settings;
  if ((settings.value("Updater/skippedVersion").toString().compare(appNewVer) == 0) && d->m_QuietNoUpdate)
    return;
  
  d->mp_NewVersionDialog->titleLabel->setText("A new version of " + appName + " is available!");
  d->mp_NewVersionDialog->descriptionLabel->setText(appName + " " + appNewVer + " is now available - you have " + appCurVer + ".");
  d->mp_NewVersionDialog->textBrowser->setHtml(appNote);
  d->mp_NewVersionDialog->appNewVer = appNewVer;
  d->mp_NewVersionDialog->textBrowser->verticalScrollBar()->setValue(0);
  
  d->mp_InstallerDialog->setWindowTitle("Updating " + appName);
  d->mp_InstallerDialog->titleLabel->setText(tr("Downloading update..."));
  
  d->mp_NewVersionDialog->exec();
};

void UpdateManager::notifyNoUpdate(const QString& appName, const QString& appCurVer)
{
  Q_D(UpdateManager);
  if (d->m_QuietNoUpdate)
    return;
  QMessageBox msg(QMessageBox::Information, tr("Software Update"), tr("You're up-to-date!"), QMessageBox::Ok, d->mp_NewVersionDialog->parentWidget());
  this->notifyMessage(&msg, appName + " " + appCurVer + tr(" is currently the newest version available."));
};

void UpdateManager::notifyUpdateError()
{
  Q_D(UpdateManager);
  if (d->m_QuietNoUpdate)
    return;
  QMessageBox msg(QMessageBox::Critical, tr("Software Update"), tr("Update Error!"), QMessageBox::Ok, d->mp_NewVersionDialog->parentWidget());
  this->notifyMessage(&msg, tr("An error occurred in retrieving update information."), tr("Please try again later."));
};

void UpdateManager::setDownloadProgress(qint64 receivedBytes, qint64 totalBytes)
{
  Q_D(UpdateManager);
  double received =  static_cast<double>(receivedBytes) / 1024.0; // KB
  if (totalBytes <= 0)
  {
    d->mp_InstallerDialog->progressBar->setRange(0,0);
    d->mp_InstallerDialog->progressBar->setValue(-1);
    d->mp_InstallerDialog->progressLabel->setText(QString("%1 KB of ???").arg(received,0,'f',1));
  }
  else
  {
    double total = static_cast<double>(totalBytes) / 1024.0; // KB
    d->mp_InstallerDialog->progressBar->setRange(0,100);
    d->mp_InstallerDialog->progressBar->setValue(static_cast<int>(received / total * 100.0));
    d->mp_InstallerDialog->progressLabel->setText(QString("%1 KB of %2 KB").arg(received,0,'f',1).arg(total,0,'f',1));
  }
};

void UpdateManager::notifyDownloadError()
{
  Q_D(UpdateManager);
  d->mp_InstallerDialog->hide();
  QMessageBox msg(QMessageBox::Critical, tr("Software Update"), tr("Download Error!"), QMessageBox::Ok, d->mp_InstallerDialog->parentWidget());
  msg.setTextFormat(Qt::RichText);
  this->notifyMessage(&msg, tr("<br/><br/>An error occurred during the verification of the download."), tr("You need to download and install this new release manually.<br/><br/><a href=\"http://b-tk.googlecode.com/svn/web/mokka/index.html\">Click here to download the latest release</a>"));
};

void UpdateManager::notifyReadyToInstall()
{
  Q_D(UpdateManager);
  d->mp_InstallerDialog->titleLabel->setText(tr("... update downloaded"));
  d->mp_InstallerDialog->progressBar->setRange(0,100);
  d->mp_InstallerDialog->progressBar->setValue(100);
  d->mp_InstallerDialog->progressLabel->hide();
  d->mp_InstallerDialog->buttonBox->button(QDialogButtonBox::Ok)->show();
  d->mp_InstallerDialog->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
  d->mp_InstallerDialog->raise();
};

void UpdateManager::notifyInstallationError()
{
  Q_D(UpdateManager);
  d->mp_InstallerDialog->hide();
  this->resetThread();
  QMessageBox msg(QMessageBox::Critical, tr("Software Update"), tr("Install Error!"), QMessageBox::Ok, d->mp_InstallerDialog->parentWidget());
  msg.setTextFormat(Qt::RichText);
  this->notifyMessage(&msg, tr("<br/><br/>An error occurred during the installation of the update."), tr("You need to download and install this new release manually.<br/><br/><a href=\"http://b-tk.googlecode.com/svn/web/mokka/index.html\">Click here to download the latest release</a>"));
};

void UpdateManager::restartApplication()
{
  Q_D(UpdateManager);
  d->mp_InstallerDialog->accept();
  this->resetThread();
  QCoreApplication* app = QCoreApplication::instance();
  QStringList args = app->arguments();
  if (!args.isEmpty() && args[0].replace("\\","/").compare(app->applicationFilePath()) == 0)
    args.removeFirst(); // No need of the path of the application.
  
  // The UAC mechanism appears starting with with Windows Vista and then 
  // need to launch another executable with admin privileges to copy the
  // update in the final folder (especialy true is this folder is "Program files") 
  if (d->m_SeparatedUpdaterWithPrivileges)
  {
    // Path for the external updater
    QString execPath = app->applicationDirPath() + "/" + QCoreApplication::applicationName() + "Updater.exe";
    if (!QFile::exists(execPath))
    {
      QMessageBox msg(QMessageBox::Critical, tr("Software Update"), tr("Install Error!"), QMessageBox::Ok, d->mp_InstallerDialog->parentWidget());
      msg.setTextFormat(Qt::RichText);
      this->notifyMessage(&msg, tr("<br/><br/>Impossible to find the external updater."), tr("You need to download and install this new release manually.<br/><br/><a href=\"http://b-tk.googlecode.com/svn/web/mokka/index.html\">Click here to download the latest release</a>"));
      return;
    }
    // Parameters:
    // 1. Source folder
    // 2. Destination folder
    // 3. Path of the executable to launch after the update
    // 4. Application process ID (PID)
    // 5. Arguments for the executable to launch after the update
    // WARNING: Need to transform space into \" \" for the parameters (See comment "lpParameters and spaces" - http://msdn.microsoft.com/en-us/library/windows/desktop/bb759784%28v=vs.85%29.aspx)
    QString sourceFolder = d->mp_Controller->installationPath(); sourceFolder = sourceFolder.replace(" ", "\" \"");
    QString destinationFolder = QCoreApplication::applicationDirPath().replace(" ", "\" \"");
    QString thirdPid = QString::number(QCoreApplication::applicationPid());
    QString thirdExec = app->applicationFilePath().replace(" ", "\" \"");
    QString thirdArgs = args.join(" ").replace(" ", "\" \"");
    QString params = sourceFolder + " " + destinationFolder + " " + thirdPid + " " + thirdExec + " " + thirdArgs;
    if (this->launchExternalUpdater(execPath, params) != 0)
    {
      QMessageBox msg(QMessageBox::Critical, tr("Software Update"), tr("Install Error!"), QMessageBox::Ok, d->mp_InstallerDialog->parentWidget());
      msg.setTextFormat(Qt::RichText);
      this->notifyMessage(&msg, tr("<br/><br/>An error occurred during the finalization of the update."), tr("You need to download and install this new release manually.<br/><br/><a href=\"http://b-tk.googlecode.com/svn/web/mokka/index.html\">Click here to download the latest release</a>"));
    }
    else
      QCoreApplication::quit();
  }
  else
  {
    QProcess::startDetached(app->applicationFilePath(), args);
    QCoreApplication::quit();
  }
};

void UpdateManager::resetThread()
{
  Q_D(UpdateManager);
  d->mp_Thread->quit();
  d->mp_Thread->wait();
};

void UpdateManager::notifyMessage(QMessageBox* msg, const QString& detail1, const QString& detail2)
{
  Q_D(UpdateManager);
  QPixmap icon(d->m_IconPath);
  if (!icon.isNull())
    msg->setIconPixmap(icon.scaled(64,64));
#ifdef Q_OS_MAC
  QString text = "<nobr>" + detail1 + "</nobr>";
  if (!detail2.isEmpty())
    text += "\n" + detail2;
  msg->setInformativeText(text);
#else
  QString text = msg->text() + "\n\n" + detail1;
  if (!detail2.isEmpty())
    text += "\n" + detail2;
  text += "\n";
  msg->setText(text);
#endif
  msg->exec();
};

int UpdateManager::launchExternalUpdater(const QString& execPath, const QString& params)
{
#ifndef Q_OS_WIN
  Q_UNUSED(execPath);
  Q_UNUSED(params);
#else
  // Without this copy, the LPCSTR (const char*) conversion is invalid (out of the scope) within the SHELLEXECUTEINFO structure
  std::string f = execPath.toStdString();
  std::string p = params.toStdString();

  SHELLEXECUTEINFO execInfo;
  memset(&execInfo, 0, sizeof(SHELLEXECUTEINFO));
  execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
  execInfo.fMask = SEE_MASK_NOASYNC;
  execInfo.hwnd = NULL;
  execInfo.lpFile = (!f.empty() ? f.c_str() : NULL);
  execInfo.lpVerb = "runas";
  execInfo.lpParameters = (!p.empty() ? p.c_str() : NULL);
  execInfo.lpDirectory = NULL;
  execInfo.nShow = SW_SHOW;
  execInfo.hInstApp = NULL;

  if (ShellExecuteEx(&execInfo) == FALSE)
    return -1;
#endif
  return 0;
};
