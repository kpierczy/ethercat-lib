# Welcome to the ethercat-lib library!

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![version](https://img.shields.io/badge/version-1.0.0-blue)

The [EtherCAT](https://en.wikipedia.org/wiki/EtherCAT) is an Ethernet-based *fieldbus system* characterized by very low communication jitter
suitable for real-time applications. The system standard provides specification for the whole communication stack (in ISO/OSI model sense)
from physical to application layer. Although the EtherCAT introduces a few application-layer protocols enabling its usage in various scenarios,
the common use case involves:

  - configuring the bus using automatically-generated ENI (*EtherCAT Network Informations*) files that are decoded and executed
    directly by the master interface PCI card at bus start
  - utilizing the [CANopen](https://en.wikipedia.org/wiki/CANopen) device model (described by the *CoE*, *CANopen over EtherCAT* protocol) 
    to provide runtime-dependent configuration of the slave devices and cyclically exchange process data
    
This simple two-step scenario can turn out to be surprisingly tough to implement taking into account all catches introduced by the EtherCAT
including bit-aligned process data storage or serialization/deserialization of application-space data structures.

The **ethercat-lib** aims to implement main utilities that are required to quickly develop and deploy EtherCAT-based Master solutions without
wrapping your mind around too low-level topics. The library takes advantage of the modern C++ approach (C++17) to implement convenient APIs 
that are not only easy to remember and use but also tend to maximize optimization capabilities of modern compilers. Easy? But how easy?

```cpp
#include <iostream>
#include "ethercat.hpp"

using namespace std::literals::chrono_literals;

int main() {

    // Create master
    Master master { "some/path/eni.xml" };

    // Wait for master to enter Operational state
    while(master.get_state() != ethercat::Master::State::Op);

    // Get handle to the slave device interface
    auto &imu_slave = master.get_slave("Imu");

    // Prepare proxy for exchanging SDO objects
    auto sdo_proxy = imu_slave.template get_sdo<uint32_t>(/* SDO index */ 0x2200);
    // Read SDO from the device
    uint32_t sdo_value = sdo_proxy.upload(/* IO timeout */ 2000ms);
    // Write a new SDO value to the device
    sdo_proxy.download(/* Value */ 1, /* IO timeout */ 2000ms);

    // Prepare proxy object for deserializing input PDO objects from the Input Process Data Image
    auto input_pdo_proxy = imu_slave
        .template get_pdo_entry<ethercat::Slave::PdoDirection::Input>(/* PDO entry name */ "InputPdo")
        .template get_reference<int64_t>();
    // Prepare proxy object for eserializing output PDO objects into the Output Process Data Image
    auto output_pdo_proxy = imu_slave
        .template get_pdo_entry<ethercat::Slave::PdoDirection::Output>(/* PDO entry name */ "OutputPdo")
        .template get_reference<int8_t>();

    int8_t output = 0;

    // Launch main loop
    while(true) {

        // Read cyclical inputs
        master.read_bus(/* IO timeout */ 1ms);

        // Read incoming input PDOs
        std::cout << "Read 0x2201 PDO value: " << input_pdo_proxy.get() << std::endl;
        // Compute cyclical outputs
        output_pdo_proxy.set(output++);

        // Write cyclical outputs
        master.write_bus(/* IO timeout */ 1ms);

    }
}

```

Cool? Quite cool, if you asked me 😃. But the final opinion is yours 😉. For those who need something more to decide:

  - You've written code that tries to *write* PDO object via *input* proxy or something similar? 
    - No problem! The library will not let you do stupid things that can be detected at compile time!
  - Your ENI file describes object X as `uint32_t`-typed, but your code tries to implicitly interpret it as `int32_t`?
    - Don't worry! I/O routines will throw exception with a verbose message which tells you what's wrong!
  - Oh, you cannot afford exceptions on the critical path? 
    - Just modify library config to disable them 😉
  - Okay, fine, but I need the library to support a custom structural EtherCAT type to represent messages queue for my
    CAN bridge... 
    - We've covered you too! Just define a custom *translator class* suitable for your needs!
  - But why translator *type*? Why not *function*? I don't need any *context data* to serialize my structure. Why do I have
    to pay additional memory for things that I don't use? 
    - Don't worry. Translation routines can be static. I know that you probably don't need such data. But if - for some reason - you do,
    you can!  
  - Fine, you support bit-aligned PDO serialization. But it's an additional computation cost. My ENI generator is smart
    enough to *don't* generate bit-aligned entries
    - Again, just modify library config and bit-aligned data support will disappear 😃
  - Okay, okay. I know my PCI card will handle the ENI configuration automatically, but my app needs to parse it nonetheless
    to detect what kind of configuration is loaded? Do I need to use a generic XML parser?
    - I'm glad you've asked. The ENI parsing module waits for you!


## It looks awesome, but what hardware can I use the library with?

Okay, you've got me. The previous example is not the full story. Have you noticed that the `Master` type is not in the `ethercat::` namespace?
No mistake here. The library has been designed to abstract out as much as possible from the actual hardware implementation. To actually use it
you need to write down two classes that will handle hardware configuration and I/O exchange. But don't worry. These are not very complex in fact.
Their interfaces look like that:

```cpp
class Master : public ::ethercat::Master<Master> {

    /// Make base class a friend to let it access implementation methods
    friend class ::ethercat::Master<Master>;

protected: /* Public ctors & dtors */

    /// Creates EtherCAT master driver configuring the underlying hardware
    Master();

protected: /* Protected EtherCAT common methods (implementation) */

    /// Reads current state of the slave device in the ESM (EtherCAT slave machine)
    inline State get_state_impl(std::chrono::milliseconds timeout);
    
    /// Requestes state change of the slave device in the ESM (EtherCAT slave machine)
    inline void set_state_impl(State state, std::chrono::milliseconds timeout);
    
protected: /* Protected EtherCAT I/O methods (implementation) */

    /// Reads Input Process Data Image from the bus
    void read_bus_impl(::ethercat::config::types::Span<uint8_t> pdi_buffer);

    /// Writes Output Process Data Image to the bus buffer
    void write_bus_impl(::ethercat::config::types::Span<const uint8_t> pdi_buffer);

};

```

```cpp
class Slave : public ethercat::Slave<Slave> {

    /// Make base class a friend to let it access implementation methods
    friend class ethercat::Slave<Slave>;

protected: /* Protected EtherCAT common methods (implementations) */

    /// Reads current state of the slave device in the ESM (EtherCAT slave machine)
    State get_state_impl(std::chrono::milliseconds timeout);
    
    /// Requestes state change of the slave device in the ESM (EtherCAT slave machine)
    void set_state_impl(State state, std::chrono::milliseconds timeout);

protected: /* Protected ctors & dtors  */

    /// Construct a new Slave interface
    inline Slave(
        ::ethercat::eni::Slave slave_eni,
        std::vector<Pdo<PdoDirection::Input>> &&inputs,
        std::vector<Pdo<PdoDirection::Output>> &&outputs
    );

protected: /* Protected implementation methods (SDO) */

    /// Downloads @p data into the SDO with the given address
    void download_sdo(
        uint16_t index,
        uint16_t subindex,
        ethercat::config::types::Span<const uint8_t> data,
        std::chrono::milliseconds timeout,
        bool complete_access
    );
    
    /// Uploads SDO with the given address into the @p data buffer
    void upload_sdo(
        uint16_t index,
        uint16_t subindex,
        ethercat::config::types::Span<uint8_t> data,
        std::chrono::milliseconds timeout,
        bool complete_access
    );

};
```

These are rather basic interfaces that you could expect from hardware-specific implementations of the bus-support library. Notice that both classes
are implemented as derivations of abstract interfaces leveraging [CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) design
pattern. Thanks to this approach the compiler can see-through your specific implementation of the library and provide better optimization than
e.g. when hardware-specific routines would be implemented as pointers to functions (which is a common pattern in low-level bus-support libraries). 

# Usage

The library ships with a standard CMake scripts that lets you build it in a simple two-step manner:

    cmake \
        -B <builddir> \
        --install-prefix <prefix> \
        <libdir>
    cmake \
        --build <builddir> \
        --target install

You can also just add the project as a subdirectory of your project 

    add_dubsirectory(<libdir>)

If you need to build the documentation (`BUILD_DOC` option `ON`), you will need both *Doxygen* and *Sphinx* installed on your system. By default, the
library uses the [range-v3](https://github.com/ericniebler/range-v3) library to provide `span` type implementation. If your platform does not provide
a suitable distribution you can either build the library from source or change the default span implementation in the `include/ethercat/config.hpp` file.

## ROS2 package

The library has been developed in context of the ROS2-based robotic system. For this reason it ships with a ROS2-ready setup that enables building it
with the [colcon](https://colcon.readthedocs.io/en/released/index.html) utility. Simply type

    colcon build --base-path <libdir>

from your workspace directory to build it.


# Afterword

The **ethercat-lib** has been developed as a part of my engineering thesis. It has been tested in a real-world robotic system utilizing PCIe 
Master interface card from [Hilscher](https://www.hilscher.com/). Not only the source code of the library is well-documented but also UML diagrams
of main classes and conceptual overview of the library are provided. However, due to being developed single-handedly, the library may suffer
from bugs that has not been covered by the current test suite or detected during deployment. If you use the library and:

  - you've found some bugs
  - you've complemented the test suite with new scenarios that cover your use case
  - you feel like you could put in your two cents to the documentation

please don't hesitate to open an issue or a pull request 😉.
