cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(bmx055-test)

pico_sdk_init()

add_executable(bmx055-test
	bmx055.cpp
	test_main.cpp
)

# enable usb output, disable uart output
pico_enable_stdio_usb(bmx055-test 1)
pico_enable_stdio_uart(bmx055-test 0)
pico_add_extra_outputs(bmx055-test)

target_link_libraries(bmx055-test pico_stdlib hardware_i2c)
