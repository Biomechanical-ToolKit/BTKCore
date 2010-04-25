/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "btkProcessObject.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class ProcessObject
   * @brief Interface to create a filter/process in a pipeline.
   *
   * An inherited class has to implement the GenerateData() method and the 
   * MakeOutput() metod. GenerateData() is the core of the process as it compute the outputs
   * based on the inputs and its algorithms. The method MakeOutput() is used in the 
   * SetOutputNumber() and SetNthOutput() to create newly valid output.
   *
   * The following code presents the minimum required to use the pipeline principle.
   * @code
   * class Source : public btk::DataObject
   * {
   * public:
   *   typedef SharedPtr<Source> Pointer;
   *   static Pointer New() {return Pointer(new Source());}; 
   *   int GetValue() {return this->m_Val;};
   *   void SetValue(int val)
   *   {
   *     this->m_Val = val;
   *     this->Modified();
   *   };
   * private:
   *   Source() {this->m_Val = 0;};
   *   int m_Val;
   * };
   *
   * class Filter : public btk::ProcessObject
   * {
   * public:
   *   typedef SharedPtr<Filter> Pointer;
   *   static Pointer New() {return Pointer(new Filter());}; 
   *   void SetInc(int inc)
   *   {
   *     this->m_Inc = inc; 
   *     this->Modified();
   *   };   
   *   Source::Pointer GetInput() {return this->GetInput(0);};
   *   void SetInput(Source::Pointer input) {this->SetNthInput(0, input);};
   *   Source::Pointer GetOutput() {return this->GetOutput(0);};
   *
   * protected:
   *   Source::Pointer GetInput(int idx) {return static_pointer_cast<Source>(this->GetNthInput(idx));};
   *   Source::Pointer GetOutput(int idx) {return static_pointer_cast<Source>(this->GetNthOutput(idx));};
   *   virtual btk::DataObject::Pointer MakeOutput(int )
   *   {
   *      return Source::New();
   *   };
   *   virtual void GenerateData()
   *   {
   *     // simple algorithm : O = I + inc
   *     this->GetOutput()->SetValue(this->GetInput()->GetValue() + this->m_Inc);
   *   };
   *
   * private:
   *   Filter()
   *   {
   *     this->SetInputNumber(1);
   *     this->SetOutputNumber(1);
   *     this->m_Inc = 1;
   *   };
   *
   *   int m_Inc;
   * };
   *
   *
   * int main ()
   * {
   *   Source::Pointer src = Source::New();
   *   Filter::Pointer incFilt = Filter::New();
   *   incFilt->SetInput(src);
   *   Source::Pointer res = incFilt->GetOutput();
   *   incFilt->Update(); // same than res->Update();
   *   return 0;
   * }
   * @endcode
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef ProcessObject::Pointer
   * Smart pointer associated with a ProcessObject object.
   */
  
  /**
   * @typedef ProcessObject::ConstPointer
   * Smart pointer associated with a const ProcessObject object.
   */
  
  /**
   * @fn int ProcessObject::GetInputNumber() const
   * Returns the number of inputs.
   */
  
  /**
   * Returns the number of inputs which are valid (i.e. not null).
   */
  int ProcessObject::GetValidInputNumber() const
  {
    int count = 0;
    for (std::vector<DataObject::Pointer>::const_iterator it = this->m_Inputs.begin() ; it != this->m_Inputs.end() ; ++it)
    {
      if (it->get() != 0)
        count++;
    }
    return count;
  }
  
  /**
   * @fn int ProcessObject::GetOutputNumber() const
   * Returns the number of outputs.
   */
  
  /**
   * Recursive method which 1) determines the processes to update and 2) 
   * generate the data by using the GenerateData() method.
   */
  void ProcessObject::Update()
  {
    if (this->m_Updating)
      return;
    this->m_Updating = true;
    
    for (size_t inc = 0 ; inc < this->m_Inputs.size() ; ++inc)
    {
      if (this->m_Inputs[inc].get() != 0)
      {
        this->m_Inputs[inc]->Update();
        if (this->m_Inputs[inc]->m_Timestamp >= this->m_Timestamp)
          this->m_Modified = true;
        if (this->m_Inputs[inc]->mp_Source)
        {
          if (this->m_Inputs[inc]->mp_Source->m_Timestamp >= this->m_Timestamp)
            this->m_Modified = true;
        }
      }
    }
    if (this->m_Modified)
    {
      this->GenerateData();
      this->Object::Modified();
      this->m_Modified = false;
      for (std::vector<DataObject::Pointer>::iterator it = this->m_Outputs.begin() ; it != this->m_Outputs.end() ; ++it)
        (*it)->Modified();
    }
    this->m_Updating = false;
  };

  /**
   * Reset the state of the process. Usefull when an exception was thrown during the generation of the data.
   */
  void ProcessObject::ResetState()
  {
    this->m_Updating = false;
  };
  
  /**
   * Process constructor with zero input and output. The inherited class set the number
   * of inputs/ouputs with the functions SetInputNumber() and SetOutputNumber().
   */
  ProcessObject::ProcessObject()
  : Object(),
    m_Inputs(std::vector<DataObject::Pointer>(0)),
    m_Outputs(std::vector<DataObject::Pointer>(0))
  {
    this->m_Modified = false;
    this->m_Updating = false;
  };
  
  /**
   * This destructor doesn't delete its ouptuts (which can be used as input by others process). 
   * In fact, it says that they are disconnected of their source.
   */
  ProcessObject::~ProcessObject()
  {
    for (size_t idx = 0; idx < this->m_Outputs.size(); ++idx)
    {
      if (this->m_Outputs[idx])
        this->m_Outputs[idx]->mp_Source = 0;
    }
  };
  
  /**
   * Gets the input at index @a idx or an empty Pointer if @a idx is out of range.
   */
  DataObject::Pointer ProcessObject::GetNthInput(int idx)
  {
    if (idx >= static_cast<int>(m_Inputs.size()))
      return DataObject::Pointer();
    return m_Inputs[idx];
  };
  
  /**
   * Gets the input at index @a idx or an empty ConstPointer if @a idx is out of range.
   */
  DataObject::ConstPointer ProcessObject::GetNthInput(int idx) const
  {
    if (idx >= static_cast<int>(m_Inputs.size()))
      return DataObject::ConstPointer();
    return m_Inputs[idx];
  };
  
  /**
   * Sets an input of the filter. If necessary, the size of inputs is increased
   * if @a idx is greater or equal than the number of inputs.
   */
  void ProcessObject::SetNthInput(int idx, DataObject::Pointer input)
  {
    if (idx >= static_cast<int>(this->m_Inputs.size()))
      this->m_Inputs.resize(idx + 1);
    else if (input == this->m_Inputs[idx])
      return;
    m_Inputs[idx] = input;
    
    this->Modified();
  };
  
  /**
   * Returns the index of the DataObject given in input. If no DataObject
   * corresponds, then the method returns -1.
   */
  int ProcessObject::GetInputIndex(DataObject::Pointer input)
  {
    for (size_t idx = 0 ; idx < this->m_Inputs.size() ; ++idx)
    {
      if (this->m_Inputs[idx] == input)
        return static_cast<int>(idx);
    }
    return -1;
  };
  
  /**
   * Sets the number of inputs.
   */
  void ProcessObject::SetInputNumber(int num)
  {
    if (num == this->m_Inputs.size())
      return;
    if (num < 0)
    {
      btkErrorMacro("Attempt to set the number of inputs to " + ToString(num));
      num = 0;
    }
    this->m_Inputs.resize(num);
    this->Modified();
  };
  
  /**
   * Gets the output at @a idx or a empty Pointer if @a idx is out of range.
   */
  DataObject::Pointer ProcessObject::GetNthOutput(int idx)
  {
    if (idx >= static_cast<int>(m_Outputs.size()))
      return DataObject::Pointer();
    return m_Outputs[idx];
  };
  
  /**
   * Gets the output at @a idx or a empty ConstPointer if @a idx is out of range.
   */
  DataObject::ConstPointer ProcessObject::GetNthOutput(int idx) const
  {
    if (idx >= static_cast<int>(this->m_Outputs.size()))
      return DataObject::ConstPointer();
    return m_Outputs[idx];
  };
  
  /**
   * Returns the index of the DataObject given in input. If no DataObject
   * corresponds, then the method returns -1.
   */  
  int ProcessObject::GetOutputIndex(DataObject::Pointer output)
  {
    for (size_t idx = 0 ; idx < this->m_Outputs.size() ; ++idx)
    {
      if (this->m_Outputs[idx] == output)
        return static_cast<int>(idx);
    }
    return -1;
  };
  
  /**
   * Set the output at the index @a idx or create a new output from the method
   * MakeOutput() if the pointer in @a output is null. If necessary, the size of 
   * the outputs is increased if @a idx is greater or equal than the number of 
   * outputs.
   */
  void ProcessObject::SetNthOutput(int idx, DataObject::Pointer output)
  {
    if (idx >= static_cast<int>(this->m_Outputs.size()))
      this->m_Outputs.resize(idx + 1);
    else if (output == this->m_Outputs[idx])
      return;
    
    DataObject::Pointer oldOutput;
    if (this->m_Outputs[idx])
    {
      oldOutput = this->m_Outputs[idx];
      oldOutput->mp_Source = 0;
    }
    if (output.get() == 0)
      output = this->MakeOutput(idx);
    output->mp_Source = this;
    this->m_Outputs[idx] = output;
    
    this->Modified();
  };
  
  /**
   * Sets the number of outputs. The outputs generated used the MakeOutput() method.
   */
  void ProcessObject::SetOutputNumber(int num)
  {
    if (num == this->m_Outputs.size())
      return;
    if (num < 0)
    {
      btkErrorMacro("Attempt to set the number of outputs to " + ToString(num));
      num = 0;
    }
    this->m_Outputs.resize(num);
    for (int inc = 0 ; inc < num ; ++inc)
    {
      this->m_Outputs[inc] = this->MakeOutput(inc);
      this->m_Outputs[inc]->mp_Source = this;
    }
    this->Modified();
  };
  
  /**
   * This method has to be called each time that the ProcessObject is modified.
   */
  void ProcessObject::Modified()
  {
    this->m_Modified = true;
    this->Object::Modified();
  };
  
  /**
   * @fn bool ProcessObject::IsModified() const
   * Indicates if the process is modified or not.
   */
  
  /**
   * @fn virtual void ProcessObject::GenerateData() = 0;
   * Generates the outputs' data.
   */
  
  /**
   * @fn virtual DataObject::Pointer ProcessObject::MakeOutput(int idx) = 0
   * Creates and returns a DataObject::Pointer at the specified index @a idx.
   */
};
