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

#include "btkExecutive.h"
#include "btkExecutive_p.h"
#include "btkAlgorithm.h"
#include "btkPort.h"
#include "btkLogger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  ExecutivePrivate::ExecutivePrivate()
  : ObjectPrivate(), Algorithm(nullptr), IsRunning(false)
  {};
  
  ExecutivePrivate::~ExecutivePrivate() = default;

};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  Executive::Executive(Algorithm* algorithm)
  : Object(*new ExecutivePrivate)
  {
    auto optr = this->pimpl();
    optr->Algorithm = algorithm;
  };
  
  Executive::~Executive() _BTK_NOEXCEPT = default;
  
  bool Executive::isPipelineModified()
  {
    auto optr = this->pimpl();
    if (optr->Algorithm == nullptr)
      return false;
    // Check first the state of the input (modification of the upstream)
    for (unsigned i = 0 ; i < optr->Algorithm->inputsNumber() ; ++i)
    {
      const auto& inputs = optr->Algorithm->inputs(i);
      for (const auto& input: inputs)
      {
        if ((input->timestamp() > this->timestamp()) || (((input->data() != nullptr) && (input->data()->timestamp() > this->timestamp()))))
          return true;
        if ((input->algorithm() == nullptr) || (input->algorithm()->executive() == nullptr))
          continue;
        auto inputExec = input->algorithm()->executive();
        if (!inputExec->pimpl()->IsRunning && inputExec->isPipelineModified())
          return true;
      }
    }
    // Maybe only the content of the associated algorithm was modified
    if (optr->Algorithm->timestamp() > this->timestamp())
      return true;
    return false;
  };
  
  void Executive::updatePipelineTimestamp()
  {
    auto optr = this->pimpl();
    if (optr->Algorithm != nullptr)
    {
      // Upstream
      for (unsigned i = 0 ; i < optr->Algorithm->inputsNumber() ; ++i)
      {
        const auto& inputs = optr->Algorithm->inputs(i);
        for (const auto& input: inputs)
        {
          if ((input->algorithm() == nullptr) || (input->algorithm()->executive() == nullptr) || (input->algorithm()->executive()->pimpl()->IsRunning))
            continue;
          input->algorithm()->executive()->updatePipelineTimestamp();
        }
      }
      // Current
      if (optr->Algorithm->timestamp() > this->timestamp())
      {
        optr->Algorithm->modified();
        this->modified();
      }
      for (unsigned i = 0 ; i < optr->Algorithm->outputsNumber() ; ++i)
      {
        auto output = optr->Algorithm->output(i);
        // Only modified outputs will have their timestamp modified
        if ((output != nullptr) && (output->data() != nullptr) && (output->data()->timestamp() > output->timestamp()))
          output->modified();
      }
    }
  };
  
  Result Executive::sendRequest(Information& info) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Algorithm == nullptr)
    {
      Logger::error("No algorithm set in the executive. Impossible to send a request.");
      return Result::Error;
    }
    Result result = Result::Success;
    for (unsigned i = 0 ; i < optr->Algorithm->inputsNumber() ; ++i)
    {
      const auto& inputs = optr->Algorithm->inputs(i);
      for (const auto& input: inputs)
      {
        if ((input->algorithm() == nullptr) || (input->algorithm()->executive() == nullptr) || (input->algorithm()->executive()->pimpl()->IsRunning))
          continue;
        if ((result = input->algorithm()->executive()->sendRequest(info)) != Result::Success)
          return result;
      }
    }
    result = optr->Algorithm->processRequest(info);
    return result;
  };
  
  /**
   * Pipeline working in the upstream direction (inputs' algorithm processed first)
   */
  Result Executive::runAlgorithm()
  {
    auto optr = this->pimpl();
    if (optr->Algorithm == nullptr)
    {
      Logger::error("No algorithm set in the executive. Impossible to run it.");
      return Result::Error;
    }
    optr->IsRunning = true;
    // First verify if the pipeline must be updated
    if (!this->isPipelineModified())
      return Result::Success;
    // Then execute all the steps required to run the algorithm
    Result result = Result::Error;
    Information request;
    // 1. Ensure that all output data exist
    request.setValue<CreateOutputData>(true);
    if ((result = this->sendRequest(request)) == Result::Success)
    {
      request.setValue<CreateOutputData>(false);
    // 2. Verify inputs consistency
      request.setValue<CheckInputConsistency>(true);
      if ((result = this->sendRequest(request)) == Result::Success)
      {
        request.setValue<CheckInputConsistency>(false);
    // 3. Execute the algorithm
        request.setValue<UpdateOutputData>(true);
        if ((result = this->sendRequest(request)) == Result::Success)
        {
          request.setValue<UpdateOutputData>(false);
    // 4. Finally, update the timestamp of the pipeline
          this->updatePipelineTimestamp();
        }
      }
    }
    optr->IsRunning = false;
    return result;
  };
};