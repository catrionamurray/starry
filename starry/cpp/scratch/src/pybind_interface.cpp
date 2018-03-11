#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <Eigen/Core>
#include <iostream>
#include "ellip.h"
#include "maps.h"
#include "basis.h"
#include "fact.h"
#include "sqrtint.h"

using namespace std;
using namespace pybind11::literals;
namespace py = pybind11;

struct ndarray {
    ndarray(py::array_t<double, py::array::c_style>& arr) {
        auto buf = arr.request();
        if (buf.ndim != 1) throw std::runtime_error("invalid array");
        size = buf.size;
        ptr = (double*)buf.ptr;
    }
    int size = 0;
    double* ptr = NULL;
};

PYBIND11_MODULE(starry, m) {
    m.doc() = R"pbdoc(
        The starry C++ library.
    )pbdoc";

    // Core Map class
    py::class_<maps::Map<double>>(m, "Map")
        .def(py::init<Eigen::Matrix<double, Eigen::Dynamic, 1>&>())
        .def(py::init<int>())
        .def("evaluate", &maps::Map<double>::evaluate);

    // Utilities
    py::module m_utils = m.def_submodule("utils");

    m_utils.def("factorial", [] (int n) { return fact::factorial(n); },
    R"pbdoc(
        Factorial of a non-negative integer.
    )pbdoc", "n"_a);

    m_utils.def("half_factorial", [] (int n) { return fact::half_factorial(n); },
    R"pbdoc(
        Factorial of `n` / 2.
    )pbdoc", "n"_a);

    m_utils.def("sqrt_int", [] (int n) { return sqrtint::sqrt_int(n); },
    R"pbdoc(
        Square root of `n`.
    )pbdoc", "n"_a);

    m_utils.def("invsqrt_int", [] (int n) { return sqrtint::invsqrt_int(n); },
    R"pbdoc(
        Inverse of the square root of `n`.
    )pbdoc", "n"_a);

    // Elliptic functions
    py::module m_ellip = m.def_submodule("elliptic");

    m_ellip.def("K", [] (double ksq) { return ellip::K(ksq); },
    R"pbdoc(
        Complete elliptic integral of the first kind.
    )pbdoc", "ksq"_a);

    m_ellip.def("E", [] (double ksq) { return ellip::E(ksq); },
    R"pbdoc(
        Complete elliptic integral of the second kind.
    )pbdoc", "ksq"_a);

    m_ellip.def("PI", [] (double n, double ksq) { return ellip::PI(n, ksq); },
    R"pbdoc(
        Complete elliptic integral of the third kind.
    )pbdoc", "n"_a, "ksq"_a);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
