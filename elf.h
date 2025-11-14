#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <stdint.h>

#define IDENTIFIER_LENGTH 16

struct ElfHeader {
    uint8_t identifier[IDENTIFIER_LENGTH];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t program_header_offset;
    uint32_t section_header_offset;
    uint32_t flags;
    uint16_t elf_header_size;
    uint16_t program_header_entry_size;
    uint16_t program_header_entry_number;
    uint16_t section_header_entry_size;
    uint16_t section_header_entry_number;
    uint16_t string_table_section_index;
};

struct SectionHeader {
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t address;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t address_alignment;
    uint32_t entry_size;
};

struct ProgramHeader {
    uint32_t type;
    uint32_t offset;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t filesz;
    uint32_t memsz;
    uint32_t flags;
    uint32_t align;
};


#endif
