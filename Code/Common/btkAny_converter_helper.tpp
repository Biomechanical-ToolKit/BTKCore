/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#ifndef __btkAny_converter_helper_tpp
#define __btkAny_converter_helper_tpp

namespace btk
{
  template <typename S, typename R>
  struct Any::Converter::HelperBase
  {
    static inline void convert(void* source, void* result) {*static_cast<R*>(result) = Helper<S,R>::convert(*static_cast<S*>(source));};
  };
  
  template <typename S, typename R>
  struct Any::Converter::Helper : Any::Converter::HelperBase<S,R>
  {
    static inline R convert(const S& val) {return static_cast<R>(val);}
  };
  
  // ----------------------------------------------------------------------- //
  //                           Partial specializations
  // ----------------------------------------------------------------------- //

  // Converting the same type returns directly the value
  template<typename U>
  struct Any::Converter::Helper<U,U> : Any::Converter::HelperBase<U,U>
  {
    static inline U convert(const U& val) {return val;}
  };

  // Converting a string to something else.
  // NOTE: This conversion must fully specialized. This partial traits is here only to generate a compiler error due to the use of a static_assert call.
  template<typename U>
  struct Any::Converter::Helper<std::string,U> : Any::Converter::HelperBase<std::string,U>
  {
    static_assert(std::is_same<U,std::string>::value,"Every conversion from a string must be fully specialized.");
    static inline U convert(const std::string& ) {return U();};
  };

  // Converting something to a string
  template<typename U>
  struct Any::Converter::Helper<U,std::string> : Any::Converter::HelperBase<U,std::string>
  {
    static inline std::string convert(const U& val) {return std::to_string(val);};
  };

  // ----------------------------------------------------------------------- //
  //    Resolve ambiguous specializations (due to partial specializations)
  // ----------------------------------------------------------------------- //

  // Converting a string to another string
  template<>
  struct Any::Converter::Helper<std::string,std::string> : Any::Converter::HelperBase<std::string,std::string>
  {
    static inline std::string convert(const std::string& val) {return val;}
  };

  // ----------------------------------------------------------------------- //
  //                           Full specializations
  // ----------------------------------------------------------------------- //

  // ----------------------------------------------------------------------- //
  //                                 boolean
  // ----------------------------------------------------------------------- //

  // A boolean can be set also to false when the value is equal to "", "0", or "false".
  template<>
  struct Any::Converter::Helper<std::string,bool> : Any::Converter::HelperBase<std::string,bool>
  {
    static inline bool convert(const std::string& val) {return (val.empty() || (val.compare("0") == 0) || (val.compare("false") == 0));};
  };
  
  // A boolean converted to a string is set to "true" when it is true or "false" when false.
  template<>
  struct Any::Converter::Helper<bool,std::string> : Any::Converter::HelperBase<bool,std::string>
  {
    static inline std::string convert(const bool& val) {return std::string(val ? "true" : "false");};
  };

  // ----------------------------------------------------------------------- //
  //                  (signed / unsigned) integers (8,16,32,64-bit)
  // ----------------------------------------------------------------------- //

  template<>
  struct Any::Converter::Helper<std::string,int8_t> : Any::Converter::HelperBase<std::string,int8_t>
  {
    static inline int8_t convert(const std::string& val) {return static_cast<int8_t>(stol(val));};
  };

  template<>
  struct Any::Converter::Helper<std::string,uint8_t> : Any::Converter::HelperBase<std::string,uint8_t>
  {
    static inline uint8_t convert(const std::string& val) {return static_cast<uint8_t>(stoul(val));};
  };

  template<>
  struct Any::Converter::Helper<std::string,int16_t> : Any::Converter::HelperBase<std::string,int16_t>
  {
    static inline int16_t convert(const std::string& val) {return static_cast<int16_t>(stol(val));};
  };

  template<>
  struct Any::Converter::Helper<std::string,uint16_t> : Any::Converter::HelperBase<std::string,uint16_t>
  {
    static inline uint16_t convert(const std::string& val) {return static_cast<uint16_t>(stoul(val));};
  };

  template<>
  struct Any::Converter::Helper<std::string,int32_t> : Any::Converter::HelperBase<std::string,int32_t>
  {
    static inline int32_t convert(const std::string& val) {return static_cast<int32_t>(stol(val));};
  };

  template<>
  struct Any::Converter::Helper<std::string,uint32_t> : Any::Converter::HelperBase<std::string,uint32_t>
  {
    static inline uint32_t convert(const std::string& val) {return static_cast<uint32_t>(stoul(val));};
  };

  template<>
  struct Any::Converter::Helper<std::string,int64_t> : Any::Converter::HelperBase<std::string,int64_t>
  {
    static inline int64_t convert(const std::string& val) {return static_cast<int64_t>(stoll(val));};
  };

  template<>
  struct Any::Converter::Helper<std::string,uint64_t> : Any::Converter::HelperBase<std::string,uint64_t>
  {
    static inline uint64_t convert(const std::string& val) {return static_cast<uint64_t>(stoull(val));};
  };

  // ----------------------------------------------------------------------- //
  //                                float / double
  // ----------------------------------------------------------------------- //

  template<>
  struct Any::Converter::Helper<std::string,float> : Any::Converter::HelperBase<std::string,float>
  {
    static inline float convert(const std::string& val) {return stof(val);};
  };

  template<>
  struct Any::Converter::Helper<std::string,double> : Any::Converter::HelperBase<std::string,double>
  {
    static inline double convert(const std::string& val) {return stod(val);};
  };
};

#endif // __btkAny_converter_helper_tpp