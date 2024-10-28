#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>

#include "atari2600_conf.h"

#define NMI_ADDR_LO 0xFFFA
#define NMI_ADDR_HI 0xFFFB

#define RES_ADDR_LO 0xFFFC
#define RES_ADDR_HI 0xFFDD

#define IRQ_ADDR_LO 0xFFFE
#define IRQ_ADDR_HI 0xFFFF

#define PUSH_8(value) \
    memory_write((_cpu.sp--)|0x100, value)
#define PUSH_16(value) \
    memory_write((_cpu.sp--)|0x100, (value) & 0xff); \
    memory_write((_cpu.sp--)|0x100, (value)>>8)

#define PULL_8() \
    memory_read((++_cpu.sp)|0x100)
#define PULL_16() \
    (_cpu.sp -= 2, (memory_read((_cpu.sp + 1)|0x100) << 8) | \
    memory_read((_cpu.sp)|0x100))

/*
 * @brief CPU instruction handler
 */ 
typedef void(*cpu_instruction_handler_t)();

/*
 * @brief An enum containing the CPU flags with their respective bitmask values
 */
typedef enum {
    CPU_FLAG_CARRY = 1 << 0,
    CPU_FLAG_ZERO = 1 << 1,
    CPU_FLAG_INTERRUPT = 1 << 2,
    CPU_FLAG_DECIMAL = 1 << 3,
    CPU_FLAG_BREAK = 1 << 4,
    CPU_FLAG_UNUSED = 1 << 5,
    CPU_FLAG_OVERFLOW = 1 << 6,
    CPU_FLAG_NEGATIVE = 1 << 7
} cpu_flag_t;

/*
 * @brief CPU instruction
 *
 * @param handler The instruction handler
 * @param cycles The number of cycles the instruction takes to execute
 */
typedef struct {
    cpu_instruction_handler_t handler;
    uint8_t cycles;
} cpu_instruction_t;

/*
 * @brief CPU state
 *
 * @param pc The program counter
 * @param sp The stack pointer
 * @param a The accumulator
 * @param x The x register
 * @param y The y register
 * @param flags The CPU flags
 */
typedef struct {
    uint16_t pc;
    uint8_t sp;
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t flags;
} cpu_t;

#ifdef ATARI2600_CONF_CPU_ENABLE

/*
 * @brief Initialize the CPU
 */
void cpu_init();

/*
 * @brief Reset the CPU
 */
void cpu_reset();

/*
 * @brief Step the CPU
 */
void cpu_step();

/*
 * @brief Fetch an immediate value
 *
 * @return The immediate value
 */
uint8_t cpu_fetch_imm();

/*
 * @brief Fetch an absolute value
 *
 * @return The absolute value
 */
uint16_t cpu_fetch_abs();

/*
 * @brief Fetch an absolute value with the x register
 *
 * @return The absolute value with the x register
 */
uint16_t cpu_fetch_absx();

/*
 * @brief Fetch an absolute value with the y register
 *
 * @return The absolute value with the y register
 */
uint16_t cpu_fetch_absy();

/*
 * @brief Fetch a zero page value
 *
 * @return The zero page value
 */
uint8_t cpu_fetch_zp();

/*
 * @brief Fetch a zero page value with the x register
 *
 * @return The zero page value with the x register
 */
uint8_t cpu_fetch_zpx();

/*
 * @brief Fetch a zero page value with the y register
 *
 * @return The zero page value with the y register
 */
uint8_t cpu_fetch_zpy();


/*
 * @brief Fetch an indirect value
 *
 * @return The indexed indirect value
 */
uint16_t cpu_fetch_ind();

/*
 * @brief Fetch an indexed indirect value
 *
 * @return The indexed indirect value
 */
uint16_t cpu_fetch_indx();

/*
 * @brief Fetch an indirect indexed value
 *
 * @return The indirect indexed value
 */
uint16_t cpu_fetch_indy();

/*
 * @brief Set cpu flag
 *
 * @param mask Flag mask to set
 * @param value The value of the flag
 */
void cpu_set_flag(const cpu_flag_t mask, const uint8_t value);

/*
 * @brief Get cpu flag
 *
 * @param mask Flag mask to get
 */
uint8_t cpu_get_flag(const cpu_flag_t mask);

static void _cpu_adc_imm();
static void _cpu_adc_zp();
static void _cpu_adc_zpx();
static void _cpu_adc_abs();
static void _cpu_adc_absx();
static void _cpu_adc_absy();
static void _cpu_adc_indx();
static void _cpu_adc_indy();
static void _cpu_and_imm();
static void _cpu_and_zp();
static void _cpu_and_zpx();
static void _cpu_and_abs();
static void _cpu_and_absx();
static void _cpu_and_absy();
static void _cpu_and_indx();
static void _cpu_and_indy();
static void _cpu_asl_a();
static void _cpu_asl_zp();
static void _cpu_asl_zpx();
static void _cpu_asl_abs();
static void _cpu_asl_absx();
static void _cpu_bcc();
static void _cpu_bcs();
static void _cpu_beq();
static void _cpu_bit_zp();
static void _cpu_bit_abs();
static void _cpu_bmi();
static void _cpu_bne();
static void _cpu_bpl();
static void _cpu_brk();
static void _cpu_bvc();
static void _cpu_bvs();
static void _cpu_clc();
static void _cpu_cld();
static void _cpu_cli();
static void _cpu_clv();
static void _cpu_cmp_imm();
static void _cpu_cmp_zp();
static void _cpu_cmp_zpx();
static void _cpu_cmp_abs();
static void _cpu_cmp_absx();
static void _cpu_cmp_absy();
static void _cpu_cmp_indx();
static void _cpu_cmp_indy();
static void _cpu_cpx_imm();
static void _cpu_cpx_zp();
static void _cpu_cpx_abs();
static void _cpu_cpy_imm();
static void _cpu_cpy_zp();
static void _cpu_cpy_abs();
static void _cpu_dec_zp();
static void _cpu_dec_zpx();
static void _cpu_dec_abs();
static void _cpu_dec_absx();
static void _cpu_dex();
static void _cpu_dey();
static void _cpu_eor_imm();
static void const _cpu_eor_zp();
static void _cpu_eor_zpx();
static void _cpu_eor_abs();
static void _cpu_eor_absx();
static void _cpu_eor_absy();
static void _cpu_eor_indx();
static void _cpu_eor_indy();
static void _cpu_inc_zp();
static void _cpu_inc_zpx();
static void _cpu_inc_abs();
static void _cpu_inc_absx();
static void _cpu_inx();
static void _cpu_iny();
static void _cpu_jmp_abs();
static void _cpu_jmp_ind();
static void _cpu_jsr();
static void _cpu_lda_imm();
static void _cpu_lda_zp();
static void _cpu_lda_zpx();
static void _cpu_lda_abs();
static void _cpu_lda_absx();
static void _cpu_lda_absy();
static void _cpu_lda_indx();
static void _cpu_lda_indy();
static void _cpu_ldx_imm();
static void _cpu_ldx_zp();
static void _cpu_ldx_zpy();
static void _cpu_ldx_abs();
static void _cpu_ldx_absy();
static void _cpu_ldy_imm();
static void _cpu_ldy_zp();
static void _cpu_ldy_zpx();
static void _cpu_ldy_abs();
static void _cpu_ldy_absx();
static void _cpu_lsr_a();
static void _cpu_lsr_zp();
static void _cpu_lsr_zpx();
static void _cpu_lsr_abs();
static void _cpu_lsr_absx();
static void _cpu_nop();
static void _cpu_ora_imm();
static void _cpu_ora_zp();
static void _cpu_ora_zpx();
static void _cpu_ora_abs();
static void _cpu_ora_absx();
static void _cpu_ora_absy();
static void _cpu_ora_indx();
static void _cpu_ora_indy();
static void _cpu_pha();
static void _cpu_php();
static void _cpu_pla();
static void _cpu_plp();
static void _cpu_rol_a();
static void _cpu_rol_zp();
static void _cpu_rol_zpx();
static void _cpu_rol_abs();
static void _cpu_rol_absx();
static void _cpu_ror_a();
static void _cpu_ror_zp();
static void _cpu_ror_zpx();
static void _cpu_ror_abs();
static void _cpu_ror_absx();
static void _cpu_rti();
static void _cpu_rts();
static void _cpu_sbc_imm();
static void _cpu_sbc_zp();
static void _cpu_sbc_zpx();
static void _cpu_sbc_abs();
static void _cpu_sbc_absx();
static void _cpu_sbc_absy();
static void _cpu_sbc_indx();
static void _cpu_sbc_indy();
static void _cpu_sec();
static void _cpu_sed();
static void _cpu_sei();
static void _cpu_sta_zp();
static void _cpu_sta_zpx();
static void _cpu_sta_abs();
static void _cpu_sta_absx();
static void _cpu_sta_absy();
static void _cpu_sta_indx();
static void _cpu_sta_indy();
static void _cpu_stx_zp();
static void _cpu_stx_zpy();
static void _cpu_stx_abs();
static void _cpu_sty_zp();
static void _cpu_sty_zpx();
static void _cpu_sty_abs();
static void _cpu_tax();
static void _cpu_tay();
static void _cpu_tsx();
static void _cpu_txa();
static void _cpu_txs();
static void _cpu_tya();

#else

#define cpu_init(cpu) (NULL)
#define cpu_reset(cpu) (NULL)
#define cpu_step(cpu) (NULL)

#endif // MODULE_CPU_ENABLE
#endif // __CPU_H__
