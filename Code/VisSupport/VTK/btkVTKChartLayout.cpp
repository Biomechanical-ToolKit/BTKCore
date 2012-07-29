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

#include "btkVTKChartLayout.h"
#include "btkVTKChartTimeSeries.h"

#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkContext2D.h>
#include <vtkContextScene.h>
#include <vtkAxis.h>
#include <vtkObjectFactory.h>

#include <vector>

namespace btk
{
  class VTKChartLayout::PIMPL
  {
  public:
    PIMPL() : Geometry(0, 0) {}
    ~PIMPL() {}

    // Container for the vtkChart objects that make up the matrix.
    std::vector< vtkSmartPointer<VTKChartTimeSeries> > Charts;
    // Spans of the charts in the matrix, default is 1x1.
    std::vector< vtkVector2i > Spans;
    vtkVector2i Geometry;
  };
  
  /**
   * @class VTKChartLayout btkVTKChartLayout.h
   * @brief Layout (vertical, horizontal, array) of VTKChartTimeSeries charts
   *
   * This implementation take into account if the chart is visible or not and adjust the size of the others in consequence.
   * This class take also the ownership of the included charts.
   * Note: The first chart starts at the top-left.
   *
   * @ingroup BTKVTK
   */
  /**
   * @var VTKChartLayout::Size
   * The number of charts in x and y.
   */
  /**
   * @var VTKChartLayout::Gutter
   * The gutter between each chart.
   */
  /**
   * @var VTKChartLayout::Borders
   * The borders (left, bottom, right, top) around this layout,
   */
  /**
   * @var VTKChartLayout::LayoutIsDirty
   * Internal variable to determine if the layout must be updated or not.
   */

  /**
   * @fn static VTKChartLayout* VTKChartLayout::New()
   * Creates a new object
   */
  vtkStandardNewMacro(VTKChartLayout)

  /**
   * Constructor
   * Set by default the size of the layout as 0 by 0 chart.
   * A gutter between the chart of 15 (horizontal and vertical) pixels.
   * And borders (left, bottom, right, top) of 50,40,50,40 pixels.
   */
  VTKChartLayout::VTKChartLayout() : Size(0, 0), Gutter(15.0, 15.0)
  {
    this->Private = new PIMPL;
    this->Borders[vtkAxis::LEFT] = 50;
    this->Borders[vtkAxis::BOTTOM] = 40;
    this->Borders[vtkAxis::RIGHT] = 50;
    this->Borders[vtkAxis::TOP] = 40;
    this->LayoutIsDirty = true;
  }

  /**
   * Destructor
   */
  VTKChartLayout::~VTKChartLayout()
  {
    delete this->Private;
  }

  /**
   * Paint event for layout of charts
   */
  bool VTKChartLayout::Paint(vtkContext2D *painter)
  {
    if (this->LayoutIsDirty ||
        this->GetScene()->GetSceneWidth() != this->Private->Geometry.X() ||
        this->GetScene()->GetSceneHeight() != this->Private->Geometry.Y())
    {
      this->UpdateLayout();
    }
    return Superclass::Paint(painter);
  };

  /**
   * Set the number (rows and columns) of charts. This will cause an immediate
   * update of the layout. The default size is 0x0 (no charts). No chart
   * objects are created until GetChart() is called.
   */
  void VTKChartLayout::SetSize(const vtkVector2i &size)
  {
    if ((this->Size.X() != size.X()) || (this->Size.Y() != size.Y()))
    {
      this->Size = size;
      if ((size.X() * size.Y()) < static_cast<int>(this->Private->Charts.size()))
      {
        for (int i = static_cast<int>(this->Private->Charts.size()-1); i >= size.X() * size.Y() ; --i)
          this->RemoveItem(this->Private->Charts[i]);
      }
      this->Private->Charts.resize(size.X() * size.Y());
      this->Private->Spans.resize(size.X() * size.Y(), vtkVector2i(1, 1));
      this->LayoutIsDirty = true;
    }
  }

  /**
   * Sets the size of the borders
   */
  void VTKChartLayout::SetBorders(int left, int bottom, int right, int top)
  {
    this->Borders[vtkAxis::LEFT] = left;
    this->Borders[vtkAxis::BOTTOM] = bottom;
    this->Borders[vtkAxis::RIGHT] = right;
    this->Borders[vtkAxis::TOP] = top;
    this->LayoutIsDirty = true;
  }
  
  /**
   * Returns the borders of the layout
   */
  void VTKChartLayout::GetBorders(int borders[4])
  {
    for(int i= 0 ; i < 4 ; ++i)
      borders[i] = this->Borders[i];
  }

  /**
   * Sets the gutter used between each chart
   */
  void VTKChartLayout::SetGutter(const vtkVector2f &gutter)
  {
    this->Gutter = gutter;
    this->LayoutIsDirty = true;
  }

  /**
   * Set the chart element, note that the chart matrix must be large enough to
   * accommodate the element being set. Note that this class will take ownership
   * of the chart object.
   * @return false if the element cannot be set.
   */
  bool VTKChartLayout::SetChart(const vtkVector2i &position, VTKChartTimeSeries* chart)
  {
    if ((position.X() < this->Size.X()) && (position.Y() < this->Size.Y()))
    {
      size_t index = position.Y() * this->Size.X() + position.X();
      if (this->Private->Charts[index])
        this->RemoveItem(this->Private->Charts[index]);
      this->Private->Charts[index] = chart;
      this->AddItem(chart);
      chart->SetLayoutStrategy(vtkChart::AXES_TO_RECT);
      return true;
    }
    else
      return false;
  };

  /**
   * Get the specified chart element, if the element does not exist (i.e. out of bounds), NULL will be
   * returned. If the chart element has not yet been allocated it will be at
   * this point.
   */
  VTKChartTimeSeries* VTKChartLayout::GetChart(const vtkVector2i &position)
  {
    if ((position.X() < this->Size.X()) && (position.Y() < this->Size.Y()))
    {
      size_t index = position.Y() * this->Size.X() + position.X();
      if (this->Private->Charts[index] == NULL)
      {
        vtkNew<VTKChartTimeSeries> chart;
        this->Private->Charts[index] = chart.GetPointer();
        this->AddItem(chart.GetPointer());
        chart->SetLayoutStrategy(vtkChart::AXES_TO_RECT);
      }
      return this->Private->Charts[index];
    }
    else
      return NULL;
  };
  
  /**
   * Extract the chart from the layout and remove it.
   */
  VTKChartTimeSeries* VTKChartLayout::TakeChart(const vtkVector2i &position)
  {
    if ((position.X() < this->Size.X()) && (position.Y() < this->Size.Y()))
    {
      size_t index = position.Y() * this->Size.X() + position.X();
      VTKChartTimeSeries* chart = this->Private->Charts[index];
      if (this->Private->Charts[index] != NULL)
      {
        chart->Register(this);
        this->RemoveItem(this->Private->Charts[index]);
        this->Private->Charts[index] = NULL;
      };
      return chart;
    }
    else
      return NULL;
  };
  
  /**
   * Updates the size of each chart by taking into account their number but also their visibility. 
   *
   * FIXME: This method was never tested for horizontal layout or array with invisible charts and should be adapted in consequence.
   */
  void VTKChartLayout::UpdateLayout()
  {
    // Update the chart element positions
    this->Private->Geometry.Set(this->GetScene()->GetSceneWidth(), this->GetScene()->GetSceneHeight());
    if ((this->Size.X() > 0) && (this->Size.Y() > 0))
    {
      int numPlotsY = 0;
      for (int j = 0 ; j < this->Size.Y() ; ++j)
      {
        size_t index = j * this->Size.X() + 0; // Only for the first column
        if (this->Private->Charts[index] && this->Private->Charts[index]->GetVisible())
          ++numPlotsY;
      }
      if (numPlotsY != 0)
      {
        // Calculate the increments without the gutters/borders that must be left
        vtkVector2f increments;
        increments.SetX((this->Private->Geometry.X() - (this->Size.X() - 1) *
                         this->Gutter.X() - this->Borders[vtkAxis::LEFT] -
                         this->Borders[vtkAxis::RIGHT]) / this->Size.X());
        increments.SetY((this->Private->Geometry.Y() - (numPlotsY - 1) * this->Gutter.Y()
                         - this->Borders[vtkAxis::TOP] - this->Borders[vtkAxis::BOTTOM]) / numPlotsY);
        float x = this->Borders[vtkAxis::LEFT];
        for (int i = 0; i < this->Size.X(); ++i)
        {
          float y = this->Private->Geometry.Y() - this->Borders[vtkAxis::TOP] - increments.Y();
          for (int j = 0; j < this->Size.Y(); ++j)
          {
            size_t index = j * this->Size.X() + i;
            if (this->Private->Charts[index] && this->Private->Charts[index]->GetVisible())
            {
              VTKChartTimeSeries* chart = this->Private->Charts[index];
              vtkVector2i& span = this->Private->Spans[index];
              chart->SetGeometry(vtkRectf(x, y,
                                     increments.X() * span.X() + (span.X() - 1) * this->Gutter.X(),
                                     increments.Y() * span.Y() + (span.Y() - 1) * this->Gutter.Y()));
              y -= increments.Y() - this->Gutter.Y();
            }
          }
          x += increments.X() + this->Gutter.X();
        }
      }
    }
    this->LayoutIsDirty = false;
  };
  
  /**
   * Set the span of a chart in the matrix. This defaults to 1x1, and cannot
   * exceed the remaining space in x or y.
   * @return false If the span is not possible.
   */
  bool VTKChartLayout::SetChartSpan(const vtkVector2i& position,
                                    const vtkVector2i& span)
  {
    if ((this->Size.X() - position.X() - span.X() < 0) || (this->Size.Y() - position.Y() - span.Y() < 0))
      return false;
    else
    {
      this->Private->Spans[position.Y() * this->Size.X() + position.X()] = span;
      this->LayoutIsDirty = true;
      return true;
    }
  }

  /**
   * Get the span of the specified chart.
   */
  vtkVector2i VTKChartLayout::GetChartSpan(const vtkVector2i& position)
  {
    size_t index = position.Y() * this->Size.X() + position.X();
    if ((position.X() < this->Size.X()) && (position.Y() < this->Size.Y()))
      return this->Private->Spans[index];
    else
      return vtkVector2i(0, 0);
  }

  /**
   * Print the information of the object
   */
  void VTKChartLayout::PrintSelf(ostream &os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os, indent);
  }
};