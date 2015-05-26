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

#ifndef __btkPluginGait_p_h
#define __btkPluginGait_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkRig_p.h"
#include "btkProperty.h"
#include "btklardXpr.h"
#include "btkMacros.h" // _BTK_CONSTEXPR

#include <unordered_map>

namespace btk
{
  class Model;
  
  class PluginGait;
  
  class PluginGaitPrivate : public RigPrivate
  {
    BTK_DECLARE_PINT_ACCESSOR(PluginGait)
    
    BTK_DECLARE_NODEID(PluginGait, Rig)
    BTK_DECLARE_STATIC_PROPERTIES(PluginGait, Rig,
      Property<PluginGait,double,&PluginGait::markerDiameter,&PluginGait::setMarkerDiameter>{"markerDiameter"},
      Property<PluginGait,double,&PluginGait::headOffset,&PluginGait::setHeadOffset>{"headOffset"},
      Property<PluginGait,double,&PluginGait::rightShoulderOffset,&PluginGait::setRightShoulderOffset>{"rightShoulderOffset"},
      Property<PluginGait,double,&PluginGait::leftShoulderOffset,&PluginGait::setLeftShoulderOffset>{"leftShoulderOffset"},
      Property<PluginGait,double,&PluginGait::rightElbowWidth,&PluginGait::setRightElbowWidth>{"rightElbowWidth"},
      Property<PluginGait,double,&PluginGait::leftElbowWidth,&PluginGait::setLeftElbowWidth>{"leftElbowWidth"},
      Property<PluginGait,double,&PluginGait::rightWristWidth,&PluginGait::setRightWristWidth>{"rightWristWidth"},
      Property<PluginGait,double,&PluginGait::leftWristWidth,&PluginGait::setLeftWristWidth>{"leftWristWidth"},
      Property<PluginGait,double,&PluginGait::rightHandThickness,&PluginGait::setRightHandThickness>{"rightHandThickness"},
      Property<PluginGait,double,&PluginGait::leftHandThickness,&PluginGait::setLeftHandThickness>{"leftHandThickness"},
      Property<PluginGait,double,&PluginGait::interAsisDistance,&PluginGait::setInterAsisDistance>{"interAsisDistance"},
      Property<PluginGait,double,&PluginGait::rightLegLength,&PluginGait::setRightLegLength>{"rightLegLength"},
      Property<PluginGait,double,&PluginGait::leftLegLength,&PluginGait::setLeftLegLength>{"leftLegLength"},
      Property<PluginGait,double,&PluginGait::rightAsisTrochanterAPDistance,&PluginGait::setRightAsisTrochanterAPDistance>{"rightAsisTrochanterAPDistance"},
      Property<PluginGait,double,&PluginGait::leftAsisTrochanterAPDistance,&PluginGait::setLeftAsisTrochanterAPDistance>{"leftAsisTrochanterAPDistance"},
      Property<PluginGait,double,&PluginGait::rightKneeWidth,&PluginGait::setRightKneeWidth>{"rightKneeWidth"},
      Property<PluginGait,double,&PluginGait::leftKneeWidth,&PluginGait::setLeftKneeWidth>{"leftKneeWidth"},
      Property<PluginGait,double,&PluginGait::rightAnkleWidth,&PluginGait::setRightAnkleWidth>{"rightAnkleWidth"},
      Property<PluginGait,double,&PluginGait::leftAnkleWidth,&PluginGait::setLeftAnkleWidth>{"leftAnkleWidth"},
      Property<PluginGait,bool,&PluginGait::rightFootFlat,&PluginGait::setRightFootFlat>{"rightFootFlat"},
      Property<PluginGait,bool,&PluginGait::leftFootFlat,&PluginGait::setRightFootFlat>{"leftFootFlat"},
      Property<PluginGait,double,&PluginGait::rightStaticPlantarFlexionOffset>{"rightStaticPlantarFlexionOffset"},
      Property<PluginGait,double,&PluginGait::rightStaticRotationOffset>{"rightStaticRotationOffset"},
      Property<PluginGait,double,&PluginGait::leftStaticPlantarFlexionOffset>{"leftStaticPlantarFlexionOffset"},
      Property<PluginGait,double,&PluginGait::leftStaticRotationOffset>{"leftStaticRotationOffset"}
    )
    
  public:
    PluginGaitPrivate(PluginGait* pint, const std::string& name, int region, int side);
    ~PluginGaitPrivate() _BTK_NOEXCEPT;
    
    void computeHipJointCenter(double* HJC, double S, double C, double xdis) const _BTK_NOEXCEPT;
    bool calibrateLowerLimb(int side, const lard::Trajectory* HJC, std::unordered_map<std::string,lard::MappedTrajectory>* landmarks) _BTK_NOEXCEPT;
    bool generateUpperLimb(Model* model, Trial* trial, int side, const lard::Vector* u_torso, const lard::Vector* o_torso, std::unordered_map<std::string,lard::MappedTrajectory>* landmarks, double sampleRate, double startTime) const _BTK_NOEXCEPT;
    bool generateLowerLimb(Trial* trial, int side, const lard::Trajectory* HJC, std::unordered_map<std::string,lard::MappedTrajectory>* landmarks, double sampleRate, double startTime) const _BTK_NOEXCEPT;
    
    virtual Node* makePint() const override;
    
    double MarkerDiameter;
    
    double HeadOffset;
    double RightShoulderOffset;
    double LeftShoulderOffset;
    double RightElbowWidth;
    double LeftElbowWidth;
    double RightWristWidth;
    double LeftWristWidth;
    double RightHandThickness;
    double LeftHandThickness;
    
    double InterAsisDistance;
    double RightLegLength;
    double LeftLegLength;
    double RightAsisTrochanterAPDistance; 
    double LeftAsisTrochanterAPDistance;
    double RightKneeWidth;
    double LeftKneeWidth;
    double RightAnkleWidth;
    double LeftAnkleWidth;
    
    bool RightFootFlat;
    bool LeftFootFlat;
    double RightStaticPlantarFlexionOffset;
    double RightStaticRotationOffset;
    double LeftStaticPlantarFlexionOffset;
    double LeftStaticRotationOffset;
  };
  
  inline void PluginGaitPrivate::computeHipJointCenter(double* HJC, double S, double C, double xdis) const _BTK_NOEXCEPT
  {
    //const double theta = 0.49567350756639; // 28.4 * M_PI / 180.0;
    //const double beta = 0.314159265358979; // 18.0 * M_PI / 180.0;
    const double theta = 0.5;
    const double beta = 0.314;
    const double r = this->MarkerDiameter / 2.0;
    HJC[0] = C * cos(theta) * sin(beta) - (xdis + r) * cos(beta),
    HJC[1] = S * (C * sin(theta) - this->InterAsisDistance / 2.0),
    HJC[2] = -1.0 * (C * cos(theta) * cos(beta) + (xdis + r) * sin(beta));
  };
};

// ------------------------------------------------------------------------- //

namespace Eigen
{
namespace internal
{
  // ----------------------------------------------------------------------- //
  //                         ChordOpValues return value
  // ----------------------------------------------------------------------- //
  
  struct ChordOpValues;

  template <>
  struct traits<ChordOpValues>
  {
    using ReturnType = Eigen::Array<double,Eigen::Dynamic,3>;
  };
  
  struct ChordOpValues : public Eigen::ReturnByValue<ChordOpValues>
  {
    using StorageType = traits<ChordOpValues>::ReturnType;
    using Index = StorageType::Index;
    StorageType m_V;
  public:
    template <typename V> ChordOpValues(const V& v) : m_V(v) {};
    template <typename R> inline void evalTo(R& result) const {result.lazyAssign(this->m_V);};
    Index rows() const {return this->m_V.rows();};
    Index cols() const {return 3;};
  };
};
};

// ------------------------------------------------------------------------- //
  
namespace btk
{
  template <typename XprOne, typename XprTwo, typename XprThree> class ChordOp;
  
  namespace lard
  {
    template <typename XprOne, typename XprTwo, typename XprThree>
    struct Traits<ChordOp<XprOne,XprTwo,XprThree>>
    {
      using Values = typename Array<3>::Values;
      using Residuals = typename Array<3>::Residuals;
      using Index = typename Values::Index;
      static _BTK_CONSTEXPR int ColsAtCompileTime = 3;
      static _BTK_CONSTEXPR int Processing = Full;
    };
    
    template <typename XprOne, typename XprTwo, typename XprThree>
    struct Nested<ChordOp<XprOne, XprTwo, XprThree>>
    {
      using type = const ChordOp<XprOne, XprTwo, XprThree>; // Plain object
    };
  };
  
  template <typename XprOne, typename XprTwo, typename XprThree>
  class ChordOp : public lard::Base<ChordOp<XprOne, XprTwo, XprThree>>
  {
    static_assert(XprOne::ColsAtCompileTime == XprTwo::ColsAtCompileTime && XprOne::ColsAtCompileTime == XprThree::ColsAtCompileTime, "The number of columns must be the same.");
    static_assert(XprOne::ColsAtCompileTime == 3 && XprTwo::ColsAtCompileTime == 3 && XprThree::ColsAtCompileTime == 3, "The chord operation is only available for array with 3 columns.");
    
    using Index = typename lard::Traits<ChordOp<XprOne,XprTwo,XprThree>>::Index;
    
    const typename lard::Nested<XprOne>::type m_Xpr1;
    const typename lard::Nested<XprTwo>::type m_Xpr2;
    const typename lard::Nested<XprThree>::type m_Xpr3;
    double Offset;
    
  public:
    ChordOp(double offset, const lard::Base<XprOne>& x1, const lard::Base<XprTwo>& x2, const lard::Base<XprThree>& x3)
    : m_Xpr1(x1), m_Xpr2(x2), m_Xpr3(x3), Offset(offset)
    {
      assert(this->m_Xpr1.rows() == this->m_Xpr2.rows());
      assert(this->m_Xpr2.rows() == this->m_Xpr3.rows());
      assert(this->m_Xpr1.rows() > 0);
      assert(this->m_Xpr2.rows() > 0);
      assert(this->m_Xpr3.rows() > 0);
    };
    
    const ChordOp& derived() const _BTK_NOEXCEPT {return *this;};
    
    Index rows() const _BTK_NOEXCEPT {return this->m_Xpr1.rows();};
    
    const Eigen::internal::ChordOpValues values() const _BTK_NOEXCEPT
    {
      const auto& I = this->m_Xpr1;
      const auto& J = this->m_Xpr2;
      const auto& K = this->m_Xpr3;
      // Construct the frame associated with I, J, K
      const lard::Vector v = (J - I).normalized();
      const lard::Vector u = v.cross(K - I).normalized();
      const lard::Motion local(u, v, u.cross(v), (J + I) / 2.0);
      // Compute the angle to project I along v
      const auto d = (I - J).norm();
      const auto theta = (d.residuals() >= 0).select((this->Offset / d.values()).asin()*2.0, 0.0);
      // Do this projection in the local frame
      lard::Trajectory t = local.inverse().transform(I);
      const lard::Scalar::Values _tempX = t.values().middleCols<1>(1);
      const lard::Scalar::Values _tempY = t.values().rightCols<1>();
      t.values().leftCols<1>().setZero();
      t.values().middleCols<1>(1) = _tempX * theta.cos() - _tempY * theta.sin() ;
      t.values().rightCols<1>() = _tempX * theta.sin() + _tempY * theta.cos() ;
      // Transform back the projected point and return the result
      return Eigen::internal::ChordOpValues(local.transform(t).values());
    };
  
    auto residuals() const _BTK_NOEXCEPT -> decltype(ChordOp::generateResiduals((_BTK_LARD_DECLVAL_NESTED(XprOne).residuals() >= 0.0) && (_BTK_LARD_DECLVAL_NESTED(XprTwo).residuals() >= 0.0) && (_BTK_LARD_DECLVAL_NESTED(XprThree).residuals() >= 0.0)))
    {
      return ChordOp::generateResiduals((this->m_Xpr1.residuals() >= 0.0) && (this->m_Xpr2.residuals() >= 0.0) && (this->m_Xpr3.residuals() >= 0.0));
    };
  };
  
  template <typename XprOne, typename XprTwo, typename XprThree>
  static inline ChordOp<XprOne,XprTwo,XprThree> generate_chord_trajectory(double offset, const lard::Base<XprOne>& I, const lard::Base<XprTwo>& J, const lard::Base<XprThree>& K)
  {
    return ChordOp<XprOne,XprTwo,XprThree>(offset,I,J,K);
  };
};

#endif // __btkObject_p_h
