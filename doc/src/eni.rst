ENI (EtherCAT Network Informations) file parser
===============================================

EtherCAT Network Information (ENI) is an XML-derivced file format describing configuration of the bus. Some of informations
contained by the format are:

    * bus cycle
    * enumeration of slave devices expected on the bus
    * PDO mapping & assignment that should be set by Master at the bus startup
    * initial CoE commands that should be send to slave devices at each transition of the ECM (EtherCAT State Machine)

As the format itself is quite complicated many producers of bus-interfacing hardware provide automated and semi-automated tools
for generating ENI files and configuring bus according to the passed configuration. As so master device application drivers
are usually free of manual parsing these files. However lifting out some of key informations may be crucial to the self-aware
drivers that e.g. need to verify whether slave device's configruation set in the ENI file is supported by the driver itself.

For these reasons simple ENI parsing module has been introduced. While it does not provide support of the full ENI specification
it allows parsing key informations in a convinient, object-oriented way. 

.. toctree:: 
    :maxdepth: 1
    :caption: Reference

    eni/common
    eni/entities
