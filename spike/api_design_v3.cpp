/* ============================================================================================================================ *//**
 * @file       api_design_v2.cpp
 * @author     Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @maintainer Krzysztof Pierczyk (krzysztof.pierczyk@gmail.com)
 * @date       Wednesday, 4th May 2022 5:24:48 pm
 * @modified   Monday, 11th July 2022 2:07:23 pm
 * @project    ethercat-lib
 * @brief      Helper pseudo-cpp file containing proposal of API structure of the ethercat driver
 * 
 * 
 * @copyright Krzysztof Pierczyk © 2022
 */// ============================================================================================================================= */

/* =========================================================== Includes =========================================================== */

#include <atomic>
#include <chrono>
#include <filesystem>
#include <array>
#include <iostream>
#include <variant>
#include <optional>
#include <type_traits>
#include <span>
#include <range/v3/span.hpp>
#include <functional>

/* ========================================================= Declarations ========================================================= */

// Forward declare Channel class
namespace cifx { class Channel; }

/* ========================================================== Namespaces ========================================================== */

using namespace std::chrono_literals;

/* =========================================================== Utilities ========================================================== */

namespace ethercat::utils {

    template<typename Enum>
    constexpr std::underlying_type_t<Enum> to_underlying(Enum e) {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }

    template<typename Enum>
    constexpr Enum to_enum(std::underlying_type_t<Enum> val) {
        return static_cast<Enum>(val);
    }

    // Helper type for the visitor
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    // Explicit deduction guide (not needed as of C++20)
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
        
} // End namespace ethercat::utils

/* ============================================================ Traits ============================================================ */

namespace ethercat::traits {

    namespace details {

        struct void_t { using type = void; };

        template<typename T> 
        struct function_traits;  

        template<typename R, typename ...Args> 
        struct function_traits<std::function<R(Args...)>>
        {
            static const size_t nargs = sizeof...(Args);

            typedef R result_type;

            template <size_t i>
            struct arg
            {
                typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
            };

            template <size_t i>
            struct arg_or_void
            {
                typedef std::conditional<
                    i < sizeof...(Args),
                    std::tuple_element<i, std::tuple<Args...>>,
                    void_t
                >::type::type type;
            };

        };

    } // End namespace details

    /**
     * @brief Helper traits used to obtain types related to the specialization of
     *    the @ref std::function template
     */
    template<typename T> 
    struct function_traits : public details::function_traits<T> { };

    // Alias for argument type of callable
    template<typename T, std::size_t i> 
    using arg_t = function_traits<T>::arg<i>:type;

    // Alias for argument type of callable
    template<typename T> 
    using result_type_t = function_traits<T>::result_type:type;

    namespace details {

        template<class T>
        struct remove_cvref {
            typedef std::remove_cv_t<std::remove_reference_t<T>> type;
        };

    } // End namespace details

    /**
     * @brief Helper trait removing cv and reference attribtues from the type @tparam T
     * 
     * @tparam T 
     *   type to be modified
     */
    template<class T>
    struct remove_cvref : public details::remove_cvref<T> { };

    /// Helper alias for remove_cvref<T>::type
    template<class T>
    using remove_cvref_t = typename remove_cvref<T>::type;

    namespace details {

        template< std::size_t N, typename T, typename Tuple>
        constexpr std::size_t get_index_of() {

            static_assert(N < std::tuple_size<Tuple>::value, "[get_index_of] The element is not in the tuple");

            if constexpr(std::is_same<T, typename std::tuple_element<N, Tuple>::type>::value )
                return N;
            else
                return get_index_of<N + 1, T, Tuple>();
        }

        template<typename T, typename Tuple>
        struct index_of {
            static constexpr size_t value = get_index_of<0, T, Tuple>();
        };

    }

    /**
     * @brief Helper trait defining @a value field storing index of the given @tparam T type
     *   in the list of types of the @p Tuple specialization of std::tuple
     * 
     * @tparam T 
     *    type to be found
     * @tparam Tuple 
     *    tuple defining list of types to search
     */
    template<typename T, typename Tuple>
    struct index_of : 
        public details::index_of<T, Tuple> 
    { };

    template<typename T, typename Tuple>
    constexpr std::size_t index_of_v = index_of<T, Tuple>::value;

} // End namespace ethercat::traits

/* ============================================================= Locks ============================================================ */

namespace ethercat::locks {

    /**
     * @brief Auxiliary class implementing atomic-based spinlock
     * 
     */
    class SpinLock {
    private:

        // State of the lock
        enum class State : bool { 
            Locked,
            Unlocked
        };

    public:

        /// Constructs the spinlock
        SpinLock() : state(State::Unlocked) {}

        /// Locks the spinlock
        void lock() { while (state.exchange(State::Locked, std::memory_order_acquire) == State::Locked) {} }
        
        /// Unlocks the spinlock
        void unlock() { state.store(State::Unlocked, std::memory_order_release); }

    private:

        // State of the lock
        std::atomic<State> state;
        
    };

} // End namespace ethercat::locks

/* ========================================================= Configruation ======================================================== */

namespace ethercat {

    // Synchronisation method
    using Lock = std::mutex;
    
    // Synchronisation method for short accesses
    using QuickLock = locks::SpinLock;

} // End namespace ethercat

/* ====================================================== Objects dictionary ====================================================== */

namespace ethercat::abstract::objects {

    /**
     * @brief Union type used to store PDO value
     */
    using ValueStorage = std::variant<
        /* DataType::Bool                                     [BOOL]        */ bool,
        /* DataType::Byte / DataType::UnsignedShortInt        [BYTE/USINT]  */ uint8_t,
        /* DataType::Word / DataType::UnsignedInt             [WORD/UINT]   */ uint16_t,
        /* DataType::DoubleWord / DataType::UnsignedDoubleInt [DWORD/UDINT] */ uint32_t,
        /* DataType::UnsignedLongInt                          [ULINT]       */ uint64_t,
        /* DataType::ShortInt                                 [SINT]        */ int8_t,
        /* DataType::Int                                      [INT]         */ int16_t,
        /* DataType::DoubleInt                                [DINT]        */ int32_t,
        /* DataType::LongInt                                  [LINT]        */ int64_t,
        /* DataType::Real                                     [REAL]        */ float,
        /* DataType::LongReal                                 [LREAL]       */ double
    >;

    /**
     * @brief Enumeration of supported PDO data types
     */
    enum class DataType : std::size_t {
        /* 8-bit boolean              [BOOL]  */ Bool              = traits::index_of_v<bool,     ValueStorage>,
        /* 8-bit bitset               [BYTE]  */ Byte              = traits::index_of_v<uint8_t,  ValueStorage>,
        /* 16-bit bitset              [WORD]  */ Word              = traits::index_of_v<uint16_t, ValueStorage>,
        /* 32-bit bitset              [DWORD] */ DoubleWord        = traits::index_of_v<uint32_t, ValueStorage>,
        /* 8-bit signed int           [SINT]  */ ShortInt          = traits::index_of_v<int8_t,   ValueStorage>,
        /* 8-bit unsigned signed int  [USINT] */ UnsignedShortInt  = traits::index_of_v<uint8_t,  ValueStorage>,
        /* 16-bit signed int          [INT]   */ Int               = traits::index_of_v<int16_t,  ValueStorage>,
        /* 16-bit unsigned signed int [UINT]  */ UnsignedInt       = traits::index_of_v<uint16_t, ValueStorage>,
        /* 32-bit signed int          [DINT]  */ DoubleInt         = traits::index_of_v<int32_t,  ValueStorage>,
        /* 32-bit unsigned signed int [UDINT] */ UnsignedDoubleInt = traits::index_of_v<uint32_t, ValueStorage>,
        /* 64-bit signed int          [LINT]  */ LongInt           = traits::index_of_v<int64_t,  ValueStorage>,
        /* 64-bit unsigned signed int [ULINT] */ UnsignedLongInt   = traits::index_of_v<uint64_t, ValueStorage>,
        /* 32-bit floating point      [REAL]  */ Real              = traits::index_of_v<float,    ValueStorage>,
        /* 64-bit floating point      [LREAL] */ LongReal          = traits::index_of_v<double,   ValueStorage>
    };

    /**
     * @brief Helper meta-function converting @ref DataType enumeration into corresponding data type
     */
    template<DataType type>
    using ValueType = std::variant_alternative_t<utils::to_underlying(type), ValueStorage>;

    /**
     * @brief Helper meta-function converting @ref DataType enumeration into index of the corresponding 
     *    @ref Value alternative
     */
    template<DataType type>
    constexpr std::size_t ValueIndex = utils::to_underlying(type);

    /// Helper getters for object value storage
    template<DataType type>
    inline ValueType<type> &get(ValueStorage &value_storage);
    template<DataType type>
    inline const ValueType<type> &get(const ValueStorage &value_storage);

    /**
     * @brief Structure storing value of the PDO
     */
    class Value {
    public:

        // Default constructor
        Value() = default;
        // Default value constructor (by data type)
        template<DataType type>
        Value(ValueType<type> value);
        // Default value constructor (by value type)
        template<typename T>
        Value(T value);

        /// Assigns new @p value to the object
        template<typename T>
        Value &operator=(T value);

    public:

        /// Accesses value of type associated with @p type
        template<DataType type>
        ValueType<type> &as();
        /// Accesses value of type associated with @p type (const version)
        template<DataType type>
        const ValueType<type> &as() const;

        /// Accesses variant of the value of type @tparam T
        template<typename T>
        T &as();
        /// Accesses variant of the value of type @tparam T (const version)
        template<typename T>
        const T &as() const;
        
    private:

        // Underlying storage
        ValueStorage value;

    };

    /**
     * @brief Helper buffer class providing synchronsied access to the value
     */
    class ValueBuffer { 
    public:
        
        // Default constructor
        ValueBuffer() = default;
        // Default value constructor (by data type)
        template<DataType type>
        ValueBuffer(ValueType<type> value);
        // Default value constructor (by value type)
        template<typename T>
        ValueBuffer(T value);
        
    public:

        /// Reads buffer content as a value of type associated with @p type
        template<DataType type>
        inline ValueType<type> get_as() const;
        /// Reads buffer content
        inline const Value get() const;

        /// Writes buffer content as a value of type associated with @p type
        template<DataType type>
        inline void set_as(ValueType<type> value);
        /// Writes buffer content
        inline void set(Value value);

    protected:

        /// Make ValueBufferReference friend class to let it access referenced data
        friend class ValueBufferReference;

        // Buffer for object accesses
        Value value;
        // Synchronisation lock for the buffer
        QuickLock lock;

    };

    /**
     * @brief Helper refernce class providing type-precise, synchronised access to the value stored in the
     *    @ref ValueBuffer object
     */
    template<DataType type>
    class ValueBufferReference { 
    public:
        
        // Default constructor
        ValueBufferReference(ValueBuffer &buffer);
        
    public:

        /// Reads referenced value
        inline const ValueType<type> get() const;
        /// Writes referenced value
        inline void set(ValueType<type> value);


    protected:

        // Handler to the referenced buffer
        ValueBuffer &buffer;

    };

    /**
     * @brief Helper proxy class providing automated parsing of raw object data
     */
    class Data { 
    public:
        
        /**
         * @brief Default constuctor. Throws error if @p data buffer has not enough storage 
         *    for object of the given @p type
         */
        Data(DataType type, std::span<uint8_t> data);
        
    public:

        /// Returns type of the object
        DataType get_type();
        
    public:

        /// Reads raw bytes as a value of the given @p type ; throws if type does not match
        template<DataType type>
        ValueType<type> read_as();
        /// Reads raw bytes as a value
        Value read();

        /// Reads raw bytes as a value of the given @p type ; throws if type does not match
        template<DataType type>
        void write_as(ValueType<type> value);
        /// Reads raw bytes as a value
        void write(Value value);

    protected:

        /// Reference to the data buffer
        uint8_t *data;
        /// Actual data type
        DataType type;

    };


    /**
     * @brief Helper class combining @ref ValueBuffer and @ref Data functionalities in the single object 
     */
    class BufferedData { 
    public:

        /**
         * @brief Default constuctor. Throws error if @p data buffer has not enough storage 
         *    for object of the given @p type
         */
        BufferedData(DataType type, std::span<uint8_t> data);    
        
    public: /* Common */

        /// Returns type of the object
        DataType get_type();
        
    public: /* Data access */

        /// Reads raw bytes to the buffer
        void buffer();
        /// Writes raw bytes from the buffer
        void flush();

        /// Reads raw bytes to the buffer and returns read value as @p type
        template<DataType type>
        ValueType<type> read_as();
        /// Reads raw bytes to the buffer and returns read value
        Value read();
        /// Writes raw bytes from the buffer and returns read value as @p type
        template<DataType type>
        void write_as(ValueType<type> value);
        /// Writes raw bytes from the buffer and returns read value
        void write(Value value);

    public: /* Buffer access */

        /// Reads buffer content as a value of type associated with @p type
        template<DataType type>
        inline ValueType<type> get_as() const;
        /// Reads buffer content
        inline const Value get() const;

        /// Writes buffer content as a value of type associated with @p type
        template<DataType type>
        inline void set_as(ValueType<type> value);
        /// Writes buffer content
        inline void set(Value value);

        /// Returns reference to the buffer
        template<DataType type>
        ValueBufferReference<type> get_reference();

    protected:

        /// Raw-data parser
        Data data;
        /// Value buffer
        ValueBuffer buffer;

    };

} // End namespace ethercat::abstract::objects

/* ============================================================== PDO ============================================================= */

namespace ethercat::abstract::pdo {

    /// PDO direction
    enum class Direction {
        
        /// Input PDOs (sent from Slave to Master)
        Input = 0,
        Tx = 0,

        /// Output PDOs (sent from Master to Slave)
        Output = 1,
        Rx = 1,

    };

    /**
     * @brief Helper wrapper around Process Data Image buffer poviding synchronisation
     * @performance ProcessDataImage is locked one per bus I/O. It's synchronisation may 
     *    be not neccesery if we assume that Master's read/write operations are not thread
     *    safe (i.e. only one thread can manage @b cyclical bus I/O [asynchronous I/O are
     *    internally synchronized by CIFX Toolkit]). PDO-level access synchronisation is 
     *    already synchronised between Master and Slave's handlers via abstract::pdo::Entry entity
     */
    class ImageBuffer {
    public:

        /// Buffer
        std::vector<uint8_t> data;

        /// Locks buffer
        inline void lock();
        /// Unlocks buffer
        inline void unlock();
        /// Returns reference to the buffer's lock
        inline Lock &get_lock();

    private:

        // Synchronisation lock
        Lock lock;

    };

    /**
     * @brief Helper type storing an arbitrary callable handling PDO value
     */
    using InputHandlerStorage = std::variant<
        /* DataType::Bool                                     [BOOL]        */  std::function<void(std::variant_alternative_t<0,  abstract::objects::ValueStorage>)>,
        /* DataType::Byte / DataType::UnsignedShortInt        [BYTE/USINT]  */  std::function<void(std::variant_alternative_t<1,  abstract::objects::ValueStorage>)>,
        /* DataType::Word / DataType::UnsignedInt             [WORD/UINT]   */  std::function<void(std::variant_alternative_t<2,  abstract::objects::ValueStorage>)>,
        /* DataType::DoubleWord / DataType::UnsignedDoubleInt [DWORD/UDINT] */  std::function<void(std::variant_alternative_t<3,  abstract::objects::ValueStorage>)>,
        /* DataType::UnsignedLongInt                          [ULINT]       */  std::function<void(std::variant_alternative_t<4,  abstract::objects::ValueStorage>)>,
        /* DataType::ShortInt                                 [SINT]        */  std::function<void(std::variant_alternative_t<5,  abstract::objects::ValueStorage>)>,
        /* DataType::Int                                      [INT]         */  std::function<void(std::variant_alternative_t<6,  abstract::objects::ValueStorage>)>,
        /* DataType::DoubleInt                                [DINT]        */  std::function<void(std::variant_alternative_t<7,  abstract::objects::ValueStorage>)>,
        /* DataType::LongInt                                  [LINT]        */  std::function<void(std::variant_alternative_t<8,  abstract::objects::ValueStorage>)>,
        /* DataType::Real                                     [REAL]        */  std::function<void(std::variant_alternative_t<9,  abstract::objects::ValueStorage>)>,
        /* DataType::LongReal                                 [LREAL]       */  std::function<void(std::variant_alternative_t<10, abstract::objects::ValueStorage>)>
    >;

    /**
     * @brief Helper meta-function converting @ref DataType enumeration into corresponding data handler type
     */
    template<abstract::objects::DataType type>
    using InputHandlerType = std::variant_alternative_t<utils::to_underlying(type), InputHandlerStorage>;

    /// Helper getters for object value storage
    template<abstract::objects::DataType type>
    inline InputHandlerType<type> &get(InputHandlerStorage &handler_storage);
    template<abstract::objects::DataType type>
    inline const InputHandlerType<type> &get(const InputHandlerStorage &handler_storage);
    
    /**
     * @brief Helper type storing an arbitrary callable handling PDO value
     */
    using OutputHandlerStorage = std::variant<
        /* DataType::Bool                                     [BOOL]        */  std::function<std::variant_alternative_t<0,  abstract::objects::ValueStorage>(void)>,
        /* DataType::Byte / DataType::UnsignedShortInt        [BYTE/USINT]  */  std::function<std::variant_alternative_t<1,  abstract::objects::ValueStorage>(void)>,
        /* DataType::Word / DataType::UnsignedInt             [WORD/UINT]   */  std::function<std::variant_alternative_t<2,  abstract::objects::ValueStorage>(void)>,
        /* DataType::DoubleWord / DataType::UnsignedDoubleInt [DWORD/UDINT] */  std::function<std::variant_alternative_t<3,  abstract::objects::ValueStorage>(void)>,
        /* DataType::UnsignedLongInt                          [ULINT]       */  std::function<std::variant_alternative_t<4,  abstract::objects::ValueStorage>(void)>,
        /* DataType::ShortInt                                 [SINT]        */  std::function<std::variant_alternative_t<5,  abstract::objects::ValueStorage>(void)>,
        /* DataType::Int                                      [INT]         */  std::function<std::variant_alternative_t<6,  abstract::objects::ValueStorage>(void)>,
        /* DataType::DoubleInt                                [DINT]        */  std::function<std::variant_alternative_t<7,  abstract::objects::ValueStorage>(void)>,
        /* DataType::LongInt                                  [LINT]        */  std::function<std::variant_alternative_t<8,  abstract::objects::ValueStorage>(void)>,
        /* DataType::Real                                     [REAL]        */  std::function<std::variant_alternative_t<9,  abstract::objects::ValueStorage>(void)>,
        /* DataType::LongReal                                 [LREAL]       */  std::function<std::variant_alternative_t<10, abstract::objects::ValueStorage>(void)>
    >;

    /**
     * @brief Helper meta-function converting @ref DataType enumeration into corresponding data handler type
     */
    template<abstract::objects::DataType type>
    using OutputHandlerType = std::variant_alternative_t<utils::to_underlying(type), OutputHandlerStorage>;

    /// Helper getters for object value storage
    template<abstract::objects::DataType type>
    inline OutputHandlerType<type> &get(OutputHandlerStorage &handler_storage);
    template<abstract::objects::DataType type>
    inline const OutputHandlerType<type> &get(const OutputHandlerStorage &handler_storage);
    
    namespace details {

        template<Direction dir, abstract::objects::DataType type = abstract::objects::DataType::Bool>
        struct HandlerTraits : public std::false_type { };

        template<abstract::objects::DataType type>
        struct HandlerTraits<Direction::Input, type> : public std::true_type {
            using type = InputHandlerType<type>;
            using storage = InputHandlerStorage;
        };

        template<abstract::objects::DataType type>
        struct HandlerTraits<Direction::Input, type> : public std::true_type {
            using type = OutputHandlerType<type>;
            using storage = OutputHandlerStorage;
        };

    }

    /**
     * @brief Helper meta-function converting @ref DataType enumeration into corresponding data handler type
     */
    template<Direction dir, abstract::objects::DataType type>
    using HandlerType = details::HandlerTraits<dir, type>::type;
    template<Direction dir>
    using HandlerStorage = details::HandlerTraits<dir>::storage;

    /**
     * @brief Helper wrapper around @ref HandlerStorage
     */
    template<Direction dir>
    class Handler {
    public:

        // Storage type
        using Storage = HandlerStorage<dir>;

    public:

        /// Constructs handler associated with the given PDO @p type
        template<abstract::objects::DataType type>
        Handler();

        /// Constructs handler associated with the given PDO @p type as @p handler
        template<abstract::objects::DataType type>
        Handler(HandlerType<dir, type> handler);

    public:

        /// Calls handler by @p type 
        template<abstract::objects::DataType type, typename ... Args>
        inline traits::result_type_t<HandlerType<dir, type>> operator()(Args... args);

    public:

        /// Accesses handler by @p type 
        template<abstract::objects::DataType type>
        inline HandlerType<dir, type> &get();

        /// Accesses handler storage
        inline HandlerStorage<dir> &get();

        /// Checks whether object stores handle for type @p type that is empty
        template<abstract::objects::DataType type>
        inline bool is_empty();
        /// Checks whether object stores handle for type @p type that is empty
        inline bool is_empty(abstract::objects::DataType type);

    private:

        // Handler storage
        HandlerStorage<dir> handler;

    };
    
    /// Helper meta-function obtaining argument type of the handler storage
    template<typename T>
    using handler_storage_arg_t = typename traits::arg_t<remove_cvref_t<typename T>, 0>;

    /// Helper meta-function obtaining result type of the handler storage
    template<typename T>
    using handler_storage_result_t = typename traits::result_type_t<remove_cvref_t<typename T>>;
    
    /// Helper meta-function obtaining argument type of the handler
    template<typename T>
    using handler_arg_t = handler_storage_arg_t<typename T::Storage>;

    /// Helper meta-function obtaining result type of the handler
    template<typename T>
    using handler_result_t = handler_storage_result_t<typename T::Storage>;

    /**
     * @brief Helper class representing named PDO entry with synchronised buffer and update handler
     * 
     * @tparam dir 
     *    PDO direction
     * 
     * @performance Entry class ( @ref update() method ) is utilized on the critical path of the 
     *    bus I/O loop. It's implementation is fully thread-safe and provides unbound access to the
     *    PDO value from both sides (bus Master and Host app) due to 3-bufferred implementation.
     *    As a consequence, execution performance suffers from 
     *    
     *      - additional data copying in both Input and Output direction; it especially
     *        influence Output direction as output data are copied at every bus Write 
     *        and not only when Host updates output value
     *      - additional locking synchronising the buffer (this is probably small drawback
     *        as for such short critical sections spinlocks can be used)
     */
    template<abstract::pdo::Direction dir>
    class Entry {
    public:

        /// Constructs new PDO entry of type @p type refereing to data in @p data buffer
        Entry(
            std::string_view name,
            abstract::objects::DataType type, 
            std::span<uint8_t> data
        );

        /// Returns name of the PDO entry
        std::string_view get_name() const;

        /// Returns reference to the entrie's buffer
        template<abstract::objects::DataType type>
        inline abstract::objects::ValueBufferReference<type> get_reference() const;

        /**
         * @brief Updates internal entry buffer from raw data. Calls handler, if registered
         */
        void update();

        /// Registers PDO handler
        template<abstract::objects::DataType type>
        inline void register_pdo_handler(std::string_view name, abstract::pdo::HandlerType<dir, type> handler);
        /// Unregisters PDO handler
        template<abstract::objects::DataType type>
        inline void unregister_pdo_handler(std::string_view name);

    private:

        /// Name of the PDO Entry in the ENI file
        const std::string name;
        // Bufferred reference to object's data in the Process Data Image
        abstract::objects::BufferedData data;
        /// Optional handler called at PDO update
        abstract::pdo::Handler<dir> handler;
        
    };

} // End namespace ethercat::abstract::pdo

/* ============================================================== Eni ============================================================= */

namespace ethercat::eni {

    /// Lazy XML parser of the ENI configuration
    class Configuration { 
    public:

        /// Constructs parser from the @p eni_path file
        Configuration(std::filesystem::path eni_path);

        /// Constructs parser from the @p eni string
        Configuration(std::string eni);

    };
    
    /// Subparser for <Master> part
    class Master { };
    /// Subparser for <Slave> and related <ProcessImage> part
    class Slave { };
    /// Subparser for <Cyclic> part
    class Cyclic { };
    /// Subparser for <ProcessImage> part
    class ProcessImage { };

} // End namespace ethercat::eni

/* ============================================================== Sdo ============================================================= */

namespace ethercat::abstract::sdo {

    /**
     * @brief Request structure for SDO access
     * @see doc/'Protocol API EtherCAT Master V4 V4.5.0', p. 74
     */
    struct Request {

        /// Current operation mode of the bus (decides which addressing mode should be used in request)
        enum class OperationMode {
            BusScan,
            Normal
        };

        /// Current operation mode
        OperationMode operation_mode;
        /// Index of the object
        uint16_t index;
        /// Subindex of the object
        uint16_t subindex;

        /// Flag indicating whether full access should be made
        bool full_access;

    };

} // End namespace ethercat::abstract::sdo

/* ============================================================= Slave ============================================================ */

namespace ethercat {

    // Foward declare Master
    class Master;

    /**
     * @brief Class representing a single slave on the EtherCAt bus
     */
    class Slave {
    public:

        /// Slave's state (in EtherCAT State Machine [ECM])
        enum class State {
            Init,
            Preop,
            Boot,
            Safeop,
            Op
        };

    public:

        /// Reads state of the slave
        State get_state(std::chrono::milliseconds timeout = 100ms);
        /// Requestes state change of the slave
        void set_state(State state, std::chrono::milliseconds timeout = 100ms);

        /// Returns thread-safe reference to the given PDO
        template<abstract::pdo::Direction dir, abstract::objects::DataType type>
        abstract::objects::ValueBufferReference<type> get_pdo_reference(std::string_view name);

        /**
         * @brief Sets update-handler for the given PDO. For input PDOs (Tx direction) handler is called
         *    each time AFTER Master reads PDO from the bus. Read value is passes as an argument. For output
         *    PDOs (Rx direction) handler is called each time BEFORE Master writes PDOs to the bus. Return
         *    value of the handler is treated as a new value for the PDO.
         * 
         * @tparam dir 
         *    PDO's direction
         * @tparam type 
         *    PDO type
         * @param name 
         *    PDO name 
         * @param handler 
         *    handler to be set
         * @returns 
         *    reference proxy to the requested PDO
         * 
         * @throws 
         *    when @p type does not match actual type of the PDO named @p name
         */
        template<abstract::pdo::Direction dir, abstract::objects::DataType type>
        inline void register_pdo_handler(std::string_view name, abstract::pdo::HandlerType<dir, type> handler);
        template<abstract::pdo::Direction dir, abstract::objects::DataType type>
        inline void unregister_pdo_handler(std::string_view name);

        /// Downloads (writes) @p data to the slave's SDO
        template<abstract::objects::DataType type>
        void download_sdo(const abstract::sdo::Request &request, abstract::objects::ValueType<type> data, std::chrono::milliseconds timeout = 1s);

        /// Uploads (reads) @p data from the slave's SDO
        template<abstract::objects::DataType type>
        abstract::objects::ValueType<type> upload_object(const abstract::sdo::Request &request, std::chrono::milliseconds timeout = 1s);

    protected:

        /// Make Master a friend to let it access constructor and update methods
        friend class Master;

        /// Constructs Slave interface
        Slave(
            std::vector<uint8_t> &input_process_data_image,
            std::vector<uint8_t> &output_process_data_image,
            eni::Configuration &eni,
            std::string_view name
        );

    private:

        /// Type-independent implementation of the @ref download_sdo(...) method template
        void download_sdo(const abstract::sdo::Request &request, std::span<const uint8_t> data, std::chrono::milliseconds timeout);
        /// Type-independent implementation of the @ref upload_object(...) method template
        void upload_object(const abstract::sdo::Request &request,  std::span<uint8_t> data, std::chrono::milliseconds timeout);

    private:
    
        /**
         * @brief Called by the master after reading data from/before writting data to the bus.
         *    Updates all PDO entry associated with the slave based on the (Input/Output) Process
         *    Data Image
         * @note This method is quaranted to be called by the master when the underlying Process
         *    Data Image can be safely accessed (i.e. associated lock is taken)
         */
        template<abstract::pdo::Direction dir>
        void update();

    private:

        /// Slave's name
        const std::string name;

        /// Slave's fixed adress (physical address)
        const uint16_t fixed_addr;
        /// Slave's auto-increment adress
        const uint16_t auto_increment_addr;
        /// Slave's topological adress
        const uint16_t topological_addr;

    private:

        /**
         * @brief Input PDO entries
         * @performance This vector is accessed on the critical path of the bus I/O loop. 
         *    switching implementation to statically-allocated container (e.g. 
         *    boost::static_vector can improve reference-locality and so cache 
         *    performance)
         */
        std::vector<abstract::pdo::Entry<abstract::pdo::Direction::Input>> inputs;

        /**
         * @brief Output PDO entries
         * @performance This vector is accessed on the critical path of the bus I/O loop. 
         *    switching implementation to statically-allocated container (e.g. 
         *    boost::static_vector can improve reference-locality and so cache 
         *    performance)
         */
        std::vector<abstract::pdo::Entry<abstract::pdo::Direction::Output>> outputs;

    };

} // End namespace ethercat

/* ============================================================ Master ============================================================ */

namespace ethercat {
        
    /**
     * @brief Enumeration of host-bus synchronisation modes 
     * @see 'doc/EtherCAT Master V4 Protocol API 06 EN.pdf'
     */
    enum class SyncMode {
        FreeRun,
        IO1,
        IO2
    };

    /**
     * @brief Description of timing parameters of the bus
     */
    struct TimingInfo {

        // Duration of the bus cycle
        std::chrono::nanoseconds bus_cycle;
        // Duration of the frame transmition
        std::chrono::nanoseconds frame_transmition_time;
        // Expected bus delay
        std::chrono::nanoseconds expected_bus_delay;
        // Expected time of the RX transaction end (from start of bus cycle transmission)
        std::chrono::nanoseconds expected_rx_end_time; 
        // Expected time of the TX transaction end (from start of bus cycle transmission)
        std::chrono::nanoseconds expected_tx_end_time; 
        
    };

    /**
     * @brief Wrapper around Channel providing EtherCAT master functionality
     */
    class Master {
    public:

        /**
         * @enum State
         * @brief Enumeration of possible master's states
         */
        enum class State {
            Busoff,
            Init,
            Preop,
            Safeop,
            Op,
            LeaveOp,
            Busscan,
            BusscanComplete,
        };

        /**
         * @brief Structure describing Master's state
         */
        struct StateInfo {

            // Currently requested target state
            State target_state;
            // Current state
            State current_state;

            // Additional flags
            struct {
                
                // If this flag is set, at least one mandatory slave is not in OP when master is in OP. But, the slave is still connected
                bool at_least_one_mandatory_slave_not_in_op;
                // If this flag is set, the DC handling stopped sending ARMW/FRMW telegrams. The DC Slaves are not synchronizing their sys time in that case.
                bool dc_xrmw_stopped;
                // If this flag is set, at least one mandatory slave is not connected to master anymore
                bool at_least_one_mandatory_slave_lost;

            } flags;

        };

        /**
         * @enum Event
         * @brief Enumeration of bus-related events
         */
        enum class Event {

            // Event triggerred at the entry to @ref read_bus(...) method
            ReadBusStart,
            // Event triggerred when the bus-read action is completed
            ReadBusComplete,
            // Event triggerred when the slave's input PDOs are succesfully updated after bus-read action
            ReadBusSlavesUpdateComplete,

            // Event triggerred at the entry to @ref write_bus(...) method
            WriteBusStart,
            // Event triggerred when the slave's output PDOs are succesfully updated before bus-write action
            WriteBusSlavesUpdateComplete,
            // Event triggerred when the bus-write action is completed
            WriteBusComplete,

        };

    public:

        /// Constructs Master interface wrapping the given channel
        Master(Channel &channel);

        /// Constructs Master interface wrapping the given channel; Registers pool of initial slave interfaces
        Master(Channel &channel, std::span<std::string_view> slaves_to_register);

    public:

        /// Returns path to the ENI configuration file
        inline const std::filesystem::path &get_eni_path() const noexcept;

    public:

        /// Registers slave interface associated with the slave device named @p name
        void register_slave(std::string_view name);
        /// Unregisters slave interface associated with the slave device named @p name
        void unregister_slave(std::string_view name);
        /// Returns reference to the slave interface associated with the slave device named @p name
        Slave &get_slave(std::string_view name);

        /// Registers custom handler for the given @p event
        void register_event_handler(Event event, std::function<void(void)> handler);
        /// Unregisters custom handler for the given @p event
        void unregister_event_handler(Event event);

        /// Reads current state of the Master
        StateInfo get_state(std::chrono::milliseconds timeout = 100ms);
        /// Requests new state of the Master
        void set_state(State state, std::chrono::milliseconds timeout = 100ms);

        /// Reads current host-bus synchronisation mode
        SyncMode get_sync_mode(std::chrono::milliseconds timeout = 100ms);
        /// Sets target host-bus synchronisation mode
        void set_sync_mode(SyncMode mode, std::chrono::milliseconds timeout = 100ms);

        /// Measures timing parameters of the bus
        TimingInfo get_timing_info(std::chrono::milliseconds timeout = 5s);

        /// Reads Input Process Data Image from the bus buffer (secondly updates slave's input PDOs)
        void read_bus();
        /// Writes Output Process Data Image to the bus buffer (firstly updates slave's output PDOs)
        void write_bus();

        /// Returns copy of the current Input Process Data Image
        std::vector<uint8_t> get_input_process_image() const noexcept;
        /// Returns copy of the current Output Process Data Image
        std::vector<uint8_t> get_output_process_image() const noexcept;

        /**
         * @note @ref get_input_process_image() / @ref get_output_process_image() should be used only
         *    for debug purposes
         * @note Calling @ref get_input_process_image() / @ref get_output_process_image() from
         *    slave's PDO handler guarantes deadlock as PDI is locked during slave's update
         */

    private:

        /// Pair-like structure describing Slave interface
        struct SlaveDescriptor {
            std::string name;
            Slave slave;
        };

    private:

        /**
         * @brief Set of handlers to master-related events
         * @performance Handlers utilize std::function<> to implement callbacks used in the critical
         *    path of the bus I/O loop. This utilizes dynamic memory and dynamic dispatch that can
         *    corrupt performance (std::function<> is not inlined very well on the GCC). Think about
         *    moving to other implementation of function<> template
         */
        struct {

            /// Handler triggerred at the entry to @ref read_bus(...) method
            std::function<void(void)> read_bus_start_handler;
            /// Handler triggerred when the bus-read action is completed
            std::function<void(void)> read_bus_complete_handler;
            /// Handler triggerred when the slave's input PDOs are succesfully updated after bus-read action
            std::function<void(void)> read_bus_slaves_update_complete_handler;

            /// Handler triggerred at the entry to @ref write_bus(...) method
            std::function<void(void)> write_bus_start_handler;
            /// Handler triggerred when the slave's output PDOs are succesfully updated before bus-write action
            std::function<void(void)> write_bus_slaves_update_complete_handler;
            /// Handler triggerred when the bus-write action is completed
            std::function<void(void)> write_bus_complete_handler;

        } handlers;

        /**
         * @brief Registered slave interfaces
         * @performance This vector is accessed on the critical path of the bus I/O loop. 
         *    switching implementation to statically-allocated container (e.g. 
         *    boost::static_vector can improve reference-locality and so cache 
         *    performance)
         */
        std::vector<SlaveDescriptor> slaves;

        // Input PDI (Process Data Image) buffer
        abstract::pdo::ImageBuffer cyclical_inputs;
        // Output PDI (Process Data Image) buffer
        abstract::pdo::ImageBuffer cyclical_outputs;

    };

} // End namespace ethercat

/* ========================================================== Slave impl ========================================================== */

namespace ethercat {

    namespace {

        template<bool flag = false>
        constexpr inline void dir_no_match() {
            static_assert(flag, "[Slave::update] Invalid direction"); 
        }

    }

    template<abstract::pdo::Direction dir>
    void Slave::update() {

        /// Pointer to the pdo entries        
        std::vector<abstract::pdo::Entry<dir>> *pdos;
        
        // Select PDO set
        if constexpr (dir == pdoDirection::Inputs)
            pdos = &inputs;
        else if constexpr (dir == pdoDirection::Outputs)
            pdos = &outputs;
        else
            dir_no_match();

        // Iterate over PDO entries and update them
        for(auto &pdo : *pdos)
            pdo.update();
        
    }

} // End namespace ethercat

/* ================================================= Slave driver (implementation) ================================================ */

// Use EtherCAT namespace for convinience
using namespace ethercat;

/**
 * @brief Example implementation of the slave driver
 * @details @ref SlaveDriver class wraps ethercat::Slave to provide thread-safe, strongly typed
 *    access to SDOs/PDOs of the EtherCAT slave device
 */
class SlaveDriver {
public:

    SlaveDriver(ethercat::Slave &slave)
    :
        // Map cyclical output objects
        object_a{ slave.get_pdo_reference<abstract::pdo::Direction::Output, abstract::objects::DataType::UnsignedDoubleInt>("a") },
        object_b{ slave.get_pdo_reference<abstract::pdo::Direction::Output, abstract::objects::DataType::LongReal>         ("b") }
    {
        // Register handlers for cyclical input objects
        slave.register_pdo_handler<abstract::pdo::Direction::Input, abstract::objects::DataType::Int> ("c", [this](int16_t val) { on_update_c(val); });
        slave.register_pdo_handler<abstract::pdo::Direction::Input, abstract::objects::DataType::Real>("d", [this](float   val) { on_update_d(val); });   
    }

public:

    /// Sets "a" output PDO of type uin32_t [DINT]
    void set_a(uint32_t value) { object_a.set(value); }
    /// Sets "b" output PDO of type double [LREAL]
    void set_b(double value) { object_b.set(value); }
    /// Handler called on update of the "c" input PDO of type int16_t [INT]
    void on_update_c(int16_t value) { c_value = value; }
    /// Handler called on update of the "d" input PDO of type float [REAL]
    void on_update_d(float value) { d_value = value; }

private:

    /// Proxy accessing "a" output PDO
    abstract::objects::ValueBufferReference<abstract::objects::DataType::UnsignedDoubleInt> object_a;
    /// Proxy accessing "b" output PDO
    abstract::objects::ValueBufferReference<abstract::objects::DataType::LongReal> object_b;
    // Helper buffer for "c" input PDO
    int16_t c_value;
    // Helper buffer for "d" input PDO
    float d_value;

};

/* ================================================================================================================================ */
