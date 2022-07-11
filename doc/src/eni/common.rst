Common utilities
================

Classes
-------

.. doxygenclass:: ethercat::eni::Error
    :members:
    :protected-members:
    :private-members:

.. doxygenclass:: ethercat::eni::Element
    :members:
    :protected-members:
    :private-members:

Functions
---------

.. doxygenfunction:: ethercat::eni::element_from_file(const std::filesystem::path &)
.. doxygenfunction:: ethercat::eni::element_from_string(const std::string &)
.. doxygenfunction:: ethercat::eni::element_from_stream(std::basic_istream<char> &)

.. doxygenfunction:: ethercat::eni::wrap_error

.. doxygenfunction:: ethercat::eni::parse_index
