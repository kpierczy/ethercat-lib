/* ============================================================================================================================ *//**
 * @file       element.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 12:11:21 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the Element class being base for parsing interface classes of the ENI (EtherCAT Network Informations)
 *             files
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_H__

/* =========================================================== Includes =========================================================== */

// Standard includes
#include <string>
#include <filesystem>
// Boost includes
#include "boost/property_tree/ptree.hpp"
// Private includes
#include "ethercat/eni/common/error.hpp"
#include "ethercat/eni/common/element/iterators_base.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ============================================================= Class ============================================================ */

/**
 * @brief Base class for parser classes for the ENI (EtherCAT Network Informations) files
 * @details Element class provides implementation-independant interface for parsing and 
 *    traversing ENI property tree as well as proper memory management model.
 * 
 *    Element class is a reference type that abstracts-out access to the underlying parsed 
 *    ENI structure handled by some implementation-specific mechanism. As so, all elements
 *    created from the same hierarchy of Element objects share access to the same instance
 *    of the ENI tree. In result concurent modifying access to such elements is not safe.
 * 
 *    Moreover user needs to keep in mind that all implications resulting from the fact
 *    that the Element type is reference-like hold. 
 * 
 * @note As Element class is implemented as reference-like type it is recommended to pass
 *    it by value.
 * 
 * @todo At the moment the element class is implemented strictly in order to implement
 *    ENI file's parsing mechanisms. In the future it may turn out handy to implement also
 *    ENI-shaping and serialization API.
 */
class Element {

public: /* ---------------------------------------------------- Public constants -------------------------------------------------- */

    /// Childs separator used by the indirect children' paths
    static constexpr auto PATH_SEPRATOR = '.';
    /// Name of the element's child holding XML attributes
    static constexpr auto ATTRIBUTES_CHILD = "<xmlattr>";

public: /* ------------------------------------------------------ Public types ---------------------------------------------------- */

    /// Internal type used for ENI tree parsing
    using property_tree_type = boost::property_tree::ptree;

    // Basic key type used to identify element's direct children
    using key_type = property_tree_type::key_type;
    // Basic key type used to identify element's indirect children
    using path_type = property_tree_type::path_type;

    /// Element's value type (for elements iteration)
    using value_type = std::pair<const key_type, Element>;
    /// Element's size type (for elements iteration)
    using size_type = property_tree_type::size_type;

    /// Basic iterator
    class iterator;
    /// Constant iterator
    class const_iterator;
    /// Reverse iterator
    class reverse_iterator;
    /// Constant reverse iterator
    class const_reverse_iterator;

public: /* ------------------------------------------------- Public ctors & dtors ------------------------------------------------- */

    /**
     * @brief Default copy constructor
     * @details Copies element object. The resulting element refers to the same
     *    node in the ENI file as @p relem
     * 
     * @param relem 
     *    element to be copied 
     * 
     * @warning In the current implementation memory used to allocate ENI property tree is shared
     *    between all Elements produced by means of copying or @ref get_child() method
     */
    Element(const Element &relem) = default;

    /**
     * @brief Default move constructor
     * @details Moves element object. The resulting element refers to the same
     *    node in the ENI file as @p relem before call to the move-constructor
     * 
     * @param relem 
     *    element to be moved
     */
    Element(Element &&relem) = default;

public: /* --------------------------------------------------- Public operators --------------------------------------------------- */

    /// Default copy-asignment operator
    Element &operator=(const Element &relem) = default;
    /// Default move-asignment operator
    Element &operator=(Element &&relem) = default;

public: /* --------------------------------------------------- Public operators --------------------------------------------------- */

    /**
     * @brief Constructs concrete element implementation from generic element
     * @details For each class `class ElementType : <inheritance_spec> Element`
     *    inheriting from the Element and inheriting all constructors with
     *    `using Element::Element` this cast allows to automatically construct
     *    target element from result of e.g. get_child(...) method.
     * 
     * @tparam ElementType 
     *    target element type
     * @returns 
     *    target element constructed from the generic element
     * 
     * @attention This is a key method that allows seamless usage of all utility 
     *    methods provided by Element class in methods of derived class creating
     *    other Element-based elements objects. Trying to construct ElementType
     *    explicitly from Element will not work even it ElementType inherited 
     *    constructors as copy-constructor is not taken into account into inherited
     *    constructors' resolution. On the contrary, trying to construct ElementType
     *    from Element implictly will trigger this cast operator that will make use
     *    if Element class's private constructor and so compiller will run smoothly.
     *    As a result ElementType class needs not to declare Element class as a friend
     *    to let it access potentially protected/private constructor.
     * 
     * @todo When C++20 is available change std::enable_if to require
     */
    template<typename ElementType, std::enable_if_t<std::is_base_of_v<Element, ElementType>, bool> = true>
    inline operator ElementType() const;

public: /* ----------------------------------------------- Public STL-like methods ------------------------------------------------ */

    /// @returns number of direct children of the element
    inline size_type size() const;
    /// @retval true if element has no direct children @retval false otherwise
    inline bool empty() const;

    /// @returns iterator to the first direct child of the element
    inline iterator begin();
    /// @returns constant iterator to the first direct child of the element
    inline const_iterator begin() const;
    /// @returns iterator to the last direct child of the element
    inline iterator end();
    /// @returns constant iterator to the last direct child of the element
    inline const_iterator end() const;

    /// @returns reverse iterator to the last direct child of the element
    inline reverse_iterator rbegin();
    /// @returns reverse constant iterator to the last direct child of the element
    inline const_reverse_iterator rbegin() const;
    /// @returns reverse iterator to the first direct child of the element
    inline reverse_iterator rend();
    /// @returns reverse constant iterator to the first direct child of the element
    inline const_reverse_iterator rend() const;

    /// @returns reference to the first direct child of the element
    inline value_type front();
    /// @returns const reference to the first direct child of the element
    inline const value_type front() const;
    /// @returns reference to the last direct child of the element
    inline value_type back();
    /// @returns const reference to the last direct child of the element
    inline const value_type back() const;

public: /* ---------------------------------------------- Public management methods ----------------------------------------------- */

    /**
     * @brief Disconnects element from the currently referenced tree by clonning content
     *    of the referenced element's subtree to the independant memory buffer.
     * @details As described in the header of the class, Element is a reference-like type.
     *    All Element objects created from the common source ( e.g. by recursive calls to
     *    @ref get_child() ) share reference to the common memory buffer storing the whole
     *    ENI tree that contains particular element pointer by the object. This is usually
     *    desired behaviour as it saves a lot of memory when a lot of elemens' references
     *    are in used.
     * 
     *    However, in some scenarious this may lead to increased memory usage. In particular,
     *    it covers scenario when only single element of the whole ENI tree is needed to be
     *    stored in the long-term. Normally it would require storing also the rest of the 
     *    parsed ENI.
     * 
     *    autonomize() creates copy of the referenced ENI tree containing only element directly
     *    referenced by the object and it's children. If object was the only one to point the
     *    main ENI tree, it's resourced will be freed.
     */
    inline void autonomize();

public: /* ----------------------------------------------- Public searching methods ----------------------------------------------- */

    /**
     * @brief Count the number of direct children with the given key
     * 
     * @param key 
     *    key of the target child
     * @returns 
     *    number of direct children with the given key
     */
    inline size_type count(const key_type &key) const;
    
    /**
     * @param child 
     *    identifier of the child to be found
     * 
     * @retval true 
     *    if element has direct child named @p child 
     * @retval false 
     *    otherwise
     */
    inline bool has_child(const key_type &child) const;

    /** 
     * @brief Finds and returns child element at the given path
     * 
     * @param path 
     *    path to the target child
     * @returns 
     *    requested child element
     * @throws Error 
     *    if @p path does not identify element's child
     * 
     * @note Depending on the path, the result at each level may not be completely deterministic, 
     *    i.e. if the same key appears multiple times, which child is chosen is not specified. 
     *    This can lead to the path not being resolved even though there is a descendant with this
     *    path.
     * 
     * @par Example
     * @code
     *   a -> b -> c
     *     -> b
     * @endcode
     *    The path "a.b.c" will succeed if the resolution of "b" chooses
     *    the first such node, but fail if it chooses the second.
     */
    inline Element get_child(const path_type &path);

    /**
     * @brief Constant version of @ref get_child
     * @overload Element get_child(const path_type &path)
     */
    inline const Element get_child(const path_type &path) const;

    /** 
     * @brief Finds and returns child element at the given path
     * 
     * @param path 
     *    path to the target child
     * @param default_element 
     *    default element to be returned if @p path does not refers to a valid child
     * 
     * @retval child
     *    requested child element if @p path refers to a valid child
     * @retval default_element
     *    otherwise
     */
    inline Element get_child_or(const path_type &path, Element default_element);

    /**
     * @brief Constant version of @ref get_child_or
     * @overload get_child_or(const path_type &path, Element default_element)
     */
    inline const Element get_child_or(const path_type &path, const Element default_element) const;

    /** 
     * @brief Finds and returns child element at the given path
     * 
     * @param path 
     *    path to the target child
     * 
     * @retval child
     *    requested child element if @p path refers to a valid child
     * @retval empty
     *    optional otherwise
     */
    inline std::optional<Element> get_child_or_empty(const path_type &path);

    /**
     * @brief Constant version of @ref get_child_or
     * @overload get_child_or_empty(const path_type &path)
     */
    inline std::optional<const Element> get_child_or_empty(const path_type &path) const;

public: /* ------------------------------------------------- Public access methods ------------------------------------------------ */

    /** 
     * @brief Take the value of this node and attempt to translate it to a @c Type object 
     * 
     * @tparam Type 
     *    target type that the value should be casted into
     * @returns 
     *    value of the element casted to @p Type
     * 
     * @throws Error
     *    if the conversion fails
     */
    template<class Type>
    inline Type get_value() const;

    /** 
     * @brief Take the value of this node and attempt to translate it to a @c Type object 
     * 
     * @tparam Type 
     *    target type that the value should be casted into
     * @param default_value
     *    default value to be returned if element has no value
     * 
     * @retval value 
     *    of the element casted to @p Type
     * @retval default_value
     *    if element has no value
     */
    template<class Type>
    inline Type get_value_or(Type default_value) const;

    /** 
     * @brief Take the value of this node and attempt to translate it to a @c Type object 
     * 
     * @tparam Type 
     *    target type that the value should be casted into
     * 
     * @retval value 
     *    of the element casted to @p Type
     * @retval empty
     *    optional if element has no value
     */
    template<class Type>
    inline std::optional<Type> get_value_or_empty() const;

    /** 
     * @brief Take the value of the child node identified by @p path and attempt to 
     *    translate it to a @c Type object 
     * 
     * @tparam Type 
     *    target type that the value should be casted into
     * @param path 
     *    path to the target child
     * @returns 
     *    value of the element casted to @p Type
     * 
     * @throws Error
     *    if the conversion fails
     */
    template<class Type>
    inline Type get_child_value(const path_type &path) const;

    /** 
     * @brief Take the value of child node identified by @p path and attempt to translate it 
     *    to a @c Type object 
     * 
     * @tparam Type 
     *    target type that the value should be casted into
     * @param path 
     *    path to the target child
     * @param default_value
     *    default value to be returned if element has no value
     * 
     * @retval value 
     *    of the element casted to @p Type
     * @retval default_value
     *    if element has no value
     */
    template<class Type>
    inline Type get_child_value_or(const path_type &path, Type default_value) const;

    /** 
     * @brief Take the value of child node identified by @p path and attempt to translate it 
     *    to a @c Type object 
     * 
     * @tparam Type 
     *    target type that the value should be casted into
     * @param path 
     *    path to the target child
     * 
     * @retval value 
     *    of the element casted to @p Type
     * @retval empty
     *    optional if element has no value
     */
    template<class Type>
    inline std::optional<Type> get_child_value_or_empty(const path_type &path) const;

private: /* --------------------------------------------------- Private friends --------------------------------------------------- */

    /// Make iterator base class a friend to let it access private constructors
    template<typename, typename, typename>
    friend class details::element_iterator_impl;

    /// Friend function loading Element from ENI file at @p path
    friend Element element_from_file(const std::filesystem::path &path);
    /// Friend function loading Element from ENI file loaded to @p eni string
    friend Element element_from_string(const std::string &eni);
    /// Friend function loading Element from ENI file read from @p stream
    friend Element element_from_stream(std::basic_istream<char> &stream);

private: /* ------------------------------------------------ Private ctors & dtors ------------------------------------------------ */

    /**
     * @brief Construct a new ENI (root) Element from the implementation-specific structure 
     * @details This constructor cannot be called directly by the user code. Instead, user
     *    shall load ENI by means of from_*() methods
     * 
     * @param root 
     *    source ENI (XML) tree to construct root element from
     */
    inline Element(std::shared_ptr<property_tree_type> root);

    /**
     * @brief Construct a new ENI Element referencing the given @p node in the @p root tree
     * 
     * @param root 
     *    source ENI (XML) tree referenced by the element
     * @param node 
     *    node referenced by the element
     */
    inline Element(std::shared_ptr<property_tree_type> root, property_tree_type &node);

private: /* ---------------------------------------------------- Private methods -------------------------------------------------- */

    /// @returns reference to the wrapped node
    inline property_tree_type &get_node();

    /// @returns constant reference to the wrapped node
    inline const property_tree_type &get_node() const;

    /// @returns element referencing given @p subnode in the same @a root tree
    inline Element make_subelement(property_tree_type &subnode) const;

private: /* ----------------------------------------------------- Private data ---------------------------------------------------- */

    /// Shared pointer to the whole XML tree buffer ( held to assert validity of the @a node reference )
    std::shared_ptr<property_tree_type> root;
    /// Reference to the XML node of the ENI file parsed by the class
    property_tree_type *node { nullptr };
    
};

/* ==================================================== Auxiliary I/O functions =================================================== */

/**
 * @brief Loads ENI property tree from the file located at @p path
 *
 * @param path
 *    path to the ENI file to be parsed
 * @returns 
 *    target element loaded from XML tree
 * 
 * @throws eni::Error
 *    if ENI file could not be loaded
 */
inline Element element_from_file(const std::filesystem::path &path);

/**
 * @brief Loads ENI property tree read into the runtime string
 *
 * @param eni
 *    string object containing ENI description
 * @returns 
 *    target element loaded from XML tree
 * 
 * @throws eni::Error
 *    if ENI file could not be loaded
 */
inline Element element_from_string(const std::string &eni);

/**
 * @brief Loads ENI property from tree the given input @p stream
 *
 * @param stream
 *    stream to load ENI from
 * @returns 
 *    target element loaded from XML tree
 * 
 * @throws eni::Error
 *    if ENI file could not be loaded
 */
inline Element element_from_stream(std::basic_istream<char> &stream);

/* ================================================================================================================================ */

} // End namespace ethercat::eni

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/eni/common/element/iterators.hpp"
#include "ethercat/eni/common/element/element_priv.hpp"
#include "ethercat/eni/common/element/element.hpp"
#include "ethercat/eni/common/element/management.hpp"
#include "ethercat/eni/common/element/loaders.hpp"
#include "ethercat/eni/common/element/stl.hpp"
#include "ethercat/eni/common/element/searching.hpp"
#include "ethercat/eni/common/element/access.hpp"

/* ================================================================================================================================ */

#endif
