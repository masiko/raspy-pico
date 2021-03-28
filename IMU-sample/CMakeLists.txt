cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(imu-sample)

pico_sdk_init()

add_executable(imu-sample
	BMX055/bmx055.cpp
	AQM1602-ST7032/aqm1602.cpp
	test_main.cpp
)

# enable usb output, disable uart output
pico_enable_stdio_usb(imu-sample 1)
pico_enable_stdio_uart(imu-sample 0)
pico_add_extra_outputs(imu-sample)

target_link_libraries(imu-sample pico_stdlib hardware_i2c)
