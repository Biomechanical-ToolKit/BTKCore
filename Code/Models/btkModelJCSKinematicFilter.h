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
 
#ifndef __btkModelJCSKinematicFilter_h
#define __btkModelJCSKinematicFilter_h

#include "btkProcessObject.h"
#include "btkAcquisition.h"
#include "btkModel.h"

#include <map>
#include <string>

namespace btk
{
  struct SequenceJCS
  {
    typedef enum {AxisX = 0, AxisY = 1, AxisZ = 2} Axis;
    static SequenceJCS Default(){SequenceJCS seq; seq.e0 = AxisZ; seq.e1 = AxisX; seq.e2 = AxisY; return seq;};
    BTK_MODELS_EXPORT friend bool operator==(const SequenceJCS& lhs, const SequenceJCS& rhs);
    friend bool operator!=(const SequenceJCS& lhs, const SequenceJCS& rhs) {return !(lhs == rhs);};
    Axis e0;
    Axis e1;
    Axis e2;
  };
  
  class ModelJCSKinematicFilter : public ProcessObject
  {
  public:
    typedef SharedPtr<ModelJCSKinematicFilter> Pointer;
    typedef SharedPtr<const ModelJCSKinematicFilter> ConstPointer;

    static Pointer New() {return Pointer(new ModelJCSKinematicFilter());};
    
    // ~ModelJCSKinematicFilter(); // Implicit
    
    void SetInput(Model::Pointer input) {this->SetNthInput(0, input);};
    Model::Pointer GetInput() {return static_pointer_cast<Model>(this->GetNthInput(0));};
    Acquisition::Pointer GetOutput() {return this->GetOutput(0);};
    
    BTK_MODELS_EXPORT const SequenceJCS GetSequenceJCS(const std::string& label) const;
    BTK_MODELS_EXPORT const SequenceJCS GetSequenceJCS(int idx) const;
    BTK_MODELS_EXPORT void SetSequenceJCS(const std::string& label, const SequenceJCS& axes);
    BTK_MODELS_EXPORT void SetSequenceJCS(int idx, const SequenceJCS& axes);
    BTK_MODELS_EXPORT void SetSequenceJCS(const std::string& label, SequenceJCS::Axis e0, SequenceJCS::Axis e1, SequenceJCS::Axis e2);
    BTK_MODELS_EXPORT void SetSequenceJCS(int idx, SequenceJCS::Axis e0, SequenceJCS::Axis e1, SequenceJCS::Axis e2);
    BTK_MODELS_EXPORT void ClearSequences();
    
    BTK_MODELS_EXPORT void SetTranslationsCalculationState(bool enable);
    bool GetTranslationsCalculationState() const {return this->m_TranslationsEnabled;};

  protected:
    BTK_MODELS_EXPORT ModelJCSKinematicFilter();
    
    Acquisition::Pointer GetOutput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthOutput(idx));};
    BTK_MODELS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_MODELS_EXPORT virtual void GenerateData();
    
  private:
    ModelJCSKinematicFilter(const ModelJCSKinematicFilter& ); // Not implemented.
    ModelJCSKinematicFilter& operator=(const ModelJCSKinematicFilter& ); // Not implemented.
    
    // *RotToEuler*: Inspired form Eigen::eulerAngles()
    class AbstractRotToEuler
    {
    public:
      virtual void Extract(const Eigen::Matrix<double,3,3>& rot, double& a0, double& a1, double& a2) = 0;

    protected:
      AbstractRotToEuler(const SequenceJCS& seq)
      {
        const int odd = ((seq.e0 + 1) % 3 == seq.e1) ? 0 : 1;
        i = seq.e0;
        j = (seq.e0 + 1 + odd) % 3;
        k = (seq.e0 + 2 - odd) % 3;
        if (!odd)
          f = -1;
        else
          f = 1;
        //std::cout << std::endl << "e0: " << seq.e0 << " ; e1: " << seq.e1 << " ; e2: " << seq.e2 << " ; i: " << i << " ; j: " << j << " ; k: " << k << " ; odd: " << odd << " ; f: " << f << std::endl;
      }
      
      int i;
      int j;
      int k;
      int f;
    };

    class RotToEulerABC : public AbstractRotToEuler
    {
    public:
      RotToEulerABC(const SequenceJCS& seq)
      : AbstractRotToEuler(seq)
      {};
      virtual void Extract(const Eigen::Matrix<double,3,3>& rot, double& a0, double& a1, double& a2)
      {
        const double epsilon = Eigen::precision<double>();
        double c = Eigen::Matrix<double,2,1>(rot.coeff(i,i), rot.coeff(i,j)).norm();
        a1 = f * Eigen::ei_atan2(-rot.coeff(i,k), c);
        if (c > epsilon)
        {
          a0 = f * Eigen::ei_atan2(rot.coeff(j,k), rot.coeff(k,k));
          a2 = f * Eigen::ei_atan2(rot.coeff(i,j), rot.coeff(i,i));
        }
        else
        {
          a0 = 0.0;
          a2 = f * (rot.coeff(i,k) > 0 ? 1 : -1) * Eigen::ei_atan2(-rot.coeff(k,j), rot.coeff(j,j));
        }
      };
    };

    class RotToEulerABA : public AbstractRotToEuler
    {
    public:
      RotToEulerABA(const SequenceJCS& seq)
      : AbstractRotToEuler(seq)
      {};
      virtual void Extract(const Eigen::Matrix<double,3,3>& rot, double& a0, double& a1, double& a2)
      {
        const double epsilon = Eigen::precision<double>();
        double s = Eigen::Matrix<double,2,1>(rot.coeff(j,i), rot.coeff(k,i)).norm();
        a1 = Eigen::ei_atan2(s, rot.coeff(i,i));
        if (s > epsilon)
        {
          a0 = f * Eigen::ei_atan2(rot.coeff(j,i), rot.coeff(k,i));
          a2 = f * Eigen::ei_atan2(rot.coeff(i,j),-rot.coeff(i,k));
        }
        else
        {
          a0 = 0.0;
          a2 = f * (rot.coeff(i,i) > 0 ? 1 : -1) * Eigen::ei_atan2(-rot.coeff(k,j), rot.coeff(j,j));
        }
      };
    };

    static AbstractRotToEuler* RotToEulerFactory(const SequenceJCS& seq)
    {
      if (seq.e0 == seq.e2)
        return new RotToEulerABA(seq);
      else
        return new RotToEulerABC(seq);
    }
    
    bool m_TranslationsEnabled;
    std::map<int, SequenceJCS> m_SequencesByIndex;
    std::map<std::string, SequenceJCS> m_SequencesByLabel;
  };
};

#endif // __btkModelJCSKinematicFilter_h