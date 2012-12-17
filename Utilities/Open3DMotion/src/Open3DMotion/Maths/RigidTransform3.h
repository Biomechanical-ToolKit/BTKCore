/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#ifndef _OPEN3DMOTION_RIGID_TRANFORM_3_H_
#define _OPEN3DMOTION_RIGID_TRANFORM_3_H_

#include "Open3DMotion/Maths/Vector3.h"
#include "Open3DMotion/Maths/Matrix3x3.h"

namespace Open3DMotion
{
  // Rigid transformation in 3-space
  class RigidTransform3
  {
  public:
    Matrix3x3 R;
    Vector3 t;

  public:
    // Construct un-initialised
    RigidTransform3()
    {
    }

    // Construct from rotation and translation
    RigidTransform3(const double* nR, const double* nt) :
      R(nR), t(nt)
    {
    }

    // Construct from 3 points in xy-plane
    RigidTransform3(const double* x0, const double* x1, const double* xy)
    {
      FromXY(*this, x0, x1, xy);
    }

    // Set to identity
    RigidTransform3& ID()
    {
      R.ID();
      t = 0.0;
      return *this;
    }

  public:

    // Define rotation from x-axis and vector in xy-plane (array form)
    static void FromXYVec(double* R, const double* vx, const double* vxy);

    // Define rotation from y-axis and vector in yz-plane (array form)
    static void FromYZVec(double* R, const double* vy, const double* yz);

    // Define rotation from z-axis and vector in zx-plane (array form)
    static void FromZXVec(double* R, const double* vz, const double* vzx);

    // Define rotation from y-axis and vector in yx-plane (array form)
    static void FromYXVec(double* R, const double* vy, const double* vyx);

    // Define rotation from x-axis and vector in xz-plane (array form)
    static void FromXZVec(double* R, const double* vx, const double* vxz);

    // Define rotation from z-axis and vector in zy-plane (array form)
    static void FromZYVec(double* R, const double* vz, const double* vzy);

    // Define rotation from 3 points in xy-plane (array form)
    static void FromXY(double* R, const double* x0, const double* x1, const double* xy)
    {
      Vector3 vx, vxy;
      Vector3::Sub(vx, x1, x0);
      Vector3::Sub(vxy, xy, x0);
      FromXYVec(R, vx, vxy);
    }

    // Define transform from 3 points in xy-plane (object form)
    static RigidTransform3& FromXY(RigidTransform3& T, const double* x0, const double* x1, const double* xy)
    { 
      FromXY(T.R, x0, x1, xy); 
      T.t = x0;
      return T;
    }

    // Define transform from 2 vectors in xy-plane (object form)
    static RigidTransform3& FromXYVec(RigidTransform3& T, const double* x0, const double* vx, const double* vxy)
    { 
      FromXYVec(T.R, vx, vxy); 
      T.t = x0;
      return T;
    }

    // Apply rigid transform a = T.b
    static double* MulVec(double* a, const RigidTransform3& T, const double* b)
    { 
      RigidTransform3::MulVec(a, T.R, T.t, b);
      return a;
    }

    // Apply rigid transform a = inverse(T).b
    static double* InvMulVec(double* a, const RigidTransform3& T, const double* b)
    { 
      RigidTransform3::InvMulVec(a, T.R, T.t, b);
      return a;
    }

    // Apply rigid transform a = R.b + t
    static double* MulVec(double* a, const double* R, const double* t, const double* b)
    {
      a[0] = R[0]*b[0] + R[1]*b[1] + R[2]*b[2] + t[0];
      a[1] = R[3]*b[0] + R[4]*b[1] + R[5]*b[2] + t[1];
      a[2] = R[6]*b[0] + R[7]*b[1] + R[8]*b[2] + t[2];
      return a;
    }

    // Apply inverse rigid transform a = transpose(R).(b-t)
    static double* InvMulVec(double* a, const double* R, const double* t, const double* b)
    {
      Vector3 offset;
      Vector3::Sub(offset, b, t);
      Matrix3x3::TransMulVec(a, R, offset);
      return a;
    }

    // Compose A = B.C
    static RigidTransform3& Mul(RigidTransform3& A, const RigidTransform3& B, const RigidTransform3& C)
    {
      Matrix3x3::Mul(A.R, B.R, C.R);
      Matrix3x3::MulVec(A.t, B.R, C.t);
      A.t += B.t;
      return A;
    }

    // Compose A = inverse(B).C
    static RigidTransform3& InvMul(RigidTransform3& A, const RigidTransform3& B, const RigidTransform3& C)
    {
      Vector3 offset;
      Vector3::Sub(offset, C.t, B.t);
      Matrix3x3::TransMulVec(A.t, B.R, offset);
      Matrix3x3::TransMul(A.R, B.R, C.R);
      return A;
    }

    // Compose A = B.inverse(C)
    static RigidTransform3& MulInv(RigidTransform3& A, const RigidTransform3& B, const RigidTransform3& C)
    {
      Matrix3x3::MulTrans(A.R, B.R, C.R);
      Vector3 offset;
      Matrix3x3::MulVec(offset,A.R,C.t);
      Vector3::Sub(A.t, B.t, offset);
      return A;
    }
  };
}
#endif