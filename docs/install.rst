Installation
============

From pip
--------

The current :py:obj:`starry` installation relies on :py:obj:`PyMC3` and, by extension, :py:obj:`theano`
which is `no longer maintained <https://groups.google.com/g/theano-users/c/7Poq8BZutbY/m/rNCIfvAEAwAJ>`_.
To function properly these packages require :py:obj:`numpy<1.22`, therefore, we strongly recommend installing
:py:obj:`starry` in a new conda environment to avoid impacting other installations, e.g.:

.. code-block:: bash

    conda create -n "starry" -c conda-forge python=3.9.18


Then you can install the latest release of :py:obj:`starry` using :py:obj:`pip`:

.. code-block:: bash

    pip install -U starry


.. note ::

    Installation via ``conda-forge`` is no longer supported.


Development version
-------------------

You can install the latest development version of :py:obj:`starry` directly
from `GitHub <https://github.com/rodluger/starry>`_:

.. code-block:: bash

    git clone https://github.com/rodluger/starry.git
    cd starry
    python setup.py develop
