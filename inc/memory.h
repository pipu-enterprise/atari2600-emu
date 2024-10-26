#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

#include "atari2600_conf.h"

#define MEMORY_TIA_REG_BASE 0x0000
#define MEMORY_TIA_REG_SIZE 0x007f

#define MEMORY_RAM_BASE 0x0080
#define MEMORY_RAM_SIZE 0x007f

#define MEMORY_RIOT_REG_BASE 0x0200
#define MEMORY_RIOT_REG_SIZE 0x00ff

#define MEMORY_ROM_BASE 0x1000
#define MEMORY_ROM_SIZE 0x0fff

typedef struct {
    uint8_t ram[MEMORY_RAM_SIZE];
    uint8_t rom[MEMORY_ROM_SIZE];
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
void memory_write(uint16_t address, uint8_t data);
 
/*
 * @brief Read data from memory
 *
 * @param address The address to read from
 *
 * @return The data read
 */
uint8_t memory_read(uint16_t address);

#else

#endif // MODULE_MEMORY_ENABLE

#endif // __MEMORY_H__
