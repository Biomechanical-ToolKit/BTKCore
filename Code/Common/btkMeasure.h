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

#ifndef __btkMeasure_h
#define __btkMeasure_h

#include "btkDataObject.h"
#include "btkLogger.h"

#include <Eigen/Core>
#include <string>

namespace btk
{
  template <typename Derived>
  struct MeasureTraits;
  
  template <typename Derived>
  class MeasureData : public DataObject
  {
  public:
    typedef typename MeasureTraits<Derived>::Values Values; ///< Measures' values along the time.
    
    /**
     * Returns values of the measure. The exact output type depend of the Derived class
     */
    Values& GetValues() {return this->m_Values;};
    /**
     * Returns values of the measure. The exact output type depend of the Derived class
     */
    const Values& GetValues() const {return this->m_Values;};
    /**
     * Sets values for the measure. The exact input type depend of the Derived class
     */
    void SetValues(const Values& v);
    
  protected:
    /**
     * Constructor which initialize the data with a matrix of zero.
     * The given @a frameNumber corresponds to the matrix's row. The number of colums is automatically determined based on the given template @a Derived.
     */
    MeasureData(int frameNumber);
    /**
     * Copy constructor
     */
    MeasureData(const MeasureData& toCopy);
    /**
     * Simply set the new values
     */
    MeasureData& operator=(const MeasureData& ); // Not implemented.
    
    typename MeasureData<Derived>::Values m_Values; ///< Values of the measure.
  };
  
  template <class Derived>
  class Measure : public DataObjectLabeled
  {
  public:
    typedef typename MeasureTraits<Derived>::Values Values; ///< Values of the measure.
    typedef typename MeasureTraits<Derived>::Data Data; ///< Associated data with the measure
    
    // ~Measure(); // Implicit.
    
    /**
     * Convenient method to return the values associated with measure's data.
     * @warning This method tries to access directly to data's values even if no data has been set. Use this method carefully or use GetData() to access to measure's data. 
     */
    typename Measure<Derived>::Values& GetValues();
    /**
     * Convenient method to return the values associated with measure's data.
     * @warning This method tries to access directly to data's values even if no data has been set. Use this method carefully or use GetData() to access to measure's data. 
     */
    const typename Measure<Derived>::Values& GetValues() const;
    /**
     * Convenient method to set the values of the measure's data.
     * If no data exists for this object, then it is created and the values are assigned to it.
     */
    void SetValues(const Values& v);
    
    /**
     * Returns the number of frames.
     */
    int GetFrameNumber() const;
    /**
     * Sets the number of frames.
     * The input @a frameNumber must be greater than 0.
     */
    void SetFrameNumber(int frameNumber);
    
    /**
     * Returns the data associated to this measure.
     */
    typename Measure<Derived>::Data::Pointer GetData() const {return this->mp_Data;};
    /**
     * Set the data for this object. By default, this object will take the parent of the data. Setting @a parenting to false will unparent the old data, assign the new one, but doesn't set the point as its parent.
     */
    void SetData(typename Measure<Derived>::Data::Pointer data, bool parenting = true);
    
  protected:
    /**
     * Constructor.
     * @warning The use of this constructor must be followed by the use of the method Measure::SetFrameNumber
     * as it creates a null matrix for the values.
     */
    Measure(const std::string& label, const std::string& desc = "");
    /**
     * Constructor.
     */
    Measure(const std::string& label, int frameNumber, const std::string& desc = "");
    /**
     * Copy constructor. Timestamp, source and parent are reset.
     */
    Measure(const Measure& toCopy);
    
    typename Measure<Derived>::Data::Pointer mp_Data; ///< Smart pointer associated with the data of this measurement.
    
  private: 
    Measure& operator=(const Measure& ); // Not implemented.
  };
 
  /**
   * @class Measure btkMeasure.h
   * @brief Measure with @a d dimensions along the time.
   *
   * @tparam Derived Class representing a kind of measurement (Point, Analog, etc.)
   *
   * The measures are stored in a matrix. The measures' dimensions correspond to the columns of the matrix. The frames correspond to the line.
   *
   * @warning The number of dimensions @a d must be greater than 0.
   *
   * @ingroup BTKCommon
   */

   template <class Derived>
   typename Measure<Derived>::Values& Measure<Derived>::GetValues()
   {
     assert(this->mp_Data.get() != 0);
     return this->mp_Data->GetValues();
   }
 
   template <class Derived>
   const typename Measure<Derived>::Values& Measure<Derived>::GetValues() const
   {
     assert(this->mp_Data.get() != 0);
     return this->mp_Data->GetValues();
   };
  
  template <class Derived>
  void Measure<Derived>::SetValues(const typename Measure<Derived>::Values& v)
  {
    if (!this->mp_Data)
      this->SetData(Measure<Derived>::Data::New(static_cast<int>(v.rows())));
    this->mp_Data->SetValues(v);
    this->Modified();
  };
  
  template <class Derived>
  int Measure<Derived>::GetFrameNumber() const 
  {
    if (!this->mp_Data)
      return 0;
    return static_cast<int>(this->mp_Data->GetValues().rows());
  };
 
  template <class Derived>
  void Measure<Derived>::SetFrameNumber(int frameNumber)
  {
    if (frameNumber < 0)
    {
      btkErrorMacro("Impossible to set a number of frames lower than 0.");
      return;
    }
    if (frameNumber == this->GetFrameNumber())
      return;
    else if (frameNumber == 0) // Remove the data
      this->SetData(Measure<Derived>::Data::Null());
    else if (!this->mp_Data)
      this->SetData(Measure<Derived>::Data::New(frameNumber));
    else 
      this->GetData()->Resize(frameNumber);
    this->Modified();
  };

  template <class Derived>
  void Measure<Derived>::SetData(typename Measure<Derived>::Data::Pointer data, bool parenting)
  {
    if (this->mp_Data == data)
      return;
    if (this->mp_Data)
      this->mp_Data->SetParent(NULL);
    this->mp_Data = data;
    if (this->mp_Data && parenting)
      this->mp_Data->SetParent(this);
    this->Modified();
  };

  template <class Derived>
  Measure<Derived>::Measure(const std::string& label, const std::string& desc)
  : DataObjectLabeled(label, desc), mp_Data()
  {};

  template <class Derived>
  Measure<Derived>::Measure(const std::string& label, int frameNumber, const std::string& desc)
  : DataObjectLabeled(label, desc), mp_Data()
  {
    if (frameNumber > 0)
      this->SetData(Measure<Derived>::Data::New(frameNumber));
  };

  template <class Derived>
  Measure<Derived>::Measure(const Measure& toCopy)
  : DataObjectLabeled(toCopy), mp_Data()
  {
    this->SetData(toCopy.mp_Data->Clone());
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @struct MeasureTraits btkMeasure.h
   * @brief Trait used to determine the type of data and the number of components used for a measure.
   *
   * @tparam Derived Class representing a kind of measurement (Point, Analog, etc.)
   */

  // ----------------------------------------------------------------------- //
  
  /**
   * @class MeasureData btkMeasure.h
   * @brief Template class to store data for any kind of measurement.
   *
   * @tparam Derived Class representing a kind of measurement (Point, Analog, etc.)
   *
   * Currently this class store a matrix defined by the given number of frames. The template @a Derived used by this class gives the number of columns (components) of the measure.
   *
   * To add a new type of data (for example for 2D pressure mat or insole), you have to inherit from this class and add the method Resize(int frameNumber). You can also add other informations in inherited classes, like btk::Point::Data which contains reconstruction residuals.
   */
  
  template <class Derived>
  MeasureData<Derived>::MeasureData(int frameNumber)
  : DataObject(), m_Values(MeasureData::Values::Zero(frameNumber,Derived::Values::ColsAtCompileTime))
  {};
  
 template <class Derived>
  MeasureData<Derived>::MeasureData(const MeasureData& toCopy)
  : DataObject(toCopy), m_Values(toCopy.m_Values)
  {};
  
  template <class Derived>
  void MeasureData<Derived>::SetValues(const MeasureData::Values& v)
  {
    this->m_Values = v;
    this->Modified();
  };
};

#endif // __btkMeasure_h
