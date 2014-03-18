/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#include "btkBinaryStream.h"

namespace btk
{
  /** 
   * Extracts @a nb characters and return them as a vector.
   */
  template <class Derived>
  std::vector<char> BinaryStream<Derived>::ReadChar(size_t nb)
  {
    std::vector<char> values(nb);
    this->ReadChar(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<char> val(45,0);
   * bfs.ReadChar(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadChar(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadChar(std::vector<char>& values)
  {
    if (values.empty())
      return;
    this->ReadChar(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb chars and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadChar(size_t nb, char* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadChar();
  };
  
  /** 
   * Extracts @a nb signed 8-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<int8_t> BinaryStream<Derived>::ReadI8(size_t nb)
  {
    std::vector<int8_t> values(nb);
    this->ReadI8(values);
    return values;
  };
     
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<int8_t> val(45,0);
   * bfs.ReadI8(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadI8(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI8(std::vector<int8_t>& values)
  {
    if (values.empty())
      return;
    this->ReadI8(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb signed 8-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI8(size_t nb, int8_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadI8();
  };
  
  /** 
   * Extracts @a nb unsigned 8-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<uint8_t> BinaryStream<Derived>::ReadU8(size_t nb)
  {
    std::vector<uint8_t> values(nb);
    this->ReadU8(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<uint8_t> val(45,0);
   * bfs.ReadU8(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadU8(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU8(std::vector<uint8_t>& values)
  {
    if (values.empty())
      return;
    this->ReadU8(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb unsigned 8-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU8(size_t nb, uint8_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadU8();
  };
  
  /** 
   * Extracts @a nb signed 16-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<int16_t> BinaryStream<Derived>::ReadI16(size_t nb)
  {
    std::vector<int16_t> values(nb);
    this->ReadI16(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<int16_t> val(45,0);
   * bfs.ReadI16(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadI16(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI16(std::vector<int16_t>& values)
  {
    if (values.empty())
      return;
    this->ReadI16(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb signed 16-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI16(size_t nb, int16_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadI16();
  };
  
  /** 
   * Extracts @a nb unsigned 16-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<uint16_t> BinaryStream<Derived>::ReadU16(size_t nb)
  {
    std::vector<uint16_t> values(nb);
    this->ReadU16(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<uint16_t> val(45,0);
   * bfs.ReadU16(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadU16(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU16(std::vector<uint16_t>& values)
  {
    if (values.empty())
      return;
    this->ReadU16(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb unsigned 16-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU16(size_t nb, uint16_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadU16();
  };
  
  /** 
   * Extracts @a nb signed 32-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<int32_t> BinaryStream<Derived>::ReadI32(size_t nb)
  {
    std::vector<int32_t> values(nb);
    this->ReadI32(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<int32_t> val(45,0);
   * bfs.ReadI32(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadI32(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI32(std::vector<int32_t>& values)
  {
    if (values.empty())
      return;
    this->ReadI32(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb signed 32-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI32(size_t nb, int32_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadI32();
  };
  
  /** 
   * Extracts @a nb unsigned 32-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<uint32_t> BinaryStream<Derived>::ReadU32(size_t nb)
  {
    std::vector<uint32_t> values(nb);
    this->ReadU32(values);
    return values;
  };
    
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<uint32_t> val(45,0);
   * bfs.ReadU32(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadU32(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU32(std::vector<uint32_t>& values)
  {
    if (values.empty())
      return;
    this->ReadU32(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb unsigned 32-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU32(size_t nb, uint32_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadU32();
  };
  
  /** 
   * Extracts @a nb signed 64-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<int64_t> BinaryStream<Derived>::ReadI64(size_t nb)
  {
    std::vector<int64_t> values(nb);
    this->ReadI64(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<int64_t> val(45,0);
   * bfs.ReadI64(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadI64(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI64(std::vector<int64_t>& values)
  {
    if (values.empty())
      return;
    this->ReadI64(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb signed 64-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadI64(size_t nb, int64_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadI64();
  };
  
  /** 
   * Extracts @a nb unsigned 64-bit integers and return them as a vector.
   */
  template <class Derived>
  std::vector<uint64_t> BinaryStream<Derived>::ReadU64(size_t nb)
  {
    std::vector<uint64_t> values(nb);
    this->ReadU64(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<uint64_t> val(45,0);
   * bfs.ReadU64(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadU64(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU64(std::vector<uint64_t>& values)
  {
    if (values.empty())
      return;
    this->ReadU64(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb unsigned 64-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadU64(size_t nb, uint64_t* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadU64();
  };
  
  /** 
   * Extracts @a nb floats and return them as a vector.
   */
  template <class Derived>
  std::vector<float> BinaryStream<Derived>::ReadFloat(size_t nb)
  {
    std::vector<float> values(nb);
    this->ReadFloat(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<float> val(45,0);
   * bfs.ReadFloat(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadFloat(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadFloat(std::vector<float>& values)
  {
    if (values.empty())
      return;
    this->ReadFloat(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb floats and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadFloat(size_t nb, float* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadFloat();
  };
  
  /** 
   * Extracts @a nb doubles and return them as a vector.
   */
  template <class Derived>
  std::vector<double> BinaryStream<Derived>::ReadDouble(size_t nb)
  {
    std::vector<double> values(nb);
    this->ReadDouble(values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<double> val(45,0);
   * bfs.ReadDouble(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadDouble(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadDouble(std::vector<double>& values)
  {
    if (values.empty())
      return;
    this->ReadDouble(values.size(), &(values[0]));
  };
  
  /**
   * Extracts @a nb doubles and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadDouble(size_t nb, double* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadDouble();
  };
  
  /** 
   * Extracts @a nb strings with @a nb Charcharacters and return them as a vector.
   */
  template <class Derived>
  std::vector<std::string> BinaryStream<Derived>::ReadString(size_t nb, size_t nbChar)
  {
    std::vector<std::string> values(nb);
    this->ReadString(nbChar, values);
    return values;
  };
  
  /**
   * Extracts exactly the number of elements set in the vector @a values
   *
   * @note In case you want to assign only a part of the vector, you can use the method using an array.
   * For example;
   * @code
   * std::vector<uint8_t> val(45,0);
   * bfs.ReadU8(10, &(val[0])); // assign value #0-9
   * // ...
   * bfs.ReadU8(5, &(val[40])); // assign value #41-45
   * @endcode
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadString(size_t nbChar, std::vector<std::string>& values)
  {
    if (values.empty())
      return;
    this->ReadString(values.size(), nbChar, &(values[0]));
  };
  
  /**
   * Extracts @a nb unsigned 8-bit integers and set them in the array @a values.
   */
  template <class Derived>
  void BinaryStream<Derived>::ReadString(size_t nb, size_t nbChar, std::string* values)
  {
    for (size_t i = 0 ; i < nb ; ++i)
      values[i] = static_cast<Derived*>(this)->ReadString(nbChar);
  };
  
  /** 
   * Writes the vector of signed 8-bit integers @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<int8_t>& values)
  {
    size_t inc = 0;
    while (inc < values.size())
      static_cast<Derived*>(this)->Write(values[inc++]);
    return values.size() * 1;
  };
  
  /** 
   * Writes the vector of unsigned 8-bit integers @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<uint8_t>& values)
  {
    size_t inc = 0;
    while (inc < values.size())
      static_cast<Derived*>(this)->Write(values[inc++]);
    return values.size() * 1;
  };
  
  /** 
   * Writes the vector of signed 16-bit integers @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<int16_t>& values)
  {
    size_t inc = 0;
    while (inc < values.size())
      static_cast<Derived*>(this)->Write(values[inc++]);
    return values.size() * 2;
  };
  
  /** 
   * Writes the vector of unsigned 16-bit integers @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<uint16_t>& values)
  {
    size_t inc = 0;
    while (inc < values.size())
      static_cast<Derived*>(this)->Write(values[inc++]);
    return values.size() * 2;
  };
  
  /**
   * Writes the vector of signed 32-bit integers @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<int32_t>& values)
  {
    size_t inc = 0;
    while (inc < values.size())
      static_cast<Derived*>(this)->Write(values[inc++]);
    return values.size() * 2;
  };
  
  /**
   * Writes the vector of unsigned 32-bit integers @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<uint32_t>& values)
  {
    size_t inc = 0;
    while (inc < values.size())
      static_cast<Derived*>(this)->Write(values[inc++]);
    return values.size() * 2;
  };
  
  /** 
   * Writes the vector of floats @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<float>& values)
  {
    size_t inc = 0;
    while (inc < values.size())
      static_cast<Derived*>(this)->Write(values[inc++]);
    return values.size() * 4;
  };
  
  /** 
   * Writes the vector of strings @a values in the stream an return its size.
   */
  template <class Derived>
  size_t BinaryStream<Derived>::Write(const std::vector<std::string>& values)
  {
    size_t inc = 0;
    size_t writedBytes = 0;
    while (inc < values.size())
      writedBytes += static_cast<Derived*>(this)->Write(values[inc++]);
    return writedBytes;
  };
};