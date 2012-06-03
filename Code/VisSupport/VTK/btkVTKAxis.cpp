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
   *
   * The value for the ticks' label can be influenced by an offset and a scale using the following operation: tick_label = (real_value + offset) * scale.
   * To set the scale and the offset, you have to use the methods SetTickScale() and SetTickOffset() respectively
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
   * @var VTKAxis::m_TickScale
   * Value used to modify the ticks' label. Set to 1.0 by default.
   */
  /**
   * @var VTKAxis::m_TickOffset
   * Value used to modify the ticks' label. Set to 0.0 by default.
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
   * Destructor.
   */
  VTKAxis::~VTKAxis()
  {};
   
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
  // void VTKAxis::SetMinimumTickSpacing(float min)
  // {
  //   if (this->m_MinimumTickSpacing == min)
  //     return;
  //   this->m_MinimumTickSpacing = min;
  //   this->TickMarksDirty = true;
  //   this->Modified();
  // };
  
  /**
   * @fn float VTKAxis::GetTitleMargin() const
   * Returns the size of the margin between the border and the title. The size is in pixels.
   */
  
  /**
   * Sets the size of the margin between the border and the title. The size is in pixels.
   */
  // void VTKAxis::SetTitleMargin(float margin)
  // {
  //   if (this->m_TitleMargin == margin)
  //     return;
  //   this->m_TitleMargin = margin;
  //   this->Modified();
  // };
  
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
  
  // void VTKAxis::Update()
  // {};
      
  /**
   * Draw the axis in the scene.
   */
  bool VTKAxis::Paint(vtkContext2D *painter)
  {
    if (!this->Visible)
      return false;

    painter->ApplyPen(this->Pen);

    // Draw this axis
    painter->DrawLine(this->Point1[0], this->Point1[1], this->Point2[0], this->Point2[1]);
    
    if (this->m_TitleVisible && this->Title && !this->Title.empty())
    {
      int x = 0;
      int y = 0;
      painter->ApplyTextProp(this->TitleProperties);
      // Draw the axis label
      if (this->Position == vtkAxis::LEFT)
      {
        // Draw the axis label
        // x = vtkContext2D::FloatToInt(this->Point1[0] - this->MaxLabel[0] - this->Margins[0]);
        x = vtkContext2D::FloatToInt(this->Margins[0]);
        y = vtkContext2D::FloatToInt(this->Point1[1] + this->Point2[1]) / 2;
      }
      else if (this->Position == vtkAxis::RIGHT)
      {
        // Draw the axis label
        // x = vtkContext2D::FloatToInt(this->Point1[0] + this->MaxLabel[0] + this->Margins[0]);
        x = vtkContext2D::FloatToInt(this->Point1[0] + 45.0f); // MUST BE UPDATED
        y = vtkContext2D::FloatToInt(this->Point1[1] + this->Point2[1]) / 2;
      }
      else if (this->Position == vtkAxis::BOTTOM)
      {
        x = vtkContext2D::FloatToInt(this->Point1[0] + this->Point2[0]) / 2;
        // y = vtkContext2D::FloatToInt(this->Point1[1] - this->MaxLabel[1] - this->Margins[1]);
        y = vtkContext2D::FloatToInt(this->Margins[1]); // this->Point1[1] - 30  // MUST BE UPDATED
      }
      else if (this->Position == vtkAxis::TOP)
      {
        x = vtkContext2D::FloatToInt(this->Point1[0] + this->Point2[0]) / 2;
        // y = vtkContext2D::FloatToInt(this->Point1[1] + this->MaxLabel[1] + this->Margins[1]);
        y = vtkContext2D::FloatToInt(this->Point1[1] + 30.0f); // MUST BE UPDATED
      }
      else if (this->Position == vtkAxis::PARALLEL)
      {
        x = vtkContext2D::FloatToInt(this->Point1[0]);
        y = vtkContext2D::FloatToInt(this->Point1[1] - this->MaxLabel[1] - this->Margins[1]);
      }
      painter->DrawString(x, y, this->Title);
    }

    // Now draw the tick marks
    painter->ApplyTextProp(this->LabelProperties);

    float* tickPos = this->TickScenePositions->GetPointer(0);
    vtkStdString* tickLabel = this->TickLabels->GetPointer(0);
    vtkIdType numMarks = this->TickScenePositions->GetNumberOfTuples();
    
    // There are four possible tick label positions, which should be set by the
    // class laying out the axes.
    if (this->Position == vtkAxis::LEFT || this->Position == vtkAxis::PARALLEL)
    {
      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(this->Point1[0] + (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[0], tickPos[i]);
        if (this->LabelsVisible)
          painter->DrawString(this->Point1[0]-this->m_LabelMargin, tickPos[i], tickLabel[i]);
      }
    }
    else if (this->Position == vtkAxis::RIGHT)
    {
      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(this->Point1[0] - (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[0], tickPos[i]);
        if (this->LabelsVisible)
          painter->DrawString(this->Point1[0]+this->m_LabelMargin, tickPos[i], tickLabel[i]);
      }
    }
    else if (this->Position == vtkAxis::BOTTOM)
    {
      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(tickPos[i], this->Point1[1] + (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[1]);
        if (this->LabelsVisible)
          painter->DrawString(tickPos[i], this->Point1[1]-this->m_LabelMargin, tickLabel[i]);
      }
    }
    else if (this->Position == vtkAxis::TOP)
    {
      for (vtkIdType i = 0; i < numMarks; ++i)
      {
        painter->DrawLine(tickPos[i], this->Point1[1] - (this->m_TickDirection == INSIDE ? this->m_TickLength : -this->m_TickLength), tickPos[i], this->Point1[1]);
        if (this->LabelsVisible)
          painter->DrawString(tickPos[i], this->Point1[1]+this->m_LabelMargin, tickLabel[i]);
      }
    }

    return true;
  };
  
  /**
   * @fn double VTKAxis::GetTickScale() const
   * Returns the scale used by the ticks.
   */
  
  /**
   * Sets the scale used by the ticks.
   *
   * Combine with the offset, you can adapt the value of the tick using the formula (value + offset) * scale.
   * This can be useful if you want to switch easily between units without generating new plots.
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
   * @fn double VTKAxis::GetTickOffset() const
   * Returns the offset used by the ticks.
   */
  
  /**
   * Sets the offset used by the ticks.
   *
   * Combine with the scale, you can adapt the value of the tick using the formula (value + offset) * scale.
   * This can be useful if you want to switch easily between units without generating new plots.
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
    // this->m_MinimumTickSpacing = 50.0f;
    // this->m_TitleMargin = 10.0f;
    this->m_LabelMargin = 5.0f;
    this->m_TickScale = 1.0;
    this->m_TickOffset = 0.0;
  };
  
  /**
   * Generate the position of the ticks and their label.
   *
   * The value for the label can be influenced by an offset and a scale using the following operation: tick_label = (real_value + offset) * scale.
   * To set the scale and the offset, you have to use the methods SetTickScale() and SetTickOffset() respectively
   */
  void VTKAxis::GenerateTickLabels(double min, double max)
  {
    std::cout << "VTKAxis::GenerateTickLabels(min,max)" << std::endl;
    this->vtkAxis::GenerateTickLabels(min,max);
    /*
    // Now calculate the tick labels, and positions within the axis range
    this->TickPositions->SetNumberOfTuples(0);
    this->TickLabels->SetNumberOfTuples(0);

    // We generate a logarithmic scale when logarithmic axis is activated and the
    // order of magnitude of the axis is higher than 0.6.
    if (this->LogScale && this->LogScaleReasonable)
    {
      // We calculate the first tick mark for lowest order of magnitude.
      // and the last for the highest order of magnitude.
      this->TickInterval = this->CalculateNiceMinMax(min, max);

      bool niceTickMark = false;
      int minOrder = 0;
      int maxOrder = 0;
      double minValue = this->LogScaleTickMark(pow(double(10.0), double(min)),
                                         true,
                                         niceTickMark,
                                         minOrder);
      double maxValue = this->LogScaleTickMark(pow(double(10.0), double(max)),
                                         false,
                                         niceTickMark,
                                         maxOrder);

      // We generate the tick marks for all orders of magnitude
      if (maxOrder - minOrder == 0)
        this->GenerateLogScaleTickMarks(minOrder, minValue, maxValue);
      else
      {
        if (maxOrder - minOrder + 1 > 5)
        {
          this->GenerateLogScaleTickMarks(minOrder, minValue, 9.0, false);
          for(int i = minOrder + 1; i < maxOrder; ++i)
            this->GenerateLogScaleTickMarks(i, 1.0, 9.0, false);
          this->GenerateLogScaleTickMarks(maxOrder, 1.0, maxValue, false);
        }
        else
        {
          this->GenerateLogScaleTickMarks(minOrder, minValue, 9.0);
          for(int i = minOrder + 1; i < maxOrder; ++i)
            this->GenerateLogScaleTickMarks(i, 1.0, 9.0);
          this->GenerateLogScaleTickMarks(maxOrder, 1.0, maxValue);
        }
      }
    }
    else
    {
      if (this->TickLabelAlgorithm == vtkAxis::TICK_WILKINSON_EXTENDED)
      {
        // Now calculate the tick labels, and positions within the axis range
        //This gets the tick interval and max, min of labeling from the Extended
        // algorithm
        double scaling = 0.0;
        bool axisVertical = false;

        // When the axis is not initialized
        if(this->Point1[0] == 0 && this->Point2[0] == 0)
        {
          // 500 is an intial guess for the length of the axis in pixels
          scaling = 500 / (this->Maximum - this->Minimum);
        }
        else
        {
          if (this->Point1[0] == this->Point2[0]) // x1 == x2, therefore vertical
          {
            scaling = (this->Point2[1] - this->Point1[1]) / (this->Maximum - this->Minimum);
            axisVertical = true;
          }
          else
          {
            scaling = (this->Point2[0] - this->Point1[0]) / (this->Maximum - this->Minimum);
          }
        }
        int fontSize = this->LabelProperties->GetFontSize();
        vtkNew<vtkAxisExtended> tickPositionExtended;

        // The following parameters are required for the legibility part in the
        // optimization tickPositionExtended->SetFontSize(fontSize);
        tickPositionExtended->SetDesiredFontSize(fontSize);
        tickPositionExtended->SetPrecision(this->Precision);
        tickPositionExtended->SetIsAxisVertical(axisVertical);

        // Value 4 is hard coded for the user desired tick spacing
        vtkVector3d values = tickPositionExtended->GenerateExtendedTickLabels(min, max, 4, scaling);
        min = values[0];
        max = values[1];
        this->TickInterval = values[2];

        if(min < this->Minimum)
          this->Minimum = min;
        if(max > this->Maximum)
          this->Maximum = max;

        this->Notation = tickPositionExtended->GetLabelFormat();
        this->LabelProperties->SetFontSize(tickPositionExtended->GetFontSize());
        if(tickPositionExtended->GetOrientation() == 1)
        {
          // Set this to 90 to make the labels vertical
          this->LabelProperties->SetOrientation(90);
        }
      }

      double mult = max > min ? 1.0 : -1.0;
      double range = 0.0;
      int n = 0;
      if (this->LogScale)
      {
        range = mult > 0.0 ? pow(10.0, max) - pow(10.0, min) : pow(10.0, min) - pow(10.0, max);
        n = vtkContext2D::FloatToInt(range / pow(10.0, this->TickInterval));
      }
      else
      {
        range = mult > 0.0 ? max - min : min - max;
        n = vtkContext2D::FloatToInt(range / this->TickInterval);
      }
      for (int i = 0; i <= n && i < 200; ++i)
      {
        double value = 0.0;
        if (this->LogScale)
          value = log10(pow(10.0, min) + double(i) * mult * pow(10.0, this->TickInterval));
        else
          value = min + double(i) * mult * this->TickInterval;
        if (this->TickInterval < 1.0)
        {
          // For small TickInterval, increase the precision of the comparison
          if (fabs(value) < (0.00000001 * this->TickInterval))
            value = 0.0;
        }
        else
        {
          if (fabs(value) < 0.00000001)
            value = 0.0;
        }
        this->TickPositions->InsertNextValue(value);
        // Make a tick mark label for the tick
        if (this->LogScale)
          value = pow(double(10.0), double(value));
        // Now create a label for the tick position
        if (this->TickLabelAlgorithm == vtkAxis::TICK_SIMPLE)
        {
          vtksys_ios::ostringstream ostr;
          ostr.imbue(std::locale::classic());
          if (this->Notation > 0)
            ostr.precision(this->Precision);
          if (this->Notation == 1)
            // Scientific notation
            ostr.setf(vtksys_ios::ios::scientific, vtksys_ios::ios::floatfield);
          else if (this->Notation == 2)
            ostr.setf(ios::fixed, ios::floatfield);
          ostr << (value + this->m_TickOffset) * this->m_TickScale;

          this->TickLabels->InsertNextValue(ostr.str());
        }
        else
          this->GenerateLabelFormat(this->Notation, value);
      }
    }
    this->TickMarksDirty = false;
    */
  };
  
  /**
   * Generate the position of the ticks and their label.
   *
   * The value for the label can be influenced by an offset and a scale using the following operation: tick_label = (real_value + offset) * scale.
   * To set the scale and the offset, you have to use the methods SetTickScale() and SetTickOffset() respectively
   */
  void VTKAxis::GenerateTickLabels()
  {
    std::cout << "VTKAxis::GenerateTickLabels()" << std::endl;
    this->TickLabels->SetNumberOfTuples(0);
    for (vtkIdType i = 0; i < this->TickPositions->GetNumberOfTuples(); ++i)
    {
      double value = this->TickPositions->GetValue(i) - this->m_TickOffset;
      // Make a tick mark label for the tick
      if (this->LogScale)
        value = pow(double(10.0), double(value));
      // Now create a label for the tick position
      vtksys_ios::ostringstream ostr;
      ostr.imbue(std::locale::classic());
      if (this->Notation > 0)
        ostr.precision(this->Precision);
      if (this->Notation == SCIENTIFIC_NOTATION)
        ostr.setf(vtksys_ios::ios::scientific, vtksys_ios::ios::floatfield);
      else if (this->Notation == FIXED_NOTATION)
        ostr.setf(ios::fixed, ios::floatfield);
      ostr << (value + this->m_TickOffset) * this->m_TickScale;

      this->TickLabels->InsertNextValue(ostr.str());
    }
  };
};