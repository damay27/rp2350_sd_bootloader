# RP2350 SD Card Bootloader
This is a bootloader for loading programs from an SD card. As of right now this only supports applications designed to run exclusively from RAM. Right now the code designed to target the Adafruit Metro RP2350 board. The main thing that would need to change is the SPI pin mapping in the `hw_config.c` file. This project depends on the [no-OS-FatFS-SD-SDIO-SPI-RPi-Pico](https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico) library to communicate with the SD card.

# Building applications for the bootloader
Two things need to be changed in the CMakeLists.txt file. The first is to binary type to `no_flash` with the line below.

    pico_set_binary_type(<APP_NAME> no_flash)

The second thing is to swap to use the custom `app.ld` linker script.
    
    set_target_properties(<APP_NAME> PROPERTIES PICO_TARGET_LINKER_SCRIPT <PATH_TO_SCRIPT>/app.ld)

# SD Card Setup
The SD card should be formatted as FAT32. Currently, the bootloader looks for a file named `main.elf` in the root of the SD card's filesystem. This can be changed by modifying the ELF_FILE_PATH macro in the main.c file.

# Building the Bootloader
The bootloader can be built with the commands below.

    git submodule update --init --recursive
    mkdir build
    cd build
    cmake ..
    make
