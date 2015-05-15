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

#ifndef __btklardModel_h
#define __btklardModel_h

#include "btklardXpr.h"
#include "btklardTrial.h"
#include "btkModel.h"
#include "btkTrial.h"
#include "btkTimeSequence.h"
#include "btkSegment.h"
#include "btkRig.h"
#include "btkLandmarksTranslator.h"
#include "btkRelativePosition.h"
#include "btkRelativeReferenceFrame.h"

#include <unordered_map>
#include <algorithm> // std::copy_n
#include <cassert>

namespace btk
{
namespace lard
{
  /**
   * Returns a collection of mapped time sequences based on the stored LandmarksTranslator in the given @a model.
   * If no translator was set in the model, try to find a Rig associated with the model and use its default translator.
   * If no translator was found, the keys used in the collection are directly the name of each time sequence.
   * If a translator was found, only the registered landmark will be extracted, their name converted and stored in the output collection.
   *
   * There is also extra outputs to determine the common sample rate and the common start time in all extracted landmarks.
   * If all the landmarks have the same sample rate, and if the output @a rate is given, its value will be assigned to the found common sample rate (-1.0 otherwise).
   * If all the landmarks have the same start time, and if the output @a start is given, its value will be assigned to the found common start time (-1.0 otherwise).
   * If all the landmarks have the same sample rate and start time, and if the output @a ok is given, its value will be assigned to true (false otherwise).
   */
  static inline std::unordered_map<std::string,lard::MappedTrajectory> extract_landmark_trajectories(Model* model, Trial* trial, double* rate = nullptr, double* start = nullptr, bool* ok = nullptr) _BTK_NOEXCEPT
  {
    std::unordered_map<std::string, lard::MappedTrajectory> trajectories;
    
    double sampleRate = -1.0, startTime = -1.0;
    bool common = true;

    auto lt = model->findChild<LandmarksTranslator*>({},{},false);
    // No defined translator? Let's use the one embedded within the rig (if any)
    if (lt == nullptr)
    {  
      auto rig = model->findChild<Rig*>({},{},false);
      if (rig != nullptr)
        lt = rig->defaultLandmarksTranslator();
    }
    auto markers = trial->timeSequences()->findChildren<TimeSequence*>({},{{"type",TimeSequence::Marker},{"components",4}},false);
    decltype(markers) landmarks;
    // No translator found? Create trajectories for all the markers found
    if (lt == nullptr)
    {
      for (auto it = markers.cbegin() ; it != markers.cend() ; ++it)
      {
        trajectories.insert(std::make_pair((*it)->name(),lard::MappedTrajectory((*it)->samples(),(*it)->data(),(*it)->data()+(*it)->samples()*3)));
        landmarks.push_back(*it);
      }
    }
    // Convert only the markers found in the translator
    else
    {
      for (auto it = markers.cbegin() ; it != markers.cend() ; ++it)
      {
        std::string name = lt->convertIfExists((*it)->name());
        if (!name.empty())
        {
          trajectories.insert(std::make_pair(name,lard::MappedTrajectory((*it)->samples(),(*it)->data(),(*it)->data()+(*it)->samples()*3)));
          landmarks.push_back(*it);
        }
      }
    }
    // Detect the common sampling information
    for (auto it = landmarks.cbegin() ; it != landmarks.cend() ; ++it)
    {
      if (sampleRate == -1.0)
      {
        sampleRate = (*it)->sampleRate();
        startTime = (*it)->startTime();
      }
      else
      {
        if (fabs(sampleRate - (*it)->sampleRate()) > std::numeric_limits<float>::epsilon())
          common = false;
        else if (fabs(startTime - (*it)->startTime()) > std::numeric_limits<float>::epsilon())
          common = false;
      }
    }
    // Assign the sampling information (if the inout parameters are given) 
    if (!common)
    {
      sampleRate = -1.0;
      startTime = -1.0;
    }
    if (rate != nullptr)
      *rate = sampleRate;
    if (start != nullptr)
      *start = startTime;
    if (ok != nullptr)
      *ok = common;
    return trajectories;
  };

  /**
   * Returns the motion associated with the given relative reference frame (@a relframe) and the segment's motion (@a motion) including all the relative reference frame between them (i.e. the relative reference frame (@a relframe) and the segment (@a seg)).
   *
   * NOTE: This function does not check if the given relative reference frame (@a relframe), segment (@a seg), and associated motion (@a motion) are not null.
   */
  template <typename T>
  static inline Motion transform_relative_frame(const RelativeReferenceFrame* relframe, const Segment* seg, const lard::Base<T>* motion)
  {
    static_assert(T::ColsAtCompileTime == 12, "Only data with 12 columns (to represent a motion) can be used with this function.");
    assert(seg->findChild<const RelativeReferenceFrame*>(relframe->name()) == relframe);
    auto path = seg->retrievePath(relframe);
    // No path found?
    if (path.empty())
      return Motion();
    path.pop_front(); // Remove the first node as it is the segment
    path.pop_back(); // Remove the last node as it is the relative reference frame
    // Compute the motion
    const double res[1] = {0.};
    Motion temp, mot(1); mot.setZeroResiduals();
    std::copy_n(relframe->data(), 12, mot.values().data());
    for (auto it = path.crbegin() ; it != path.crend() ; ++it)
    {
      auto relrefframe = node_cast<const RelativeReferenceFrame*>(*it);
      if (relrefframe == nullptr)
        continue;
      temp = Map<const Motion>(1,relrefframe->data(),res).transform(mot);
      mot = temp;
    }
    const T* _motion = static_cast<const T*>(motion);
    temp = _motion->transform(mot.replicate(_motion->rows()));
    mot = temp;
    return mot;
  };
  
  /**
   * Returns the trajectory associated with the given relative position (@a relpos) and the segment's motion (@a motion) including all the relative reference frame between them (i.e. the relative position (@a relpos) and the segment (@a seg)).
   *
   * NOTE: This function does not check if the given relative position (@a relpos), segment (@a seg), and associated motion (@a motion) are not null.
   */
  template <typename T>
  static inline Trajectory transform_relative_position(const RelativePosition* relpos, const Segment* seg, const lard::Base<T>* motion)
  {
    assert(seg->findChild<const RelativePosition*>(relpos->name()) == relpos);
    auto path = seg->retrievePath(relpos);
    path.pop_front(); // Remove the first node as it is the segment
    path.pop_back(); // Remove the last node as it is the relative position
    // Compute the trajectory
    const double res[1] = {0.};
    Trajectory temp, traj(1); traj.setZeroResiduals();
    std::copy_n(relpos->data(), 3, traj.values().data());
    for (auto it = path.crbegin() ; it != path.crend() ; ++it)
    {
      auto relrefframe = node_cast<const RelativeReferenceFrame*>(*it);
      if (relrefframe == nullptr)
        continue;
      temp = Map<const Motion>(1,relrefframe->data(),res).transform(traj);
      traj = temp;
    }
    const T* _motion = static_cast<const T*>(motion);
    temp = _motion->transform(traj.replicate(_motion->rows()));
    traj = temp;
    return traj;
  };
};
};

#endif // __btklardModel_h