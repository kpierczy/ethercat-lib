/* ============================================================================================================================ *//**
 * @file       translation.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Tuesday, 7th June 2022 7:20:00 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Declarations of translation-module utilities
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_TRANSLATION_H__
#define __ETHERCAT_COMMON_TRANSLATION_H__

/* =========================================================== Includes =========================================================== */

#include "ethercat/common/translation/translator_traits.hpp"
#include "ethercat/common/translation/sizing_translator_traits.hpp"
#include "ethercat/common/translation/translator_wrapper.hpp"
#include "ethercat/common/translation/sizing_translator_wrapper.hpp"
#include "ethercat/common/translation/translation_error.hpp"
#include "ethercat/common/translation/default_translator.hpp"

/* ========================================================== Namespaces ========================================================== */

/**
 * @details Each communication protocol - including EtherCAT - is based on the notion of transporting 
 *    some kind of 'objects' in a form of byte-serialized messages. When writting support library for 
 *    the communication protocol the mechanism of translation  between 'serialized' and 'app domain' 
 *    formats of these objects is required.  To maximize ease and flexibility of the process, `ethercat` 
 *    library provides user with ability to automate this process with the help of so called 
 *    <b><i>translator<i/><b/> types. 
 *    
 *    <i>Translator</i> types provide routines @b translate_to()/translate_from() translating an arbitrary 
 *    object between two domains. The library provides translators for builtin EtherCAT (precisely CoE) 
 *    types such as booleans, arithmetic types, strings and plain array of such objects. These translators 
 *    can be additionally configrued by corresponding constants in the ethercat/config.hpp file.
 * 
 * @par "Translators overview"
 * @parblock
 * 
 *    User can define custom translators to support arbitrary objects types defined by slave devices on 
 *    the bus. Translator can provide serialization routine (translate_from()), deserialization routine 
 *    (translate_to()) or both. Depending on the provided routines it is called <i>Output</i> translator,
 *    <i>Input</i> translator or <i>Bidirectional</i> translator. Trasnaltion routines should implement 
 *    signatures compatible with following:
 * 
 *        /// Input translation
 *        void/bool translate_to(Span<const uint8_t> data, T &obj)
 *        void/bool translate_to(Span<const uint8_t> data, T &obj, std::size_t bitoffset)
 *        
 *        /// Output translation
 *        void/bool translate_from(Span<uint8_t> data, const T &obj)
 *        void/bool translate_from(Span<uint8_t> data, const T &obj, std::size_t bitoffset)
 *    
 *    <i>Compatible<i> in this context means that the library should be able to call these routines 
 *    <i>as if</i> they were defined with such signatures. E.g. translate_from() can be implemented in 
 *    the following form:
 * 
 *        void translate_from(Span<uint8_t> data, T obj)
 * 
 *    However the following signature would be invalid
 * 
 *        void translate_to(Span<const uint8_t> data, T obj)
 * 
 *    @p Span type can be configured in the configuration header file (by default library uses ranges::span).
 *    Translation routines can optionally take a @p bitoffset parameter to properly handle bit-aligned 
 *    objects. Support for bit-aligned required additional bit-banging steps and so it is potentially 
 *    more expensive. That's why many ENI generators as well as slave device providers designs PDI 
 *    (Process Data Images) and OD (Objects Dictionaries) that do not require bit-aligned data access. 
 *    For this reason library provides configuration parameters that can:
 * 
 *       - enable bit-aligned objects handling
 *       - enable check of proper bit-aligned objects handling (in such a scenario translators
 *         are not required to provide bit-aligned translation but when non-byte-aligned object
 *         is attempted to be translated, a runtime exception is thrown) 
 * 
 * @endparblock
 * 
 * @par "Translators categories"
 * @parblock
 * 
 *    To maximize performance of objects-translation (that often occur on the critical path of the
 *    critical path of the EtherCAT driver applications) library provides compile-time categorization
 *    mechanism of translator types that aims to enable proper inlining of translation routines in the
 *    caller's context.
 * 
 *    Each translator type is categorized in the context of the target <b>translated (target) type</b> 
 *    @p T . For the given translator type @p TranslatorT and translated type @p T there are two orthogonal 
 *    criterions that the translator type is categorized by:
 * 
 *      - @b stateless/statefull : stateless translators does not require local instance to provide 
 *        translation routine. Instead they define static versions of translate_to()/translate_from() 
 *        method(s). On the other side statefull translators are also supported. They will be 
 *        automatically created and destroyed by the library when the translation is required. Usually 
 *        translation process does not require contextual data and so stateless translators are more 
 *        common. However some specific scenarios (e.g. debugging purposes) may require internal state 
 *        of the translator to be held in the memory.
 * 
 *      - @b specialized/generic : this categorization is mainly needed by the library to know whether
 *        translation function should be called with `translate*(...)` or `translate*<T>(...)` signature,
 *        i.e. either translation method is automatically choosen based on the @p T type (and so it is
 *        'specialized') or it needs explicit type guidance as it may support multiple @p T types (and
 *        so it is generic)
 * 
 *   Statefull catageory has precedence over stateless category and so if the translator type provides
 *   both static and non-static translation method it is categorized as a statefull translator and the
 *   later routine type is used by the library. When it comes to specialized/generic categorization, the 
 *   library follows standard overloads-resolution rules:
 *  
 *      - exact-match non-template overload is prefered, otherwise
 *      - exact-match template overload is prefered, otherwise
 *      - castable-match non-template overload is prefered, otherwise
 *      - castable-match template overload is prefered
 * 
 *   For exact categorizing algorithm read "Target type deduction and categorization algorithm" paragraph
 * 
 * @endparblock
 * 
 * @par "Target type deduction and categorization algorithm"
 * @parblock
 * 
 *   To categorize the translator type @p TranslatorT the library needs to know the target type @p T 
 *   that the translator will be used for. Library provides user with two ways to pass this information:
 * 
 *     - <b>explicit</b>: library functions/methods requiring objects translation provide template overloads
 *       with signature `template<..., typename TranslatorT, typename T, ...>` that let user explicitly specify
 *       target type for the translator
 * 
 *     - <b>implicit</b>: library functions/methods requiring obejcts translation provide template overloads 
 *       with signature `template<..., typename TranslatorT, ...>` that use automatic deduction of the target 
 *       type
 * 
 *   Depending on the way that the @p T type is obtained by the library the transltor-categorization
 *   algorithm performs in one of the following ways:
 * 
 *      1) <b>explicit deduction</b>: for explicitly given target type, the categorization algorithm
 *         performs in the following way:
 * 
 *            - if translator provides non-template, non-static translation routine that is callable with 
 *              @p T it is categorized as <i>statefull specialized<i>; otherwise
 *            - if translator provides non-template, static translation routine that is callable with 
 *              @p T it is categorized as <i>stateless specialized<i>; otherwise
 *            - if translator provides @p T-specializable template, non-static translation that is callable 
 *              with @p T it is categorized as <i>statefull generic<i>; otherwise
 *            - if translator provides @p T-specializable template, static translation that is callable 
 *              with @p T it is categorized as <i>stateless generic<i>; otherwise
 *            - compilation error is triggerred
 * 
 *      2) <b>implicit deduction</b>: for automatically deduced target type, the categorization algorithm
 *         performs in the following way: 
 * 
 *            - if @p TranslatorT defined @p Type type alias it is treated as the target type and 
 *              categorization algorithm is performed in the way described for <b>implicit deduction</b>;
 *              otherwise
 *            - if translator provides non-overloaded, non-template, non-static translation routine,
 *              target type @p T is deduced from it's (potentially cv-ref-qualified) second argument
 *              and translator is categorized as <i>statefull specialized<i>; otherwise
 *            - if translator provides non-overloaded, non-template, static translation routine,
 *              target type @p T is deduced from it's (potentially cv-ref-qualified) second argument
 *              and translator is categorized as <i>stateless specialized<i>; otherwise
 *            - compilation error is triggerred;
 *  
 *   Note that for <i>Bidirectional</i> translator categorization takes place independently for both
 *   translation directions.
 * 
 * @endparblock
 * 
 * @par "Example 1 - basic translator"
 * @parblock 
 * 
 *   Assume that there is slave managing a CoE object of a custom structural type <b>Info</b> that 
 *   consists of two fields - [BYTE] id, [REAL] value. Such an object is represented by EtherCAT
 *   as a 5-byte entity and could be represented in the application domain as the following structure:
 *   
 *       struct Info {
 *           uint8_t id;
 *           float value;
 *       };
 * 
 *   To enable the library with capability of handling such kind of objects user could implement the 
 *   following translator:
 * 
 *       struct InfoTranslator {
 *           
 *           // Translated type
 *           using Type = Info;
 *           
 *           // Input translation
 *           static inline void translate_to(Span<const uint8_t> data, Info &obj) {
 *               std::copy_n(&data[0], 1, reinterpret_cast<uint8_t*>(&obj.id));
 *               std::copy_n(&data[1], 4, reinterpret_cast<uint8_t*>(&obj.value));
 *           }
 *           
 *           // Output translation
 *           static inline void translate_from(Span<uint8_t> data, const Info &obj) {
 *               std::copy_n(reinterpret_cast<const uint8_t*>(&obj.id),    1, &data[0]);
 *               std::copy_n(reinterpret_cast<const uint8_t*>(&obj.value), 4, &data[1]);
 *           }
 *           
 *       };
 * 
 *   That would enable automatic-target-type deduction in the library (note that the @p Type alias could
 *   be not-defined and the library would still be able to deduce @p Info as a target type based on the
 *   second argument to translating methods). As both translating methods are @p static the InfoTranslator
 *   object will not be created by the library when the translation in either direction is needed. Such a 
 *   translator could be used by the user e.g. for SDO communication with the slave in the following form:
 * 
 *       Info info = slave.get_sdo(0x1).upload<InfoTranslator>()
 * 
 * @endparblock
 * 
 * 
 * @par "`make_buffer()` utility"
 * @parblock
 * 
 *   EtherCAT (CoE) defines two ways of exchanging data between master and slave nodes - cyclical objects
 *   (Process Data Objects, PDO) and acyclical objects (Service Data Objects, SDO). Memory layout of PDOs
 *   mapped into PDI (Process Data Image) is described in the ENI file providing configuration for the bus.
 *   Thanks to it, the library can analyze this file to get size (and bitoffset) of binary buffers required
 *   to store binary image of these objects. E.g. if "Some PDO object" is of type @c SINT[0..2] and is mapped
 *   in the @c 5th bit of the PDI, the library will know that to store binary image of this object it will need
 *   to prepare @c 4 byte buffer ( @c 3 bytes for each element + @c 1 byte resulting from bitoffset ). Next,
 *   user code can simply request translating "Some PDO object" into e.g. @c std::array<uint8_t,3> entity.
 * 
 *   Things look slightly different when it comes to SDOs for which ENI file does not provide any description.
 *   Let assume that user code requests library to read "Some SDO object" of index @c 0x0 and subindex @c 0x0 
 *   from the slave's memory. Assume that this object is of a custom structural type @c <Struct> . In general,
 *   the app-domain memory layout will NOT be identical to it's binary-image memory layout (if it was the case,
 *   the whole concept of translators would be unneccesarry). translate_to()/translate_from() method of the
 *   translator predefined for this type will probably expect binary-image of an exact size and will (potentially)
 *   enforce this constraint (e.g. throwing an exception if a binary-image buffer of a wrong size is given). However,
 *   as the reference to such a buffer is passed to these function as a generic @c <Span> type (that stores runtime
 *   pointer to the buffer along with it's runtime size), the library has no means to deduce size of the buffer
 *   required to store binary image of the requested read object. For this reason, to support also SDO translation,
 *   the translator type needs to provide additional utility - @b make_buffer() - that will enable library to
 *   do so [1]. In the librarie's documentation his utility is described as <i>sizing method</i>.
 * 
 *   @c make_buffer() utility comes into two flavours - <i><b>statically sized</b></i> and <i><b>dynamically sized</b></i>.
 *   This categorization aims to mimic categorization conditions of data types in general. For some objects 
 *   types, the size of the buffer required to store it's binary image is known at compile time. An example 
 *   of such a type is already mentioned @c SINT[0..2] . For such types, translators that need to be used in 
 *   the SDO context need to provide a method with the following signature:
 * 
 *       some_buffer_type make_buffer()
 * 
 *   Whenever library needs to store content of incoming/outgoing SDO, it will call this method to create
 *   an appropriate buffer. @c some_buffer_type in most cases will be object of type @c std::array<uint8_t,N> 
 *   (this is what @ref DefaultTranslator returns for all <i>statically sized</i> types). However, user can use an
 *   arbitrary data type that can be used to construct object of type @ref config::types::Span<uint8_t> .
 * 
 *   For <i>dynamically sized</i> types, size of the required buffer is not known at compile time, and instead, 
 *   programm will need to provide this value at runtime. It is important to note, that when we talk about 
 *   <i>dynamically sized</i> types we refer only to app-domain representation of the SDO. In EtherCAT (CoE)
 *   all types are <i>statically sized</i>. However - for convinience - user can use dynamic C++ types 
 *   (e.g. std::vector) to represent them. This may be for at least two reasons:
 * 
 *     - to decouple buffer lifetime from the execution flow
 *     - to provide unified translation model for twin structural types that are parametrized
 *       with size of some member
 *    
 *    In such cases, translator type is obliged to provide method with the following signature:
 * 
 *       some_buffer_type make_buffer(const T &object)
 * 
 *    Again - @c some_buffer_type may be any type compatible with @c std::array<uint8_t,N> with respect to 
 *    creation of @ref config::types::Span<uint8_t> instance.
 * 
 *    It is worth to add that like in case of translating methods these @c make_buffer() methods can be implemented
 *    in <i>statefull</i>/<i>stateless</i> and <i>specialized</i>/<i>generic</i> fashion. It means that any of the
 *    following methods' signatures is accepted by the library:
 * 
 * 
 *               some_buffer_type make_buffer()                   // Statefull, specialized utility for statically-sized types
 *        static some_buffer_type make_buffer()                   // Stateless, specialized utility for statically-sized types
 *               some_buffer_type make_buffer<T>()                // Statefull, generic utility for statically-sized types
 *        static some_buffer_type make_buffer<T>()                // Stateless, generic utility for statically-sized types
 * 
 *               some_buffer_type make_buffer(const T &object)    // Statefull, specialized utility for dynamically-sized types
 *        static some_buffer_type make_buffer(const T &object)    // Stateless, specialized utility for dynamically-sized types
 *               some_buffer_type make_buffer<T>(const T &object) // Statefull, generic utility for dynamically-sized types
 *        static some_buffer_type make_buffer<T>(const T &object) // Stateless, generic utility for dynamically-sized types
 * 
 *    The library will automatically pick utility following rules described in the "Target type deduction and 
 *    categorization algorithm" paragraph. If both are possible, <i>statically-sized</i> utility is preferred over
 *    <i>dynamically-sized</i>. To differentiate between PDO-ready and SDO-ready translator types the library 
 *    uses simple <i>Translator<i> name for the former and <i>Sizing Translator</i> for the later.
 * 
 * @endparblock
 * 
 * @note [1] If objects of @c <Struct> type are used only in PDO context, user is not obliged to provide such
 *   an utility. In such a case, the library will complain only if user tries to translate SDO-exchange operation
 *   using this custom translator type.
 */
namespace ethercat::common::translation { }

/* ================================================================================================================================ */

#endif
