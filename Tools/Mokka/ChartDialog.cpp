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

#include "ChartDialog.h"
#include "ChartWidget.h"
#include "Acquisition.h"

#include <QBoxLayout>

ChartDialog::ChartDialog(QWidget* parent)
: QDialog(parent)
{
  this->chart = new ChartWidget(this);
  this->chart->initialize();
  QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(this->chart);
  
  this->resize(640,480);
};

bool ChartDialog::computeDistance(int id1, int id2)
{
  Acquisition* acq = this->chart->acquisition();
  QMap<int,Point*> points = acq->points();
  btk::PointCollection::Pointer btkPoints = acq->btkAcquisition()->GetPoints();
  // Compute
  btk::Point::Pointer pt1, pt2;
  std::string desc = "Distance between the markers ";
  QMap<int,Point*>::const_iterator it1 = points.find(id1);
  QMap<int,Point*>::const_iterator it2 = points.find(id2);
  if ((it1 != points.end()) && (it2 != points.end()))
  {
    if (((*it1)->btkidx < btkPoints->GetItemNumber())
        && ((*it2)->btkidx < btkPoints->GetItemNumber()))
    {
      pt1 = btkPoints->GetItem((*it1)->btkidx);
      pt2 = btkPoints->GetItem((*it2)->btkidx);
      desc += pt1->GetLabel() + " and " + pt2->GetLabel();
    }
    else
    {
      qDebug("At least one of the selected markers was not found in BTK!");
      return false;
    }
  }
  else
  {
    qDebug("At least one of the selected markers was not found!");
    return false;
  }
  btk::Point::Pointer diff = btk::Point::New("Dist*", acq->pointFrameNumber(), btk::Point::Marker, desc);
  for (int i = 0  ; i < acq->pointFrameNumber() ; ++i)
  {
    if ((pt1->GetResiduals().coeff(i) < 0.0) || (pt2->GetResiduals().coeff(i) < 0.0))
      diff->SetFrame(i, 0.0, 0.0, 0.0, -1.0, -1.0);
    else
    {
      diff->SetFrame(i, pt2->GetValues().coeff(i,0) - pt1->GetValues().coeff(i,0),
                        pt2->GetValues().coeff(i,1) - pt1->GetValues().coeff(i,1),
                        pt2->GetValues().coeff(i,2) - pt1->GetValues().coeff(i,2));
    } 
  }
  btk::Point::Residuals norm = diff->GetValues().rowwise().norm();
  diff->GetValues().setZero();
  diff->GetValues().col(0) = norm;
  // Display the data
  this->chart->displayPointChart();
  this->chart->displayPointComponentX(Qt::Checked);
  this->chart->displayPointComponentY(Qt::Unchecked);
  this->chart->displayPointComponentZ(Qt::Unchecked);
  this->chart->setPointUnitAxisY("Distance (" + acq->pointUnit(Point::Marker) + ")", "", "");
  this->chart->addPointPlot(diff, "Y");
  this->setWindowTitle(QString::fromStdString(desc));
  
  return true;
};

bool ChartDialog::computeAngleFromMarkers(int id1, int id2, int id3)
{
  Acquisition* acq = this->chart->acquisition();
  QMap<int,Point*> points = acq->points();
  btk::PointCollection::Pointer btkPoints = acq->btkAcquisition()->GetPoints();
  // Compute
  btk::Point::Pointer pt1, pt2, pt3;
  std::string desc = "Angle between the markers ";
  QMap<int,Point*>::const_iterator it1 = points.find(id1);
  QMap<int,Point*>::const_iterator it2 = points.find(id2);
  QMap<int,Point*>::const_iterator it3 = points.find(id3);
  if ((it1 != points.end()) && (it2 != points.end()) && (it3 != points.end()))
  {
    if (((*it1)->btkidx < btkPoints->GetItemNumber())
        && ((*it2)->btkidx < btkPoints->GetItemNumber())
        && ((*it3)->btkidx < btkPoints->GetItemNumber()))
    {
      pt1 = btkPoints->GetItem((*it1)->btkidx);
      pt2 = btkPoints->GetItem((*it2)->btkidx);
      pt3 = btkPoints->GetItem((*it3)->btkidx);
      desc += pt1->GetLabel() + ", " + pt2->GetLabel() + " and " + pt3->GetLabel();
    }
    else
    {
      qDebug("At least one of the selected markers was not found in BTK!");
      return false;
    }
  }
  else
  {
    qDebug("At least one of the selected markers was not found!");
    return false;
  }
  btk::Point::Pointer ang = btk::Point::New("Angle*", acq->pointFrameNumber(), btk::Point::Angle, desc);
  for (int i = 0  ; i < acq->pointFrameNumber() ; ++i)
  {
    if ((pt1->GetResiduals().coeff(i) < 0.0) || (pt2->GetResiduals().coeff(i) < 0.0) || (pt3->GetResiduals().coeff(i) < 0.0))
      ang->SetFrame(i, 0.0, 0.0, 0.0, -1.0, -1.0);
    else
    {
      Eigen::Matrix<double,3,1> a, b;
      a << pt1->GetValues().coeff(i,0)-pt2->GetValues().coeff(i,0), pt1->GetValues().coeff(i,1)-pt2->GetValues().coeff(i,1), pt1->GetValues().coeff(i,2)-pt2->GetValues().coeff(i,2);
      b << pt3->GetValues().coeff(i,0)-pt2->GetValues().coeff(i,0), pt3->GetValues().coeff(i,1)-pt2->GetValues().coeff(i,1), pt3->GetValues().coeff(i,2)-pt2->GetValues().coeff(i,2);
      double angle = acos(a.dot(b) / a.norm() / b.norm()) * 180.0 / M_PI;
      ang->SetFrame(i, angle, 0.0, 0.0);
    } 
  }
  // Display the data
  this->chart->displayPointChart();
  this->chart->displayPointComponentX(Qt::Checked);
  this->chart->displayPointComponentY(Qt::Unchecked);
  this->chart->displayPointComponentZ(Qt::Unchecked);
  this->chart->setPointUnitAxisY("Angle (" + acq->pointUnit(Point::Angle) + ")", "", "");
  this->chart->addPointPlot(ang, "Y");
  this->setWindowTitle(QString::fromStdString(desc));
  
  return true;
};

bool ChartDialog::computeAngleFromVectors(const QList<int>& ids)
{
  Acquisition* acq = this->chart->acquisition();
  QMap<int,Point*> points = acq->points();
  btk::PointCollection::Pointer btkPoints = acq->btkAcquisition()->GetPoints();
  // Compute
  btk::Point::Pointer pt1, pt2, pt3, pt4;
  std::string desc = "Angle between the vectors ";
  QMap<int,Point*>::const_iterator it1 = points.find(ids[0]);
  QMap<int,Point*>::const_iterator it2 = points.find(ids[1]);
  QMap<int,Point*>::const_iterator it3 = points.find(ids[2]);
  QMap<int,Point*>::const_iterator it4;
  if (ids.size() == 4)
    it4 = points.find(ids[3]);
  else
  {
    it4 = it3;
    it3 = it2;
  }
  if ((it1 != points.end()) && (it2 != points.end()) && (it3 != points.end()) && (it4 != points.end()))
  {
    if (((*it1)->btkidx < btkPoints->GetItemNumber())
        && ((*it2)->btkidx < btkPoints->GetItemNumber())
        && ((*it3)->btkidx < btkPoints->GetItemNumber())
        && ((*it4)->btkidx < btkPoints->GetItemNumber()))
    {
      pt1 = btkPoints->GetItem((*it1)->btkidx);
      pt2 = btkPoints->GetItem((*it2)->btkidx);
      pt3 = btkPoints->GetItem((*it3)->btkidx);
      pt4 = btkPoints->GetItem((*it4)->btkidx);
      desc += "(" + pt1->GetLabel() + ", " + pt2->GetLabel() + ") and (" + pt3->GetLabel() + ", " + pt4->GetLabel() + ")";
    }
    else
    {
      qDebug("At least one of the selected markers was not found in BTK!");
      return false;
    }
  }
  else
  {
    qDebug("At least one of the selected markers was not found!");
    return false;
  }
  btk::Point::Pointer ang = btk::Point::New("Angle*", acq->pointFrameNumber(), btk::Point::Angle, desc);
  for (int i = 0  ; i < acq->pointFrameNumber() ; ++i)
  {
    if ((pt1->GetResiduals().coeff(i) < 0.0) || (pt2->GetResiduals().coeff(i) < 0.0)
        || (pt3->GetResiduals().coeff(i) < 0.0) || (pt4->GetResiduals().coeff(i) < 0.0))
      ang->SetFrame(i, 0.0, 0.0, 0.0, -1.0, -1.0);
    else
    {
      Eigen::Matrix<double,3,1> a, b;
      a << pt2->GetValues().coeff(i,0)-pt1->GetValues().coeff(i,0), pt2->GetValues().coeff(i,1)-pt1->GetValues().coeff(i,1), pt2->GetValues().coeff(i,2)-pt1->GetValues().coeff(i,2);
      b << pt4->GetValues().coeff(i,0)-pt3->GetValues().coeff(i,0), pt4->GetValues().coeff(i,1)-pt3->GetValues().coeff(i,1), pt4->GetValues().coeff(i,2)-pt3->GetValues().coeff(i,2);
      double angle = acos(a.dot(b) / a.norm() / b.norm()) * 180.0 / M_PI;
      ang->SetFrame(i, angle, 0.0, 0.0);
    } 
  }
  // Display the data
  this->chart->displayPointChart();
  this->chart->displayPointComponentX(Qt::Checked);
  this->chart->displayPointComponentY(Qt::Unchecked);
  this->chart->displayPointComponentZ(Qt::Unchecked);
  this->chart->setPointUnitAxisY("Angle (" + acq->pointUnit(Point::Angle) + ")", "", "");
  this->chart->addPointPlot(ang, "Y");
  this->setWindowTitle(QString::fromStdString(desc));
  
  return true;
};

void ChartDialog::updateChartRendering()
{
  this->chart->render();
};