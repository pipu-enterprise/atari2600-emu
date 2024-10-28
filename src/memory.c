#include "atari2600_conf.h"
#include "memory.h"

#ifdef ATARI2600_CONF_MEMORY_ENABLE

#include <string.h>

static memory_t _memory;

void memory_init() {
    memset(_memory.ram, 0, sizeof(_memory.ram));
}

void memory_reset() {
    memory_init();
}

void memory_write(uint16_t address, uint8_t data) {
    if (address <= MEMORY_RAM_BASE + MEMORY_RAM_MIRROR_SIZE) {
        address = (address - MEMORY_RAM_BASE) % MEMORY_RAM_SIZE;
        _memory.ram[address] = data;
    } else if (
            address >= MEMORY_PPU_REG_BASE &&
            address <= MEMORY_PPU_REG_BASE + MEMORY_PPU_REG_SIZE) {
        address = (address - MEMORY_PPU_REG_BASE) % MEMORY_PPU_REG_SIZE;
        /** @todo */

    } else if (
            address >= MEMORY_APU_IO_REG_BASE &&
            address <= MEMORY_APU_IO_REG_BASE + MEMORY_APU_IO_REG_SIZE) {
        address = (address - MEMORY_APU_IO_REG_BASE) % MEMORY_APU_IO_REG_SIZE;
        /** @todo */
        
    } else {
        _memory.cartridge.cart_write(address, data);
    }
}

uint8_t memory_read(uint16_t address) {

    if (address <= MEMORY_RAM_BASE + MEMORY_RAM_MIRROR_SIZE) {
        address = (address - MEMORY_RAM_BASE) % MEMORY_RAM_SIZE;
        return _memory.ram[address];
    } else if (
            address >= MEMORY_PPU_REG_BASE &&
            address <= MEMORY_PPU_REG_BASE + MEMORY_PPU_REG_SIZE) {
        address = (address - MEMORY_PPU_REG_BASE) % MEMORY_PPU_REG_SIZE;
        /** @todo */

    } else if (
            address >= MEMORY_APU_IO_REG_BASE &&
            address <= MEMORY_APU_IO_REG_BASE + MEMORY_APU_IO_REG_SIZE) {
        address = (address - MEMORY_APU_IO_REG_BASE) % MEMORY_APU_IO_REG_SIZE;
        /** @todo */

    }
    return _memory.cartridge.cart_read(address);
}

#endif // MODULE_MEMORY_ENABLE
