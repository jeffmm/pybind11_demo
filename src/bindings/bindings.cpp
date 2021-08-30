#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../double_pendulum/double_pendulum.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_double_pendulum, m) {
    m.doc() = R"pbdoc(
        Module for simulating a double pendulum.

        Classes:
            DoublePendulum
    )pbdoc";
    py::class_<DoublePendulum>(m, "DoublePendulum")
    .def(py::init<
            double, // m1
            double, // m2
            double, // l1
            double, // l2
            double, // th1
            double>(), // th2
        R"pbdoc(
        Class for simulating a double pendulum.

        Initialize pendulum parameters then use the Simulation() method to simulate.

        Methods:
            Simulate: Run a double pendulum simulation.

        Args:
           m1: Mass of pendulum 1
           m2: Mass of pendulum 2
           l1: Length of pendulum 1
           l2: Length of pendulum 2
           th1: Initial angle of pendulum 1 relative to resting
           th2: Initial angle of pendulum 2 relative to resting
        )pbdoc",
            py::arg("m1") = 1.0,
            py::arg("m2") = 1.0,
            py::arg("l1") = 1.0,
            py::arg("l2") = 1.0,
            py::arg("th1") = 0.6,
            py::arg("th2") = 0.25)
    .def("Simulate", &DoublePendulum::Simulate,
        R"pbdoc(
        Run a simulation of a double pendulum.

        Args:
            n_steps: Number of integration steps to perform
            dt: Time delta to use in integration scheme
            n_record: Frequency at which to record pendulum data for analysis
        )pbdoc",
            py::arg("n_steps"),
            py::arg("dt") = 0.001,
            py::arg("n_record") = 100)
    .def("SetMasses", &DoublePendulum::SetMasses,
        R"pbdoc(
        Set pendulum masses.
        )pbdoc",
            py::arg("m1"),
            py::arg("m2"))
    .def("SetLengths", &DoublePendulum::SetLengths,
        R"pbdoc(
        Set pendulum lengths.
        )pbdoc",
            py::arg("l1"),
            py::arg("l2"))
    .def("SetAngles", &DoublePendulum::SetAngles,
        R"pbdoc(
        Set initial pendulum angles relative to resting.
        )pbdoc",
            py::arg("th1"),
            py::arg("th2"))
    .def("Data", [] (DoublePendulum &pendulum) {
        float** data = pendulum.GetData();
        size_t N = pendulum.GetNData();
        size_t M = 7;

        py::array_t<float, py::array::c_style> arr({N, M});

        auto ra = arr.mutable_unchecked();

        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                ra(i, j) = data[i][j];
            }
        }
        return arr;
    });



#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
