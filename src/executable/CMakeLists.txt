# Create double_pendulum_sim executable and link double_pendulum lib
add_executable(double_pendulum_sim "main.cpp")
target_link_libraries(double_pendulum_sim double_pendulum)
