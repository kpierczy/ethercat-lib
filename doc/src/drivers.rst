Drivers API
===========

**Drivers API** consist of two classes - `Slave` and `Master`. These are abstract models of slave and master EtherCAT devices.
Their interfaces provides most of common routines used to controll bus and devices. Both classes are implemented as a CRTP-based
base classes that require to be Derived to types implementing hardware-specific I/O routines. 


.. toctree:: 
    :maxdepth: 1
    :caption: Reference

    drivers/slave
    drivers/master
