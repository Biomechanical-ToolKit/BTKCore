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

#include "btkVTKAxis.h"

#include <vtkObjectFactory.h>
#include <vtkContext2D.h>
#include <vtkTextProperty.h>
#include <vtkFloatArray.h>
#include <vtkStdString.h>
#include <vtkStringArray.h>
#include <vtkPen.h>
#include <vtkTransform2D.h>
#include <vtkDoubleArray.h>
#include <vtkStringArray.h>
#include <vtkStdString.h>

#include <vtksys/ios/sstream>

namespace btk
{
  /**
   * @class VTKAxis btkVTKAxis.h
   * @brief Class to display axis into the chart with ticks drawn inside the chart.
   *
   * The direction of the ticks can be set with the method SetTickDirection().
   * This class take into account the transformation given to the object to paint correctly the legend into the scene (only the scale and translation are supported).
   */
  /**
   * @var VTKAxis::m_TitleVisible
   * Status for the visibility of the title.
   */
  /**
   * @var VTKAxis::m_TickLength
   * Length in pixel of the ticks.
   */
  /**
   * @var VTKAxis::m_TickDirection
   * Direction of the ticks. Can be set to be drawn inside or outside the chart.
   */
  /**
   * @var VTKAxis::m_MinimumTickSpacing
   * Minimum space between two ticks in pixels.
   */
  /**
   * @var VTKAxis::m_TitleMargin
   * Size of the margin (in pixels) between the border and the title.
   */
  /**
   * @var VTKAxis::m_LabelMargin
   * Size of the margin (in pixels) between the axis and the labels.
   */

  /**
   * @var VTKAxis::INSIDE
   * To draw the ticks inside the charts.
   */
  /**
   * @var VTKAxis::OUTSIDE
   * To draw the ticks outside the charts.
   */
  
  /**
   * @fn static VTKAxis* VTKAxis::New()
   * Constructs a VTKAxis object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKAxis);
  vtkCxxRevisionMacro(VTKAxis, "$Revision: 0.1 $");
  
  /**
   * @fn virtual VTKAxis::~VTKAxis();
   * Destructor.
   */
   
  /**
   * @fn float VTKAxis::GetTitleVisible() const
   * Returns the status for the visibility of the title.
   */
  
  /**
   * Sets the status for the visibility of the title.
   */
  void VTKAxis::SetTitleVisible(bool visible)
  {
    if (this->m_TitleVisible == visible)
      return;
    this->m_TitleVisible = visible;
    this->Modified();
  };
  
  /**
   * @fn float VTKAxis::GetTickLength() const
   * Returns the length of the ticks.
   */
  
  /**
   * Sets the length of the ticks.
   */
  void VTKAxis::SetTickLength(float len)
  {
    if (this->m_TickLength == len)
      return;
    this->m_TickLength = len;
    this->TickMarksDirty = true;
    this->Modified();
  };
  
  /**
   * @fn int VTKAxis::GetTickDirection() const
   * Returns the direction of the ticks. 
   *
   * The value 0 correspond to the enum value INSIDE. The value 1 correspond to the enum value OUTSIDE.
   */ 

  /**
   * Set the direction of this ticks related to the chart.
   * Use the enum values INSIDE and OUTSIDE.
   */
  void VTKAxis::SetTickDirection(int dir)
  {
    if (this->m_TickDirection == dir)
      return;
    this->m_TickDirection = dir;
    this->TickMarksDirty = true;
    this->Modified();
  };
  
  /**
   * @fn float VTKAxis::GetMinimumTickSpacing() const
   * Returns the minimum spacing between two ticks. The spacing is in pixels.
   */
  
  /**
   * Sets the minimum spacing between two ticks. The spacing is in pixels. 
   */
  void VTKAxis::SetMinimumTickSpacing(float min)
  {
    if (this->m_MinimumTickSpacing == min)
      return;
    this->m_MinimumTickSpacing = min;
    this->TickMarksDirty = true;
    this->Modified();
  };
  
  /**
   * @fn float VTKAxis::GetTitleMargin() const
   * Returns the size of the margin between the border and the title. The size is in pixels.
   */
  
  /**
   * Sets the size of the margin between the border and the title. The size is in pixels.
   */
  void VTKAxis::SetTitleMargin(float margin)
  {
    if (this->m_TitleMargin == margin)
      return;
    this->m_TitleMargin = margin;
    this->Modified();
  };
  
  /**
   * @fn float VTKAxis::GetLabelMargin() const
   * Returns the size of the margin between the axis and the labels. The size is in pixels.
   */
  
  /**
   * Sets the size of the margin between the axis and the labels. The size is in pixels.
   */
  void VTKAxis::SetLabelMargin(float margin)
  {
    if (this->m_LabelMargin == margin)
      return;
    this->m_LabelMargin = margin;
    this->Modified();
  };
  
  /**
   * Draw the axis in the scene.
   */
  bool VTKAxis::Paint(vtkContext2D *painter)
  {
    // FIXME: THIS CODE MUST BE ADAPTED FOR VTK 5.8 AS THE TEXT PROPERTIES ARE NO MORE SET IN THE PAINT METHOD.
    if (!this->Visible)
      return false;

    painter->ApplyPen(this->Pen);

    // Draw this axis
    painter->DrawLine(this->Point1[0], this->Point1[1], this->Point2[0], this->Point2[1]);
    vtkTextProperty* prop = painter->GetTextProp();

    // Draw the axis title if there is one
    if (this->m_TitleVisible && this->Title && this->Title[0])
    {
      float x = 0.0f;
      float y = 0.0f;
      painter->ApplyTextProp(this->TitleProperties);

      if (this->Position == vtkAxis::LEFT)
      {
        // Draw the axis label
        x = this->m_TitleMargin; //  + bounds[3] // this->Point1[0] - 35
        y = (this->Point1[1] + this->Point2[1]) / 2.0f;
        prop->SetOrientation(90.0);
        prop->SetVerticalJustificationToTop(); // SetVerticalJustificationToBottom();
      }
      else if (this->Position == vtkAxis::RIGHT)
      {
        x = this->Point1[0] + 45.0f; // MUST BE UPDATED
        y = (this->Point1[1] + this->Point2[1]) / 2.0f;
        prop->SetOrientation(90.0);
        prop->SetVerticalJustificationToTop();
      }
      else if (this->Position == vtkAxis::BOTTOM)
      {
        x = (this->Point1[0] + this->Point2[0]) / 2.0f;
        y = this->m_TitleMargin; // this->Point1[1] - 30
        prop->SetOrientation(0.0);
        prop->SetVerticalJustificationToBottom(); // SetVerticalJustificationToTop();
      }
      else if (this->Position == vtkAxis::TOP)
      {
        x = (this->Point1[0] + this->Point2[0]) / 2;
        y = this->Point1[1] + 30.0f; // MUST BE UPDATED
        prop->SetOrientation(0.0);
        prop->SetVerticalJustificationToBottom();
      }
      else if (this->Position == vtkAxis::PARALLEL)
      {
        x = this->Point1[0];  // MUST BE UPDATED
        y = this->Point1[1] - 10.0f;  // MUST BE UPDATED
        prop->SetOrientation(0.0);
        prop->SetVerticalJustificationToTop();
      }

      float pt[2] = {x, y};
      if (this->GetTransform() != NULL)
        this->GetTransform()->TransformPoints(pt, pt, 1);
      painter->DrawString(pt[0], pt[1], this->Title);
    }

    // Now draw the tick marks
    painter->ApplyTextProp(this->LabelProperties);

    float* tickPos = this->TickScenePositions->GetPointer(0);
    vtkStdString *tickLabel = this->TickLabels->GetPointer(0);
    vtkIdType numMarks = this->TickScenePositions->GetNumberOfTuples();
    
    // There are four possible tick label positions, which should be set by the
    // class laying out the axes.
    if (this->Position == vtkAxis::LEFT || this->Position == vtkAxis::PARALLEL)
    {
      prop->SetJustificationToRight();
      prop->SetVerticalJustificationToCentered();

      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(this->Point1[0] + (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[0], tickPos[i]);
        if (this->LabelsVisible)
        {
          float pt[2] = {this->Point1[0] - this->m_LabelMargin, tickPos[i]};
          if (this->GetTransform() != NULL)
            this->GetTransform()->TransformPoints(pt, pt, 1);
          painter->DrawString(pt[0], pt[1], tickLabel[i]);
        }
      }
    }
    else if (this->Position == vtkAxis::RIGHT)
    {
      prop->SetJustificationToLeft();
      prop->SetVerticalJustificationToCentered();

      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(this->Point1[0] - (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[0],     tickPos[i]);
        if (this->LabelsVisible)
        {
          float pt[2] = {this->Point1[0] + this->m_LabelMargin, tickPos[i]};
          if (this->GetTransform() != NULL)
            this->GetTransform()->TransformPoints(pt, pt, 1);
          painter->DrawString(pt[0], pt[1], tickLabel[i]);
        }
      }
    }
    else if (this->Position == vtkAxis::BOTTOM)
    {
      prop->SetJustificationToCentered();
      prop->SetVerticalJustificationToTop();

      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(tickPos[i], this->Point1[1] + (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[1]);
        if (this->LabelsVisible)
        {
          float pt[2] = {tickPos[i], this->Point1[1] - this->m_LabelMargin};
          if (this->GetTransform() != NULL)
            this->GetTransform()->TransformPoints(pt, pt, 1);
          painter->DrawString(pt[0], pt[1], tickLabel[i]);
        }
      }
    }
    else if (this->Position == vtkAxis::TOP)
    {
      prop->SetJustificationToCentered();
      prop->SetVerticalJustificationToBottom();

      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(tickPos[i], this->Point1[1] - (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[1]);
        if (this->LabelsVisible)
        {
          float pt[2] = {tickPos[i], this->Point1[1] + this->m_LabelMargin};
          if (this->GetTransform() != NULL)
            this->GetTransform()->TransformPoints(pt, pt, 1);
          painter->DrawString(pt[0], pt[1], tickLabel[i]);
        }
      }
    }

    return true;
  };
  
  /**
   * Calculate the tick spacing and generate the ticks label taking into account the offset and scale.
   */
  // TODO: Check the method used with VTK 5.8.
  void VTKAxis::RecalculateTickSpacing()
  {
    if (this->Behavior < 2)
    {
      double min = this->Minimum;
      double max = this->Maximum;
      this->TickInterval = this->CalculateNiceMinMax(min, max);
      if (this->UsingNiceMinMax)
        this->GenerateTickLabels(this->Minimum, this->Maximum);
      else if (this->TickInterval == 0)
        return;
      else
      {
        while (min < this->Minimum)
          min += this->TickInterval;
        while (max > this->Maximum)
          max -= this->TickInterval;
        this->GenerateTickLabels(min, max);
      }
    }
  };
  
  /**
   * @fn double GetTickScale() const
   * Returns the scale used by the ticks.
   */
  
  /**
   * Sets the scale used by the ticks.
   *
   * Combine with the offset, you can adapt the value of the tick using the formula (value + offset) * scale.
   * This can be useful if you want to switch easily between unit without generating new plots.
   */
  void VTKAxis::SetTickScale(double scale)
  {
    if (this->m_TickScale == scale)
      return;
    this->m_TickScale = scale;
    this->TickMarksDirty = true;
    this->Modified();
  };
  
  /**
   * @fn double GetTickOffset() const
   * Returns the offset used by the ticks.
   */
  
  /**
   * Sets the offset used by the ticks.
   *
   * Combine with the scale, you can adapt the value of the tick using the formula (value + offset) * scale.
   * This can be useful if you want to switch easily between unit without generating new plots.
   */
  void VTKAxis::SetTickOffset(double offset)
  {
    if (this->m_TickOffset == offset)
      return;
    this->m_TickOffset = offset;
    this->TickMarksDirty = true;
    this->Modified();
  };
  
  /**
   * Default constructor
   */
  VTKAxis::VTKAxis()
  : vtkAxis()
  {
    this->m_TitleVisible = true;
    this->Pen->SetWidth(0.5);
    this->GridPen->SetWidth(0.5);
    this->m_TickLength = 5.0f;
    this->m_TickDirection = VTKAxis::INSIDE;
    this->m_MinimumTickSpacing = 50.0f;
    this->m_TitleMargin = 10.0f;
    this->m_LabelMargin = 5.0f;
    this->m_TickScale = 1.0;
    this->m_TickOffset = 0.0;
  };
  
  void VTKAxis::GenerateTickLabels(double min, double max)
  {
    this->TickPositions->SetNumberOfTuples(0);
    this->TickLabels->SetNumberOfTuples(0);
    int n = static_cast<int>((max - min) / this->TickInterval);
    for (int i = 0; i <= n && i < 200; ++i)
    {
      double value = min + double(i) * this->TickInterval - this->m_TickOffset;
      this->TickPositions->InsertNextValue(value);
      // Make a tick mark label for the tick
      if (this->LogScale)
        value = pow(double(10.0), double(value));

      // Now create a label for the tick position
      vtksys_ios::ostringstream ostr;
      ostr.imbue(vtkstd::locale::classic());
      if (this->Notation > 0)
        ostr.precision(this->Precision);
      if (this->Notation == 1)
      {
        // Scientific notation
        ostr.setf(vtksys_ios::ios::scientific, vtksys_ios::ios::floatfield);
      }
      else if (this->Notation == 2)
      {
        ostr.setf(ios::fixed, ios::floatfield);
      }
      ostr << (value + this->m_TickOffset) * this->m_TickScale;

      this->TickLabels->InsertNextValue(ostr.str());
    }
    this->TickMarksDirty = false;
  };
  
  /**
   * Compute the number of ticks and the spacing between them.
   */
  double VTKAxis::CalculateNiceMinMax(double& min, double& max)
  {
    // First get the order of the range of the numbers
    if (this->Maximum == this->Minimum)
    {
      this->Minimum *= 0.95;
      this->Maximum *= 1.05;
    }
    else if ((this->Maximum - this->Minimum) < 1.0e-20)
    {
      this->Minimum *= 0.95;
      this->Maximum *= 1.05;
    }

    double range = this->Maximum - this->Minimum;
    bool isNegative = false;
    if (range < 0.0f)
    {
      isNegative = true;
      range *= -1.0f;
    }

    // Calculate an upper limit on the number of tick marks - at least 30 pixels
    // should be between each tick mark.
    float pt[4] = {this->Point1[0], this->Point1[1], this->Point2[0], this->Point2[1]};
    if (this->GetTransform() != NULL)
      this->GetTransform()->TransformPoints(pt, pt, 2);
    float pixelRange = pt[0] == pt[2] ?
                       pt[3] - pt[1] :
                       pt[2] - pt[0];
    int maxTicks = static_cast<int>(pixelRange / this->m_MinimumTickSpacing);
    if (maxTicks == 0)
    {
      // The axes do not have a valid set of points - return
      return 0.0f;
    }
    double tickSpacing = range / maxTicks;

    int order = static_cast<int>(floor(log10(tickSpacing)));
    double normTickSpacing = tickSpacing * pow(10.0f, -order);
    double niceTickSpacing = this->NiceNumber(normTickSpacing, true);
    niceTickSpacing *= pow(10.0f, order);
    // if (niceTickSpacing < this->m_MinimumTickSpacing)
    //   niceTickSpacing = this->m_MinimumTickSpacing;

    if (isNegative)
    {
      min = ceil(this->Minimum / niceTickSpacing) * niceTickSpacing;
      max = floor(this->Maximum / niceTickSpacing) * niceTickSpacing;
    }
    else
    {
      min = floor(this->Minimum / niceTickSpacing) * niceTickSpacing;
      max = ceil(this->Maximum / niceTickSpacing) * niceTickSpacing;
    }

    float newRange = max - min;
    this->NumberOfTicks = static_cast<int>(floor(newRange / niceTickSpacing)) + 1;

    return niceTickSpacing;
  };
};