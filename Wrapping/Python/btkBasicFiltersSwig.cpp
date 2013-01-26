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
 
#include "btkBasicFiltersSwig.h"

// ------------------------------------------------------------------------- //
//                          AcquisitionUnitConverter                         //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(AcquisitionUnitConverter);

// ------------------------------------------------------------------------- //
//                             AnalogOffsetRemover                           //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(AnalogOffsetRemover);

// ------------------------------------------------------------------------- //
//                              CollectionAssembly                           //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                               DownsampleFilter                            //
// ------------------------------------------------------------------------- //
// Warning: C++ template. Need concrete implementation
// - Wrench
BTK_SWIG_DEFINE_DEFAULT_CTOR_TEMPLATE(WrenchDownsampleFilter, DownsampleFilter, btk::Wrench);
// - WrenchCollection
BTK_SWIG_DEFINE_DEFAULT_CTOR_TEMPLATE(WrenchCollectionDownsampleFilter, DownsampleFilter, btk::WrenchCollection);

// ------------------------------------------------------------------------- //
//                           ForcePlatformsExtractor                         //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(ForcePlatformsExtractor);

// ------------------------------------------------------------------------- //
//                          ForcePlatformWrenchFilter                         //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(ForcePlatformWrenchFilter);

// ------------------------------------------------------------------------- //
//                         GroundReactionWrenchFilter                        //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(GroundReactionWrenchFilter);

// ------------------------------------------------------------------------- //
//                                IMUsExtractor                              //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(IMUsExtractor);

// ------------------------------------------------------------------------- //
//                            MeasureFrameExtractor                          //
// ------------------------------------------------------------------------- //
// Check SWIG with C++ template

// ------------------------------------------------------------------------- //
//                           MergeAcquisitionFilter                          //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(MergeAcquisitionFilter);

// ------------------------------------------------------------------------- //
//                    SeparateKnownVirtualMarkersFilter                      //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(SeparateKnownVirtualMarkersFilter);

// ------------------------------------------------------------------------- //
//                         SpecializedPointsExtractor                        //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(SpecializedPointsExtractor);

// ------------------------------------------------------------------------- //
//                            SubAcquisitionFilter                           //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(SubAcquisitionFilter);

// ------------------------------------------------------------------------- //
//                VerticalGroundReactionForceGaitEventDetector               //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(VerticalGroundReactionForceGaitEventDetector)

// ------------------------------------------------------------------------- //
//                         WrenchDirectionAngleFilter                        //
// ------------------------------------------------------------------------- //
BTK_SWIG_DEFINE_DEFAULT_CTOR(WrenchDirectionAngleFilter);
