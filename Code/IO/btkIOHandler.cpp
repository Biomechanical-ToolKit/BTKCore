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

#include "btkIOHandler.h"
#include "btkIOHandler_p.h"
#include "btkIODevice.h"
#include "btkNode.h"
#include "btkMacros.h"

#include <iostream>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  IOHandlerPrivate::IOHandlerPrivate()
  : Device(nullptr), ErrorCode(IOHandler::Error::None), ErrorMessage()
  {};
  
  IOHandlerPrivate::~IOHandlerPrivate() noexcept = default; // Cannot be inlined
}

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class IOHandler
   * @brief To read/write data from/to an I/O device in a specific format
   *
   * This interface is the core to implement data reader/writer (e.g. trials, models).
   * Several methods and mecanisms are already implemented to facilitate the inclusion
   * of new formats. For example, it is not necessary to manage exception as this is done internaly.
   * Some methods are available to set errors, retrieve them. The extraction of handler's options 
   * (see Option class) is implemented. In fact, only three methods must be implemented for inherting handler:
   *  - readDevice() method: extract data from a device and set it into a node
   *  - writeDevice() method: write data to a given device
   *  - validateSignature() method: test if the read device has a signature valid for this handler.
   *
   * TODO Finalize the documentation. Give an example for the implementation of a handler.
   */
  
  /**
   * Destructor (default)
   */
  IOHandler::~IOHandler() noexcept = default;
  
  /**
   *
   */
  IOHandler::Capability IOHandler::capability() const noexcept
  {
    return Capability::None;
  };
  
  /**
   *
   */
  bool IOHandler::read(Node* output)
  {
    auto optr = this->pimpl();
    if (output == nullptr)
    {
      this->setError(Error::Unexpected, "Impossible to load the content of a device into a null output");
      return false;
    }
    else if (optr->Device == nullptr)
    {
      this->setError(Error::Device, "No device assigned");
      return false;
    }
    else if (!optr->Device->isOpen())
    {
      this->setError(Error::Device, "Device not open");
      return false;
    }
    else if (this->validateSignature() == Signature::Invalid)
    {
      this->setError(Error::UnsupportedFormat, "Format not supported by this I/O handler");
      return false;
    }
    
    try
    {
      this->setError(); // reset
      Node temp("_TIOHR"); // _THIOR: Temporary I/O Handler Root
      this->readDevice(&temp);
      if (optr->ErrorCode == Error::None) // In case the handler does not use exception but only error code/message.
      {
        for (auto& child: temp.children())
          output->appendChild(child);
      }
    }
    catch (IODevice::Failure& )
    {
      std::string excmsg; 
      if (optr->Device->atEnd())
        excmsg = "Unexpected end of device";
      else if(optr->Device->hasError())
        excmsg = "Loss of integrity in the device";
      else if(optr->Device->hasFailure())
        excmsg = "Internal logic operation error associated with the device";
      else
        excmsg = "Unknown error associated with the device";
      this->setError(Error::Device, excmsg);
    }
    catch (IOHandler::FormatError& e)
    {
      this->setError(Error::InvalidData, e.what());
    }
    catch (std::exception& e)
    {
      this->setError(Error::Unexpected, "Unexpected exception during the reading of a device. Please report this to the support: " + std::string(e.what()));
    }
    catch(...)
    {
      this->setError(Error::Unknown, "Unknown exception during the reading of a device. Please report this to the support");
    }
    
    if (optr->ErrorCode == Error::None)
      return true;
    return false;
  };

  /**
   *
   */
  bool IOHandler::write(const Node* input)
  {
    auto optr = this->pimpl();
    if (input == nullptr)
    {
      this->setError(Error::Unexpected, "Impossible to write a null input to a device");
      return false;
    }
    else if (optr->Device == nullptr)
    {
      this->setError(Error::Device, "No device assigned");
      return false;
    }
    else if (!optr->Device->isOpen())
    {
      this->setError(Error::Device, "Device not open");
      return false;
    }
    
    try
    {
      this->setError(); // reset
      this->writeDevice(input);
    }
    catch (IOHandler::FormatError& e)
    {
      this->setError(Error::InvalidData, e.what());
    }
    catch (std::exception& e)
    {
      this->setError(Error::Unexpected, "Unexpected exception during the writing of a device. Please report this to the support: " + std::string(e.what()));
    }
    catch(...)
    {
      this->setError(Error::Unknown, "Unknown exception during the writing of a device. Please report this to the support");
    }
    
    if (optr->ErrorCode == Error::None)
      return true;
    return false;
  };
  
  /**
   *
   */
  IODevice* IOHandler::device() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Device;
  };
  
  /**
   *
   */
  void IOHandler::setDevice(IODevice* device) noexcept
  {
    auto optr = this->pimpl();
    optr->Device = device;
  };
  
  /**
   *
   */
  std::vector<const char*> IOHandler::availableOptions() const noexcept
  {
    auto optr = this->pimpl();
    return optr->availableOptions();
  };
  
  /**
   *
   */
  std::vector<const char*> IOHandler::availableOptionChoices(const char* option) const noexcept
  {
    auto optr = this->pimpl();
    return optr->availableOptionChoices(option);
  };
  
  /**
   *
   */
  IOHandler::Error IOHandler::errorCode() const noexcept
  {
    auto optr = this->pimpl();
    return optr->ErrorCode;
  };
  
  /**
   *
   */
  const std::string& IOHandler::errorMessage() const noexcept
  {
    auto optr = this->pimpl();
    return optr->ErrorMessage;
  };
  
  /**
   *
   */
  IOHandler::Signature IOHandler::detectSignature() const noexcept
  {
    auto optr = this->pimpl();
    if (optr->Device == nullptr)
      return Signature::Invalid;
    return this->validateSignature();
  };
  
  /**
   *
   */
  void IOHandler::readDevice(Node* output)
  {
    _BTK_UNUSED(output);
    this->setError(Error::Unexpected, "You called the default IOHandler::readDevice method. The instanced I/O handler has certainly not the capability to read data.");
  };
  
  /**
   *
   */
  void IOHandler::writeDevice(const Node* input)
  {
    _BTK_UNUSED(input);
    this->setError(Error::Unexpected, "You called the default IOHandler::writeDevice method. The instanced I/O handler has certainly not the capability to write data.");
  };
  
  /**
   * Constructor for inheriting class with extended private implementation
   */
  IOHandler::IOHandler(IOHandlerPrivate& pimpl) noexcept
  : mp_Pimpl(&pimpl)
  {};
  
  /**
   *
   */
  void IOHandler::option(const char* name, void* value) const noexcept
  {
    auto optr = this->pimpl();
    optr->option(name,value);
  };

  /**
   *
   */
  void IOHandler::setOption(const char* name, const void* value) noexcept
  {
    auto optr = this->pimpl();
    optr->setOption(name,value);
  };
  
  /**
   * Set a code and a detailed message for an error which happened during the reading or writing of a device.
   */
  void IOHandler::setError(Error code, const std::string& msg) noexcept
  {
    auto optr = this->pimpl();
    optr->ErrorCode = code;
    optr->ErrorMessage = msg;
  };
};
