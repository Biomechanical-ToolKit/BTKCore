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

#include "btkOpaque.h"
#include "btkEndianFormat.h"
#include "btkException.h"
#include "btkIOHandlerOption.h"
#include "btkLogger.h"

#include <list>
#include <string>
#include <memory> // std::unique_ptr
#include <vector>
#include <type_traits> // std::is_same

static constexpr const char _btk_IOHandler_Encoding[] = "Encoding";
static constexpr const char _btk_IOHandler_ByteOrder[] = "ByteOrder";
static constexpr const char _btk_IOHandler_DataStorage[] = "DataStorage";

namespace btk
{
  class Node;
  class IODevice;
  
  class IOHandlerPrivate;
  
  class IOHandler
  {
    BTK_DECLARE_PIMPL_ACCESSOR(IOHandler)
    
  public:
    enum class Signature : int {Valid, Invalid, NotAvailable};
    enum class Capability : int {None = 0x00, Read = 0x01, Write = 0x02, ReadWrite = Read|Write};
    enum class Error : int {None, Device, UnsupportedFormat, InvalidData, Unexpected, Unknown};
    
    using ByteOrder = EndianFormat;
    enum class Encoding : int {Text = 0x01, Binary, Mix = Text|Binary};
    enum class DataStorage : int {NotApplicable, Integer, Float};

    using EncodingFormat = IOHandlerOption<_btk_IOHandler_Encoding,Encoding>;
    using ByteOrderFormat = IOHandlerOption<_btk_IOHandler_ByteOrder,ByteOrder>;
    using DataStorageFormat = IOHandlerOption<_btk_IOHandler_DataStorage,DataStorage>;
    
    virtual ~IOHandler() noexcept;
    
    IOHandler() = delete;
    IOHandler(const IOHandler& ) = delete;
    IOHandler(IOHandler&& ) noexcept = delete;
    IOHandler& operator=(const IOHandler& ) = delete;
    IOHandler& operator=(const IOHandler&& ) noexcept = delete;
    
    virtual Capability capability() const noexcept;

    Signature detectSignature() const noexcept;
    bool read(Node* output);
    bool write(const Node* input);
 
    IODevice* device() const noexcept;
    void setDevice(IODevice* device) noexcept;
    
    std::vector<const char*> availableOptions() const noexcept;
    std::vector<const char*> availableOptionChoices(const char* option) const noexcept;
    
    template <typename O> typename O::Format option() const noexcept;
    template <typename O, typename V> void setOption(const V& value) noexcept;
    // const char* option(const const char* name) const noexcept;
    // void setOption(const char* name, const char* value) noexcept;
    
    Error errorCode() const noexcept;
    const std::string& errorMessage() const noexcept;
  
  protected:
    template <typename V, V v> struct stringify_option_value;
    
    class FormatError : public Exception
    {
    public:
      explicit FormatError(const std::string& msg)
      : Exception(msg)
      {};
    };
    
    IOHandler(IOHandlerPrivate& pimpl) noexcept;
    
    void option(const char* name, void* value) const noexcept;
    void setOption(const char* name, const void* value) noexcept;
    
    void setError(Error code = Error::None, const std::string& msg = "") noexcept;
    
    virtual Signature validateSignature() const noexcept = 0;
    virtual void readDevice(Node* output);
    virtual void writeDevice(const Node* input);
    
    std::unique_ptr<IOHandlerPrivate> mp_Pimpl;
  };
  
  // ----------------------------------------------------------------------- //
  
  inline constexpr IOHandler::Capability operator& (IOHandler::Capability lhs, IOHandler::Capability rhs)
  {
    return static_cast<IOHandler::Capability>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
  
  // ----------------------------------------------------------------------- //
  
  template <>
  struct stringify_option_value<IOHandler::Encoding,IOHandler::Encoding::Binary>
  {
    static constexpr const char* c_str = "Binary";
  };

  template <>
  struct stringify_option_value<IOHandler::Encoding,IOHandler::Encoding::Text>
  {
    static constexpr const char* c_str = "Text";
  };
  
  template <>
  struct stringify_option_value<IOHandler::Encoding,IOHandler::Encoding::Mix>
  {
    static constexpr const char* c_str = "Mix";
  };
  
  template <>
  struct stringify_option_value<IOHandler::ByteOrder,IOHandler::ByteOrder::VAXLittleEndian>
  {
    static constexpr const char* c_str = "VAXLittleEndian";
  };

  template <>
  struct stringify_option_value<IOHandler::ByteOrder,IOHandler::ByteOrder::IEEELittleEndian>
  {
    static constexpr const char* c_str = "IEEELittleEndian";
  };
  
  template <>
  struct stringify_option_value<IOHandler::ByteOrder,IOHandler::ByteOrder::IEEEBigEndian>
  {
    static constexpr const char* c_str = "IEEEBigEndian";
  };
  
  template <>
  struct stringify_option_value<IOHandler::ByteOrder,IOHandler::ByteOrder::NotApplicable>
  {
    static constexpr const char* c_str = "NotApplicable";
  };
  
  template <>
  struct stringify_option_value<IOHandler::DataStorage,IOHandler::DataStorage::NotApplicable>
  {
    static constexpr const char* c_str = "NotApplicable";
  };
  
  template <>
  struct stringify_option_value<IOHandler::DataStorage,IOHandler::DataStorage::Integer>
  {
    static constexpr const char* c_str = "Integer";
  };
  
  template <>
  struct stringify_option_value<IOHandler::DataStorage,IOHandler::DataStorage::Float>
  {
    static constexpr const char* c_str = "Float";
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename O>
  typename O::Format IOHandler::option() const noexcept
  {
    typename O::Format value;
    this->option(O::name(),static_cast<void*>(&value));
    return value;
  };
  
  template <typename O, typename V>
  inline void IOHandler::setOption(const V& value) noexcept
  {
    static_assert(std::is_same<typename O::Format,V>::value, "The type of the given value does not correspond to the type of the option's value.");
    this->setOption(O::name(),static_cast<const void*>(&value));
  };
};
  
#endif // __btkIOHandler_h
