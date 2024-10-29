#ifndef __CARTRIDGE_H__
#define __CARTRIDGE_H__

#include "nes_conf.h"

#include <stdint.h>

#define CARTRIDGE_START 0x6000U
#define CARTRIDGE_SIZE 0xA000U

/*
 * @brief NROM cartridge memory map
 */
#define CARTRIDGE_NROM_RAM_START 0x6000U
#define CARTRIDGE_NROM_RAM_SIZE 0x2000U
#define CARTRIDGE_NROM_ROM_START 0x8000U
#define CARTRIDGE_NROM_ROM_SIZE 0x8000U


/*
 * @brief Cartridge types
 *
 * @value CARTRIDGE_TYPE_NROM  
 */
typedef enum {
    CARTRIDGE_TYPE_NROM = 0x00,
} cartridge_type_e;

/*
 * @brief Cartridge write handler function
 *
 * @param address Address to write to
 * @param data Data to write
 */
typedef void(*cartridge_write_handler_t)(uint16_t address, uint8_t data);

/*
 * @brief Cartridge read handler function
 *
 * @param address Address to read from
 *
 * @return Data read
 */
typedef uint8_t(*cartridge_read_handler_t)(uint16_t address);

/*
 * @brief NROM cartridge data
 *
 * @warning This structure should not be used outside of the cartridge module
 *
 * @attribute mem Cartridge memory
 */
typedef struct {
    uint8_t mem[0xA000U];
} _cartridge_nrom_t;

/*
 * @brief Cartridge data
 *
 * @warning This structure should not be used outside of the cartridge module
 *
 * @attribute type Cartridge type
 * @attribute write Write handler
 * @attribute read Read handler
 * @attribute data Cartridge data
 */
typedef struct {

    cartridge_type_e type;

    cartridge_write_handler_t write;
    cartridge_read_handler_t read;

    union {
        _cartridge_nrom_t nrom;
    } data;

} cartridge_t;

#ifdef NES_CONF_CARTRIDGE_ENABLE

/*
 * @brief Initialize the cartridge
 */
void cartridge_init(cartridge_type_e type);

/*
 * @brief Write to the cartridge
 *
 * @param address Address to write to
 * @param data Data to write
 */
void cartridge_write(uint16_t address, uint8_t data);

/*
 * @brief Read from the cartridge
 *
 * @param address Address to read from
 *
 * @return Data read
 */
uint8_t cartridge_read(uint16_t address);

#else

#define cartridge_init() (NULL)
#define cartridge_write(address, data) (NULL)
#define cartridge_read(address) (0U)

#endif //NES_CONF_CARTRIDGE_ENABLE

#endif //__CARTRIDGE_H__
