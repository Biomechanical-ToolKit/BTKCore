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

#include "btkModelCalibrationFilter.h"
#include "btkUpperLimbModelConfigurator.h"

#include <Eigen/Array>
#include <Eigen/Geometry>
#include <Eigen/LU>

namespace btk
{
 /**
   * @class ModelCalibrationFilter
   * @brief TODO
   *
   * @ingroup BTKModels
   */
  
  /**
   * @typedef ModelCalibrationFilter::Pointer
   * Smart pointer associated with a ModelCalibrationFilter object.
   */
  
  /**
   * @typedef ModelCalibrationFilter::ConstPointer
   * Smart pointer associated with a const ModelCalibrationFilter object.
   */
    
  /**
   * @fn static Pointer ModelCalibrationFilter::New();
   * Creates a smart pointer associated with a ModelCalibrationFilter object.
   */
  
  /**
   * @fn Model::Pointer ModelCalibrationFilter::GetOutput()
   * Gets the output created with this process.
   */

  /**
   * @fn void ModelCalibrationFilter::SetAcquisition(Acquisition::Pointer input)
   * Sets the acquisition used to configure the model.
   */ 
   
  /**
   * @fn Acquisition::Pointer ModelCalibrationFilter::GetAcquisition()
   * Gets the acquisition used to configure the model.
   */
   
  /**
   * @fn void ModelCalibrationFilter::SetModel(Model::Pointer input)
   * Sets the model used to determine the segment to calibrate.
   */
   
  /**
   * @fn Model::Pointer ModelCalibrationFilter::GetModel()
   * Gets the input model.
   */
   
  /**
   * @fn void ModelCalibrationFilter::SetLandmarksTranslator(LandmarksTranslator::Pointer input)
   * Sets the translator between the acquisition's landmark and these used in the model.
   */
   
  /**
   * @fn LandmarksTranslator::Pointer ModelCalibrationFilter::GetLandmarksTranslator()
   * Gets the translator
   */
   
  /**
   * @fn void ModelCalibrationFilter::SetSubject(Subject::Pointer input)
   * Sets the subject associated with the acquisition.
   *
   * The use of an acquisitions containing the metadata SUJECT:SEX and SUBJECT:WEIGHT can replace the use this input.
   */
   
  /**
   * @fn Subject::Pointer ModelCalibrationFilter::GetSubject()
   * Gets the subject associated with the acquisition.
   *
   * The use of an acquisitions containing the metadata SUJECT:SEX and SUBJECT:WEIGHT can replace the use this input.
   */
   
  /**
   * Sets the calibration procedure.
   */
  void ModelCalibrationFilter::SetCalibrationProcedure(CalibrationProcedure::Pointer procedure)
  {
    if (this->m_CalibrationProcedure == procedure)
      return;
    this->m_CalibrationProcedure = procedure;
    this->Modified();
  };
   
  /**
   * @fn CalibrationProcedure::Pointer ModelCalibrationFilter::GetCalibrationProcedure()
   * Gets the procedure used to calibrate the model.
   */
   
  /**
   * Constructor.
   */
  ModelCalibrationFilter::ModelCalibrationFilter()
  : ProcessObject(), m_CalibrationProcedure()
  {
    this->SetInputNumber(4);
    this->SetOutputNumber(1);
  };
  
  /**
   * @fn PointCollection::Pointer ModelCalibrationFilter::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a Model:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer ModelCalibrationFilter::MakeOutput(int /* idx */)
  {
    return Model::New();
  };
  
  /**
   * Generates the outputs' data.
   *
   * @todo Add a long description
   */
  void ModelCalibrationFilter::GenerateData()
  {
    // Inputs
    Acquisition::Pointer iA = this->GetAcquisition();
    Model::Pointer iM = this->GetModel();
    Subject::Pointer iS = this->GetSubject();
    LandmarksTranslator::Pointer iLt = this->GetLandmarksTranslator();
    // Output
    Model::Pointer oM = this->GetOutput();
    
    if (!iA.get() || !iM.get())
    {
      btkErrorMacro("Missing inputs. Impossible to initialize the model.");
      return;
    }
    if (!this->m_CalibrationProcedure.get())
    {
      btkErrorMacro("No calibration procedure builder. Impossible to calibrate the model.");
      return;
    }
    
    // Copy input's information;
    oM->Clear();
    oM->SetLabel(iM->GetLabel());
    oM->SetDescription(iM->GetDescription());
    for (Model::SegmentConstIterator it = iM->BeginSegment() ; it != iM->EndSegment() ; ++it)
    {
      Segment::Pointer seg = (*it)->Clone();
      seg->ResetMotion();
      oM->AppendSegment(seg);
    }
    // Copy joints and adapt the associated segment.
    for (Model::JointConstIterator it = iM->BeginJoint() ; it != iM->EndJoint() ; ++it)
      oM->AppendJoint((*it)->Clone());
    Model::SegmentConstIterator itST = oM->BeginSegment();
    for (Model::SegmentConstIterator itS = iM->BeginSegment() ; itS != iM->EndSegment() ; ++itS)
    {
      for (Model::JointIterator itJ = oM->BeginJoint() ; itJ != oM->EndJoint() ; ++itJ)
      {
        if ((*itJ)->GetProximalSegment().get() && (*itS == (*itJ)->GetProximalSegment()))
          (*itJ)->SetProximalSegment(*itST);
        else if ((*itJ)->GetDistalSegment().get() && (*itS == (*itJ)->GetDistalSegment()))
          (*itJ)->SetDistalSegment(*itST);
      }
      ++itST;
    }
    
    // Calibration
    this->m_CalibrationProcedure->Calibrate(oM, iA, iS, iLt);
  };
};