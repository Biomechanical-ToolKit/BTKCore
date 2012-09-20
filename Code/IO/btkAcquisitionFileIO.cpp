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

#include "btkAcquisitionFileIO.h"

namespace btk
{
  /**
   * @class AcquisitionFileIO btkAcquisitionFileIO.h
   * @brief Interface to read/write acquisition's files.
   *
   * @ingroup BTKIO
   */
  /**
   * @var AcquisitionFileIO::m_FileType
   * File type.
   */
  /**
   * @var AcquisitionFileIO::m_ByteOrder
   * Byte order.
   */
  /**
   * @var AcquisitionFileIO::m_StorageFormat
   * Storage format.
   */
  
  /**
   * @typedef AcquisitionFileIO::Pointer
   * Smart pointer associated with an AcquisitionFileIO object.
   */
  
  /**
   * @typedef AcquisitionFileIO::ConstPointer
   * Smart pointer associated with a const AcquisitionFileIO object.
   */
  
  /**
   * @enum AcquisitionFileIO::FileType
   * Enums used to specify the type of the file.
   */
  /**
   * @var AcquisitionFileIO::FileType AcquisitionFileIO::TypeNotApplicable
   * Enum value used to indicate the type of the file is not yet known.
   */
  /**
   * @var AcquisitionFileIO::FileType AcquisitionFileIO::ASCII
   * Enum value for ASCII file.
   */
  /**
   * @var AcquisitionFileIO::FileType AcquisitionFileIO::Binary
   * Enum value for binary file.
   */
  
  /**
   * @enum AcquisitionFileIO::ByteOrder
   * Enums used to specify the byte order of the file (integer and float).
   */
  /**
   * @var AcquisitionFileIO::ByteOrder AcquisitionFileIO::OrderNotApplicable
   * Enum value used to indicate the byte order is not yet known.
   */
  /**
   * @var AcquisitionFileIO::ByteOrder AcquisitionFileIO::VAX_LittleEndian
   * Enum value for a byte order in little endian with a VAX float representation.
   */
  /**
   * @var AcquisitionFileIO::ByteOrder AcquisitionFileIO::IEEE_LittleEndian
   * Enum value for a byte order in little endian with a IEEE float representation.
   */
  /**
   * @var AcquisitionFileIO::ByteOrder AcquisitionFileIO::IEEE_BigEndian
   * Enum value for a byte order in big endian with a IEEE float representation.
   */

  /**
   * @enum AcquisitionFileIO::StorageFormat
   * Enums used to specify the format of the stored data.
   */
  /**
   * @var AcquisitionFileIO::StorageFormat AcquisitionFileIO::StorageNotApplicable
   * Acquisition's data have no storage format.
   */
  /**
   * @var AcquisitionFileIO::StorageFormat AcquisitionFileIO::Float
   * Acquisition's data are stored as float values.
   */
  /**
   * @var AcquisitionFileIO::StorageFormat AcquisitionFileIO::Integer
   * Acquisition's data are stored as integer values.
   */
  
  /**
   * @fn virtual const Extensions& AcquisitionFileIO::GetSupportedExtensions() const = 0;
   * Return the suppored extensions by this file IO.
   */
  
  /**
   * @fn FileType AcquisitionFileIO::GetFileType() const
   * Gets the type of the file.
   */
  
  /**
   * @fn ByteOrder AcquisitionFileIO::GetByteOrder() const
   * Gets the byte order of the file (only for binary file).
   */

  /**
   * Gets the byte order as a string.
   */
  std::string AcquisitionFileIO::GetByteOrderAsString() const
  {
    std::string str = "";
    switch (this->m_ByteOrder)
    {
    case OrderNotApplicable:
      str = "OrderNotApplicable";
      break;
    case IEEE_LittleEndian:
      str = "IEEE_LittleEndian";
      break;
    case VAX_LittleEndian:
      str = "VAX_LittleEndian";
      break;
    case IEEE_BigEndian:
      str = "IEEE_BigEndian";
      break;
    }
    return str;
  };
  
  
  /**
   * Gets the storage format as a string.
   */
  std::string AcquisitionFileIO::GetStorageFormatAsString() const
  {
    std::string str = "";
    switch (this->m_StorageFormat)
    {
    case StorageNotApplicable:
      str = "StorageNotApplicable";
      break;
    case Float:
      str = "Float";
      break;
    case Integer:
      str = "Integer";
      break;
    }
    return str;
  };

  /**
   * @fn double AcquisitionFileIO::GetStorageFormat() const
   * Return the format used to store points and analog channels.
   */

  /**
   * @fn virtual bool AcquisitionFileIO::CanReadFile(const std::string& filename) = 0
   * Checks if @a filename can be read by this AcquisitionFileIO. This methods 
   * should try to read the file header instead to check the file's suffix.
   */
  
  /**
   * @fn virtual bool AcquisitionFileIO::CanWriteFile(const std::string& filename) = 0
   * Checks if @a filename can be write by this AcquisitionFileIO. This method 
   * should try to determine if the file's suffix corresponds to one of the supported
   * file's suffixes by this AcquisitionFileIO.
   */
  
  /**
   * @fn virtual void AcquisitionFileIO::Read(const std::string& filename, Acquisition::Pointer output) = 0
   * Read the file designated by @a filename and fill @a output.
   */
  
  /**
   * @fn virtual void AcquisitionFileIO::Write(const std::string& filename, Acquisition::Pointer input) = 0
   * Write the file designated by @a filename with the content of @a input.
   */
   
  /**
   * Constructor.
   */
  AcquisitionFileIO::AcquisitionFileIO(FileType f, ByteOrder b, StorageFormat s)
  {
    this->m_FileType = f;
    this->m_ByteOrder = b;
    this->m_StorageFormat = s;
  };
  
  /**
   * @fn virtual AcquisitionFileIO::~AcquisitionFileIO()
   * Empty destructor.
   */
  
  /**
   * @fn void AcquisitionFileIO::SetFileType(FileType f)
   * Sets the type of the file. This method is protected because some file can
   * be only ASCII file or binary file. An inherited class can use this method as
   * public with @c using @c AcquisitionFileIO::SetFileType; in its public part.
   */
   
  /**
   * @class AcquisitionFileIO::Extension
   * @brief Native extension used with an acquisition file IO.
   *
   * @note This class should be only used with the macro BTK_IO_FILE_SUPPORTED_EXTENSIONS during the declaration of a class inheriting from btk::AcquisitionFileIO if several file formats are supported.
   * For example:
   * @code
   * class FooFileIO : public AcquisitionFileIO
   * {
   *   BTK_IO_FILE_SUPPORTED_EXTENSIONS(Extension("FOO") | Extension("BAR"));
   * public:
   * // ...
   * };
   * @endcode
   */
  /**
   * @var AcquisitionFileIO::Extension::name
   * Name of the supported file format.
   */
  /**
   * @var AcquisitionFileIO::Extension::desc
   * Description associated with the file format.
   */
  
  /**
   * @fn AcquisitionFileIO::Extension::Extension(const std::string& n)
   * Constructor.
   */

  /**
   * @fn AcquisitionFileIO::Extension::Extension(const std::string& n, const std::string& d)
   * Constructor where the name and the description can be set.
   */
   
  /**
    * @class AcquisitionFileIO::Extensions
    * @brief List of AcquisitionFileIO::Extension object.
    * @note This class should not be used. The macro BTK_IO_FILE_SUPPORTED_EXTENSIONS embedded the construction of list of extensions.
    * To set several extensions for a same acquisition file IO, you can use the operator AcquisitionFileIO::Extensions::operator|()
    * For example:
    * @code
    * class FooFileIO : public AcquisitionFileIO
    * {
    *   BTK_IO_FILE_SUPPORTED_EXTENSIONS(Extension("FOO") | Extension("BAR"));
    * public:
    * // ...
    * };
    * @endcode
    */
    
  /**
   * @typedef AcquisitionFileIO::Extensions::Iterator
   * Iterator for items contained in the Extensions object.
   */
  
  /**
   * @typedef AcquisitionFileIO::Extensions::ConstIterator
   * Const iterator for items contained in the Extensions object.
   */
  
  /**
   * @fn AcquisitionFileIO::Extensions::Extensions()
   * Simple constructor which initiate an empty list of extensions.
   */
  
  /**
   * @fn AcquisitionFileIO::Extensions::Iterator AcquisitionFileIO::Extensions::Begin()
   *  Returns an iterator to the beginning of the list of extensions.
   */
  
  /**
   * @fn AcquisitionFileIO::Extensions::ConstIterator AcquisitionFileIO::Extensions::Begin() const
   *  Returns a const iterator to the beginning of the list of extensions.
   */
  
  /**
   * @fn AcquisitionFileIO::Extensions::Iterator AcquisitionFileIO::Extensions::End()
   * Returns an iterator just past the last extensions.
   */
  
  /**
   * @fn AcquisitionFileIO::Extensions::ConstIterator AcquisitionFileIO::Extensions::End() const
   * Returns a const iterator just past the last extensions.
   */

  /**
   * @fn void AcquisitionFileIO::Extensions::Append(const Extensions& items)
   * Append a list of extensions to this list.
   */
  
  /**
   * @fn int AcquisitionFileIO::Extensions::GetSize() const
   * Returns the number of extensions in the list.
   */
   
  /**
   * @fn AcquisitionFileIO::Extensions& AcquisitionFileIO::Extensions::operator<< (const std::string& name)
   * Append an extension to the list.
   */
   
  /**
   * @fn AcquisitionFileIO::Extensions& AcquisitionFileIO::Extensions::operator<< (const AcquisitionFileIO::Extension& item) 
   * Append an extension to the list.
   */
   
  /**
   * @fn AcquisitionFileIO::Extensions& AcquisitionFileIO::Extensions::operator| (const AcquisitionFileIO::Extension& item)
   * Append an extension to the list.
   */
};
