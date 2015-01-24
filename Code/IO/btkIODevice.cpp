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

#include "btkIODevice.h"
#include "btkIODevice_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  IODevicePrivate::IODevicePrivate()
  : State(IODevice::State::Good), Exception(IODevice::State::Good)
  {};
  
  IODevicePrivate::~IODevicePrivate() noexcept = default; // Cannot be inlined
}

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class IODevice btkIODevice.h
   * @brief Interface to read and write on any type of device (e.g. File, Buffer, etc.).
   *
   * There is two types of devices. The first one is called "random access" (e.g. File, Buffer), while the second is called "sequential" (e.g. SerialPort).
   * The use of a IODevice class should be used with a BinaryStream or TextStream which interact at a higher level to read/write data in a device.
   *
   * This interface proposes also an exception mechanism in case the device fails to do some internal operation, or if there are error during I/O operations.
   * You can use the method setExceptions() to set the states which can trigger an exception. The exception thrown by this class corresponds to a IODevice::Failure exception.
   *
   * To implement a new device, several methods must me implemented. To facilitate this implementation some protected methods are proposed like verifyMode().
   */
  
  /**
   * Destructor. 
   * This methods does nothing. It is the responsability of the inherited class to decide if their destructor does something specific regarding the state of the device (like closing it).
   */
  IODevice::~IODevice() noexcept = default;
  
  /**
   * Returns true if the device's state is set to State::Good.
   */
  bool IODevice::isGood() const noexcept
  {
    auto optr = this->pimpl();
    return optr->State == State::Good;
  };
  
  /**
   * Returns true if the device's state contain the flag State::End.
   */
  bool IODevice::atEnd() const noexcept
  {
    auto optr = this->pimpl();
    return (this->state() & State::End) == State::End;
  };
 
  /**
   * Returns true if the device's state contain the flag State::Fail and/or State::Error.
   */
  bool IODevice::hasFailure() const noexcept
  {
    auto optr = this->pimpl();
    return (optr->State & (State::Fail | State::Error)) != State::Good;
  };
 
  /**
   * Returns true if the device's state contain the flag State::Fail
   */
  bool IODevice::hasError() const noexcept
  {
    auto optr = this->pimpl();
    return (optr->State & State::Error) == State::Error;
  };
 
  /**
   * Returns true current state of the device.
   */
  IODevice::State IODevice::state() const noexcept
  {
    auto optr = this->pimpl();
    return optr->State;
  };
 
  /**
   * Sets the state of the device.
   * The given @a state is added to the current state.
   * Use the method Clear() if you want to reset the state of the device.
   */
  void IODevice::setState(IODevice::State state)
  {
    this->clear(this->state() | state);
  };
 
  /**
   * Sets the state of the device.
   * If the given state @a flags meets some part of the exception's mask, then a Failure exception is thrown.
   * @note Setting the state to State::Good will reset the possible current failure/errors.
   */
  void IODevice::clear(IODevice::State state)
  {
    auto optr = this->pimpl();
    optr->State = state;
    if ((this->exceptions() & this->state()) != State::Good)
      throw Failure("IODevice::clear");
  };
   
  /**
   * Returns the mask used to possibly throws an exception.
   */
  IODevice::State IODevice::exceptions() noexcept
  {
    auto optr = this->pimpl();
    return optr->Exception;
  };
 
  /**
   * Sets the mask which will be used to throw an exceptions.
   * Setting the mask to State::Good will cancel the use of exception with the device. 
   */
  void IODevice::setExceptions(IODevice::State mask)
  {
    auto optr = this->pimpl();
    optr->Exception = mask;
    this->clear(optr->State);
  };
  
  /**
   * @fn virtual bool IODevice::isOpen() = 0
   * Returns true if the device is opened otherwise returns false.
   */
  
  /**
   * @fn virtual void IODevice::close() = 0
   * Close the device.
   * @note The inherited class should set the State::Fail state flag if any failure happens during the closing of the device.
   */
  
  /**
   * @fn virtual void IODevice::read(char* s, Size n) = 0
   * Gets from the device a sequence of characters of size @a n and store it in @a s. The State::Error state flag is set if any issue happens during the reading operation. 
   */
    
  /**
   * @fn virtual void IODevice::write(const char* s, Size n) = 0
   * Puts the sequence of characters @a s of size @a n to the device. The State::Error state flag is set if any issue happens during the writing operation. 
   */
    
  /**
   * @fn virtual void IODevice::seek(Offset offset, SeekDir dir) = 0
   * Moves the pointer associated with a random access device of the given @a offset in the given direction @a dir.
   * @note The use of this method with a sequential device would set the flag State::Fail to true.
   */
  
  /**
   * @fn virtual Position IODevice::tell() const = 0
   * Returns the position of the pointer associated with a random access device. 
   * @note The use of this method with a sequential device would set the flag State::Fail to true.
   */
  
  /**
   * @fn virtual bool IODevice::isSequential() const = 0
   * Returns true if the device is sequential otherwise false.
   */
  
  /**
   * Verify the coherency of the given open mode.
   * This method should be used by every inherited class in their Open() method before trying to open the implemented device.
   * Internally, this will set the flag State::Fail to true if one of the verification is not valid.
   * The following checks are realized by this method:
   *  - The device is not already opened.
   *  - The modes Append and Truncate cannot be set at the same time.
   *  - The mode Truncate is set but not the mode Out.
   */
  bool IODevice::verifyMode(Mode mode)
  {
    bool valid = true;
    if (this->isOpen()
      || (((mode & Mode::Append) == Mode::Append) && ((mode & Mode::Truncate) == Mode::Truncate))
      || (((mode & Mode::Truncate) == Mode::Truncate) && ((mode & Mode::Out) != Mode::Out)))
    {
      valid = false;
      this->setState(State::Fail);
    }
    return valid;
  };
  
  /**
   * Constructor.
   * Set the device's state to State::Good without exception enabled.
   */
  IODevice::IODevice() noexcept
  : mp_Pimpl(new IODevicePrivate)
  {};
  
  IODevice::IODevice(IODevicePrivate& pimpl) noexcept
  : mp_Pimpl(&pimpl)
  {};
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class IODevice::Failure btkIODevice.h
   * @brief Exception thrown by the inherited classes of the interface IODevice.
   */
  
  /**
   * @fn IODevice::Failure::Failure(const std::string& msg);
   * Constructor.
   */
  
  /**
   * @fn virtual IODevice::Failure::~Failure()
   * Destructor (empty).
   */
};