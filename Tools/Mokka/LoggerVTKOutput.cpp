/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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

#include "LoggerVTKOutput.h"
#include "LoggerMessage.h"

#include <vtkObjectFactory.h>

#if defined(_MSC_VER)
  // Disable unsafe warning (use of the function 'strcpy' instead of 
  // 'strcpy_s' for portability reasons;
  #pragma warning( disable : 4996 )
#endif

#define LOGGER_VTK_OUTPUT(fn, str) \
  const char* sub = strchr(str,'\n')+1; \
  size_t len = strlen(sub) - 2; \
  char* msg = (char*)malloc(len+1); \
  strncpy(msg, sub, len); \
  msg[len] = '\0'; \
  fn(msg); \
  free(msg);

vtkStandardNewMacro(LoggerVTKOutput);
vtkCxxRevisionMacro(LoggerVTKOutput, "$Revision: 0.1 $");

void LoggerVTKOutput::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
};

void LoggerVTKOutput::DisplayText(const char* str)
{
  LOGGER_VTK_OUTPUT(LOG_INFO, str);
};

void LoggerVTKOutput::DisplayErrorText(const char* str)
{
  LOGGER_VTK_OUTPUT(LOG_CRITICAL, str);
};

void LoggerVTKOutput::DisplayWarningText(const char* str)
{
  LOGGER_VTK_OUTPUT(LOG_WARNING, str);
};

void LoggerVTKOutput::DisplayGenericWarningText(const char* str)
{
  size_t len = strlen(str) - 2; \
  char* msg = (char*)malloc(len+1); \
  strncpy(msg, str, len); \
  msg[len] = '\0'; \
  LOG_INFO(msg); \
  free(msg);
};

void LoggerVTKOutput::DisplayDebugText(const char* str)
{
  LOGGER_VTK_OUTPUT(LOG_INFO, str);
};

LoggerVTKOutput::LoggerVTKOutput()
: vtkOutputWindow()
{};

