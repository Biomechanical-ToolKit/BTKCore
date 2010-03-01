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

#include "btkLandmarksTranslator.h"

namespace btk
{
  /**
   * @class LandmarksTranslator
   * @brief Translator between the acquisition's landmark and these used in the model.
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef LandmarksTranslator::Pointer
   * Smart pointer associated with a Model object.
   */

  /**
   * @typedef LandmarksTranslator::ConstPointer
   * Smart pointer associated with a const Model object.
   */
   
  /**
   * @typedef LandmarksTranslator::Iterator
   * Iterator for translation contained in the LandmarksTranslator object.
   */
  
  /**
   * @typedef LandmarksTranslator::ConstIterator
   * Const iterator for translation contained in the LandmarksTranslator object.
   */
   
  /**
   * @fn LandmarksTranslator::Pointer LandmarksTranslator::New()
   * Creates a smart pointer associated with a LandmarksTranslator object.
   */
   
  /**
   * @fn LandmarksTranslator::Iterator LandmarksTranslator::Begin()
   * Returns an iterator to the beginning of the list of translations.
   */
  
  /**
   * @fn LandmarksTranslator::ConstIterator LandmarksTranslator::Begin() const
   * Returns a const iterator to the beginning of the list of translations.
   */
  
  /**
   * @fn LandmarksTranslator::Iterator LandmarksTranslator::End()
   * Returns an iterator just past the last translation.
   */
  
  /**
   * @fn LandmarksTranslator::ConstIterator LandmarksTranslator::End() const
   * Returns a const iterator just past the last translation.
   */
   
  /**
   * @fn int LandmarksTranslator::GetLandmarkNumber()
   * Gets the number of landmarks' translation.
   */
  
  /**
   * Sets the translation for the landmark labeled @a landmark.
   */
  void LandmarksTranslator::SetLandmark(const std::string& landmark, const std::string& translation)
  {
    if (landmark.empty() || translation.empty())
    {
      btkErrorMacro("Impossible to set an empty label for the landmark or its translation.")
      return;
    }
    this->m_Translator[landmark] = translation;
    this->Modified();  
  };
  
  /**
   * Gets the translation assocated with the landmark labeled @a landmark.
   */
  const std::string LandmarksTranslator::GetTranslation(const std::string& landmark) const
  {
    ConstIterator it = this->m_Translator.find(landmark);
    if (it == this->End())
      return "";
    else
      return it->second;
  };
  
  /**
   * @fn Pointer LandmarksTranslator::Clone() const
   * Returns a deep copy of the object as a smart pointer.
   */

  /**
   * Constructor
   */
  LandmarksTranslator::LandmarksTranslator()
  : DataObject(), m_Translator()
  {};
  
  LandmarksTranslator::LandmarksTranslator(const LandmarksTranslator& toCopy)
  : DataObject(toCopy), m_Translator(toCopy.m_Translator)
  {};
}
