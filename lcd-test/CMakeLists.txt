cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(lcd-test)

pico_sdk_init()

add_executable(lcd-test
	lcd-test.c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(lcd-test 1)
pico_enable_stdio_uart(lcd-test 0)

pico_add_extra_outputs(lcd-test)

target_link_libraries(lcd-test pico_stdlib hardware_i2c)
