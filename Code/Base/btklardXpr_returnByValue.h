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

#ifndef __btklardXpr_returnByValue_h
#define __btklardXpr_returnByValue_h

#include <Eigen/Geometry> // eulerAngles

#include <type_traits>

namespace Eigen
{
namespace internal
{ 
  // ----------------------------------------------------------------------- //
  //                            CrossOp return value
  // ----------------------------------------------------------------------- //

  template<typename V1, typename V2> struct CrossOpValues;

  template<typename V1, typename V2>
  struct traits<CrossOpValues<V1,V2>>
  {
    using ReturnType = typename btk::lard::Traits<btk::lard::Array<std::decay<V1>::type::ColsAtCompileTime>>::Values;
  };
  
  template<typename V1, typename V2>
  struct CrossOpValues : public Eigen::ReturnByValue<CrossOpValues<V1,V2>>
  {
    using InputType1 = typename std::decay<V1>::type;
    using InputType2 = typename std::decay<V2>::type;
    using Index = typename InputType1::Index;
    typename InputType1::Nested m_V1;
    typename InputType2::Nested m_V2;
  public:
    CrossOpValues(const V1& v1, const V2& v2) : m_V1(v1), m_V2(v2) {};
    template <typename R> inline void evalTo(R& result) const
    {
      // TODO: IS THERE A BETTER WAY TO IMPLEMENT THE CROSS PRODUCT?
      const auto& v1x = this->m_V1.col(0);
      const auto& v1y = this->m_V1.col(1);
      const auto& v1z = this->m_V1.col(2);
      const auto& v2x = this->m_V2.col(0);
      const auto& v2y = this->m_V2.col(1);
      const auto& v2z = this->m_V2.col(2);
      // result.resize(this->rows(),Eigen::NoChange);
      result.col(0) = (v1y * v2z) - (v1z * v2y);
      result.col(1) = (v1z * v2x) - (v1x * v2z);
      result.col(2) = (v1x * v2y) - (v1y * v2x);
    };
    Index rows() const {return this->m_V1.rows();};
    Index cols() const {return this->m_V1.cols();};
  };
  
  // ----------------------------------------------------------------------- //
  //                         TransformOp return value
  // ----------------------------------------------------------------------- //

  template<typename V1, typename V2> struct TransformOpValues;
  
  template<typename V1, typename V2>
  struct traits<TransformOpValues<V1,V2>>
  {
    using ReturnType = typename btk::lard::Traits<btk::lard::Array<std::decay<V2>::type::ColsAtCompileTime>>::Values;
  };
  
  // Motion against motion (12 vs 12)
  
  template<typename V1, typename V2>
  struct TransformOpValues : public Eigen::ReturnByValue<TransformOpValues<V1,V2>>
  {
    using InputType1 = typename std::decay<V1>::type;
    using InputType2 = typename std::decay<V2>::type;
    using Index = typename InputType1::Index;
    typename InputType1::Nested m_V1;
    typename InputType2::Nested m_V2;
    
    template <typename R, typename U1, typename U2> static inline void evaluate_12x12(R& result, const U1& v1, const U2& v2)
    {
      // lhs
      const auto& l11 = v1.col(0);
      const auto& l21 = v1.col(1);
      const auto& l31 = v1.col(2);
      const auto& l12 = v1.col(3);
      const auto& l22 = v1.col(4);
      const auto& l32 = v1.col(5);
      const auto& l13 = v1.col(6);
      const auto& l23 = v1.col(7);
      const auto& l33 = v1.col(8);
      const auto& l14 = v1.col(9);
      const auto& l24 = v1.col(10);
      const auto& l34 = v1.col(11);
      // rhs
      const auto& r11 = v2.col(0);
      const auto& r21 = v2.col(1);
      const auto& r31 = v2.col(2);
      const auto& r12 = v2.col(3);
      const auto& r22 = v2.col(4);
      const auto& r32 = v2.col(5);
      const auto& r13 = v2.col(6);
      const auto& r23 = v2.col(7);
      const auto& r33 = v2.col(8);
      const auto& r14 = v2.col(9);
      const auto& r24 = v2.col(10);
      const auto& r34 = v2.col(11);
      // Computation
      // result.resize(v1.rows(),Eigen::NoChange);
      result.col(0)  = l11 * r11 + l12 * r21 + l13 * r31;
      result.col(1)  = l21 * r11 + l22 * r21 + l23 * r31;
      result.col(2)  = l31 * r11 + l32 * r21 + l33 * r31;
      result.col(3)  = l11 * r12 + l12 * r22 + l13 * r32;
      result.col(4)  = l21 * r12 + l22 * r22 + l23 * r32;
      result.col(5)  = l31 * r12 + l32 * r22 + l33 * r32;
      result.col(6)  = l11 * r13 + l12 * r23 + l13 * r33;
      result.col(7)  = l21 * r13 + l22 * r23 + l23 * r33;
      result.col(8)  = l31 * r13 + l32 * r23 + l33 * r33;
      result.col(9)  = l11 * r14 + l12 * r24 + l13 * r34 + l14;
      result.col(10) = l21 * r14 + l22 * r24 + l23 * r34 + l24;
      result.col(11) = l31 * r14 + l32 * r24 + l33 * r34 + l34;
    };
    
    template <typename R, typename U1, typename U2> static inline void evaluate_12x3(R& result, const U1& v1, const U2& v2)
    {
      // lhs
      const auto& l11 = v1.col(0);
      const auto& l21 = v1.col(1);
      const auto& l31 = v1.col(2);
      const auto& l12 = v1.col(3);
      const auto& l22 = v1.col(4);
      const auto& l32 = v1.col(5);
      const auto& l13 = v1.col(6);
      const auto& l23 = v1.col(7);
      const auto& l33 = v1.col(8);
      const auto& l14 = v1.col(9);
      const auto& l24 = v1.col(10);
      const auto& l34 = v1.col(11);
      // rhs
      const auto& px = v2.col(0);
      const auto& py = v2.col(1);
      const auto& pz = v2.col(2);
      // Computation
      // result.resize(v1.rows(),Eigen::NoChange);
      result.col(0)  = l11 * px + l12 * py + l13 * pz + l14;
      result.col(1)  = l21 * px + l22 * py + l23 * pz + l24;
      result.col(2)  = l31 * px + l32 * py + l33 * pz + l34;
    };
    
    template <typename R, typename U1, typename U2> static inline typename std::enable_if<std::decay<U1>::type::ColsAtCompileTime == 12 && std::decay<U2>::type::ColsAtCompileTime == 12>::type evaluate(R& result, const U1& v1, const U2& v2)
    {
      TransformOpValues::evaluate_12x12(result,v1,v2);
    };
    
    template <typename R, typename U1, typename U2> static inline typename std::enable_if<std::decay<U1>::type::ColsAtCompileTime == 12 && std::decay<U2>::type::ColsAtCompileTime == 3>::type evaluate(R& result, const U1& v1, const U2& v2)
    {
      TransformOpValues::evaluate_12x3(result,v1,v2);
    };
    
    template <typename R, typename U1, typename U2> static inline typename std::enable_if<std::decay<U1>::type::ColsAtCompileTime == 12 && std::decay<U2>::type::ColsAtCompileTime == Dynamic>::type evaluate(R& result, const U1& v1, const U2& v2)
    {
      if (v2.cols() == 12)
        TransformOpValues::evaluate_12x12(result,v1,v2);
      else if (v2.cols() == 3)
        TransformOpValues::evaluate_12x3(result,v1,v2);
      else
        result.setZero(); // Potentially crash the binary, but this should not happen.
    };
    
  public:
    TransformOpValues(const V1& v1, const V2& v2) : m_V1(v1), m_V2(v2) {};
    
    template <typename R> inline void evalTo(R& result) const
    {
      return TransformOpValues::evaluate(result, this->m_V1, this->m_V2);
    };
    
    Index rows() const {return this->m_V2.rows();};
    Index cols() const {return this->m_V2.cols();};
  };
 
  // ----------------------------------------------------------------------- //
  //                         InverseOp return value
  // ----------------------------------------------------------------------- //
  
  template<typename V> struct InverseOpValues;

  template<typename V>
  struct traits<InverseOpValues<V>>
  {
    using ReturnType = typename btk::lard::Traits<btk::lard::Array<std::decay<V>::type::ColsAtCompileTime>>::Values;
  };
  
  template<typename V>
  struct InverseOpValues : public Eigen::ReturnByValue<InverseOpValues<V>>
  {
    using InputType = typename std::decay<V>::type;
    using Index = typename InputType::Index;
    typename InputType::Nested m_V;
  public:
    InverseOpValues(const V& v) : m_V(v) {};
    template <typename R> inline void evalTo(R& result) const
    {
      const auto& v11 = this->m_V.col(0);
      const auto& v21 = this->m_V.col(1);
      const auto& v31 = this->m_V.col(2);
      const auto& v12 = this->m_V.col(3);
      const auto& v22 = this->m_V.col(4);
      const auto& v32 = this->m_V.col(5);
      const auto& v13 = this->m_V.col(6);
      const auto& v23 = this->m_V.col(7);
      const auto& v33 = this->m_V.col(8);
      const auto& v14 = this->m_V.col(9);
      const auto& v24 = this->m_V.col(10);
      const auto& v34 = this->m_V.col(11);
      // result.resize(this->rows(),Eigen::NoChange);
      // Rotation (v(1:3,1:3) transposed)
      result.col(0) = v11; // i11
      result.col(1) = v12; // i21
      result.col(2) = v13; // i31
      result.col(3) = v21; // i12
      result.col(4) = v22; // i22
      result.col(5) = v23; // i32
      result.col(6) = v31; // i13
      result.col(7) = v32; // i23
      result.col(8) = v33; // i33
      // Translation (-transpose(v(1:3,1:3))*v(1:3,4))
      result.col(9)  = -v11 * v14 - v21 * v24 - v31 * v34;
      result.col(10) = -v12 * v14 - v22 * v24 - v32 * v34;
      result.col(11) = -v13 * v14 - v23 * v24 - v33 * v34;
    };
    Index rows() const {return this->m_V.rows();};
    Index cols() const {return this->m_V.cols();};
  };
  
 
  // ----------------------------------------------------------------------- //
  //                         EulerAnglesOp return value
  // ----------------------------------------------------------------------- //
  
  template<typename V> struct EulerAnglesOpValues;

  template<typename V>
  struct traits<EulerAnglesOpValues<V>>
  {
    using ReturnType = typename btk::lard::Traits<btk::lard::Array<3>>::Values;
  };
  
  template<typename V>
  struct EulerAnglesOpValues : public Eigen::ReturnByValue<EulerAnglesOpValues<V>>
  {
    using InputType = typename std::decay<V>::type;
    using Index = typename InputType::Index;
    typename InputType::Nested m_V;
    const unsigned m_A0;
    const unsigned m_A1;
    const unsigned m_A2;
  public:
    EulerAnglesOpValues(const V& v, const unsigned& a0, const unsigned& a1, const unsigned& a2) : m_V(v), m_A0(a0), m_A1(a1), m_A2(a2) {};
    template <typename R> inline void evalTo(R& result) const
    {
      using MapStride = Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>;
      using MapMatrix33 = Eigen::Map<const Eigen::Matrix<double,3,3>, Eigen::Unaligned, MapStride>;
      const Index rows = this->m_V.rows();
      const Eigen::Array<double,Eigen::Dynamic,9> values = this->m_V.block(0,0,rows,9);
      // result.resize(rows, Eigen::NoChange);
      const MapStride stride(3*rows,rows);
      MapMatrix33 rot(nullptr,stride);
      for (Index i = 0 ; i < rows ; ++i)
      {
        // See http://eigen.tuxfamily.org/dox/group__TutorialMapClass.html (section Changing the mapped array) for the syntax
        new (&rot) MapMatrix33(values.data()+i,stride);
        result.row(i) = rot.eulerAngles(this->m_A0, this->m_A1, this->m_A2);
      }
    };
    Index rows() const {return this->m_V.rows();};
    Index cols() const {return 3;};
  };
};
};

#endif // __btklardXpr_helper