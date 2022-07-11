Welcome to `ethercat` library !
===============================

**ethercat** is a C++ library providing implementation of main subset of abstractractions defined by the EDP (EtherCAT Device Protocol).
It's aim is to speed-up implementation of EtherCAT master device drivers in a way that it independent from the underlying hardware 
implementation. General structure of the library can be described with the following picture:

.. image:: /img/lib-model.png

`Hardware Access` layer is not an integral part of the library and instead needs to be provided by the user. It mainly consist of a few 
classes providing implementation of I/O methods required by abstract classes defined in the `Drivers API` module. Thanks to such approach
library can be used on a range of platforms from PC to small, embedded ones like Raspberry PI.

.. toctree:: 
    :maxdepth: 1
    :caption: Modules:

    src/abstraction
    src/eni
    src/drivers
