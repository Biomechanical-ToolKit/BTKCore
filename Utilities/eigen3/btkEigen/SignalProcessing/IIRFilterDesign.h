/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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
 
#ifndef __btkEigenIIRFilterDesign_h
#define __btkEigenIIRFilterDesign_h

#include "btkMacro.h"
#include "Utilities/maths/comb.h"

#include <Eigen/Core>
#include <Eigen/Geometry> // M_PI

namespace btkEigen
{
  using namespace Eigen;

  // Note: The following code is largely inspired by the code in the Python module SciPy.signal.filter_design 

  typedef enum {Elliptic = 0, Butterworth, ChebyshevI, ChebyshevII, Bessel} FilterType;
  typedef enum {LowPass = 0, HighPass, BandPass, BandStop} BandType;

  bool iirfilter(Eigen::Matrix<double, Eigen::Dynamic, 1>* b, Eigen::Matrix<double, Eigen::Dynamic, 1>* a, int order, double Wn, double* rp = NULL, double* rs = NULL, BandType btype = LowPass, FilterType ftype = Butterworth);
  bool iirfilter(Eigen::Matrix<double, Eigen::Dynamic, 1>* b, Eigen::Matrix<double, Eigen::Dynamic, 1>* a, int order, double Wn[2], double* rp = NULL, double* rs = NULL, BandType btype = BandPass, FilterType ftype = Butterworth);

  bool butter(Eigen::Matrix<double, Eigen::Dynamic, 1>* b, Eigen::Matrix<double, Eigen::Dynamic, 1>* a, int order, double Wn, BandType btype = LowPass)
  {
    return iirfilter(b, a, order, Wn, NULL, NULL, btype, Butterworth);
  };
  
  bool butter(Eigen::Matrix<double, Eigen::Dynamic, 1>* b, Eigen::Matrix<double, Eigen::Dynamic, 1>* a, int order, double Wn[2], BandType btype = BandPass)
  {
    return iirfilter(b, a, order, Wn, NULL, NULL, btype, Butterworth);
  };
  
  // See the  paper "Design and responses of Butterworth and critically damped digital filters", Robertson & Dowling, Journal of Electromyography and Kinesiology, 2003.
  // or the paragraph 3.4.4.2 in the book "Biomechanics and Motor Control of Human Movement" (David A. Winter)
  // for more explanation on the need to adjust the order and the cutoff frequency.
  void adjustZeroLagButterworth(int& n, double (*wn)[2])
  {
    const double c = 1.0 / std::pow(std::pow(2,1.0/static_cast<double>(n))-1.0, 0.25);
    (*wn)[0] *= c;
    (*wn)[1] *= c;
    n /= 2;
  };
  
  void adjustZeroLagButterworth(int& n, double& wn)
  {
    double wn_[2] = {wn, 0.0};
    adjustZeroLagButterworth(n, &wn_);
    wn = wn_[0];
  };

  // ------------------------------------------------------------------------- //

  // This code is adapted from the function roots_to_monicPolynomial in the file 
  // PolynomialUtils.h distributed in the source code of Eigen 3.1.1
  // Compared to the original function, this one take into account the case where 
  // the input is empty but also the coefficients of the polynom are in the opposite order
  // (i.e. the first coeff corresponds to the biggest degree of the polynom).
  template<typename MatrixType, typename ResultType>
  void poly(ResultType* p, const MatrixType& r)
  {
    typedef typename MatrixType::Scalar Scalar;
    eigen_assert(r.cols() == 1);
    eigen_assert(p->cols() == 1);
  
    p->setZero(r.rows()+1);
    if (r.rows() == 0)
    {
      p->coeffRef(0) = Scalar(1);
      return;
    }
    const int end = p->rows()-1;
    p->coeffRef(end) = -r.coeff(0);
    p->coeffRef(end-1) = Scalar(1);
    for (int i = end-1 ; i > 0 ; --i)
    {
      for (int j = i-1 ; j < end ; ++j)
        p->coeffRef(j) = p->coeff(j+1) - r.coeff(i) * p->coeff(j);
      p->coeffRef(end) = -r.coeff(i) * p->coeff(end);
    }
  };

  template <class T>
  void normalize(T* b, T* a)
  {
    while ((a->coeff(0) == 0.0) && a->rows() > 1)
      *a = a->block(1,0,a->rows()-1,1);
    *b /= a->coeff(0);
    *a /= a->coeff(0);
    // TODO: Check for bad conditioned filter coefficients (numerator).
  };

  // ------------------------------------------------------------------------- //

  void buttap(Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* /* z */, Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* p, double* k, int n)
  {
    // z is set to [], so no modification.
    std::complex<double> _1j(0.0, 1.0);
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> temp(n);
    for (int i = 0 ; i < n ; ++i)
      temp.data()[i] = (2.0 * static_cast<double>(i+1) - 1.0) * _1j;
    *p = (temp / (2.0 * static_cast<double>(n)) * M_PI).array().exp() * _1j;
    *k = 1.0;
  };

  void zpk2tf(Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* b, Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* a, const Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>& z, const Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>& p, double k)
  {
    poly(b, z); *b *= k;
    poly(a, p);
  };

  void lp2lp(Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* b, Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* a, double wo = 1.0)
  {
    const int d = a->rows();
    const int n = b->rows();
    const int m = std::max(d,n);
    Eigen::Matrix<double, Eigen::Dynamic, 1> pwo(m);
    for (int i = 0 ; i < m ; ++i)
      pwo.data()[i] = std::pow(wo, static_cast<double>(m-i-1));
    const int start1 = std::max(n-d,0);
    const int start2 = std::max(d-n,0);
    *b *= pwo.coeff(start1);
    *b = b->cwiseQuotient(pwo.block(start2,0,pwo.rows()-start2,1).cast< std::complex<double> >());
    *a *= pwo.coeff(start1);
    *a = a->cwiseQuotient(pwo.block(start1,0,pwo.rows()-start1,1).cast< std::complex<double> >());
    normalize(b,a);
  };
  
  void lp2hp(Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* b, Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* a, double wo = 1.0)
  {
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> a_ = *a, b_ = *b;
    const int d = a->rows();
    const int n = b->rows();
    const int m = std::max(d,n);
    Eigen::Matrix<double, Eigen::Dynamic, 1> pwo(m);
    for (int i = 0 ; i < m ; ++i)
      pwo.data()[i] = std::pow(wo, static_cast<double>(i));
    if (d >= n)
    {
      for (int i = 0 ; i < m ; ++i)
        a->coeffRef(i) = a_.coeff(m-i-1);
      *a = a->cwiseProduct(pwo);
      *b = Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>::Zero(m);
      for (int i = 0 ; i < n ; ++i)
        b->coeffRef(i) = b_.coeff(n-i-1) * pwo.coeff(i);
    }
    else
    {
      for (int i = 0 ; i < m ; ++i)
        b->coeffRef(i) = b_.coeff(m-i-1);
      *b = b->cwiseProduct(pwo);
      *a = Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>::Zero(m);
      for (int i = 0 ; i < d ; ++i)
        a->coeffRef(i) = a_.coeff(d-i-1) * pwo.coeff(i);
    }
    normalize(b,a);
  };
  
  void lp2bp(Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* b, Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* a, double wo = 1.0, double bw = 1.0)
  {
    const int d = a->rows() - 1;
    const int n = b->rows() - 1;
    const int m = std::max(d,n);
    const int dp = d + m;
    const int np = n + m;
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> a_ = Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>::Zero(dp + 1);
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> b_ = Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>::Zero(np + 1);
    double wosq = wo * wo;
    for (int j = 0 ; j < (np + 1) ; ++j)
    {
      std::complex<double> val(0.0, 0.0);
      for (int i = 0 ; i < (n + 1) ; ++i)
      {
        for (int k = 0 ; k < i+1 ; ++k)
        {
          if ((m - i + 2 * k) == j)
            val += comb(static_cast<double>(i), static_cast<double>(k)) * b->coeff(n-i) * std::pow(wosq, i-k) / std::pow(bw, i);
        }
      }
      b_.coeffRef(np-j) = val; 
    }
    for (int j = 0 ; j < (dp + 1) ; ++j)
    {
      std::complex<double> val(0.0, 0.0);
      for (int i = 0 ; i < (d + 1) ; ++i)
      {
        for (int k = 0 ; k < i+1 ; ++k)
        {
          if ((m - i + 2 * k) == j)
            val += comb(static_cast<double>(i), static_cast<double>(k)) * a->coeff(d-i) * std::pow(wosq, i-k) / std::pow(bw, i);
        }
      }
      a_.coeffRef(dp-j) = val; 
    }
    *b = b_; *a = a_;
    normalize(b,a);
  };
  
  void lp2bs(Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* b, Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>* a, double wo = 1.0, double bw = 1.0)
  {
    const int d = a->rows() - 1;
    const int n = b->rows() - 1;
    const int m = std::max(d,n);
    const int dp = m + m;
    const int np = m + m;
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> a_ = Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>::Zero(dp + 1);
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> b_ = Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>::Zero(np + 1);
    double wosq = wo * wo;
    for (int j = 0 ; j < (np + 1) ; ++j)
    {
      std::complex<double> val(0.0, 0.0);
      for (int i = 0 ; i < (n + 1) ; ++i)
      {
        for (int k = 0 ; k < m-i+1 ; ++k)
        {
          if ((i + 2 * k) == j)
            val += comb(static_cast<double>(m-i), static_cast<double>(k)) * b->coeff(n-i) * std::pow(wosq, m-i-k) * std::pow(bw, i);
        }
      }
      b_.coeffRef(np-j) = val; 
    }
    for (int j = 0 ; j < (dp + 1) ; ++j)
    {
      std::complex<double> val(0.0, 0.0);
      for (int i = 0 ; i < (d + 1) ; ++i)
      {
        for (int k = 0 ; k < m-i+1 ; ++k)
        {
          if ((i + 2 * k) == j)
            val += comb(static_cast<double>(m-i), static_cast<double>(k)) * a->coeff(d-i) * std::pow(wosq, m-i-k) * std::pow(bw, i);
        }
      }
      a_.coeffRef(dp-j) = val; 
    }
    *b = b_; *a = a_;
    normalize(b,a);
  };

  void bilinear(Eigen::Matrix<double, Eigen::Dynamic, 1>* b, Eigen::Matrix< double, Eigen::Dynamic, 1>* a, const Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>& b_, const Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1>& a_, double fs = 1.0)
  {
    const int d = a_.rows() - 1;
    const int n = b_.rows() - 1;
    const int m = std::max(d,n) + 1;
    *b = Eigen::Matrix<double, Eigen::Dynamic, 1>::Zero(m);
    *a = Eigen::Matrix<double, Eigen::Dynamic, 1>::Zero(m);
    for (int j = 0 ; j < m ; ++j)
    {
      double val = 0.0;
      for (int i = 0 ; i < n+1 ; ++i)
      {
        for (int k = 0 ; k < i+1 ; ++k)
        {
          for (int l = 0 ; l < m-i ; ++l)
          {
            if ((k+l) == j)
            {
              val += (comb(static_cast<double>(i), static_cast<double>(k)) * comb(static_cast<double>(m - i - 1), static_cast<double>(l)) * b_.coeff(n - i).real() * std::pow(2.0*fs, i) * pow(-1.0,k));
            }
          }
        }
      }
      b->coeffRef(j) = val;
    }
    for (int j = 0 ; j < m ; ++j)
    {
      double val = 0.0;
      for (int i = 0 ; i < d+1 ; ++i)
      {
        for (int k = 0 ; k < i+1 ; ++k)
        {
          for (int l = 0 ; l < m-i ; ++l)
          {
            if ((k+l) == j)
            {
              val += (comb(static_cast<double>(i), static_cast<double>(k)) * comb(static_cast<double>(m - i - 1), static_cast<double>(l)) * a_.coeff(d - i).real() * std::pow(2.0*fs, i) * pow(-1.0,k));
            }
          }
        }
      }
      a->coeffRef(j) = val;
    }
    normalize(b,a);
  };

  // ------------------------------------------------------------------------- //

  /**
   * btype:
   *  - 0: lowpass
   *  - 1: highpass
   *  - 2: bandpass
   *  - 3: bandstop
   *
   * ftype:
   *  - 0: elliptic
   *  - 1: Butterworth
   *  - 2: Chebyshev I
   *  - 3: Chebyshev II
   *  - 4: Bessel
   */
  bool iirfilter(Eigen::Matrix<double, Eigen::Dynamic, 1>* b, Eigen::Matrix<double, Eigen::Dynamic, 1>* a, int order, double Wn, double* rp, double* rs, BandType btype, FilterType ftype)
  {
    // This function is only for low pass or high pass filter
    if ((btype == 2) || (btype == 3))
    {
      btkErrorMacro("This function is not adapted for bandpass/bandstop filter as Wn must contain 2 values.");
      return false;
    }
    double Wn_[2] = {Wn, -1.0};
    return iirfilter(b, a, order, Wn_, rp, rs, btype, ftype);
  };

  bool iirfilter(Eigen::Matrix<double, Eigen::Dynamic, 1>* b, Eigen::Matrix<double, Eigen::Dynamic, 1>* a, int order, double Wn[2], double* /*rp*/, double* /*rs*/, BandType btype, FilterType ftype)
  {
    // This function is only for band pass or band stop filter
    if (((btype == 0) || (btype == 1)) && (Wn[1] != -1.0))
    {
      btkErrorMacro("This function is not adapted for lowpass/highpass filter as Wn must contain only 1 value.");
      return false;
    }

    // Pre-warp frequencies for digital filter design
    const double fs = 2.0;
    double warped[2] = {2.0 * fs * tan(M_PI * Wn[0] / fs), 2.0 * fs * tan(M_PI * Wn[1] / fs)};
  
    // Convert to low-pass prototype
    double wo = 0.0;
    double bw = 0.0;
    switch (btype)
    {
    case LowPass:
    case HighPass:
      wo = warped[0];
      break;
    case BandPass:
    case BandStop:
      bw = warped[1] - warped[0];
      wo = sqrt(warped[0] * warped[1]);
      break;
    default:
      btkErrorMacro("Invalid type of filter");
      return false;
    }
  
    // Get analog lowpass prototype
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> z, p;
    double k = 0.0;
    switch (ftype)
    {
    case Butterworth:
      buttap(&z, &p, &k, order);
      break;
  
    /*
    case Elliptic:
      if ((rp == NULL) || (rs == NULL))
      {
        btkErrorMacro("Both rp and rs must be provided to design an elliptic filter.");
        return false;
      }
      break;
    case ChebyshevI:
      if (rp == NULL)
      {
        btkErrorMacro("Passband ripple (rp) must be provided to design a Chebyshev I filter.");
        return false;
      }
      break;
    case ChebyshevII:
      if (rs == NULL)
      {
        btkErrorMacro("Stopband attenuation (rs) must be provided to design an Chebyshev II filter.");
        return false;
      }
      break;
    case Bessel:
      break;
    */
  
    case Elliptic:
    case ChebyshevI:
    case ChebyshevII:
    case Bessel:
      btkErrorMacro("The chosen filter is not yet implemented. Please contact the developers for more informations.");
      return false;
      break;
  
    default:
      btkErrorMacro("Invalid basic IIR filter.");
      return false;
    }
  
    Eigen::Matrix< std::complex<double>, Eigen::Dynamic, 1> b_, a_;
    zpk2tf(&b_, &a_, z, p, k);
  
    // Transform to lowpass, bandpass, highpass, or bandstop
    if (btype == 0) // lowpass
      lp2lp(&b_, &a_, wo);
    else if (btype == 1) // highpass
      lp2hp(&b_, &a_, wo);
    else if (btype == 2) // bandpass
      lp2bp(&b_, &a_, wo, bw);
    else // bandstop
      lp2bs(&b_, &a_, wo, bw);
  
    // Find discrete equivalent
    bilinear(b, a, b_, a_, fs);
  
    return true;
  };
};

#endif // __btkEigenIIRFilterDesign_h