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

%define DOCSTRING
"Biomechanical ToolKit (BTK) for Python
High level classes and functions to open, edit and modify biomechanical acquisitions."
%enddef

%module(docstring=DOCSTRING) btk
%{
#include "btkException.h"
%}
%include <exception.i>

// Handle exceptions from BTK
%exception
{
  try {
      $action
  } catch(btk::Exception& e) {
      SWIG_exception(SWIG_RuntimeError, e.what());
  } catch(...) {
      SWIG_exception(SWIG_RuntimeError,"Unknown exception");
  }
}

// Extra python code in the btk namespace
%pythoncode %{
def Iterate(c):
    """
    Python generator to iterate through a collection of BTK objects (Points, Analogs, Events and Metadata).
    
    For example:
  
    for i in btk.Iterate(acq.GetPoints()):
        print i.GetLabel()
    """
    i = c.Begin()
    while i != c.End():
        yield i.value()
        i.incr()
%}

%feature("autodoc", "0");

// One file to govern the others.
%include "btkCommonSwig.i"
%include "btkIOSwig.i"
%include "btkBasicFiltersSwig.i"