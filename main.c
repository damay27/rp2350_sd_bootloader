/* main.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "hw_config.h"
#include "f_util.h"
#include "ff.h"

#include "elf.h"

#define ELF_FILE_PATH "main.elf"

int main() {
    stdio_init_all();

    FATFS fs;
    FRESULT fr = f_mount(&fs, "", 1);
    if (FR_OK != fr) {
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    // Open a file and write to it
    FIL file;
    const char* const filename = ELF_FILE_PATH;
    fr = f_open(&file, filename, FA_READ);
    if (FR_OK != fr) {
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    }
    
    struct ElfHeader elf_header;
    uint32_t elf_header_read_count = 0;
    // Read the ELF header
    fr = f_read(&file, &elf_header, sizeof(struct ElfHeader), (UINT*)&elf_header_read_count);
    if (FR_OK != fr || elf_header_read_count != sizeof(struct ElfHeader)) {
        printf("Error reading ELF header!\n");
    }

    // Seek to the start of the program header
    fr = f_lseek(&file, elf_header.program_header_offset);
    if (FR_OK != fr) {
        printf("Error seeking program header!\n");
    }

    // Read in the program header
    uint32_t offset = 0;
    for (int i = 0; i < elf_header.program_header_entry_number; i++) {
        struct ProgramHeader program_header;
        uint32_t program_header_read_count = 0;
        uint32_t program_segment_read_count = 0;

        offset = elf_header.program_header_offset + i * elf_header.program_header_entry_size;

        // Seek to the start of the program header
        fr = f_lseek(&file, offset);
        if (FR_OK != fr) {
            printf("Error seeking program header!\n");
        }

        fr = f_read(&file, &program_header, sizeof(struct ProgramHeader), (UINT*)&program_header_read_count);
        if (FR_OK != fr || program_header_read_count != sizeof(struct ProgramHeader)) {
            printf("Error reading program header entry %d!\n", i);
        }

        printf("PH%d: 0x%X\n", i, program_header.paddr);

        fr = f_lseek(&file, program_header.offset);
        if (fr != FR_OK) {
            printf("Error seeking program segment! Index: %d, Offset:0x%X\n", i, program_header.offset);
        }
        fr = f_read(&file, (uint8_t*)program_header.paddr, program_header.filesz, (UINT*)&program_segment_read_count);
        if (fr != FR_OK | program_header.filesz != program_segment_read_count) {
            printf(
                "Error loading program segment! Index: %d, Offset:0x%X, Size: %d\n",
                i,
                program_header.offset,
                program_header.filesz
            );
        }
    }

    // Close the file
    fr = f_close(&file);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    // Unmount the SD card
    f_unmount("");

    
    asm volatile (
            "bx %0\n" 
            :
            : "r" (elf_header.entry)
        );
}
