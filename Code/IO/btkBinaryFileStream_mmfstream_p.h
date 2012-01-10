/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *   * Redistributions of source code must retain the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials
 *     provided with the distribution.
 *   * Neither the name(s) of the copyright holders nor the names
 *     of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
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

#ifndef __btkBinaryFileStream_mmfstream_p_h
#define __btkBinaryFileStream_mmfstream_p_h

#if defined(_MSC_VER) // Windows
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
  // Defining NOMINMAX to prevent compiler error with std::min/std::max when including windows.h
  #ifndef NOMINMAX
    #define NOMINMAX
  # endif
  #include <windows.h>
  #define BTK_MMFILEBUF_NO_FILE INVALID_HANDLE_VALUE
#else // POSIX
  static const int BTK_MMFILEBUF_NO_FILE = -1;
#endif

#include <ios>

namespace btk
{
  class mmfilebuf
  {
  public:
    inline mmfilebuf();
    ~mmfilebuf() {this->close();};
    
    mmfilebuf* open(const char* s, std::ios_base::openmode mode);
    bool is_open() const {return !(this->m_File == BTK_MMFILEBUF_NO_FILE);};
    mmfilebuf* close();
    
    bool is_eob() const {return (this->m_Position == this->m_BufferSize + 1);}; // End of buffer
    bool writemode() const {return this->m_Writing;};
    
    std::streamsize size() const {return this->m_BufferSize;};
    const char* data() const {return this->mp_Buffer;};
    
    std::streampos pubseekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) {return this->seekoff(off, way, which);};
    std::streampos pubseekpos(std::streampos pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out ) {return this->seekpos(pos, which);};
    std::streamsize sgetn(char* s, std::streamsize n);
    std::streamsize sputn(const char* s, std::streamsize n);
    
  protected:
    std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
    std::streampos seekpos(std::streampos pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );
    
    mmfilebuf* mapfile();
    mmfilebuf* unmapfile();
    mmfilebuf* resizemap();
    
    static int granularity();
    
  private:
    char* mp_Buffer;
    std::streamsize m_BufferSize;
    std::streamsize m_LogicalSize;
#if defined(HAVE_SYS_MMAP)
    int m_File;
#else
    HANDLE m_File;
    HANDLE m_Map;
#endif
    std::streamoff m_Position;
    bool m_Writing;
  };
  
  class mmfstream
  {
  public:
    typedef std::ios_base::failure failure;
    
    inline mmfstream();
    inline mmfstream(const char* s, std::ios_base::openmode mode);
    // ~mmfstream(); // Implicit;
    
    // Open/Close
    const mmfilebuf* rdbuf() const {return &this->m_Filebuf;};
    inline void open(const char* s, std::ios_base::openmode mode);
    bool is_open() const {return m_Filebuf.is_open();};
    inline void close();
    
    // Stream buffer state
    std::ios_base::iostate rdstate() const {return this->m_FilebufState;};
    bool good() const {return this->rdstate() == 0;};
    bool eof() const {return (this->rdstate() & std::ios_base::eofbit) != 0;};
    bool fail() const {return (this->rdstate() & (std::ios_base::badbit | std::ios_base::failbit)) != 0;};
    bool bad() const {return (this->rdstate() & std::ios_base::badbit) != 0;};
    std::ios_base::iostate exceptions() const {return this->m_Exception;};
    inline void exceptions(std::ios_base::iostate except);
    inline void clear(std::ios_base::iostate state = std::ios_base::goodbit);
    void setstate(std::ios_base::iostate state) {this->clear(this->rdstate() | state);};
    
    // Read
    mmfstream& read(char* s, std::streamsize n);
    std::streampos tellg() {return !this->fail() ? this->m_Filebuf.pubseekoff(0,std::ios_base::cur,std::ios_base::in) : std::streampos(std::streamoff(-1));};
    inline mmfstream& seekg(std::streampos pos);
    inline mmfstream& seekg(std::streamoff off, std::ios_base::seekdir dir);
    
    // Write
    mmfstream& write(const char* s, std::streamsize n);
    inline mmfstream& seekp(std::streampos pos);
    inline mmfstream& seekp(std::streamoff off, std::ios_base::seekdir dir);    
    
  private:    
    mmfstream(const mmfstream& ); // Not implemented.
    mmfstream& operator=(const mmfstream& ); // Not implemnted
    
    mmfilebuf m_Filebuf;
    std::ios_base::iostate m_FilebufState;
    std::ios_base::iostate m_Exception;
  };
    
  // ------------------------------------------------------------ //
  //                 Memory Mapped File Buffer                    //
  // ------------------------------------------------------------ //
  
  mmfilebuf::mmfilebuf()
  {
    this->mp_Buffer = 0;
    this->m_BufferSize = -1;
    this->m_LogicalSize = 0;
    this->m_File = BTK_MMFILEBUF_NO_FILE;
#if defined(_MSC_VER)
    this->m_Map = 0;
#endif
    this->m_Position = -1;
    this->m_Writing = false;
  };
  
  // ------------------------------------------------------------ //
   //                 Memory Mapped File Stream                    //
   // ------------------------------------------------------------ //
  
  mmfstream::mmfstream()
  : m_Filebuf()
  {
    this->m_FilebufState = std::ios_base::goodbit;
    this->m_Exception = std::ios_base::goodbit;
  };
  
  mmfstream::mmfstream(const char* s, std::ios_base::openmode mode)
  : m_Filebuf()
  {
    this->m_FilebufState = std::ios_base::goodbit;
    this->m_Exception = std::ios_base::goodbit;
    this->open(s, mode);
  };
  
  void mmfstream::open(const char* s, std::ios_base::openmode mode)
  {
    if (!this->m_Filebuf.open(s, mode))
      this->setstate(std::ios_base::failbit);
    else
      this->clear();
  };
  
  void mmfstream::close()
  {
    if (!this->m_Filebuf.close())
      this->setstate(std::ios_base::failbit);
  };
  
  void mmfstream::exceptions(std::ios_base::iostate except)
  {
    this->m_Exception = except;
    this->clear(this->m_FilebufState);
  };
  
  void mmfstream::clear(std::ios_base::iostate state)
  {
    this->m_FilebufState = state;
    if (this->exceptions() & this->rdstate())
      throw failure("mmfstream::clear");
  };
  
  mmfstream& mmfstream::seekg(std::streampos pos)
  {
    if (!this->fail()
        && (this->m_Filebuf.pubseekpos(pos, std::ios_base::in) == std::streampos(std::streamoff(-1))))
      this->setstate(std::ios_base::failbit);
    return *this;
  };
  
  mmfstream& mmfstream::seekg(std::streamoff off, std::ios_base::seekdir dir)
  {
    if (!this->fail()
        && (this->m_Filebuf.pubseekoff(off, dir, std::ios_base::in) == std::streampos(std::streamoff(-1))))
      this->setstate(std::ios_base::failbit);
    return *this;
  };
  
  mmfstream& mmfstream::seekp(std::streampos pos)
  {
    if (!this->fail()
        && (this->m_Filebuf.pubseekpos(pos, std::ios_base::out) == std::streampos(std::streamoff(-1))))
      this->setstate(std::ios_base::failbit);
    return *this;
  };
  
  mmfstream& mmfstream::seekp(std::streamoff off, std::ios_base::seekdir dir)
  {
    if (!this->fail()
        && (this->m_Filebuf.pubseekoff(off, dir, std::ios_base::out) == std::streampos(std::streamoff(-1))))
      this->setstate(std::ios_base::failbit);
    return *this;
  };
};

#endif // __btkBinaryFileStream_mmfstream_p_h