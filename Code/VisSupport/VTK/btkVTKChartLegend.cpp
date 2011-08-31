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

#include "btkVTKChartLegend.h"

#include <vtkObjectFactory.h>
#include <vtkChartLegend.h>
#include <vtkContext2D.h>
#include <vtkPen.h>
#include <vtkTextProperty.h>
#include <vtkPlot.h>
#include <vtkChart.h>
#include <vtkBrush.h>
#include <vtkStdString.h>
#include <vtkTransform2D.h>

#include <vtkstd/vector>

// No choice to add this code as VTK define it in the CXX file.
//! @cond
class vtkChartLegend::Private
{
 public:
   Private() {};
   ~Private() {};
   
   vtkVector2f Point;
   vtkWeakPointer<vtkChart> Chart;
   vtkstd::vector<vtkPlot*> ActivePlots;
};
//! @endcond

namespace btk
{
  /**
   * @class VTKChartLegend btkVTKChartLegend.h
   * @brief Class to take into account the anchor set by the horizontal and vertical alignment.
   *
   * This inherited class is necessary as in VTK 5.6 the anchor is not taken into account for the location of the legend.
   * Moreover, this class take into account the transformation given to the object to paint correctly the legend into the scene (only the scale and translation are supported).
   */
  /**
   * @var VTKChartLegend::mp_Padding
   * Padding around the content of the legend.
   */
  /**
   * @var VTKChartLegend::m_SymbolWidth
   * Width of the symbol used for each plot.
   */
  
  /**
   * @fn static VTKChartLegend* VTKChartLegend::New()
   * Constructs a VTKChartLegend object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKChartLegend);
  vtkCxxRevisionMacro(VTKChartLegend, "$Revision: 0.1 $");
  
  /**
   * Destructor.
   */
  VTKChartLegend::~VTKChartLegend()
  {};
  
  /**
   * @fn float* VTKChartLegend::GetPadding() const
   * Returns the padding's values as an array of 4 elements in the order: left, bottom, right, top.
   */
   
  /**
   * @fn void VTKChartLegend::SetPadding(float left, float bottom, float right, float top)
   * Convenient method to set the padding.
   */
   
  /**
   * Sets the padding. The order is left, bottom, right, top.
   */
  void VTKChartLegend::SetPadding(float padding[4])
  {
    if ((this->mp_Padding[0] == padding[0]) && (this->mp_Padding[1] == padding[1]) && (this->mp_Padding[2] == padding[2]) && (this->mp_Padding[3] == padding[3]))
      return;
    this->mp_Padding[0] = padding[0]; // Left
    this->mp_Padding[1] = padding[1]; // Bottom
    this->mp_Padding[2] = padding[2]; // Right
    this->mp_Padding[3] = padding[3]; // Top
    this->Modified();
  };
   
  /**
   * @fn float VTKChartLegend::GetSymbolWidth() const
   * Returns the width of the symbol used for the plots
   */
   
  /**
   * Sets the width of the symbol used for the plots.
   */
  void VTKChartLegend::SetSymbolWidth(float width)
  {
    if (this->m_SymbolWidth == width)
      return;
    this->m_SymbolWidth = width;
    this->Modified();
  };
  
  /**
   * Method to draw the legend into the scene.
   */
  bool VTKChartLegend::Paint(vtkContext2D *painter)
  {
    painter->GetPen()->SetWidth(0.5);
    vtkTextProperty *prop = painter->GetTextProp();
    prop->SetFontSize(this->LabelSize);
    prop->SetColor(0.0, 0.0, 0.0);
    prop->SetJustificationToLeft();
    prop->SetVerticalJustificationToBottom();

    vtkVector2f stringBounds[2];
    float sizeText[2] = {0.0f, 0.0f};
    // Calculate the widest legend label - needs the context to calculate font metrics, but these could be cached.
    for(size_t i = 0; i < this->Storage->ActivePlots.size(); ++i)
    {
      if (this->Storage->ActivePlots[i]->GetLabel())
      {
        painter->ComputeStringBounds(this->Storage->ActivePlots[i]->GetLabel(), stringBounds->GetData());
        if (stringBounds[1].X() > sizeText[0])
          sizeText[0] = stringBounds[1].X();
      }
    }
    // Compute the maximum height for the text.
    painter->ComputeStringBounds("Typo", stringBounds->GetData()); // String containing ascender and descender glyphs.
    sizeText[1] = stringBounds[1].Y();
    
    if (this->GetTransform() != NULL)
    {
      sizeText[0] /= this->GetTransform()->GetMatrix()->GetElement(0,0);
      sizeText[1] /= this->GetTransform()->GetMatrix()->GetElement(1,1);
    }
    
    float spacing[2] = {(this->mp_Padding[0] + this->mp_Padding[2]) * 0.5f, (this->mp_Padding[1] + this->mp_Padding[3]) * 0.5f};
    
    // Figure out the size of the legend box and store locally.
    vtkRectf box(this->Storage->Point.X(), this->Storage->Point.Y(), // LEFT BOTTOM by default
                 sizeText[0] + this->mp_Padding[0] + this->mp_Padding[2] + spacing[0] + this->m_SymbolWidth,
                 this->Storage->ActivePlots.size() * sizeText[1] + (this->Storage->ActivePlots.size() - 1) * spacing[1] + this->mp_Padding[1] + this->mp_Padding[3]);
    // Adapt the corner to be the left bottom point independantly of the anchor
    // - Horizontal
    if (this->HorizontalAlignment == vtkChartLegend::RIGHT)
      box[0] -= box[2];
    else if (this->HorizontalAlignment == vtkChartLegend::CENTER)
      box[0] -= box[2] * 0.5f;
    // - Vertical
    if (this->VerticalAlignment == vtkChartLegend::TOP)
      box[1] -= box[3];
    else if (this->VerticalAlignment == vtkChartLegend::CENTER)
      box[1] -= box[3] * 0.5f;
      
    // Now draw a box for the legend.
    painter->GetBrush()->SetColor(255, 255, 255, 255);
    painter->DrawRect(box[0], box[1], box[2], box[3]);

    float pos[2] = {box[0] + this->mp_Padding[0] + this->m_SymbolWidth + spacing[0], box[1] + box[3] - this->mp_Padding[3] - sizeText[1]};
    float rectSymbol[4] = {box[0] + this->mp_Padding[0], pos[1], this->m_SymbolWidth, sizeText[1]};

    // Draw all of the legend labels and marks
    painter->ComputeStringBounds("ascT", stringBounds->GetData()); // String without descender glyph.
    float baseHeight = stringBounds[1].Y();
    for(size_t i = 0; i < this->Storage->ActivePlots.size(); ++i)
    {
      if (this->Storage->ActivePlots[i]->GetLabel())
      {
        // painter->DrawRect(pos[0], pos[1], sizeText[0], sizeText[1]);
        float pt[2] = {pos[0], pos[1]};
        if (this->GetTransform() != NULL)
          this->GetTransform()->TransformPoints(pt, pt, 1);
        // Only tested with the vtkQtLabelRenderStrategy class to render text
        // Seems to work for any font (arial, courrir, times)
        if (this->LabelSize < 20)
        {
          vtkStdString testString = this->Storage->ActivePlots[i]->GetLabel(); testString += "T";
          painter->ComputeStringBounds(testString, stringBounds->GetData());
          painter->DrawString(pt[0], pt[1] - (stringBounds[1].Y() - baseHeight), this->Storage->ActivePlots[i]->GetLabel());
        }
        else
          painter->DrawString(pt[0], pt[1], this->Storage->ActivePlots[i]->GetLabel());
        // Paint the legend mark and increment out y value.
        this->Storage->ActivePlots[i]->PaintLegend(painter, rectSymbol);
        pos[1] -= sizeText[1] + spacing[1];
        rectSymbol[1] = pos[1];
      }
    }

    return true;
  };
  
  /**
   * Constructor.
   */
  VTKChartLegend::VTKChartLegend()
  : vtkChartLegend()
  {
    this->mp_Padding[0] = 5;
    this->mp_Padding[1] = 5;
    this->mp_Padding[2] = 5;
    this->mp_Padding[3] = 5;
    this->m_SymbolWidth = 15;
  };
};