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

%module btkCommon
%{
#include "btkCommonSwig.h"
%}
%include <std_string.i>
%include <std_list.i>
%include <std_vector.i>
%include <eigen.i>

#undef BTK_SWIG_HEADER_DECLARATION

#include "btkSwigMacros.h"

%include "btkCommonSwig_docstring.i"

// ------------------------------------------------------------------------- //
//                                 DataObject                                //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS(DataObject)
{
public:
  void Update();
  unsigned long int GetTimestamp() const;
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(DataObject);
};

// ------------------------------------------------------------------------- //
//                                    Event                                  //
// ------------------------------------------------------------------------- //

%include "Common/btkCommonSwig_Event.h"

BTK_SWIG_DECLARE_IMPL_CLASS_DATA(Event)
{
public:
  typedef enum {Unknown = btk::Event::Unknown, Manual = btk::Event::Manual, Automatic = btk::Event::Automatic, FromForcePlatform = btk::Event::FromForcePlatform} DetectionFlag;
  BTK_SWIG_DECLARE_IMPL_GETSET(Label, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Description, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Context, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Subject, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Time, double);
  BTK_SWIG_DECLARE_IMPL_GETSET(Frame, int);
  BTK_SWIG_DECLARE_IMPL_GETSET(DetectionFlags, int);
  bool HasDetectionFlag(DetectionFlag );
  BTK_SWIG_DECLARE_IMPL_GETSET(Id, int);
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(Event);
};

// ------------------------------------------------------------------------- //
//                                    Analog                                 //
// ------------------------------------------------------------------------- //

%eigen_typemaps(btk::Analog::Values)

%include "Common/btkCommonSwig_Analog.h"

BTK_SWIG_EXTEND_CLASS_GETSET_VECTOR(Analog, Value);
BTK_SWIG_DECLARE_IMPL_CLASS_DATA(Analog)
{
public:
  typedef enum {Unknown = btk::Analog::Unknown, PlusMinus10 = btk::Analog::PlusMinus10, PlusMinus5 = btk::Analog::PlusMinus5, PlusMinus2Dot5 = btk::Analog::PlusMinus2Dot5, PlusMinus1Dot25 = btk::Analog::PlusMinus1Dot25, PlusMinus1 = btk::Analog::PlusMinus1, PlusMinus0Dot5 = btk::Analog::PlusMinus0Dot5, PlusMinus0Dot25 = btk::Analog::PlusMinus0Dot25, PlusMinus0Dot1 = btk::Analog::PlusMinus0Dot1, PlusMinus0Dot05 = btk::Analog::PlusMinus0Dot05} Gain;
  BTK_SWIG_DECLARE_IMPL_GETSET(Label, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Description, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Values, btk::Analog::Values&);
  BTK_SWIG_DECLARE_IMPL_GETSET(FrameNumber, int);
  BTK_SWIG_DECLARE_IMPL_GETSET(Unit, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Gain, Gain);
  BTK_SWIG_DECLARE_IMPL_GETSET(Offset, int);
  BTK_SWIG_DECLARE_IMPL_GETSET(Scale, double);
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(Analog);
};

// ------------------------------------------------------------------------- //
//                                    Point                                  //
// ------------------------------------------------------------------------- //

%eigen_typemaps(btk::Point::Values)
%eigen_typemaps(btk::Point::Residuals)
%eigen_typemaps(btk::Point::Masks)

%include "Common/btkCommonSwig_Point.h"

BTK_SWIG_EXTEND_CLASS_GETSET_MATRIX(Point, Value);
BTK_SWIG_EXTEND_CLASS_GETSET_VECTOR(Point, Residual);
BTK_SWIG_EXTEND_CLASS_GETSET_VECTOR(Point, Mask);
BTK_SWIG_DECLARE_IMPL_CLASS_DATA(Point)
{
public:
  typedef enum {Marker = btk::Point::Marker, Angle = btk::Point::Angle, Force = btk::Point::Force, Moment = btk::Point::Moment, Power = btk::Point::Power, Scalar = btk::Point::Scalar, Reaction = btk::Point::Reaction} Type;
  BTK_SWIG_DECLARE_IMPL_GETSET(Label, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Description, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Values, btk::Point::Values&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Residuals, btk::Point::Residuals&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Masks, btk::Point::Masks&);
  BTK_SWIG_DECLARE_IMPL_GETSET(FrameNumber, int);
  BTK_SWIG_DECLARE_IMPL_GETSET(Type, Type);
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(Point);
};

// ------------------------------------------------------------------------- //
//                                 ForcePlatform                             //
// ------------------------------------------------------------------------- //

%eigen_typemaps(btk::ForcePlatform::Corners)
%eigen_typemaps(btk::ForcePlatform::Corner)
%eigen_typemaps(btk::ForcePlatform::Origin)
%eigen_typemaps(btk::ForcePlatform::CalMatrix)

%include "Common/btkCommonSwig_ForcePlatform.h"

BTK_SWIG_DECLARE_IMPL_CLASS_DATA(ForcePlatform)
{
public:
  typedef btkAnalogIterator Iterator; // Defined by the macro BTK_SWIG_DECLARE_IMPL_COLLECTION(Analog);
  Iterator Begin();
  Iterator End();
  int GetChannelNumber() const;
  btkAnalog GetChannel(int );
  void SetChannel(int , btkAnalog );
  btkAnalogCollection GetChannels() const;
  BTK_SWIG_DECLARE_IMPL_GETSET(Origin, btk::ForcePlatform::Origin& );
  void SetOrigin(double , double , double );
  const btk::ForcePlatform::Corner GetCorner(int ) const;
  void SetCorner(int , int , double );
  void SetCorner(int , double , double , double );
  void SetCorner(int , const btk::ForcePlatform::Corner& );
  BTK_SWIG_DECLARE_IMPL_GETSET(Corners, btk::ForcePlatform::Corners& );
  BTK_SWIG_DECLARE_IMPL_GETSET(CalMatrix, btk::ForcePlatform::CalMatrix& );
  int GetType() const;
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(ForcePlatform);
};

BTK_SWIG_DECLARE_IMPL_CLASS(ForcePlatformType1) : public btkForcePlatform_impl
{};

BTK_SWIG_DECLARE_IMPL_CLASS(ForcePlatformType2) : public btkForcePlatform_impl
{};

BTK_SWIG_DECLARE_IMPL_CLASS(ForcePlatformType3) : public btkForcePlatform_impl
{};

BTK_SWIG_DECLARE_IMPL_CLASS(ForcePlatformType4) : public btkForcePlatform_impl
{};

BTK_SWIG_DECLARE_IMPL_CLASS(ForcePlatformType5) : public btkForcePlatform_impl
{};

BTK_SWIG_DECLARE_IMPL_CLASS(ForcePlatformType6) : public btkForcePlatform_impl
{};

// ------------------------------------------------------------------------- //
//                                    Wrench                                 //
// ------------------------------------------------------------------------- //

%include "Common/btkCommonSwig_Wrench.h"

BTK_SWIG_DECLARE_IMPL_CLASS_DATA(Wrench)
{
public:
  BTK_SWIG_DECLARE_IMPL_GETSET(Position, btkPoint );
  BTK_SWIG_DECLARE_IMPL_GETSET(Force, btkPoint );
  BTK_SWIG_DECLARE_IMPL_GETSET(Moment, btkPoint );
  btkPoint GetComponent(int );
  void SetFrameNumber(int );
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(Wrench);
};

// ------------------------------------------------------------------------- //
//                                  Collection                               //
// ------------------------------------------------------------------------- //

%include "Common/btkCommonSwig_Collection.h"

BTK_SWIG_DECLARE_IMPL_COLLECTION(Event);
BTK_SWIG_DECLARE_IMPL_COLLECTION(Analog);
BTK_SWIG_DECLARE_IMPL_COLLECTION(Point);
BTK_SWIG_DECLARE_IMPL_COLLECTION(ForcePlatform);
BTK_SWIG_DECLARE_IMPL_COLLECTION(Wrench);

// ------------------------------------------------------------------------- //
//                                     MetaData                              //
// ------------------------------------------------------------------------- //

// NOTE: The inclusion of the header "btkCommonSwig_MetaData.h" after the declaration
//       of the templates gives lots of warning from SWIG. This problem is known [1],
//       but everything works in Python.
//       Including the header after removes the warnings but you won't be able to transform
//       Python's list into vectors (e.g. a = btk.btkMetaDataInfo([1,2,3,4])).
//       Using the command %warnfilter silent the problem.
//       [1] http://thread.gmane.org/gmane.comp.programming.swig/15334/focus=15426

namespace std
{
  %template(btkStringArray) vector<string>;
  %template(btkIntArray) vector<int>;
  %template(btkDoubleArray) vector<double>;
}

// Remove warnings about shadowed constructors and shadowed methods
%warnfilter(509) btkMetaDataInfo::btkMetaDataInfo;
%warnfilter(509) btkMetaDataInfo::SetValues;
%warnfilter(509) btkMetaData::btkMetaData;

%include "Common/btkCommonSwig_MetaData.h"

%extend btkMetaDataInfo
{
  int GetDimension(int idx) const {return (*$self)->GetDimension(idx);};
  void SetDimension(int idx, int val) {(*$self)->SetDimension(idx, static_cast<uint8_t>(val));};
  const std::vector<int> GetDimensions() const {return btkSwigConvert<int>((*$self)->GetDimensions());};
  void SetDimensions(const std::vector<int>& dims) {(*$self)->SetDimensions(btkSwigConvert<uint8_t>(dims));};
  int GetValueNumber() const {return (*$self)->GetValues().size();};
  void SetValue(int idx, const std::string& val) {(*$self)->SetValue(idx, val);};
  void SetValue(int idx, int val) {(*$self)->SetValue(idx, static_cast<int16_t>(val));};
  void SetValue(int idx, double val) {(*$self)->SetValue(idx, static_cast<float>(val));};
  void SetValues(int val) {(*$self)->SetValues(static_cast<int16_t>(val));};
  void SetValues(double val) {(*$self)->SetValues(static_cast<float>(val));};
  void SetValues(const std::string& val) {(*$self)->SetValues(val);};
  void SetValues(const std::vector<int>& val) {(*$self)->SetValues(btkSwigConvert<int16_t>(val));};
  void SetValues(const std::vector<double>& val) {(*$self)->SetValues(btkSwigConvert<float>(val));};
  void SetValues(const std::vector<std::string>& val) {(*$self)->SetValues(val);};
  void SetValues(const std::vector<int>& dims, const std::vector<int>& val) {(*$self)->SetValues(btkSwigConvert<uint8_t>(dims), btkSwigConvert<int16_t>(val));};
  void SetValues(const std::vector<int>& dims, const std::vector<double>& val) {(*$self)->SetValues(btkSwigConvert<uint8_t>(dims), btkSwigConvert<float>(val));};
  void SetValues(const std::vector<int>& dims, const std::vector<std::string>& val) {(*$self)->SetValues(btkSwigConvert<uint8_t>(dims), val);};
  // Operator
  bool operator==(const btkMetaDataInfo& rRHS) {return *(*$self) == *rRHS;};
  bool operator!=(const btkMetaDataInfo& rRHS) {return *(*$self) != *rRHS;};
}

BTK_SWIG_DECLARE_IMPL_CLASS(MetaDataInfo)
{
public:
  typedef enum {Char = btk::MetaDataInfo::Char, Byte = btk::MetaDataInfo::Byte, Integer = btk::MetaDataInfo::Integer, Real = btk::MetaDataInfo::Real} Format;
  Format GetFormat() const;
  std::string GetFormatAsString() const;
  void SetFormat(Format format);
  
  int GetDimensionsProduct(int start = 0) const;
  void ResizeDimensions(int nb);

  // Conversion
  const std::string ToString(int idx) const;
  int ToInt(int idx) const;
  double ToDouble(int idx) const;
  const std::vector<std::string> ToString() const;
  const std::vector<int> ToInt() const;
  const std::vector<double> ToDouble() const;
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(MetaDataInfo);
};

%extend btkMetaData
{
  // Operator
  bool operator==(const btkMetaData& rRHS) {return *(*$self) == *rRHS;};
  bool operator!=(const btkMetaData& rRHS) {return *(*$self) != *rRHS;};
}

BTK_SWIG_DECLARE_IMPL_CLASS_DATA(MetaData)
{
public:
  typedef btkMetaDataIterator Iterator;
  BTK_SWIG_DECLARE_IMPL_GETSET(Label, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(Description, std::string&);
  BTK_SWIG_DECLARE_IMPL_GETSET(UnlockState, bool);
  BTK_SWIG_DECLARE_IMPL_GETSET(Info, btkMetaDataInfo);
  bool HasInfo() const;
  btkMetaDataInfo ExtractChildInfo(const std::string& , btk::MetaDataInfo::Format , int , bool noPossibleEmptyValue = true);
  bool HasMetaDataAsParent() const;
  //void SetParent(DataObject* parent = 0);
  //void SetParent(MetaData* parent = 0);
  Iterator Begin();
  //MetaData::ConstIterator Begin() const;
  Iterator End();
  //MetaData::ConstIterator End() const;
  btkMetaData GetChild(int );
  //btkMetaData GetChild(int ) const;
  btkMetaData GetChild(const std::string& );
  //btkMetaData GetChild(const std::string& ) const;
  bool AppendChild(btkMetaData );
  bool InsertChild(Iterator , btkMetaData );
  bool InsertChild(int , btkMetaData );
  void SetChild(int , btkMetaData );
  btkMetaData TakeChild(Iterator );
  btkMetaData TakeChild(int );
  btkMetaData TakeChild(const std::string& );
  Iterator RemoveChild(Iterator );
  void RemoveChild(int );
  void RemoveChild(const std::string& );
  void ClearChildren();
  bool HasChildren() const;
  int GetChildNumber() const;
  Iterator FindChild(const std::string& );
  //ConstIterator FindChild(const std::string& ) const;
protected:
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(MetaData);
};

%warnfilter(509) btkMetaDataCreateChild;
%warnfilter(509) btkMetaDataCreateChild2D;
%inline
{
  btkMetaData btkMetaDataCreateChild(btkMetaData parent, const std::string& label) {return btk::MetaDataCreateChild(parent, label);};
  void btkMetaDataCreateChild(btkMetaData parent, const std::string& label, int val) {btk::MetaDataCreateChild(parent, label, static_cast<int16_t>(val));};
  void btkMetaDataCreateChild(btkMetaData parent, const std::string& label, double val) {btk::MetaDataCreateChild(parent, label, static_cast<float>(val));};
  void btkMetaDataCreateChild(btkMetaData parent, const std::string& label, const std::string& val) {btk::MetaDataCreateChild(parent, label, val);};
  void btkMetaDataCreateChild(btkMetaData parent, const std::string& label, const std::vector<int>& val, int inc = 1) {btk::MetaDataCreateChild(parent, label, btkSwigConvert<int16_t>(val), inc);};
  void btkMetaDataCreateChild(btkMetaData parent, const std::string& label, const std::vector<double>& val, int inc = 1) {btk::MetaDataCreateChild(parent, label, btkSwigConvert<float>(val), inc);};
  void btkMetaDataCreateChild(btkMetaData parent, const std::string& label, const std::vector<std::string>& val, int inc = 1) {btk::MetaDataCreateChild(parent, label, val, inc);};
  void btkMetaDataCreateChild2D(btkMetaData parent, const std::string& label, const std::vector<int>& val, int numCol, int inc = 1) {btk::MetaDataCreateChild2D(parent, label, btkSwigConvert<int16_t>(val), numCol, inc);};
  void btkMetaDataCreateChild2D(btkMetaData parent, const std::string& label, const std::vector<double>& val, int numCol, int inc = 1) {btk::MetaDataCreateChild2D(parent, label, btkSwigConvert<float>(val), numCol, inc);};
  void btkMetaDataCreateChild2D(btkMetaData parent, const std::string& label, const std::vector<std::string>& val, int numCol, int inc = 1) {btk::MetaDataCreateChild2D(parent, label, val, numCol, inc);};
  std::vector<int> btkMetaDataCollapseChildrenValuesInt(btkMetaData parent, const std::string& baselabel, int targetFinalSize = -1, int blankReplacement = 0)
  {
    std::vector<int16_t> target;
    btk::MetaDataCollapseChildrenValues<int16_t>(target, parent, baselabel, targetFinalSize, static_cast<int16_t>(blankReplacement));
    return btkSwigConvert<int>(target);
  };
  std::vector<double> btkMetaDataCollapseChildrenValuesDouble(btkMetaData parent, const std::string& baselabel, int targetFinalSize = -1, double blankReplacement = 0.0)
  {
    std::vector<float> target;
    btk::MetaDataCollapseChildrenValues<float>(target, parent, baselabel, targetFinalSize, static_cast<float>(blankReplacement));
    return btkSwigConvert<double>(target);
  };
  std::vector<std::string> btkMetaDataCollapseChildrenValuesString(btkMetaData parent, const std::string& baselabel, int targetFinalSize = -1, const std::string& blankReplacement = "")
  {
    std::vector<std::string> target;
    btk::MetaDataCollapseChildrenValues<std::string>(target, parent, baselabel, targetFinalSize, blankReplacement);
    return target;
  };
}

// ------------------------------------------------------------------------- //
//                                  Acquisition                              //
// ------------------------------------------------------------------------- //

%include "Common/btkCommonSwig_Acquisition.h"

BTK_SWIG_DECLARE_IMPL_CLASS_DATA(Acquisition)
{
public:
  typedef enum {Bit8 = btk::Acquisition::Bit8, Bit12 = btk::Acquisition::Bit12, Bit14 = btk::Acquisition::Bit14, Bit16 = btk::Acquisition::Bit16}  AnalogResolution;
  typedef btkEventCollectionIterator EventIterator;
  typedef btkPointCollectionIterator PointIterator;
  typedef btkAnalogCollectionIterator AnalogIterator;
  typedef btkMetaDataIterator MetaDataIterator;
  // Metadata
  MetaDataIterator BeginMetaData();
  MetaDataIterator EndMetaData();
  btkMetaData GetMetaData();
  void SetMetaData(btkMetaData);
  // Event
  EventIterator BeginEvent();
  EventIterator EndEvent();
  btkEvent GetEvent(int );
  void SetEvent(int , btkEvent );
  btkEventCollection GetEvents();
  void SetEvents(btkEventCollection );
  bool IsEmptyEvent() const;
  int GetEventNumber() const;
  void SetEventNumber(int );
  void ClearEvents();
  EventIterator FindEvent(const std::string& );
  void AppendEvent(btkEvent );
  void RemoveEvent(int );
  EventIterator RemoveEvent(EventIterator );
  // Point
  PointIterator BeginPoint();
  PointIterator EndPoint();
  btkPoint GetPoint(int );
  btkPoint GetPoint(const std::string& );
  void SetPoint(int , btkPoint );
  btkPointCollection GetPoints();
  void SetPoints(btkPointCollection );
  bool IsEmptyPoint() const;
  int GetPointNumber() const;
  void SetPointNumber(int );
  void ClearPoints();
  PointIterator FindPoint(const std::string& );
  void AppendPoint(btkPoint );
  void RemovePoint(const std::string& );
  void RemovePoint(int );
  PointIterator RemovePoint(PointIterator );
  // Analog
  AnalogIterator BeginAnalog();
  AnalogIterator EndAnalog();
  bool IsEmptyAnalog() const;
  int GetAnalogNumber() const;
  void SetAnalogNumber(int );
  btkAnalog GetAnalog(int );
  btkAnalog GetAnalog(const std::string& );
  void SetAnalog(int , btkAnalog );
  btkAnalogCollection GetAnalogs();
  void SetAnalogs(btkAnalogCollection );
  void ClearAnalogs();
  AnalogIterator FindAnalog(const std::string& );
  void AppendAnalog(btkAnalog );
  void RemoveAnalog(const std::string& );
  void RemoveAnalog(int );
  AnalogIterator RemoveAnalog(AnalogIterator );
  // Others
  void Init(int , int , int analogNumber = 0, int analogSampleNumberPerPointFrame = 1);
  void Resize(int , int , int analogNumber = 0, int analogSampleNumberPerPointFrame = 1);
  void ResizePointNumber(int );
  void ResizeAnalogNumber(int );
  void ResizeFrameNumber(int );
  void ResizeFrameNumberFromEnd(int );
  void Reset();
  double GetDuration() const;
  int GetFirstFrame() const;
  void SetFirstFrame(int );
  int GetLastFrame() const;
  const std::string GetPointUnit() const;
  void SetPointUnit(const std::string& units = "mm");
  const std::string GetPointUnit(btkPoint_impl::Type ) const;
  void SetPointUnit(btkPoint_impl::Type , const std::string& );
  int GetPointFrameNumber() const;
  double GetPointFrequency() const;
  void SetPointFrequency(double );
  int GetAnalogFrameNumber() const;
  int GetNumberAnalogSamplePerFrame() const;
  double GetAnalogFrequency() const;
  AnalogResolution GetAnalogResolution() const;
  void SetAnalogResolution(AnalogResolution r);
  int GetMaxInterpolationGap() const;
  void SetMaxInterpolationGap(int gap);  
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(Acquisition);
};

// ------------------------------------------------------------------------- //
//                               ProcessObject                               //
// ------------------------------------------------------------------------- //

BTK_SWIG_DECLARE_IMPL_CLASS(ProcessObject)
{
public:
  int GetInputNumber() const;
  int GetValidInputNumber() const;
  int GetOutputNumber() const;
  void ResetState();
  void Update();
  unsigned long int GetTimestamp() const;
protected:  
  BTK_SWIG_DECLARE_IMPL_DEFAULT_CTOR(ProcessObject);
};