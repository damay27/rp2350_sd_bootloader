cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/pico-sdk/)
set(PICO_PLATFORM rp2350)

include(pico_sdk_import.cmake)

project(main C CXX ASM)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(main
	main.c
	hw_config.c
)

pico_enable_stdio_usb(main 0)

pico_enable_stdio_uart(main 1)

pico_add_extra_outputs(main)

add_subdirectory(./no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)

target_link_libraries(
	main
	pico_stdlib
	hardware_spi
	no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
)
