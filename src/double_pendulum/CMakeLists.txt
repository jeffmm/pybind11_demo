file(GLOB HEADERS CONFIGURE_DEPENDS "double_pendulum.hpp")
file(GLOB SOURCES "double_pendulum.cpp")

add_library(double_pendulum SHARED ${SOURCES} ${HEADERS})
