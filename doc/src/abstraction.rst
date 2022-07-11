Abstract components
===================

Aim of the **Abstract** module is to provide representation of the entities of interest described by the (EDP) EtherCAT Device Protocol.
As the protocol itself is very complex only crucial elements has been modeled by the library. These are mainly related to three entities
introduced by the protocol

    * **Objects** -- objects introduce notion of generalized 'registers' in the slave devices that can be read and/or written 
      by the Master device to control them. Each object has it's own unique *index* and *type*. EtherCAT defines range of 
      standard types as well as provides tools to define custom ones. Moreover each *Object* can consist of 'subobjects'
      referred by *subindeces*. As with the classis 'hardware' registers both read/write, read-only and write-only objects
      are possible. Set of *Objects* provided by the slave device is called *Objects Dictionary* (OD)

    * **PDOs** -- Process Data Objects refer to subset of *Objects* that are exchanged between slave and master at each bus
      cycle with the constant rate. In general *Objects* that can be exchanged cyclically (or synchronously in other words) 
      are gathered into structure-like objects. These are actual *PDOs*. Each PDO gathers set of *Objects* by storing their
      indices in it's 'subobjects'. Such a PDO can be mapped into so called *Process Data Image* (PDI) by assigning PDO object
      to the hardware module prrocessing bus data in the slave. Input as well as output PDOs are possible (and so Input 
      PDI - IPDI - and Output PDI - OPDI). Same PDI is a raw bytes buffer stored by the master device whose content is mapped
      to PDOs exchanged with all slave devices on the bus. PDOs can be either configruable or predefined by the slave device.

    * **SDOs** -- Service Data Objects refer to subset of *Objects* that are exchanged between slave and master in the 
      request-response manner (asynchronously). At any time, the master device can send package to the slave requesting
      reading or writting the SDO. In general **Objects** can be referred as PDOs as well as SDOs at the same time depending
      on the access method context

Next to abstract representations of these objects module provides auxiliary routines enabling parsing cyclically exchanged object from/to PDIs
in a thread-safe manner as well as parsing abstract SDO requests incomming from higher layers of the master device drivers.

.. note::

    Descibed terms refer to the specific **Application Layer** of the EDP called *CoE* (CANopen over EtherCAT) protocol. 
    Same EDP is much wider standard defining not only application layer but also link and hardware layers (in sens of the
    ISO/OSI network model). 

.. toctree:: 
    :maxdepth: 1
    :caption: Reference

    abstraction/objects
    abstraction/types
    abstraction/translation
