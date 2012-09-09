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

#ifndef __btkAcquisitionFileIO_h
#define __btkAcquisitionFileIO_h

#include "btkAcquisition.h"

#include <string>

namespace btk
{
  class AcquisitionFileIO
  {
  public:
    class Extensions;
    
    typedef SharedPtr<AcquisitionFileIO> Pointer;
    typedef SharedPtr<const AcquisitionFileIO> ConstPointer;
    
    typedef enum {TypeNotApplicable, ASCII, Binary} FileType;
    typedef enum {OrderNotApplicable = 0, IEEE_LittleEndian, VAX_LittleEndian, IEEE_BigEndian} ByteOrder;
    typedef enum {StorageNotApplicable = 0, Float = -1, Integer = 1} StorageFormat;

    virtual const Extensions& GetSupportedExtensions() const = 0;

    FileType GetFileType() const {return this->m_FileType;};
    ByteOrder GetByteOrder() const {return this->m_ByteOrder;};
    BTK_IO_EXPORT std::string GetByteOrderAsString() const;
    StorageFormat GetStorageFormat() const {return this->m_StorageFormat;};
    BTK_IO_EXPORT std::string GetStorageFormatAsString() const;

    virtual bool CanReadFile(const std::string& filename) = 0;
    virtual bool CanWriteFile(const std::string& filename) = 0;
    virtual void Read(const std::string& filename, Acquisition::Pointer output) = 0;
    virtual void Write(const std::string& filename, Acquisition::Pointer input) = 0;
    
    struct Extension
    {
      Extension(const std::string& n) : name(n) {};
      Extension(const std::string& n, const std::string& d) : name(n), desc(d) {};
      std::string name;
      std::string desc;
    };
    
    class Extensions
    {
    public:
      typedef std::list<Extension>::iterator Iterator;
      typedef std::list<Extension>::const_iterator ConstIterator;
      Extensions() : m_Items() {};
      Iterator Begin() {return this->m_Items.begin();};
      ConstIterator Begin() const {return this->m_Items.begin();};
      Iterator End() {return this->m_Items.end();};
      ConstIterator End() const {return this->m_Items.end();};
      void Append(const Extensions& items) {this->m_Items.insert(this->End(), items.Begin(), items.End());};
      int GetSize() const {return static_cast<int>(this->m_Items.size());};
      Extensions& operator<< (const std::string& name) {this->m_Items.push_back(Extension(name)); return *this;};
      Extensions& operator<< (const Extension& item) {this->m_Items.push_back(item); return *this;};
      Extensions& operator| (const Extension& item) {this->m_Items.push_back(item); return *this;};
    private:
      std::list<Extension> m_Items;
    };
    
  protected:
    BTK_IO_EXPORT AcquisitionFileIO();
    virtual ~AcquisitionFileIO() {};
    
    void SetFileType(FileType f) {this->m_FileType = f;};
        
    FileType m_FileType;
    ByteOrder m_ByteOrder;
    StorageFormat m_StorageFormat;
    
  private:
    AcquisitionFileIO(const AcquisitionFileIO& ); // Not implemented.
    AcquisitionFileIO& operator=(const AcquisitionFileIO& ); // Not implemented.
  };
};

#define BTK_IO_ENABLE_BYTEORDER_MUTATOR \
  public: \
    void SetByteOrder(ByteOrder b) {this->m_ByteOrder = b;};
  
#define BTK_IO_ENABLE_STORAGEFORMAT_MUTATOR \
  public: \
    void SetStorageFormat(StorageFormat s) {this->m_StorageFormat = s;};
  
#define BTK_IO_FILE_ONLY_READ_OPERATION \
  public: \
    virtual bool CanWriteFile(const std::string& ) {btkErrorMacro("Writing operations not supported. Wrong macro?");return false;}; \
    virtual void Write(const std::string& , btk::Acquisition::Pointer ) {btkErrorMacro("Writing operations not supported. Wrong macro?");};
  
#define BTK_IO_FILE_ONLY_WRITE_OPERATION \
  public: \
    virtual bool CanReadFile(const std::string& ) {btkErrorMacro("Reading operations not supported. Wrong macro?");return false;}; \
    virtual void Read(const std::string& , btk::Acquisition::Pointer ) {btkErrorMacro("Reading operations not supported. Wrong macro?");};
  
#define BTK_IO_FILE_SUPPORTED_EXTENSIONS(ext) \
   public: \
    virtual const btk::AcquisitionFileIO::Extensions& GetSupportedExtensions() const {static const btk::AcquisitionFileIO::Extensions SupportedExtensions(btk::AcquisitionFileIO::Extensions() << ext); return SupportedExtensions;};
  
#endif // __btkAcquisitionFileIO_h
