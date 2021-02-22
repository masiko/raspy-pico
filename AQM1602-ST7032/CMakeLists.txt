cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(aqm1602-test)

pico_sdk_init()

add_executable(aqm1602-test
	test_main.cpp
	aqm1602.cpp
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(aqm1602-test 1)
#pico_enable_stdio_uart(aqm1602-test 0)
pico_add_extra_outputs(aqm1602-test)

target_link_libraries(aqm1602-test pico_stdlib hardware_i2c)
