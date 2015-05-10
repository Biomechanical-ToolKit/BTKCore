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

#ifndef __btklardTrial_h
#define __btklardTrial_h

#include "btklardXpr.h"
#include "btkTrial.h"
#include "btkTimeSequence.h"

#include <algorithm> // std::copy_n
#include <cassert>

namespace btk
{
namespace lard
{
  /**
   * Return the mapping of a time sequence known as a TimeSequence::Marker stored in @a parent.
   * If no time sequence exists with the given @a name, an empty (unvalid) mapping is returned.
   * Use the method MappedTrajectory::isValid() to know if the returned mapping is valid or not.
   */
  static inline MappedTrajectory extract_trajectory(const std::string name, Node* parent)
  {
    auto ts = parent->findChild<TimeSequence*>(name,{{"type",TimeSequence::Marker},{"components",4}},false);
    if (ts == nullptr)
      return MappedTrajectory(0,nullptr,nullptr);
    return MappedTrajectory(ts->samples(),ts->data(),ts->data()+ts->samples()*3);
  }
  
 /**
  * Return the mapping of a time sequence known as a TimeSequence::Pose stored in @a parent.
  * If no time sequence exists with the given @a name, an empty (unvalid) mapping is returned.
  * Use the method MappedMotion::isValid() to know if the returned mapping is valid or not.
  */
 static inline MappedMotion extract_motion(const std::string name, Node* parent)
 {
   auto ts = parent->findChild<TimeSequence*>(name,{{"type",TimeSequence::Pose},{"components",13}},false);
   if (ts == nullptr)
     return MappedMotion(0,nullptr,nullptr);
   return MappedMotion(ts->samples(),ts->data(),ts->data()+ts->samples()*12);
 }
  
 /**
   * Export raw arrays to a time sequence.
   * WARNING: the @a components must contains the number of columns of the @a values plus one (column of the @a residuals).
   * The behaviour of this function is the following:
   *  - First, look for an existing time sequence with the given name and type.
   *  - If a time sequence is found, its content is replaced by the given one
   *  - Otherwise, a new time sequence is created
   */
  static inline TimeSequence* export_timesequence(unsigned components, unsigned samples, const double* values, const double* residuals, const std::string& name, double rate, double start, int type, const std::string& unit, Node* parent)
  {
    assert(components > 0);
    assert(samples > 0);
    auto ts = parent->findChild<TimeSequence*>(name,{{"type",type}},false);
    if (ts != nullptr)
    {
      assert(ts->components() == components);
      ts->resize(samples); // Grow / shrink the data (if necessary)
      ts->setSampleRate(rate); // Assign possibly a new rate
      ts->setStartTime(start); // Same for the start time
      ts->setUnit(unit); // Same for the unit
    }
    else
    {
      ts = new TimeSequence(name, components, samples, rate, start, type, unit, parent);
    }
    if ((values != nullptr) && (residuals != nullptr))
    {  
      const unsigned valuesComponents = components - 1;
      const unsigned offset = valuesComponents * samples;
      std::copy_n(values, samples * valuesComponents, ts->data());
      std::copy_n(residuals, samples, ts->data() + offset);
    }
    return ts;
  };
  
  /**
   * Convenient method to transform lard data to a time sequence.
   */
  template <typename T>
  static inline TimeSequence* export_timesequence(const lard::Data<T>* source, const std::string& name, double rate, double start, int type, const std::string& unit, Node* parent)
  {
    return export_timesequence(source->cols()+1, source->rows(), source->values().data(), source->residuals().data(), name, rate, start, type, unit, parent);
  };
 
  /**
   * Convenient method to export the vectors U, V, W and orign O (aka the content of lard::Motion) to a timesequence 
   */
  template <typename U, typename V, typename W, typename O>
  inline TimeSequence* export_timesequence(const lard::Data<U>* u,const lard::Data<V>* v, const lard::Data<W>* w, const lard::Data<O>* o, const std::string& name, double rate, double start, Node* parent)
  {
    static_assert(U::ColsAtCompileTime == 3, "Only data with 3 columns (e.g to represent a vector) can be used with this function.");
    static_assert(V::ColsAtCompileTime == 3, "Only data with 3 columns (e.g to represent a vector) can be used with this function.");
    static_assert(W::ColsAtCompileTime == 3, "Only data with 3 columns (e.g to represent a vector) can be used with this function.");
    static_assert(O::ColsAtCompileTime == 3, "Only data with 3 columns (e.g to represent a vector) can be used with this function.");
    assert(u->rows() == v->rows());
    assert(v->rows() == w->rows());
    assert(w->rows() == o->rows());
    auto ts = export_timesequence(13, u->rows(), nullptr, nullptr, name, rate, start, TimeSequence::Pose, "", parent);
    const Motion::Residuals residuals = Motion::generateResiduals((u->residuals() >= 0) && (v->residuals() >= 0) && (w->residuals() >= 0) && (o->residuals() >= 0));
    const unsigned samples = 3 * residuals.rows();
    std::copy_n(u->values().data(), samples, ts->data());
    std::copy_n(v->values().data(), samples, ts->data() + samples);
    std::copy_n(w->values().data(), samples, ts->data() + 2 * samples);
    std::copy_n(o->values().data(), samples, ts->data() + 3 * samples);
    std::copy_n(residuals.data(), residuals.rows(), ts->data() + 4 * samples);
    return ts;
  };
};
};

#endif // __btklardTrial_h