/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud BarrÈ
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
 
#ifndef __gammaln_h
#define __gammaln_h

/**
 * Based on the Fortran function algama (http://www.netlib.org/specfun/algama)
 * by W. J. Cody and L. Stoltz (Argonne National Laboratory, June 16, 1988).
 */

template <typename T>
T _gammaln(T x)
{ 
  const T zero = T(0);
  const T one = T(1);
  const T thrhal = T(1.5);
  const T half = T(0.5);
  const T four = T(4);
  const T twelve = T(12);

  // Only for non-negative value
  assert(x >= zero);
  
  // Declared here as it is used in conditions (eps < x <= 1.5) and (1.5 < x <= 4)
  const T d2 = 4.227843350984671393993777e-1;
  const T p2[8] = {4.974607845568932035012064e0, 5.424138599891070494101986e2, 
                    1.550693864978364947665077e4, 1.847932904445632425417223e5, 
                    1.088204769468828767498470e6, 3.338152967987029735917223e6, 
                    5.106661678927352456275255e6, 3.074109054850539556250927e6};
  const T q2[8] = {1.830328399370592604055942e2, 7.765049321445005871323047e3, 
                    1.331903827966074194402448e5, 1.136705821321969608938755e6, 
                    5.267964117437946917577538e6, 1.346701454311101692290052e7, 
                    1.782736530353274213975932e7, 9.533095591844353613395747e6};
  
  T res = zero;
  if (x == zero)
    res = std::numeric_limits<T>::infinity();
  // 0 < x < eps
  else if (x <= std::numeric_limits<T>::epsilon()) 
    res = -one * std::log(x);
  // eps < x <= 1.5
  else if ((x > std::numeric_limits<T>::epsilon()) && (x <= thrhal))
  {
    const T pnt68 = T(0.6796875);
    T corr = zero;
    T xm1 = x;
    if (x < pnt68)
      corr = -one*std::log(x);
    else
      xm1 -= one; //  Original code: xm1 = (x - 0.5) - 0.5;
    if ((x <= half) || (x >= pnt68))
    {
      const T d1 = -5.772156649015328605195174e-1;
      const T p1[8] = {4.945235359296727046734888e0, 2.018112620856775083915565e2, 
                       2.290838373831346393026739e3, 1.131967205903380828685045e4, 
                       2.855724635671635335736389e4, 3.848496228443793359990269e4, 
                       2.637748787624195437963534e4, 7.225813979700288197698961e3};
      const T q1[8] = {6.748212550303777196073036e1, 1.113332393857199323513008e3, 
                       7.738757056935398733233834e3, 2.763987074403340708898585e4, 
                       5.499310206226157329794414e4, 6.161122180066002127833352e4, 
                       3.635127591501940507276287e4, 8.785536302431013170870835e3};
      T xden = one;
      T xnum = zero;
      for (int i = 0 ; i < 8 ; ++i) 
      {
        xnum = xnum * xm1 + p1[i];
        xden = xden * xm1 + q1[i];
      }
      res  = corr + (xm1 * (d1 + xm1 * (xnum / xden)));
    }
    else
    {
      T xm2 = x - one; //  Original code: xm2 = (x - 0.5) - 0.5;
      T xden = one;
      T xnum = zero;
      for (int i = 0 ; i < 8 ; ++i)
      {
        xnum = xnum * xm2 + p2[i];
        xden = xden * xm2 + q2[i];
      }
      res = corr + xm2 * (d2 + xm2 * (xnum / xden));
    }
  }
  // 1.5 < x <= 4
  else if ((x > thrhal) && (x <= four))
  {
    const T two = T(2);
    const T xm2 = x - two;
    T xden = one;
    T xnum = zero;
    for (int i = 0 ; i < 8 ; ++i)
    {
      xnum = xnum * xm2 + p2[i];
      xden = xden * xm2 + q2[i];
    }
    res = xm2 * (d2 + xm2 * (xnum / xden));
  }
  // 4 < x <= 12
  else if ((x > four) && (x <= twelve))
  {
    const T d4 = 1.791759469228055000094023e0;
    const T p4[8] = {1.474502166059939948905062e4, 2.426813369486704502836312e6, 
                     1.214755574045093227939592e8, 2.663432449630976949898078e9, 
                     2.940378956634553899906876e10, 1.702665737765398868392998e11, 
                     4.926125793377430887588120e11, 5.606251856223951465078242e11};
    const T q4[8] = {2.690530175870899333379843e3, 6.393885654300092398984238e5, 
                     4.135599930241388052042842e7, 1.120872109616147941376570e9, 
                     1.488613728678813811542398e10, 1.016803586272438228077304e11, 
                     3.417476345507377132798597e11, 4.463158187419713286462081e11};
    const T xm4 = x - four;
    T xden = -one;
    T xnum = zero;
    for (int i = 0 ; i < 8 ; ++i)
    {
      xnum = xnum * xm4 + p4[i];
      xden = xden * xm4 + q4[i];
    }
    res = d4 + xm4 * (xnum / xden);
  }
  // x > 12
  else if (x > twelve)
  {
    const T c[7] = {-1.910444077728e-03, 8.4171387781295e-04, 
                    -5.952379913043012e-04, 7.93650793500350248e-04, 
                    -2.777777777777681622553e-03, 8.333333333333333331554247e-02, 
                    5.7083835261e-03};
    // NOTE: The test to know if X < FRTBIG (2.25D76) was removed as it seems to be used only to reduce the computation time
    res = c[6];
    const T ysq = x * x;
    for (int i = 0 ; i < 6 ; ++i)
      res = res / ysq + c[i];
    res = res / x;
    T corr = std::log(x);
    const T sqrtpi = 0.9189385332046727417803297;
    res = res + sqrtpi - 0.5 * corr + x * (corr - 1.0);
  }
  return res;
};

template <typename T> T gammaln(T x) {return (x == T(0)) ? std::numeric_limits<T>::infinity() : static_cast<T>(_gammaln<double>(static_cast<double>(x)));};
template <> double gammaln<double>(double x) {return _gammaln(x);};
template <> float gammaln<float>(float x) {return _gammaln(x);};

template <typename T> std::complex<T> gammaln(const std::complex<T>& x)
{
  // Note: the function gammaln is not defined for complex scalar.
  assert(fabs(x.imag()) < std::numeric_limits<T>::epsilon());
  return std::complex<T>(_gammaln(x.real()), T(0));
};

#endif // __gammaln_h