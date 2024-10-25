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

void memory_write(uint16_t address, uint8_t data) {
    if (address <= MEMORY_TIA_REG_SIZE) {
        // TODO: TIA registers
    } else if (
            address >= MEMORY_RAM_BASE && 
            address <= MEMORY_RAM_BASE + MEMORY_RAM_SIZE) {
        // RAM
        _memory.ram[address - MEMORY_RAM_BASE] = data;
    } else if (
            address >= MEMORY_RIOT_REG_BASE && 
            address <= MEMORY_RIOT_REG_BASE + MEMORY_RIOT_REG_SIZE) {
        // TODO: RIOT registers
    } else if (
            address >= MEMORY_ROM_BASE && 
            address <= MEMORY_ROM_BASE + MEMORY_ROM_SIZE) {
        // ROM
        _memory.rom[address - MEMORY_ROM_BASE] = data;
    }
}

uint8_t memory_read(uint16_t address) {

    if (address <= MEMORY_TIA_REG_SIZE) {
        // TODO: TIA registers
        return 0;
    } else if (
            address >= MEMORY_RAM_BASE && 
            address <= MEMORY_RAM_BASE + MEMORY_RAM_SIZE) {
        // RAM
        return _memory.ram[address - MEMORY_RAM_BASE];
    } else if (
            address >= MEMORY_RIOT_REG_BASE && 
            address <= MEMORY_RIOT_REG_BASE + MEMORY_RIOT_REG_SIZE) {
        // TODO: RIOT registers
        return 0;
    } 

    // ROM
    return _memory.rom[address - MEMORY_ROM_BASE];
}

#endif // MODULE_MEMORY_ENABLE
