# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.17)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/tom/pico/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(test C CXX)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(test
        test.cpp
        TLC5947.cpp
        TLC5947.h
        TLC5947Matrix.cpp
        TLC5947Matrix.h
        )

pico_generate_pio_header(test ${CMAKE_CURRENT_LIST_DIR}/TLC5947PIO.pio)

pico_set_program_name(test "test")
pico_set_program_version(test "0.1")

pico_enable_stdio_uart(test 1)
pico_enable_stdio_usb(test 0)

# Add the standard library to the build
target_link_libraries(test
        pico_stdlib
        pico_multicore
        hardware_pio
        )

pico_add_extra_outputs(test)

