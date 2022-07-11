/* ============================================================================================================================ *//**
 * @file       iterators.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Monday, 16th May 2022 5:58:11 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the base class for public iterator types of the Element class
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ITEARTORS_BASE_H__
#define __ETHERCAT_COMMON_ENI_COMMON_ELEMENT_ITEARTORS_BASE_H__

/* =========================================================== Includes =========================================================== */

// Boost includes
#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::eni {

/* ==================================================== Iterator implementation =================================================== */

namespace details {

    /**
     * @brief Generic class implementing bidirectional iterator referencing direct children of some Element. 
     *   Wraps iterators API of the implementation-specific type handling ENI parsing
     * 
     * @tparam root_type 
     *    type of the root node that is referenced by all elements
     * @tparam wrapped_iterator_type 
     *    type of the wrapped iterator
     * @tparam value_type 
     *    type of the iterator's value and reference, it is expected to be std::pair ( same as
     *    value_type of the @p wrapped_iterator_type ) holding std::string providing name of the
     *    element's child and element object as the second argument
     * 
     * @see https://www.boost.org/doc/libs/1_55_0/libs/iterator/doc/iterator_facade.html#tutorial-example
     */
    template<typename root_type, typename wrapped_iterator_type, typename value_type>
    class element_iterator_impl : 
        public boost::iterator_facade<element_iterator_impl<root_type, wrapped_iterator_type, value_type>, 
            /* Value type     */ value_type,
            /* Category       */ typename boost::iterator_traversal<wrapped_iterator_type>::type,
            /* Reference type */ value_type>
    {
    private: /* Private types */

        /// Auxiliary type-guidance structure
        struct enabler { };

        /// Make all specialization friendly to allow potential interoperability
        template<typename other_root_type, typename other_wrapped_iterator_type, typename other_value_type>
        friend class element_iterator_impl;

    public: /* Public ctors */

        /// Default constructor
        element_iterator_impl() { }

        /// Constructs iterator from implementation-specific
        explicit element_iterator_impl(
            std::shared_ptr<root_type> root,
            wrapped_iterator_type it
        ) : 
            root{ root },
            it{ it }
        { }

        /// Copy constructor
        template<typename other_root_type, typename other_wrapped_iterator_type, typename other_value_type>
        element_iterator_impl(
            element_iterator_impl<other_root_type, other_wrapped_iterator_type, other_value_type> const &other,
            typename boost::enable_if<
                boost::is_convertible<other_wrapped_iterator_type, wrapped_iterator_type>,
            enabler>::type = enabler()
        ) : 
            root{ other.root },
            it{ other.it }
        { }

    public: /* Public methods */

        /// Let boost acces implementation methods
        friend class boost::iterator_core_access;

        /// Compares two iterators
        template<typename other_root_type, typename other_wrapped_iterator_type, typename other_value_type>
        bool equal(element_iterator_impl<other_root_type, other_wrapped_iterator_type, other_value_type> const &other) const {
            return ((root == other.root) and (it == other.it));
        }
        
        /// Increments iterator
        void increment() { ++it; }

        /// Decrements iterator
        void decrement() { --it; }

        // Dereferences iterator
        value_type dereference() const {
            auto &ref = *it;
            return  value_type{ 
                ref.first,
                typename value_type::second_type{ root, const_cast<root_type&>(ref.second) } 
            };
        }

    private: /* Private members */

        /// Root of the property tree referenced by the referenced element
        std::shared_ptr<root_type> root;
        /// Implementation-specific iterator 
        wrapped_iterator_type it;

    };

    /**
     * @brief Dispatch of the @ref element_iterator_impl template for the specific element type
     * 
     * @tparam element_type 
     *    element type that the iterator is defined for; if needs to define following public types:
     * 
     *       * property_tree_type - type of the underlying implementation-specific representation
     *                              of the ENI tree shared by all elements
     *       * value_type         - type of the iterator's value and reference, it is expected to 
     *                              be std::pair ( same as value_type of the @p wrapped_iterator_type ) 
     *                              holding std::string providing name of the element's child and element
     *                              object as the second argument
     * 
     * @tparam wrapped_iterator_type 
     *    type of the wrapped iterator
     */
    template<typename element_type, typename wrapped_iterator_type>
    using element_iterator = 
        element_iterator_impl<
            typename element_type::property_tree_type,
            wrapped_iterator_type,
            typename element_type::value_type
        >;

    /**
     * @brief Const dispatch of the @ref element_iterator template for the specific element type
     * @see element_iterator
     */
    template<typename element_type, typename wrapped_iterator_type>
    using const_element_iterator =
        element_iterator_impl<
            typename element_type::property_tree_type,
            wrapped_iterator_type,
            const typename element_type::value_type
        >;

}

/* ================================================================================================================================ */

} // End namespace ethercat::eni

#endif
