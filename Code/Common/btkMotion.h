/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#ifndef __btkMotion_h
#define __btkMotion_h

#include "btkDataObject.h"
#include "btkGeometry.h"

#include <string>
#include <vector>

#define EIGEN_USE_NEW_STDVECTOR
#include <Eigen/StdVector>


namespace btk
{
  class AbstractMotion: public DataObjectLabeled
  {
  public:
    typedef SharedPtr<AbstractMotion> Pointer;
    typedef SharedPtr<const AbstractMotion> ConstPointer;
    
    virtual Frame& GetFrame(int idx) = 0;
    virtual const Frame& GetFrame(int idx) const = 0;
    
    virtual Frame& operator[](int idx) = 0;
    virtual const Frame& operator[](int idx) const = 0;
    
  protected:
    AbstractMotion()
    : DataObjectLabeled()
    {};
    
    AbstractMotion(const AbstractMotion& toCopy)
    : DataObjectLabeled(toCopy)
    {};

  private:
    AbstractMotion& operator=(const AbstractMotion& ); // Not implemented.
  };
  
  class NoMotion: public AbstractMotion
  {
  public:
    typedef SharedPtr<NoMotion> Pointer;
    typedef SharedPtr<const NoMotion> ConstPointer;
    
    static Pointer New() {return Pointer(new NoMotion());};
    
    virtual ~NoMotion() {delete this->m_Fix;};
    
    virtual Frame& GetFrame(int idx) {return *(this->m_Fix);};
    virtual const Frame& GetFrame(int idx) const {return *(this->m_Fix);};
    
    virtual Frame& operator[](int idx) {return *(this->m_Fix);};
    virtual const Frame& operator[](int idx) const {return *(this->m_Fix);};
    
  private:
    NoMotion()
    : AbstractMotion()
    {
      this->m_Fix = new Frame();
    };
    NoMotion(const NoMotion& ); // Not implemented.
    NoMotion& operator=(const NoMotion& ); // Not implemented.
    
    Frame* m_Fix;
  };
  
  class Motion: public AbstractMotion
  {
  public:
    typedef SharedPtr<Motion> Pointer;
    typedef SharedPtr<const Motion> ConstPointer;
    
    typedef std::vector< Frame, Eigen::aligned_allocator<Frame> >::iterator Iterator;
    typedef std::vector< Frame, Eigen::aligned_allocator<Frame> >::const_iterator ConstIterator;
    
    static Pointer New() {return Pointer(new Motion());};
    
    Iterator Begin() {return this->m_Frames.begin();}
    ConstIterator Begin() const {return this->m_Frames.begin();}
    Iterator End() {return this->m_Frames.end();}
    ConstIterator End() const {return this->m_Frames.end();}
    
    BTK_MODELS_EXPORT virtual Frame& GetFrame(int idx);
    BTK_MODELS_EXPORT virtual const Frame& GetFrame(int idx) const;
    
    int GetFrameNumber() const {return this->m_Frames.size();};
    BTK_MODELS_EXPORT void SetFrameNumber(int num);
    
    Pointer Clone() const {return Pointer(new Motion(*this));};
    
    virtual Frame& operator[](int idx) {return this->m_Frames[idx];};
    virtual const Frame& operator[](int idx) const {return this->m_Frames[idx];};
    
  protected:
    BTK_MODELS_EXPORT Motion();

  private:
    BTK_MODELS_EXPORT Motion(const Motion& toCopy);
    Motion& operator=(const Motion& ); // Not implemented.
    
    std::vector< Frame, Eigen::aligned_allocator<Frame> > m_Frames;
  };
}

#endif // __btkMotion_h