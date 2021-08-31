# pybind11 demo

This project serves as a demonstration of how to convert a C++ library into a Python package using pybind11.

## Motivation

Python is the language of modern machine learning and AI, but C++ is superior to Python in its fast and optimized execution of code. This is why most "fast" libraries in the Python data stack are simply pre-compiled C++ code wrapped in a Python interface (e.g. `numpy`). Many cutting edge algorithms that are being developed and implemented by academics are written in low-level languages like C++ due to its speed and flexibility.

I love Python, but as a former C++ developer, I also know that it is occasionally preferable to write algorithms in a low-level language, where one no longer needs to fear nested for-loops as "bad practice", nor wonder whether the code being written is "pythonic" enough.

But it doesn't have to be zero sum; we can have the best of both worlds---the speed of C++ and the zen of Python---by referencing C++ functions and classes from within Python. And with pybind11, it's really easy to do.

### Case study

Recently, I needed to train a Dynamic Topic Model. The model is currently available as a pure Python algorithm in the `gensim` package, but it is orders of magnitude slower than the C++ version released by the Blei lab. The Python version would've been fine if I just needed to train one model, but I also needed to do hyperparameter tuning. It was actually faster for me to wrap the C++ code in Python bindings and train the models than it was to use the available `gensim` version. As a bonus, the Python-wrapped DTM model is now available as a Python package that I can use in the future: [`dtmpy`](https://github.com/jeffmm/dtmpy).

## Demo

This project wraps a C++ library named `double_pendulum` with Python bindings and integrates it into a Python package of the same name.

The library itself contains a single class `DoublePendulum`, which manages a physics simulation of a double pendulum. Python bindings allow the class to be initialized and its methods accessed from within Python. The bindings also include a special method that converts an internal C++ data array of simulation data into a `numpy` array.

The configuration in `setup.py` uses `cmake` to compile the C++ library and export the bindings as an intermediary Python module named `_double_pendulum`, which is imported and used by the main Python module `double_pendulum`. This setup allows you to write functions and classes in both Python and C++, and use them all in one module---speed and zen.

### Project structure

The project has the following organization structure:

```
├── LICENSE
├── README.md
├── Makefile                    <- Makefile for building/launching Docker container
├── docker                      <- Directory for Docker build and config files
│   ├── Dockerfile
│   └── docker-compose.yml
├── CMakeLists.txt              <- Top-level CMakeLists, builds all C++ code using CMake
├── setup.py                    <- Setup file compiles C++ and builds Python package
├── double_pendulum             <- Python module directory
│   ├── __init__.py             <- Identifies directory as a Python module
│   └── simulation.py           <- Module Python code
└── src                         <- Folder for all C++ source code
    ├── CMakeLists.txt          <- References CMakeLists in child directories
    ├── double_pendulum         <- Source directory for a C++ library
    │   ├── CMakeLists.txt      <- CMakeLists for compiling library
    │   ├── double_pendulum.hpp <- Library header file
    │   └── double_pendulum.cpp <- Library source file
    ├── executable              <- Source directory for a C++ executable
    │   ├── CMakeLists.txt      <- CMakeLists for compiling executable
    │   └── main.cpp            <- Executable source code
    └── bindings                <- Source directory for Python bindings with pybind11
        ├── CMakeLists.txt      <- Compiles Python bindings
        └── bindings.cpp        <- Source code for Python bindings for C++ library
```

## Noteworthy files

#### `setup.py`

This file is used by `pip` to compile the C++ code with `cmake` and build and install the Python module. This is all done using one command:

```
pip install .
```

The only section of the code that needs to be modified for other projects is the final call to `setup()`

```python
setup(
    name="double_pendulum",
    version="0.1.0",
    author="Jeff Moore",
    author_email="jmoore@manifold.ai",
    description="Python wrapper for double pendulum simulation",
    long_description="",
    ext_modules=[CMakeExtension("_double_pendulum")],
    cmdclass={"build_ext": CMakeBuild},
    install_requires=["numpy"],
    zip_safe=False,
    packages=find_packages(),
)
```

#### `CMakeLists.txt`

This is the file that `cmake` uses to find and compile all C++ code in the project (for more insight into how it works, visit my [`cmake_tutorial`](https://github.com/jeffmm/cmake_tutorial) project). Subdirectories with `CMakeLists.txt` are referenced by their parents. The top-level `CMakeLists.txt` is referenced by `setup.py` to automatically compile the C++ library and bindings.

You can build the C++ code manually using `cmake` with:

```
mkdir build
cd build
cmake ..
make
```

This will build the library and executable binary. Since there are no install targets, the compiled binary is located in the `build/src/executable` directory. The binary can be ran with:

```
cp src/executable/double_pendulum_sim .
./double_pendulum_sim 1.0 1.0 1.0 1.0 45 75 100000 0.0001 1000
```

#### `bindings.cpp`

This file contains the C++ code that generates the Python bindings. It contains all references to the pybind11 library. This is an example of how the bindings file is formatted:

```c++
#include <pybind11/pybind11.h>
#include "path/to/my_lib.hpp"

namespace py = pybind11;

// Create a Python module named py_lib from C++ library my_lib. The C++ and Python
// names need not be different, but they are here for added clarity.
PYBIND11_MODULE(py_lib, m) {
    m.doc() = R"pbdoc(
        This is my module docstring.
    )pbdoc";

    // Reference a function named my_func defined in my_lib.hpp to create a Python
    // function py_lib.py_func
    m.def("py_func", &my_func,
    R"pbdoc(
    This is the docstring for function py_func.

    Args:
        x: Description of arg x.

    Returns:
        Return value description.
    )pbdoc",
    // Define arguments and their defaults.
    py::arg("x"));

    // Reference a class named MyClass defined in my_lib.hpp to create a Python
    // class py_lib.PyClass
    py::class_<MyClass>(m, "PyClass")
    // Assuming the initializer takes an integer: MyClass(int)
    .def(py::init<int>(),
        R"pbdoc(
        This is the docstring for class py_lib.PyClass

        Args:
           x: Description of kwarg x, defaults to 0.
        )pbdoc",
            py::arg("x") = 0)
    // Binding for public class method Foo
    .def("Foo", &MyClass::Foo,
        R"pbdoc(
        This is the docstring for class method PyClass.Foo.

        Args:
            x: Description of arg x
            y: Description of kwarg y, defaults to 1.
        )pbdoc",
            py::arg("x"),
            py::arg("y") = 1)
    // Create a lambda function to add capability beyond existing class methods:
    // use attribute MyClass.name to generate a __repr__ string.
    .def("__repr__", [] (MyClass &a) {
            return "<py_lib.PyClass named '" + a.name + "'>";
        }
    );

// Add version info to the module
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
```

## Build and install

### Using Docker and `Makefile`

Build the project Docker container with:

```
make dev-start
```

This will automatically `pip install` the contents of the project in the container, which uses the `setup.py` file to build the C++ library and install the `double_pendulum` Python package.

#### Test with Jupyterlab

Use `docker ps` to check which local port is mapped to Jupyterlab, and point your browser to the corresponding `localhost:<PORT>`. Alternatively, if on Mac, you can do `make nb`.

You can test the build of the package by importing the package in a fresh notebook and running a test simulation:

```python
import double_pendulum as dp

# Initialize pendulum parameters
p = dp.DoublePendulum()
# Run a simulation
p.Simulate(n_steps=100000)
# Read simulation data
p.data.head()
```

There is also a sample notebook in the `notebooks` directory that animates the motion of the double pendulum.

#### Test using CLI in container

You can also test the build from the CLI within the Docker container by running `make bash`, then opening a Python console and running the above code.

### Building manually

To build manually, you need to have `gcc`, `cmake`, and `pybind11-dev` installed. Then you can build the package with:

```
git clone https://github.com/jeffmm/pybind11_demo
cd pybind11_demo
pip install .
```

You can then test the build by opening a Python console and running the Python code above.
