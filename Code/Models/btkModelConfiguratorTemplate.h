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

#ifndef __btkModelConfigurator_h
#define __btkModelConfigurator_h

#include "btkModel.h"

namespace btk
{ 
  template <int T>
  class ModelConfiguratorTemplate : public ModelConfigurator
  {
  public:
    typedef SharedPtr<ModelConfiguratorTemplate> Pointer;
    typedef SharedPtr<const ModelConfiguratorTemplate> ConstPointer;
    
    static Pointer New() {return Pointer(new ModelConfiguratorTemplate());};
    // ~ModelConfiguratorTemplate(); // Implicit.
    void Configure(Model* model, Side s) {this->ModelConfigurator::Configure(model, s);};
    
  protected:
    ModelConfiguratorTemplate()
    : ModelConfigurator()
    {};
    
  private:
    ModelConfiguratorTemplate(const ModelConfiguratorTemplate& ); // Not implemented.
    ModelConfiguratorTemplate& operator=(const ModelConfiguratorTemplate& ); // Not implemented.
  };
  
  /**
   * @class ModelConfiguratorTemplate
   * @brief Generic model configurator. Used to facilate the creation of models.
   *
   * @tparam T Unique ID to represent a model. See enum values in ModelConfigurator.
   * 
   * By using this class you have only to specialize the method Configure(). For example:
   * @code
   * // File UpperLimbModelConfigurator.h
   * typedef ModelConfiguratorTemplate<ModelConfigurator::UpperLimb> UpperLimbModelConfigurator;
   * template <> void ModelConfiguratorTemplate<ModelConfigurator::UpperLimb>::Configure(Model* model, Side s);
   * // File UpperLimbModelConfigurator.cpp
   * template class ModelConfiguratorTemplate<ModelConfigurator::UpperLimb>; // MUST be here to resolve LNK2001 error in MSVC
   * template <> void ModelConfiguratorTemplate<ModelConfigurator::UpperLimb>::Configure(Model* model, Side s)
   * {
   *   // Configure the segments and append them here.
   * };
   * @endcode
   * 
   * To intialize your model you can now call the following code:
   * @code
   * btk::Model::Pointer model = btk::Model::New();
   * model->Init(btk::UpperLimbModelConfigurator::New(), btk::Both);
   * @endcode 
   *
   * @ingroup BTKModels
   */
  
  /**
   * @typedef ModelConfiguratorTemplate<T>::Pointer
   * Smart pointer associated with a ModelConfiguratorTemplate object.
   */
  
  /**
   * @typedef ModelConfiguratorTemplate<T>::ConstPointer
   * Smart pointer associated with a const ModelConfiguratorTemplate object.
   */
   
  /**
   * @fn template <int T> static ModelConfiguratorTemplate<T>::Pointer ModelConfiguratorTemplate<T>::New()
   * Creates a smart pointer associated with a ModelConfiguratorTemplate object.
   */
   
  /**
   * @fn template <int T> void ModelConfiguratorTemplate<T>::Configure(Model* model, Side s)
   * Generic configuration method. Do nothing.
   */
   
  /**
   * @fn template <int T> ModelConfiguratorTemplate<T>::ModelConfiguratorTemplate()
   * Constructor.
   */
};

#endif // __btkModelConfigurator_h