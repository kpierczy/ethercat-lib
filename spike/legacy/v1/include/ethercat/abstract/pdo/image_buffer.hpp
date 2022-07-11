/* ============================================================================================================================ *//**
 * @file       image_buffer.hpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 11th May 2022 4:40:01 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Definition of the helper wrapper class around Process Data Image buffer poviding synchronisation
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

#ifndef __ETHERCAT_ABSTRACT_PDO_IMAGE_BUFFER_H__
#define __ETHERCAT_ABSTRACT_PDO_IMAGE_BUFFER_H__

/* =========================================================== Includes =========================================================== */

// Private includes
#include "ethercat/abstract/pdo.hpp"

/* ========================================================== Namespaces ========================================================== */

namespace ethercat::abstract::pdo {

/* ======================================================= Class definition ======================================================= */

/**
 * @brief Helper wrapper around Process Data Image buffer poviding synchronisation
 * 
 * @performance ProcessDataImage is locked one per bus I/O. It's synchronisation may 
 *    be not neccesery if we assume that Master's read/write operations are not thread
 *    safe (i.e. only one thread can manage @b cyclical bus I/O [asynchronous I/O are
 *    internally synchronized by CIFX Toolkit]). PDO-level access synchronisation is 
 *    already synchronised between Master and Slave's handlers via abstract::pdo::Entry entity
 * @performance Think about switching to boost::static_vector storage for improved cache
 *    performance.
 */
class ImageBuffer {

public: /* ----------------------------------------------------- Public types ----------------------------------------------------- */

    /**
     * @brief Helper reference class providing RAII-thread-safe access to the @ref ImageBuffer
     * 
     * @note Currently, the ImageBuffer::Refernce class is not a true RAII implementation.
     *    In particular, it does not handle copying/moving properly. This feature was ommitted
     *    as the only use case of the class is returning it's instance from the 
     *    @ref ImageBuffer::acquired() call. This, in turn, since C++17 is guarenteed to provide
     *    standard-required semantically correct copy elision ( see [1] )
     * 
     * @see [1] https://en.cppreference.com/w/cpp/language/copy_elision
     * @see [2] https://stackoverflow.com/questions/60382455/return-an-raii-object-from-a-function
     * @warning Before C++17 returning Reference object from the function call would result in calls 
     *    to constructor, copy constructor and destructor as the temproary object would be used to
     *    construct the result storage. This would, in turn, result in compilation error as move
     *    constructor is deleted. Since C++17 the same action results in a single call to the constructor
     *    and leads to single lock of the @p lock which is desired behaviour.
     */
    class Reference {

        /// Make ImageBuffer class a friend to let it access constructor
        friend class ImageBuffer;

    public: /* Deleted  methods */

        /// Disable copy-consturction
        Reference(const Reference &rref) = delete;
        /// Disable move-consturction
        Reference(Reference &&rref) = delete;
        /// Disable copy-asignment
        Reference &operator=(const Reference &rref) = delete;
        /// Disable move-asignment
        Reference &operator=(Reference &&rref) = delete;
        
    public: /* Public members */

        /**
         * @returns 
         *    refernece to the buffer's data
         */
        inline std::vector<uint8_t> &get_data();

        /**
         * @returns 
         *    const reference to the buffer's data
         */
        inline const std::vector<uint8_t> &get_data() const;

    public: /* Public dtors */
    
        /**
         * @brief Destructs the referenc object releasing referenced buffer
         */
        inline ~Reference();

    protected: /* Protected ctors */

        /**
         * @brief Construct a new Reference object refernceing given @p buffer
         */
        inline Reference(const ImageBuffer &buffer);

    protected: /* Protected members */

        /// Reference to the image's buffer
        std::vector<uint8_t> &data;
        /// Guard holding refernced image's lock
        config::Lock &lock;

    };

    /// Make Reference class a friend to let it access members
    friend class Reference;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /// Default constructor
    inline ImageBuffer() = default;

    /// Deleted copying
    ImageBuffer(const ImageBuffer& rbuf) = delete;

public: /* ----------------------------------------------------- Public ctors ----------------------------------------------------- */

    /// Deleted copying
    ImageBuffer &operator=(const ImageBuffer& rbuf) = delete;

public: /* ---------------------------------------------------- Public methods ---------------------------------------------------- */

    /**
     * @returns 
     *    reference to the image buffer
     */
    inline Reference acquire();

    /**
     * @returns 
     *    constant reference to the image buffer
     */
    inline const Reference acquire() const;

protected: /* ------------------------------------------------- Protected data ---------------------------------------------------- */

    /// Buffer
    std::vector<uint8_t> data;
    // Synchronisation lock
    config::Lock lock;

};

/* ================================================================================================================================ */

} // End namespace ethercat::abstract::pdo

/* ==================================================== Implementation includes =================================================== */

#include "ethercat/abstract/pdo/image_buffer/image_buffer.hpp"

/* ================================================================================================================================ */

#endif
