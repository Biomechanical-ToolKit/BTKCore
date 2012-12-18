/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#pragma once

#include "Open3DMotion/Maths/ValueN.h"
#include <math.h>

namespace Open3DMotion
{
  // Fixed vector of 3 double values
	class Vector3 : public ValueN<double,3>
  {
  public:
    // Construct uninitialised
		Vector3()
    {
    }

    // Construct with all elements equal to specified value
		Vector3(double xinit)
    {
			x[0] = x[1] = x[2] = xinit;
    }

		// Construct from given values
    Vector3(double x0,double x1,double x2)
    {
      x[0] = x0;
      x[1] = x1;
      x[2] = x2;
    }

    // Construct from array
    Vector3(const double* arr)
    {
      x[0] = arr[0];
      x[1] = arr[1];
      x[2] = arr[2];
    }

    // Add a vector in-place
    Vector3& operator+=(const double* rhs)
    {
      x[0] += rhs[0];
      x[1] += rhs[1];
      x[2] += rhs[2];
      return *this;
    }

    // Subtract a vector in-place
    Vector3& operator-=(const double* rhs)
    {
      x[0] -= rhs[0];
      x[1] -= rhs[1];
      x[2] -= rhs[2];
      return *this;
    }

    // Multiply in-place by a scalar
    Vector3& operator*=(double s)
    {
       x[0] *= s;
       x[1] *= s;
       x[2] *= s;
       return *this;
    }
    
    // Divide in-place by a scalar
    Vector3& operator/=(double s)
    {
       x[0] /= s;
       x[1] /= s;
       x[2] /= s;
       return *this;
    }

    // Set all values equal to a single constant
    Vector3& operator=(double s)
    {
      x[0] = x[1] = x[2] = s;
      return *this;
    }

    // Inner product of vector with this one
    double Dot(double* a) const
    {
      return x[0]*a[0] + x[1]*a[1] + x[2]*a[2];
    }

    // Squared modulus of this vector
    double Modulus2() const
    {
      return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    }

    // Modulus of this vector
    double Modulus() const
    {
      return sqrt(Modulus2());
    }

    // Squared modulus of given vector
    static double Modulus2(const double* x)
    {
      return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
    }

    // Modulus of given vector
    static double Modulus(const double* x)
    {
      return sqrt(Modulus2(x));
    }

    // add two vectors
    static double* Add(double* a, const double* b, const double* c)
    {
      a[0] =  b[0] + c[0];
      a[1] =  b[1] + c[1];
      a[2] =  b[2] + c[2];
      return a;
    }

    // subtract one vector from another
    static double* Sub(double* a, const double* b, const double* c)
    {
      a[0] =  b[0] - c[0];
      a[1] =  b[1] - c[1];
      a[2] =  b[2] - c[2];
      return a;
    }

    // Inner product of two vectors
    static double Dot(const double* a, const double* b)
    {
      return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }

    // Cross product of two vectors
    static double* Cross(double* a, const double* b, const double* c)
    {
      a[0] = b[1]*c[2] - b[2]*c[1];
      a[1] = b[2]*c[0] - b[0]*c[2];
      a[2] = b[0]*c[1] - b[1]*c[0];
      return a;
    }

    // scalar multiple of a vector
    static double* Mul(double* a, const double s, const double* b)
    {
      a[0] = s*b[0];
      a[1] = s*b[1];
      a[2] = s*b[2];
      return a;
    }

    // ratio of two vectors
    static double* Ratio(double* a, const double s, const double* b, const double* c)
    {
      const double r(1.0-s);
      a[0] = r*b[0]+s*c[0];
      a[1] = r*b[1]+s*c[1];
      a[2] = r*b[2]+s*c[2];
      return a;
    }
  };
}
