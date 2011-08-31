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

#include "UpdateChecker.h"

#include <QThread>
#include <QDialog>
#include <QSettings>
#include <QDesktopServices>
#include <QXmlStreamReader>
#include <QScrollBar>
#include <QMessageBox>

#include <QNetworkAccessManager>
#include <QNetworkReply>

UpdateChecker::UpdateChecker(const QString& appVer, const QString& url, const QString& image, QWidget* parent)
: QObject(parent)
{
  this->mp_Dialog = new UpdateCheckerDialog(image, parent);
  this->mp_Thread = new QThread;
  this->mp_Parser = new UpdateParser(appVer, url);
  this->mp_Parser->moveToThread(this->mp_Thread);
  this->m_QuietNoUpdate = false;
  
  connect(this->mp_Thread, SIGNAL(started()), this->mp_Parser, SLOT(check()));
  connect(this->mp_Parser, SIGNAL(updateFound(QString, QString, QString, QString, QString)), this, SLOT(notifyUpdate(QString, QString, QString, QString, QString)));
  connect(this->mp_Parser, SIGNAL(updateNotFound(QString, QString)), this, SLOT(notifyNoUpdate(QString, QString)));
  connect(this->mp_Parser, SIGNAL(updateIncomplete()), this, SLOT(notifyUpdateError()));
  connect(this->mp_Parser, SIGNAL(parsingFinished()), this, SLOT(resetThread()));
}

UpdateChecker::~UpdateChecker()
{
  this->mp_Thread->quit();
  this->mp_Thread->wait();
  delete this->mp_Thread;
  delete this->mp_Parser;
};

void UpdateChecker::check(bool quietNoUpdate)
{
  this->m_QuietNoUpdate = quietNoUpdate;
  this->mp_Thread->start();
};

void UpdateChecker::notifyUpdate(const QString& appName, const QString& appCurVer, const QString& appNewVer, const QString& appNote, const QString& appUrl)
{
  QSettings settings;
  if ((settings.value("Updater/skippedVersion").toString().compare(appNewVer) == 0) && this->m_QuietNoUpdate)
    return;
  
  this->mp_Dialog->titleLabel->setText("A new version of " + appName + " is available!");
  this->mp_Dialog->descriptionLabel->setText(appName + " " + appNewVer + " is now available - you have " + appCurVer + ".");
  this->mp_Dialog->textBrowser->setHtml(appNote);
  this->mp_Dialog->url = appUrl;
  this->mp_Dialog->appNewVer = appNewVer;
  this->mp_Dialog->textBrowser->verticalScrollBar()->setValue(0);
  
  this->mp_Dialog->exec();
};

void UpdateChecker::notifyNoUpdate(const QString& appName, const QString& appCurVer)
{
  if (this->m_QuietNoUpdate)
    return;

  QMessageBox msg(QMessageBox::NoIcon, tr("Software Update"), tr("You're up-to-date!"), QMessageBox::Ok, this->mp_Dialog->parentWidget());
  if (this->mp_Dialog->iconLabel->pixmap() != 0)
    msg.setIconPixmap(this->mp_Dialog->iconLabel->pixmap()->scaled(64,64));
  else
    msg.setIcon(QMessageBox::Information);
  msg.setInformativeText("<nobr>" + appName + " " + appCurVer + tr(" is currently the newest version available.") + "</nobr>");
  msg.exec();
};

void UpdateChecker::notifyUpdateError()
{
  if (this->m_QuietNoUpdate)
    return;

  QMessageBox msg(QMessageBox::NoIcon, tr("Software Update"), tr("Update Error!"), QMessageBox::Ok, this->mp_Dialog->parentWidget());
  if (this->mp_Dialog->iconLabel->pixmap() != 0)
    msg.setIconPixmap(this->mp_Dialog->iconLabel->pixmap()->scaled(64,64));
  else
    msg.setIcon(QMessageBox::Critical);
  msg.setInformativeText("<nobr>An error occurred in retrieving update information.</nobr>\nPlease try again later.");
  msg.exec();
};

void UpdateChecker::resetThread()
{
  this->mp_Thread->quit();
  this->mp_Thread->wait();
};

void UpdateChecker::setIcon(const QString& path)
{
  this->mp_Dialog->iconLabel->setPixmap(QPixmap(path));
};

void UpdateChecker::setUrl(const QString& path)
{
  this->mp_Parser->url = path;
};

// Parser

UpdateParser::UpdateParser(const QString& appVer, const QString& u)
: QObject(NULL), url(u), m_CurrentVersion(appVer.split("."))
{
  this->mp_Manager = 0;
};

void UpdateParser::check()
{
  this->mp_Manager = new QNetworkAccessManager(this);
  connect(this->mp_Manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseReply(QNetworkReply*)));
  this->mp_Manager->get(QNetworkRequest(QUrl(this->url)));
};

void UpdateParser::parseReply(QNetworkReply* reply)
{
  bool updateAvailable = false, updateError = true;  
  QString appName, appNewVer, appLatestNewVer, appNote, appUrl, appPubDate;
  if ((reply->error() == QNetworkReply::NoError) && reply->open(QIODevice::ReadOnly))
  {
    QXmlStreamReader xmlReader(reply);
    if (xmlReader.readNextStartElement())
    {
      appLatestNewVer = this->m_CurrentVersion.join(".");
      if ((xmlReader.name() == "UpdateChecker") && (xmlReader.attributes().value("version") == "1.0"))
      {
        appName = xmlReader.attributes().value("name").toString();
        appUrl = xmlReader.attributes().value("url").toString();
        updateError = false;
        while (xmlReader.readNextStartElement())
        {
          if (xmlReader.name() == "Release")
          {
            appPubDate = xmlReader.attributes().value("pubdate").toString();
            appNewVer = xmlReader.attributes().value("version").toString();
            if (this->isNewRelease(appNewVer.split(".")))
            {
              updateAvailable = true;
              if (this->isGreaterRelease(appNewVer.split("."), appLatestNewVer.split(".")))
                appLatestNewVer = appNewVer;
              appNote += "<div style=\"margin-top:15px;\">";
              appNote += "<div style=\"background-color:#A4DDED;\"><b>&nbsp;" + appPubDate + " - " + appName + " " + appNewVer + " released</b><div>";
              appNote += "<div><ul style=\"margin:5 0 0 -25;\">";
              while (xmlReader.readNextStartElement())
              {
                if (xmlReader.name() == "Enhancement")
                {
                  appNote += "<li><b>Enhancements</b></li><ul style=\"margin:5 0 5 -25;\">";
                  while (xmlReader.readNextStartElement())
                  {
                    if (xmlReader.name() == "item")
                      appNote += "<li>" + xmlReader.readElementText() + "</li>";
                  }
                  appNote += "</ul>";
                }
                else if (xmlReader.name() == "Update")
                {
                  appNote += "<li><b>Updates</b></li><ul style=\"margin:5 0 5 -25;\">";
                  while (xmlReader.readNextStartElement())
                  {
                    if (xmlReader.name() == "item")
                      appNote += "<li>" + xmlReader.readElementText() + "</li>";
                  }
                  appNote += "</ul>";
                }
                else if (xmlReader.name() == "Fix")
                {
                  appNote += "<li><b>Fixes</b></li><ul style=\"margin:5 0 5 -25;\">";
                  while (xmlReader.readNextStartElement())
                  {
                    if (xmlReader.name() == "item")
                      appNote += "<li>" + xmlReader.readElementText() + "</li>";
                  }
                  appNote += "</ul>";
                }
                xmlReader.readNext();
              }
              appNote += "</ul></div>";
              appNote += "</div>";
            }
            else
              xmlReader.skipCurrentElement();
          }
          else
            xmlReader.skipCurrentElement();
        }
      }
    }
    reply->close();
  }
  
  if (updateError)
    emit updateIncomplete();
  else if (updateAvailable)
    emit updateFound(appName, this->m_CurrentVersion.join("."), appLatestNewVer, appNote, appUrl);
  else
    emit updateNotFound(appName, this->m_CurrentVersion.join("."));
  
  emit parsingFinished();
  
  reply->deleteLater();
  reply->manager()->deleteLater();
};

bool UpdateParser::isGreaterRelease(const QStringList& max, const QStringList& rel) const
{
  for (int i = 0 ; i < 3 ; ++i)
  {
    if (!this->isGreaterRelease(max.value(i), rel.value(i)))
      return false;
  }
  return true;
};

bool UpdateParser::isGreaterRelease(const QString& max, const QString& rel) const
{
  int n = qMax(max.count(), rel.count());
  for (int i = 0 ; i < n ; ++i)
  {
    QChar m, r;
    if (i >= max.count())
      m = QChar::fromAscii(0x00);
    else
      m = max[i];
    if (i >= rel.count())
      r = QChar::fromAscii(0x00);
    else
      r = rel[i];
    
    if (r.isDigit())
    {
      if (!m.isDigit())
        return false;
      else if (m.digitValue() < r.digitValue())
        return false;
    }
    else if (!m.isDigit())
    {
      if (m <  r)
        return false;
    }
    else
      break;
  }
  return true;
};

// Dialog

UpdateCheckerDialog::UpdateCheckerDialog(const QString& image, QWidget* parent)
: QDialog(parent), appNewVer(), url()
{
  this->setupUi(this);
  if (!image.isEmpty())
    this->iconLabel->setPixmap(QPixmap(image));
#ifdef Q_OS_MAC
  QFont f = this->noteLabel->font(); // Bold by default
  f.setPointSize(11);
  this->noteLabel->setFont(f);
  f = this->descriptionLabel->font();
  f.setPointSize(11);
  this->descriptionLabel->setFont(f);
  f = this->textBrowser->font();
  f.setPointSize(11);
  this->textBrowser->setFont(f);
#endif
  
  connect(this->downloadButton, SIGNAL(clicked(bool)), this, SLOT(download()));
  connect(this->remindButton, SIGNAL(clicked(bool)), this, SLOT(remind()));
  connect(this->skipButton, SIGNAL(clicked(bool)), this, SLOT(skip()));
};

void UpdateCheckerDialog::download()
{
  QSettings settings;
  settings.setValue("Updater/skippedVersion", "");
  QDesktopServices::openUrl(QUrl(this->url, QUrl::TolerantMode));
  this->accept();
};

void UpdateCheckerDialog::remind()
{
  QSettings settings;
  settings.setValue("Updater/skippedVersion", "");
  this->reject();
};

void UpdateCheckerDialog::skip()
{
  QSettings settings;
  settings.setValue("Updater/skippedVersion", this->appNewVer);
  this->reject();
};