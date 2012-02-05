/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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
 
// These declaration are different from the C++ class to fit the know basic type in Python (i.e. int, double and string).
// This has for consequence to not be able to construct MetaData(Info) with the format 'Byte'.
// To be able to have a MetaDataInfo with the format 'Byte', you need to use the method 'SetFormat'

BTK_SWIG_DECLARE_CLASS(MetaDataInfo)
{
public:
  enum {Char = btk::MetaDataInfo::Char, Byte = btk::MetaDataInfo::Byte, Integer = btk::MetaDataInfo::Integer, Real = btk::MetaDataInfo::Real};
  btkMetaDataInfo(int val);
  btkMetaDataInfo(double val);
  btkMetaDataInfo(const std::string& val);
  btkMetaDataInfo(const std::vector<int>& val);
  btkMetaDataInfo(const std::vector<double>& val);
  btkMetaDataInfo(const std::vector<std::string>& val);
  btkMetaDataInfo(const std::vector<int>& dim, const std::vector<int>& val);
  btkMetaDataInfo(const std::vector<int>& dim, const std::vector<double>& val);
  btkMetaDataInfo(const std::vector<int>& dim, const std::vector<std::string>& val);
  BTK_SWIG_DECLARE_CLONE(MetaDataInfo);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(MetaDataInfo);
};

BTK_SWIG_DECLARE_CLASS(MetaData)
{
public:
  btkMetaData(const std::string& label); // , const std::string& desc = "", bool isUnlocked = true); // SWIG error: CommonSwig_MetaData.h(69) : Warning 302: Identifier 'btkMetaData' redefined (ignored) -> MSVC error: ambiguous call to overloaded function.
  btkMetaData(const std::string& label, int val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, double val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, const std::string& val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, const std::vector<int>& val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, const std::vector<double>& val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, const std::vector<std::string>& val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, const std::vector<int>& dim, const std::vector<int>& val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, const std::vector<int>& dim, const std::vector<double>& val, const std::string& desc = "", bool isUnlocked = true);
  btkMetaData(const std::string& label, const std::vector<int>& dim, const std::vector<std::string>& val, const std::string& desc = "", bool isUnlocked = true);
  BTK_SWIG_DECLARE_CLONE(MetaData);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(MetaData);
};
BTK_SWIG_DECLARE_ITERATOR(MetaData,MetaData);