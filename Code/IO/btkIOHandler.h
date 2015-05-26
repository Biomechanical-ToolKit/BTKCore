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

#ifndef __btkIOHandler_h
#define __btkIOHandler_h

#include "btkIOExport.h"
#include "btkOpaque.h"
#include "btkEndianFormat.h"
#include "btkException.h"
#include "btkIOHandlerOption.h"
#include "btkLogger.h"
#include "btkMacros.h" // _BTK_CONSTEXPR, _BTK_NOEXCEPT

#include <list>
#include <string>
#include <memory> // std::unique_ptr
#include <vector>
#include <type_traits> // std::is_same

namespace btk
{
  class Node;
  class IODevice;
  
  class IOHandlerPrivate;
  
  class BTK_IO_EXPORT IOHandler
  {
    BTK_DECLARE_PIMPL_ACCESSOR(IOHandler)
    
  public:
    enum class Signature : int {Valid, Invalid, NotAvailable};
    enum class Capability : int {None = 0x00, Read = 0x01, Write = 0x02, ReadWrite = Read|Write};
    enum class Error : int {None, Device, UnsupportedFormat, InvalidData, Unexpected, Unknown};
    
    enum class Encoding : int {Text = 0x01, Binary, Mixed = Text|Binary};
    using EncodingFormat = IOHandlerOption<Encoding>;
    
    using ByteOrder = EndianFormat;
    using ByteOrderFormat = IOHandlerOption<ByteOrder>;
    
    enum class DataStorage : int {NotApplicable, Integer, Float};
    using DataStorageFormat = IOHandlerOption<DataStorage>;
    
    virtual ~IOHandler() _BTK_NOEXCEPT;
    
    IOHandler() = delete;
    IOHandler(const IOHandler& ) = delete;
    IOHandler(IOHandler&& ) _BTK_NOEXCEPT = delete;
    IOHandler& operator=(const IOHandler& ) = delete;
    IOHandler& operator=(const IOHandler&& ) _BTK_NOEXCEPT = delete;
    
    virtual Capability capability() const _BTK_NOEXCEPT;

    Signature detectSignature() const _BTK_NOEXCEPT;
    bool read(Node* output);
    bool write(const Node* input);
 
    IODevice* device() const _BTK_NOEXCEPT;
    void setDevice(IODevice* device) _BTK_NOEXCEPT;
    
    std::vector<const char*> availableOptions() const _BTK_NOEXCEPT;
    std::vector<const char*> availableOptionChoices(const char* option) const _BTK_NOEXCEPT;
    
    template <typename O> typename O::ValueType option() const _BTK_NOEXCEPT;
    template <typename O, typename V> void setOption(const V& value) _BTK_NOEXCEPT;
    // const char* option(const const char* name) const _BTK_NOEXCEPT;
    // void setOption(const char* name, const char* value) _BTK_NOEXCEPT;
    
    Error errorCode() const _BTK_NOEXCEPT;
    const std::string& errorMessage() const _BTK_NOEXCEPT;
  
  protected:
    template <typename V, V v> struct stringify_option_value;
    
    class FormatError : public Exception
    {
    public:
      explicit FormatError(const std::string& msg)
      : Exception(msg)
      {};
    };
    
    IOHandler(IOHandlerPrivate& pimpl) _BTK_NOEXCEPT;
    
    void option(const char* name, void* value) const _BTK_NOEXCEPT;
    void setOption(const char* name, const void* value) _BTK_NOEXCEPT;
    
    void setError(Error code = Error::None, const std::string& msg = "") _BTK_NOEXCEPT;
    
    virtual Signature validateSignature() const _BTK_NOEXCEPT = 0;
    virtual void readDevice(Node* output);
    virtual void writeDevice(const Node* input);
    
    std::unique_ptr<IOHandlerPrivate> mp_Pimpl;
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename O>
  typename O::ValueType IOHandler::option() const _BTK_NOEXCEPT
  {
    typename O::ValueType value;
    this->option(O::name(),static_cast<void*>(&value));
    return value;
  };
  
  template <typename O, typename V>
  inline void IOHandler::setOption(const V& value) _BTK_NOEXCEPT
  {
    static_assert(std::is_same<typename O::ValueType,V>::value, "The type of the given value does not correspond to the type of the option's value.");
    this->setOption(O::name(),static_cast<const void*>(&value));
  };
  
  // ----------------------------------------------------------------------- //
  
  inline _BTK_CONSTEXPR IOHandler::Capability operator& (IOHandler::Capability lhs, IOHandler::Capability rhs)
  {
    return static_cast<IOHandler::Capability>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
  
  // ----------------------------------------------------------------------- //
  
  BTK_STRINGIFY_OPTION_NAME(IOHandler::EncodingFormat, "Encoding");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::EncodingFormat, IOHandler::Encoding::Binary, "Binary");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::EncodingFormat, IOHandler::Encoding::Text, "Text");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::EncodingFormat, IOHandler::Encoding::Mixed, "Mixed");
  
  BTK_STRINGIFY_OPTION_NAME(IOHandler::ByteOrderFormat, "ByteOrder");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::ByteOrderFormat, IOHandler::ByteOrder::VAXLittleEndian, "VAXLittleEndian");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::ByteOrderFormat, IOHandler::ByteOrder::IEEELittleEndian, "IEEELittleEndian");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::ByteOrderFormat, IOHandler::ByteOrder::IEEEBigEndian, "IEEEBigEndian");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::ByteOrderFormat, IOHandler::ByteOrder::NotApplicable, "NotApplicable");
  
  BTK_STRINGIFY_OPTION_NAME(IOHandler::DataStorageFormat, "DataStorage");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::DataStorageFormat, IOHandler::DataStorage::NotApplicable, "NotApplicable");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::DataStorageFormat, IOHandler::DataStorage::Integer, "Integer");
  BTK_STRINGIFY_OPTION_VALUE(IOHandler::DataStorageFormat, IOHandler::DataStorage::Float, "Float");
};
  
#endif // __btkIOHandler_h
