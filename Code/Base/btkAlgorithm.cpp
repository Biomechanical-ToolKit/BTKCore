/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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

#include "btkAlgorithm.h"
#include "btkAlgorithm_p.h"
#include "btkPort.h"
#include "btkExecutive.h"
#include "btkLogger.h"
#include "btkNode.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  AlgorithmPrivate::AlgorithmPrivate()
  : ObjectPrivate(),
    Information(), InputPorts(), OutputPorts(), Executive(nullptr)
  {
    this->Information.setValue<ManageInputDataPerSlice>(true);
    
  };
    
  AlgorithmPrivate::~AlgorithmPrivate() = default;
    
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  Algorithm::Algorithm()
  : Object(*new AlgorithmPrivate)
  {};
  
  Algorithm::Algorithm(AlgorithmPrivate& pimpl) _BTK_NOEXCEPT
  : Object(pimpl)
  {};
  
  Algorithm::~Algorithm() _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    for (auto port: optr->InputPorts)
      delete port;
    for (auto port: optr->OutputPorts)
      delete port;
  };
  
  Any Algorithm::information(const char* key, const btk::Any& defaultValue) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Information.value(key,std::move(defaultValue));
  };

  void Algorithm::setInformation(const char* key, const btk::Any& value)
  {
    auto optr = this->pimpl();
    if (value.isValid() ? optr->Information.add(key,std::move(value)) : optr->Information.remove(key))
      this->modified();
  };
  
  unsigned Algorithm::inputsNumber() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->InputPorts.size();
  };
  
  unsigned Algorithm::outputsNumber() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->OutputPorts.size();
  };
  
  Port* Algorithm::input(unsigned port, unsigned connection) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkInputIndex(port))
      return nullptr;
    return optr->InputPorts[port]->connection(connection);
  };
  
  std::list<Port*> Algorithm::inputs(unsigned port) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkInputIndex(port))
      return std::list<Port*>();
    return optr->InputPorts[port]->connections();
  };
  
  void Algorithm::setInput(unsigned port, Port* input) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkInputIndex(port))
      return;
    optr->InputPorts[port]->setConnection(input);
    if (input != nullptr)
      input->addConnection(optr->InputPorts[port]);
  };
  
 /** 
  * Convenient way to set the connection with the port input #0
  */  
  void Algorithm::setInput(Port* input) _BTK_NOEXCEPT
  {
    this->setInput(0, input);
  };
  
  void Algorithm::addInput(unsigned port, Port* input) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkInputIndex(port))
      return;
    optr->InputPorts[port]->addConnection(input);
    if (input != nullptr)
      input->addConnection(optr->InputPorts[port]);
  };
  
 /** 
  * Convenient way to add a connection to the port input #0
  */  
  void Algorithm::addInput(Port* input) _BTK_NOEXCEPT
  {
    this->addInput(0, input);
  };
  
  void Algorithm::removeInput(unsigned port, Port* input) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkInputIndex(port))
      return;
    optr->InputPorts[port]->removeConnection(input);
    if (input != nullptr)
      input->removeConnection(optr->InputPorts[port]);
  };

  void Algorithm::removeInput(unsigned port, unsigned connection) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkInputIndex(port))
      return;
    auto input = optr->InputPorts[port]->connection(connection);
    this->removeInput(port, input);
  };
  
  void Algorithm::clearInputs() _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    for (auto& port : optr->InputPorts)
      port->clearConnections();
  };
  
  Port* Algorithm::output(unsigned port) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (!this->checkOutputIndex(port))
      return nullptr;
    return optr->OutputPorts[port];
  };
  
  /** 
   * Convenient way to retrieve the port output #0
   */
  Port* Algorithm::output() _BTK_NOEXCEPT
  {
    return this->output(0);
  };
  
  /**
   * Returns the Executive object associated with this algorithm.
   * If no object was set using the method setExecutive(), a default executive object is created.
   * The defaut executive can be modifined using the static method setDefaultExecutiveFactory().
   */
  Executive* Algorithm::executive()
  {
    // TODO: Use a factory to give the possibility to create a custom executive
    auto optr = this->pimpl();
    if (optr->Executive == nullptr)
      optr->Executive = new Executive(this);
    return optr->Executive;
  };
  
  Result Algorithm::processRequest(const Information& info)
  {
    auto optr = this->pimpl();
    if (info.value<CreateOutputData>())
    {
      for (size_t i = 0 ; i < optr->OutputPorts.size() ; ++i)
      {
        auto& outport = optr->OutputPorts[i];
        // Output information filled?
        if (!outport->information<InformationFilled>())
        {
          if (!this->fillOutputInformation(outport))
            return Result::Error;
          else
            outport->setInformation<InformationFilled>(true);
        }
        // Continue if already created
        if (outport->information<OutputDataCreated>())
          continue;
        // If data doest not exist or if existing held data cannot be casted to the required type, then create new output data
        if ((outport->data(0) == nullptr) || !outport->data(0)->isCastable(outport->information<OutputDataTypeid>()))
        {
          auto data = this->createOutputData(i);
          if ((data == nullptr) || !data->isCastable(outport->information<OutputDataTypeid>()))
            return Result::Error;
          outport->holdData(data);
        }
        // Set the data as created.
        outport->setInformation<OutputDataCreated>(true);
      }
    }
    if (info.value<CheckInputConsistency>())
    {
      for (size_t i = 0 ; i < optr->InputPorts.size() ; ++i)
      {
        if (!this->checkInputConsistency(i))
          return Result::Error;
      }
    }
    if (info.value<UpdateOutputData>())
    {
      bool updateNeeded = false;
      // Check input modification
      for (unsigned i = 0 ; i < this->inputsNumber() ; ++i)
      {
        const auto& ports = this->inputs(i);
        for (const auto& port: ports)
        {
          if ((port->timestamp() > this->timestamp()) || ((port->data() != nullptr) && (port->data()->timestamp() > this->timestamp())))
          {
            updateNeeded = true;
            break;
          }
        }
        if (updateNeeded)
          break;
      }
      // Check internal modification
      if (!updateNeeded && (this->timestamp() > optr->Executive->timestamp()))
        updateNeeded = true;
      // Update if necessary
      if (updateNeeded)
      {
        // Prepare the input/ouput data
        // - Maximum number of slices
        unsigned numSlicesMax = 0;
        for (unsigned i = 0 ; i < this->inputsNumber() ; ++i)
        {
          unsigned numSlices = 0;
          const auto& ports = this->inputs(i);
          for (const auto& port: ports)
          {
            if (port->data() != nullptr)
            {
              numSlices += port->data()->children().size();
            }
          }
          if ((numSlicesMax > 1) && (numSlices != numSlicesMax))
          {
            Logger::error("Incompatible number of data children between different inputs. Data update aborted.");
            return Result::Error;
          }
          numSlicesMax = std::max(numSlicesMax, numSlices);
        }
        // - Set the content of each slice
        std::vector<std::vector<const Node*>> in(numSlicesMax, std::vector<const Node*>(this->inputsNumber(), nullptr));
        std::vector<std::vector<Node*>> out(numSlicesMax, std::vector<Node*>(this->outputsNumber(), nullptr));
        for (unsigned i = 0 ; i < numSlicesMax ; ++i)
        {
          // Input(s)
          for (unsigned j = 0 ; j < this->inputsNumber() ; ++j)
          {
            const auto& ports = this->inputs(j);
            unsigned inc = 0;
            for (const auto& port: ports)
            {
              if ((port->data() != nullptr))
              {
                unsigned num = port->data()->children().size();
                if ((ports.size() == 1) && (num == 1))
                {
                  in[i][j] = port->data(0);
                  break;
                }
                else if ((inc + num) > i)
                {
                  in[i][j] = port->data(i - inc);
                  break;
                }
                else
                  inc += num;
              }
            }
          }
          // Output(s)
          for (unsigned j = 0 ; j < this->outputsNumber() ; ++j)
          {
            auto data = optr->OutputPorts[j]->data()->children();
            // Already created
            if (data.size() > i)
            {
              out[i][j] = optr->OutputPorts[j]->data(i);
            }
            // Neet to generate it and add it
            else
            {
              data.push_back(this->createOutputData(j));
              optr->OutputPorts[j]->holdData(data);
              out[i][j] = data.back();
            }
          }
        }
        // Launch the update
        if (!this->updateOutputData(in,out))
          return Result::Error;
      }
    }
    return Result::Success;
  };
  
  Result Algorithm::update()
  {
    Result res = Result::Error;
    auto executive = this->executive();
    if (executive != nullptr)
      res = executive->runAlgorithm();
    return res;
  };
  
  void Algorithm::setInputsNumber(unsigned num)
  {
    auto optr = this->pimpl();
    if (optr->InputPorts.size() == num)
      return;
    optr->InputPorts.resize(num,nullptr);
    for (size_t i = 0 ; i < optr->InputPorts.size() ; ++i)
    {
      if (optr->InputPorts[i] == nullptr)
        optr->InputPorts[i] = new Port(i,this);
    }
    this->modified();
  };
  
  void Algorithm::setOutputsNumber(unsigned num)
  {
    auto optr = this->pimpl();
    if (optr->OutputPorts.size() == num)
      return;
    optr->OutputPorts.resize(num,nullptr);
    for (size_t i = 0 ; i < optr->OutputPorts.size() ; ++i)
    {
      if (optr->OutputPorts[i] == nullptr)
        optr->OutputPorts[i] = new Port(i,this);
    }
    this->modified();
  };
  
  bool Algorithm::checkInputIndex(unsigned port) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (port >= optr->InputPorts.size())
    {
      Logger::error("Requested input outside of the predefined number of ports.");
      return false;
    }
    return true;
  };
  
  bool Algorithm::checkOutputIndex(unsigned port) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (port >= optr->OutputPorts.size())
    {
      Logger::error("Requested output outside of the predefined number of ports.");
      return false;
    }
    return true;
  };
  
  bool Algorithm::fillInputInformation(Port* port) const
  {
    _BTK_UNUSED(port);
    return true;
  };
  
  bool Algorithm::fillOutputInformation(Port* port) const
  {
    _BTK_UNUSED(port);
    return true;
  };
  
  Node* Algorithm::createOutputData(unsigned port) const
  {
    _BTK_UNUSED(port);
    return nullptr;
  };
  
  /**
   * The validity of the given index is not tested as this method is only used internally. 
   * This is the same for nullity of the input port object which is not tested.
   */
  bool Algorithm::checkInputConsistency(unsigned port) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    auto internal = optr->InputPorts[port];
    // Input information filled?
    if (!internal->information<InformationFilled>())
    {
      if (!this->fillInputInformation(internal))
        return false;
      else
        internal->setInformation<InformationFilled>(true);
    }
    // Input not optional but no connection?
    if (!internal->information<InputIsOptional>() && internal->connections().empty())
      return false;
    // Check the content of the connected ports
    auto requiredTypeid = internal->information<InputDataRequiredTypeid>();
    for (const auto& external: internal->connections())
    {
      // Output information filled?
      if (!external->information<InformationFilled>())
      {
        if ((external->algorithm() == nullptr) || !external->algorithm()->fillOutputInformation(external))
          return false;
        else
          external->setInformation<InformationFilled>(true);
      }
      // Created data?
      if (!external->information<OutputDataCreated>())
        return false;
      // Compatible type?
      if (external->data() == nullptr)
      {
        Logger::error("No data was found while the flag 'OutputDataCreated' said the opposite...");
        return false; // Should not be possible
      }
      for (const auto& child: external->data()->children())
      {
        if (!child->isCastable(requiredTypeid))
          return false;
      }
    }
    return true;
  };
  
  bool Algorithm::updateOutputData(const std::vector<std::vector<const Node*>>& in, const std::vector<std::vector<Node*>>& out)
  {
    // Sanity checks
    if (!this->information<ManageInputDataPerSlice>())
    {
      Logger::error("The management of the input data per slice was disabled but you did not overload the main updateOutputData() method. Update aborted.");
      return false;
    }
    else if (in.size() != out.size())
    {
      Logger::error("The number of slice for the input and output data is not the same. Update aborted.");
      return false;
    }
    // Loop around the slices
    bool res = true;
    for (unsigned i = 0, num = in.size() ; i < num ; ++i)
      res &= this->updateOutputData(in[i],out[i]);
    return res;
  };
  
  bool Algorithm::updateOutputData(const std::vector<const Node*>& in, const std::vector<Node*>& out)
  {
    _BTK_UNUSED(in)
    _BTK_UNUSED(out)
    return true;
  };
};