#include "atari2600_conf.h"
#include "memory.h"

#ifdef ATARI2600_CONF_MEMORY_ENABLE

#include <string.h>

static memory_t _memory;

void memory_init() {
    memset(&_memory, 0, sizeof(memory_t));
}

void memory_reset() {
    memory_init();
}

void memory_write(const uint16_t address, const uint8_t data) {
    if (address <= MEMORY_RAM_BASE + MEMORY_RAM_MIRROR_SIZE) {
        
    } else if (
            address >= MEMORY_PPU_REG_BASE &&
            address <= MEMORY_PPU_REG_BASE + MEMORY_PPU_REG_SIZE) {

    } else if (
            address >= MEMORY_PPU_REG_BASE &&
            address <= MEMORY_PPU_REG_BASE + MEMORY_PPU_REG_MIRROR_SIZE) {

    } else if (
            address >= MEMORY_APU_IO_REG_BASE &&
            address <= MEMORY_APU_IO_REG_BASE + MEMORY_APU_IO_REG_SIZE) {

    } else {
    
    }
}

uint8_t memory_read(const uint16_t address) {

    if (address <= MEMORY_RAM_BASE + MEMORY_RAM_MIRROR_SIZE) {
        
    } else if (
            address >= MEMORY_PPU_REG_BASE &&
            address <= MEMORY_PPU_REG_BASE + MEMORY_PPU_REG_SIZE) {

    } else if (
            address >= MEMORY_PPU_REG_BASE &&
            address <= MEMORY_PPU_REG_BASE + MEMORY_PPU_REG_MIRROR_SIZE) {

    } else if (
            address >= MEMORY_APU_IO_REG_BASE &&
            address <= MEMORY_APU_IO_REG_BASE + MEMORY_APU_IO_REG_SIZE) {

    } 

    return _memory.cartridge[address - MEMORY_CARTRIDGE_BASE];
}

#endif // MODULE_MEMORY_ENABLE
