#include "cartridge.h"

#include <string.h>

#ifdef ATARI2600_CONF_CARTRIDGE_ENABLE

/*
 * @brief Cartridge data
 */
static cartridge_t _cartridge;

/*
 * @brief NROM cartridge write handler
 */
static void _cartridge_nrom_write(uint16_t address, uint8_t data) {
    if (address >= CARTRIDGE_NROM_RAM_START && 
        address < CARTRIDGE_NROM_RAM_START + CARTRIDGE_NROM_RAM_SIZE) {

        _cartridge.data.nrom.mem[address - CARTRIDGE_NROM_RAM_START] = data;
    } 
}

/*
 * @brief NROM cartridge read handler
 */
static uint8_t _cartridge_nrom_read(uint16_t address) {
    return _cartridge.data.nrom.mem[address - CARTRIDGE_START];
}

/*
 * @brief Structure to hold cartridge handlers
 *
 * @attribute write Write handler
 * @attribute read Read handler
 */
typedef struct {
    cartridge_write_handler_t write;
    cartridge_read_handler_t read;
} cartridge_handler_t;

/*
 * @brief Cartridge handlers
 */
static const cartridge_handler_t _cartridge_handlers[] = {
    [CARTRIDGE_TYPE_NROM] = {
        .write = _cartridge_nrom_write,
        .read = _cartridge_nrom_read
    }
};

void cartridge_init(cartridge_type_e type) {

    memset(&_cartridge, 0, sizeof(cartridge_t));

    _cartridge.type = type;

    _cartridge.write = _cartridge_handlers[type].write;
    _cartridge.read = _cartridge_handlers[type].read;
}

void cartridge_write(uint16_t address, uint8_t data) {
    _cartridge.write(address, data);
}

uint8_t cartridge_read(uint16_t address) {
    return _cartridge.read(address);
}

#endif //ATARI2600_CONF_CARTRIDGE_ENABLE
