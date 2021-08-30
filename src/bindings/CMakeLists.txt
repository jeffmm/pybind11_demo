find_package(pybind11 REQUIRED)

pybind11_add_module(_double_pendulum SHARED bindings.cpp)

target_link_libraries(_double_pendulum PRIVATE double_pendulum)
