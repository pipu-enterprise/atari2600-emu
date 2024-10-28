#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

#include "atari2600_conf.h"
#include "cartridge.h"

#define MEMORY_RAM_BASE 0x0000
#define MEMORY_RAM_SIZE 0x0800
#define MEMORY_RAM_MIRROR_SIZE 0x1800

#define MEMORY_PPU_REG_BASE 0x2000
#define MEMORY_PPU_REG_SIZE 0x0008
#define MEMORY_PPU_REG_MIRROR_SIZE 0x1FF8

#define MEMORY_APU_IO_REG_BASE 0x4000
#define MEMORY_APU_IO_REG_SIZE 0x0020

#define MEMORY_CARTRIDGE_BASE 0x4020
#define MEMORY_CARTRIDGE_SIZE 0xBFE0

typedef struct {
    uint8_t ram[MEMORY_RAM_SIZE];
    cartridge_t cartridge;
} memory_t;

#ifdef ATARI2600_CONF_MEMORY_ENABLE

/*
 * @brief Initialize the memory
 */
void memory_init();

/*
 * @brief Reset the memory
 */
void memory_reset();

/*
 * @brief Write data to memory
 *
 * @param address The address to write to
 * @param data The data to write
 */
void memory_write(const uint16_t address, const uint8_t data);
 
/*
 * @brief Read data from memory
 *
 * @param address The address to read from
 *
 * @return The data read
 */
uint8_t memory_read(const uint16_t address);

#else

#endif // MODULE_MEMORY_ENABLE

#endif // __MEMORY_H__
