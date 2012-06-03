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

#include "btkVTKQtStringToImage.h"

#include <vtkObjectFactory.h>
#include <vtkUnicodeString.h>
#include <vtkVector.h>
#include <vtkTextProperty.h>
#include <vtkQImageToImageSource.h>
#include <vtkImageData.h>
#include <vtkStdString.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>

#include <QApplication>
#include <QFont>
#include <QColor>
#include <QPainter>
#include <QFontMetrics>

// Required as this internal class is implemented in the CXX file in VTK
class vtkQtStringToImage::Internals
{
public:
  QFont TextPropertyToFont(vtkTextProperty* tprop)
  {
    QFont fontSpec(tprop->GetFontFamilyAsString());
    fontSpec.setBold(tprop->GetBold());
    fontSpec.setItalic(tprop->GetItalic());
    fontSpec.setPixelSize(tprop->GetFontSize());
    return fontSpec;
  };
  QColor TextPropertyToColor(double* fc, double opacity)
  {
    QColor textColor(
      static_cast<int>(fc[0]*255),
      static_cast<int>(fc[1]*255),
      static_cast<int>(fc[2]*255),
      static_cast<int>(opacity*255));
    return textColor;
  };
};

namespace btk
{
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 8)) || (VTK_MAJOR_VERSION >= 6))
  /**
   * @class VTKQtStringToImage btkVTKQtStringToImage.h
   * @brief Text renderer inheriting from vtkQtStringToImage.
   *
   * Note: this class and the overloaded methods fix the issues #13199 and #13206 (http://www.vtk.org/Bug/view.php?id=13199, http://www.vtk.org/Bug/view.php?id=13206). It should be used only with VTK 5.8 and VTK 5.10.
   *
   * @ingroup BTKVTK
   */
 
  /**
   * @fn static VTKQtStringToImage* VTKQtStringToImage::New()
   * Constructs a VTKQtStringToImage object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKQtStringToImage);

  vtkVector2i VTKQtStringToImage::GetBounds(vtkTextProperty* property, const vtkUnicodeString& string)
  {
    vtkVector2i recti(0, 0);
    if (!QApplication::instance())
    {
      vtkErrorMacro("You must initialize a QApplication before using this class.");
      return recti;
    }
    if (!property)
      return recti;
#if 1
    QFontMetrics fm(this->Implementation->TextPropertyToFont(property));
    QRect rect = fm.boundingRect(QString::fromUtf8(string.utf8_str()));
    QMatrix mat;
    mat.rotate(-property->GetOrientation());
    rect = mat.mapRect(rect);
    recti.SetX(rect.width());
    recti.SetY(rect.height());
#else
    QFont fontSpec = this->Implementation->TextPropertyToFont(property);
    
    QString text = QString::fromUtf8(string.utf8_str());
    QPainterPath path;
    path.addText(0, 0, fontSpec, text);
    
    QMatrix mat;
    mat.rotate(-property->GetOrientation());
    path = mat.map(path);
    
    QRectF rect = path.boundingRect();
    recti.SetX(static_cast<int>(rect.width()));
    recti.SetY(static_cast<int>(rect.height()));
#endif
    return recti;
  };
  
  vtkVector2i VTKQtStringToImage::GetBounds(vtkTextProperty* property, const vtkStdString& string)
  {
    return this->GetBounds(property, vtkUnicodeString::from_utf8(string));
//     vtkVector2i recti(0, 0);
//     if (!QApplication::instance())
//     {
//       vtkErrorMacro("You must initialize a QApplication before using this class.");
//       return recti;
//     }
//     if (!property)
//       return recti;
// #if 1
//     QFontMetrics fm(this->Implementation->TextPropertyToFont(property));
//     QRect rect = fm.boundingRect(string.c_str());
//     QMatrix mat;
//     mat.rotate(-property->GetOrientation());
//     rect = mat.mapRect(rect);
//     recti.SetX(rect.width());
//     recti.SetY(rect.height());
// #else
//     QFont fontSpec = this->Implementation->TextPropertyToFont(property);
//     
//     QString text(string.c_str());
//     QPainterPath path;
//     path.addText(0, 0, fontSpec, text);
//     
//     QMatrix mat;
//     mat.rotate(-property->GetOrientation());
//     path = mat.map(path);
//     
//     QRectF rect = path.boundingRect();
//     recti.SetX(static_cast<int>(rect.width()));
//     recti.SetY(static_cast<int>(rect.height()));
// #endif
//     return recti;
  };

  /**
   * Overload method which compare to the original one reduce the size of the generated image and locate it correctly in the view.
   */
  int VTKQtStringToImage::RenderString(vtkTextProperty *property, const vtkUnicodeString& string, vtkImageData *data)
  {
    if (!QApplication::instance())
    {
      vtkErrorMacro("You must initialize a QApplication before using this class.");
      return 0;
    }
    // Get the required size, and initialize a new QImage to draw on.
    vtkVector2i box = this->GetBounds(property, string);
    if (box.GetX() == 0 || box.GetY() == 0)
      return 0;

    QString text = QString::fromUtf8(string.utf8_str());
    QFont fontSpec = this->Implementation->TextPropertyToFont(property);

    // Get properties from text property
    double rotation = -property->GetOrientation();
    QColor textColor = this->Implementation->TextPropertyToColor(property->GetColor(), property->GetOpacity());

    int shOff[2];
    property->GetShadowOffset(shOff);
    double pixelPadding = 0.0; // Why a padding is needed?
    double pixelPaddingX = pixelPadding;
    double pixelPaddingY = pixelPadding;
    if (property->GetShadow())
    {
      pixelPaddingX += static_cast<double>(shOff[0]);
      pixelPaddingY -= static_cast<double>(shOff[1]);
    }

    QPainterPath path;
    path.addText(pixelPadding/2.0, pixelPadding/2.0, fontSpec, text);
    QRectF bounds = path.boundingRect();
    bounds.setWidth(bounds.width() + pixelPaddingX);
    bounds.setHeight(bounds.height() + pixelPaddingY);
    QTransform trans;
    trans.rotate(rotation);
    QRectF rotBounds = trans.mapRect(bounds);
    QImage image(static_cast<int>(ceil(rotBounds.width())), static_cast<int>(ceil(rotBounds.height())), QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0,0,0,0));
    QPainter p(&image);
    p.setRenderHint(QPainter::TextAntialiasing, this->Antialias);
    p.setRenderHint(QPainter::Antialiasing, this->Antialias);
    p.translate(-rotBounds.left(), -rotBounds.top());
    p.rotate(rotation);
    
    if (property->GetShadow())
    {
      p.save();
      p.translate(shOff[0], -shOff[1]);
      double sc[3];
      property->GetShadowColor(sc);
      QColor shadowColor = this->Implementation->TextPropertyToColor(sc, property->GetOpacity());
      p.fillPath(path, shadowColor);
      p.restore();
    }
    
    p.fillPath(path, textColor);

    this->QImageToImage->SetQImage(&image);
    this->QImageToImage->Modified();
    this->QImageToImage->Update();
    data->DeepCopy(vtkImageData::SafeDownCast(this->QImageToImage->GetOutputDataObject(0)));
    this->QImageToImage->SetQImage(NULL);

    return 1;
  };

  /**
   * Constructor
   */
  VTKQtStringToImage::VTKQtStringToImage()
  : vtkQtStringToImage()
  {};
#endif
};