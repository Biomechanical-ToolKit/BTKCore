/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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

#ifndef __btkOption_h
#define __btkOption_h

#include "btkLogger.h"

#include <vector>
#include <tuple>
#include <array>

// Note: This is a large macro but all the code is inlined.
#define BTK_DECLARE_IOHANDLER_OPTIONS(...) \
  public: \
    class _Options \
    { \
      static inline auto make_options() -> decltype(std::make_tuple(__VA_ARGS__)) \
      { \
        return { __VA_ARGS__ }; \
      } \
    public: \
      using _Tuple = decltype(_Options::make_options()); \
      _Tuple Tuple; \
      _Options() : Tuple(_Options::make_options()) {}; \
      ~_Options() = default; \
      _Options(const _Options& ) = delete; \
      _Options(_Options&& ) noexcept = delete; \
      _Options& operator=(const _Options& ) = delete; \
      _Options& operator=(const _Options&& ) noexcept = delete; \
      template<class T, size_t I, size_t N> \
      struct iterate \
      { \
        static inline void extract_name(std::vector<const char*>* output) \
        { \
          output->operator[](I) = std::tuple_element<I,T>::type::name(); \
          iterate<T,I+1,N>::extract_name(output); \
        }; \
        static inline std::vector<const char*> extract_choices(const char* option) \
        { \
          using _Elt = typename std::tuple_element<I,T>::type; \
          if (strcmp(_Elt::name(),option) == 0) \
            return _Elt::choices(); \
          else \
            return iterate<T,I+1,N>::extract_choices(option); \
        }; \
        static inline void get_value(const T* tuple, const char* option, void* value) \
        { \
          using _Elt = typename std::tuple_element<I,T>::type; \
          if (strcmp(_Elt::name(),option) == 0) \
            *static_cast<typename _Elt::Format*>(value) = std::get<I>(*tuple).value(); \
          else \
            return iterate<T,I+1,N>::get_value(tuple, option, value); \
        }; \
        static inline void set_value(T* tuple, const char* option, const void* value) \
        { \
          using _Elt = typename std::tuple_element<I,T>::type; \
          if (strcmp(_Elt::name(),option) == 0) \
            std::get<I>(*tuple).setValue(*static_cast<const typename _Elt::Format*>(value)); \
          else \
            return iterate<T,I+1,N>::set_value(tuple, option, value); \
        }; \
      }; \
      template<class T, size_t N> \
      struct iterate<T,N,N> \
      { \
        static inline void extract_name(std::vector<const char*>* ) {}; \
        static inline std::vector<const char*> extract_choices(const char* ) {return std::vector<const char*>();}; \
        static inline void get_value(const T* , const char* , void* ) {}; \
        static inline void set_value(T* , const char* , const void* ) {}; \
      }; \
    }; \
    virtual std::vector<const char*> availableOptions() const noexcept override \
    { \
      using _TupleSize = std::tuple_size<_Options::_Tuple>; \
      std::vector<const char*> options(_TupleSize::value); \
      _Options::iterate<_Options::_Tuple,0,_TupleSize::value>::extract_name(&options); \
      return options; \
    }; \
    virtual std::vector<const char*> availableOptionChoices(const char* option) const noexcept override \
    { \
      using _TupleSize = std::tuple_size<_Options::_Tuple>; \
      return _Options::iterate<_Options::_Tuple,0,_TupleSize::value>::extract_choices(option); \
    }; \
    virtual void option(const char* option, void* value) const noexcept override \
    { \
      using _TupleSize = std::tuple_size<_Options::_Tuple>; \
      return _Options::iterate<_Options::_Tuple,0,_TupleSize::value>::get_value(&(this->Options.Tuple),option,value); \
    }; \
    virtual void setOption(const char* option, const void* value) noexcept override \
    { \
      using _TupleSize = std::tuple_size<_Options::_Tuple>; \
      return _Options::iterate<_Options::_Tuple,0,_TupleSize::value>::set_value(&(this->Options.Tuple),option,value); \
    }; \
    _Options Options; \
  private:

namespace btk
{
  template <typename V, V v> struct stringify_option_value;
  
  template <typename O>
  struct make_option
  {
    using V = typename O::Format;
    template <V... vs> using D = typename O::template Details<vs...>;
    
    template <V... vs>
    struct multiple_choices
    {
      static inline D<vs...> init(V&& value)
      {
        return D<vs...>(std::forward<V>(value));
      };
      multiple_choices() = delete;
      ~multiple_choices() noexcept = delete;
      multiple_choices(const multiple_choices& ) = delete;
      multiple_choices(multiple_choices&& ) noexcept = delete;
      multiple_choices& operator=(const multiple_choices& ) = delete;
      multiple_choices& operator=(multiple_choices&& ) noexcept = delete;
    };
    template <V v>
    static inline D<v> single_choice()
    {
      return D<v>(std::forward<V>(v));
    };
  };
  
  template <const char* S, typename V>
  class Option
  {
  public:
    using Format = V;
    
    static inline constexpr const char* name() noexcept {return S;};
    
    Option() = delete;
    ~Option() noexcept = delete;
    Option(const Option& ) = delete;
    Option(Option&& ) noexcept = delete;
    Option& operator=(const Option& ) = delete;
    Option& operator=(Option&& ) noexcept = delete;
    
    template <V... vs>
    class Details
    {
    public:
      using Format = V;

      static inline constexpr const char* name() noexcept {return S;};
      static inline std::vector<const char*> choices() noexcept {return {stringify_option_value<V,vs>::c_str...};};
    
      Details(V&& v)
      : Value(std::forward<V>(v))
      {};
      
      ~Details() = default;
      Details(Details&& ) = default;
      
      Details() = delete;
      Details(const Details& ) = delete;
      Details& operator=(const Details& ) = delete;
      Details& operator=(Details&& ) noexcept = delete;
      
      inline const V& value() const noexcept {return this->Value;};
      inline void setValue(const V& value) noexcept
      {
        constexpr size_t num = sizeof...(vs);
        constexpr std::array<V,num> choices{{vs...}};
        for (size_t i = 0 ; i < num ; ++i)
        {
          if (choices[i] == value)
          {
            this->Value = value;
            return;
          }
        }
        Logger::warning("Impossible to set an option with a value not in the predefined choices.");
      };
      
    private:
      V Value;
    };
  };
  
  /**
   * @class Option btkOption.h
   * @brief To define I/O handler options.
   *
   * A I/O handler (class IOHandler) has capacity to read and/or write data from/to a device.
   * By default a I/O handler has options to set encoding format, byte order format, etc.
   * These options can be set using scoped enumarotrs or associated strings. The former could be used 
   * in the implementation of the handler, while the second is more for dynamic code (e.g. reader/writer 
   * of several handler, language wrappers)
   * 
   * But a specific handler could have other dedicated option (for example, C3DHandler::AnalogStorageFormat).
   * It would be then interesting to have a simple way to create them giving the same behaviour than what presented before.
   * The class Option fill this role. More exaclty the addition of option in a handler is realised using the macro BTK_DECLARE_IOHANDLER_OPTIONS
   * and the class make_option. All the declaration must be done in the private implementation of the handler.
   * 
   * For example, in the C3D handler (class C3DHandler), the option C3DHandler::AnalogStorageForma was added simply.
   *
   * First a scoped enumeration was created in the public interface as well as a type definition.
   *
   * @code
   * // Scoped enumartor
   * enum class AnalogStorage : int {Signed, Unsigned};
   * // Option type
   * //  - The template variable _btk_C3DHandler_AnalogStorage store a static array of charater related to the option's name.
   * //  - The template typename AnalogStorage indicates the type to use for this kind of option
   * using AnalogStorageFormat = Option<_btk_C3DHandler_AnalogStorage,AnalogStorage>;
   * @endcode
   *
   * Then, in the private implementation, all the options to define for the handler are finalized. Tipically this is used to set the possible values to store (indeed, not all enumartors could be allowed)
   * Inside the macro BTK_DECLARE_IOHANDLER_OPTIONS, it is needed to set the optons. For example
   *
   * @code
   * BTK_DECLARE_IOHANDLER_OPTIONS(
   *   make_option<C3DHandler::EncodingFormat>::single_choice<C3DHandler::Encoding::Binary>(),
   *   make_option<C3DHandler::ByteOrderFormat>::multiple_choices<C3DHandler::ByteOrder::VAXLittleEndian,C3DHandler::ByteOrder::IEEELittleEndian,C3DHandler::ByteOrder::IEEEBigEndian>::init(C3DHandler::ByteOrder::Native),
   *   make_option<C3DHandler::DataStorageFormat>::multiple_choices<C3DHandler::DataStorage::Integer,C3DHandler::DataStorage::Float>::init(C3DHandler::DataStorage::Float),
   *   make_option<C3DHandler::AnalogStorageFormat>::multiple_choices<C3DHandler::AnalogStorage::Signed,C3DHandler::AnalogStorage::Unsigned>::init(C3DHandler::AnalogStorage::Signed)
   * )
   * @endcode
   *
   * Internally this creates a member Options set as a tuple. The use of make_option::single_choice or make_option::multiple_choices::init depend of the possibilities of the implemented handler.
   * In the previous example, there are 4 options.
   *  - EncodingFormat can be set only to Binary (not Text or Mix)
   *  - ByteOrderFormat can be set only to VAXLittleEndian, IEEELittleEndian, and IEEEBigEndian (NotApplicable cannot be set)
   *  - DataStorageFormat can be set to Integer or Float (not NotApplicable)
   *  - AnalogStorageFormat can be set to signed or Unsigned
   *
   * Finally, one missing part is the associated string for the values associated with the option AnalogStorageFormat.
   * In the implementation (CPP) file, it is needed to specialized one function: stringify_option_value.
   * This is required to use the setting of options using strings. The specialization is done simple like that:
   *
   * @code
   * template <>
   * struct stringify_option_value<C3DHandler::AnalogStorage,C3DHandler::AnalogStorage::Signed>
   * {
   *   static constexpr const char* c_str = "Signed";
   * };
   * 
   * template <>
   * struct stringify_option_value<C3DHandler::AnalogStorage,C3DHandler::AnalogStorage::Unsigned>
   * {
   *   static constexpr const char* c_str = "Unsigned";
   * };
   * @endcode
   */
};

#endif // __btkOption_h