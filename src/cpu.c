#include "cpu.h"
#include "memory.h"

#include <string.h>

#ifdef ATARI2600_CONF_CPU_ENABLE

static cpu_instruction_t _instr_table[0xff] = {
    // ADC
    [0x69] = { .handler = _cpu_adc_imm, .cycles = 2 },
    [0x65] = { .handler = _cpu_adc_zp, .cycles = 3 },
    [0x75] = { .handler = _cpu_adc_zpx, .cycles = 4 },
    [0x6d] = { .handler = _cpu_adc_abs, .cycles = 4 },
    [0x7d] = { .handler = _cpu_adc_absx, .cycles = 4 },
    [0x79] = { .handler = _cpu_adc_absy, .cycles = 4 },
    [0x61] = { .handler = _cpu_adc_indx, .cycles = 6 },
    [0x71] = { .handler = _cpu_adc_indy, .cycles = 5 },

    // AND
    [0x29] = { .handler = _cpu_and_imm, .cycles = 2 },
    [0x25] = { .handler = _cpu_and_zp, .cycles = 3 },
    [0x35] = { .handler = _cpu_and_zpx, .cycles = 4 },
    [0x2d] = { .handler = _cpu_and_abs, .cycles = 4 },
    [0x3d] = { .handler = _cpu_and_absx, .cycles = 4 },
    [0x39] = { .handler = _cpu_and_absy, .cycles = 4 },
    [0x21] = { .handler = _cpu_and_indx, .cycles = 6 },
    [0x31] = { .handler = _cpu_and_indy, .cycles = 5 },

    // ASL
    [0x0a] = { .handler = _cpu_asl_a, .cycles = 2 },
    [0x06] = { .handler = _cpu_asl_zp, .cycles = 5 },
    [0x16] = { .handler = _cpu_asl_zpx, .cycles = 6 },
    [0x0e] = { .handler = _cpu_asl_abs, .cycles = 6 },
    [0x1e] = { .handler = _cpu_asl_absx, .cycles = 7 },

    // BCC
    [0x90] = { .handler = _cpu_bcc, .cycles = 2 },

    // BCS
    [0xb0] = { .handler = _cpu_bcs, .cycles = 2 },

    // BEQ
    [0xf0] = { .handler = _cpu_beq, .cycles = 2 },

    // BIT
    [0x24] = { .handler = _cpu_bit_zp, .cycles = 3 },
    [0x2c] = { .handler = _cpu_bit_abs, .cycles = 4 },

    // BMI
    [0x30] = { .handler = _cpu_bmi, .cycles = 2 },

    // BNE
    [0xd0] = { .handler = _cpu_bne, .cycles = 2 },

    // BPL
    [0x10] = { .handler = _cpu_bpl, .cycles = 2 },

    // BRK
    [0x00] = { .handler = _cpu_brk, .cycles = 7 },

    // BVC
    [0x50] = { .handler = _cpu_bvc, .cycles = 2 },

    // BVS
    [0x70] = { .handler = _cpu_bvs, .cycles = 2 },

    // CLC
    [0x18] = { .handler = _cpu_clc, .cycles = 2 },

    // CLD
    [0xd8] = { .handler = _cpu_cld, .cycles = 2 },

    // CLI
    [0x58] = { .handler = _cpu_cli, .cycles = 2 },

    // CLV
    [0xb8] = { .handler = _cpu_clv, .cycles = 2 },

    // CMP
    [0xc9] = { .handler = _cpu_cmp_imm, .cycles = 2 },
    [0xc5] = { .handler = _cpu_cmp_zp, .cycles = 3 },
    [0xd5] = { .handler = _cpu_cmp_zpx, .cycles = 4 },
    [0xcd] = { .handler = _cpu_cmp_abs, .cycles = 4 },
    [0xdd] = { .handler = _cpu_cmp_absx, .cycles = 4 },
    [0xd9] = { .handler = _cpu_cmp_absy, .cycles = 4 },
    [0xc1] = { .handler = _cpu_cmp_indx, .cycles = 6 },
    [0xd1] = { .handler = _cpu_cmp_indy, .cycles = 5 },

    // CPX
    [0xe0] = { .handler = _cpu_cpx_imm, .cycles = 2 },
    [0xe4] = { .handler = _cpu_cpx_zp, .cycles = 3 },
    [0xec] = { .handler = _cpu_cpx_abs, .cycles = 4 },

    // CPY
    [0xc0] = { .handler = _cpu_cpy_imm, .cycles = 2 },
    [0xc4] = { .handler = _cpu_cpy_zp, .cycles = 3 },
    [0xcc] = { .handler = _cpu_cpy_abs, .cycles = 4 },

    // DEC
    [0xc6] = { .handler = _cpu_dec_zp, .cycles = 5 },
    [0xd6] = { .handler = _cpu_dec_zpx, .cycles = 6 },
    [0xce] = { .handler = _cpu_dec_abs, .cycles = 6 },
    [0xde] = { .handler = _cpu_dec_absx, .cycles = 7 },

    // DEX
    [0xca] = { .handler = _cpu_dex, .cycles = 2 },

    // DEY
    [0x88] = { .handler = _cpu_dey, .cycles = 2 },

    // EOR
    [0x49] = { .handler = _cpu_eor_imm, .cycles = 2 },
    [0x45] = { .handler = _cpu_eor_zp, .cycles = 3 },
    [0x55] = { .handler = _cpu_eor_zpx, .cycles = 4 },
    [0x4d] = { .handler = _cpu_eor_abs, .cycles = 4 },
    [0x5d] = { .handler = _cpu_eor_absx, .cycles = 4 },
    [0x59] = { .handler = _cpu_eor_absy, .cycles = 4 },
    [0x41] = { .handler = _cpu_eor_indx, .cycles = 6 },
    [0x51] = { .handler = _cpu_eor_indy, .cycles = 5 },

    // INC
    [0xe6] = { .handler = _cpu_inc_zp, .cycles = 5 },
    [0xf6] = { .handler = _cpu_inc_zpx, .cycles = 6 },
    [0xee] = { .handler = _cpu_inc_abs, .cycles = 6 },
    [0xfe] = { .handler = _cpu_inc_absx, .cycles = 7 },

    // INX
    [0xe8] = { .handler = _cpu_inx, .cycles = 2 },

    // INY
    [0xc8] = { .handler = _cpu_iny, .cycles = 2 },

    // JMP
    [0x4c] = { .handler = _cpu_jmp_abs, .cycles = 3 },
    [0x6c] = { .handler = _cpu_jmp_ind, .cycles = 5 },

    // JSR
    [0x20] = { .handler = _cpu_jsr, .cycles = 6 },

    // LDA
    [0xa9] = { .handler = _cpu_lda_imm, .cycles = 2 },
    [0xa5] = { .handler = _cpu_lda_zp, .cycles = 3 },
    [0xb5] = { .handler = _cpu_lda_zpx, .cycles = 4 },
    [0xad] = { .handler = _cpu_lda_abs, .cycles = 4 },
    [0xbd] = { .handler = _cpu_lda_absx, .cycles = 4 },
    [0xb9] = { .handler = _cpu_lda_absy, .cycles = 4 },
    [0xa1] = { .handler = _cpu_lda_indx, .cycles = 6 },
    [0xb1] = { .handler = _cpu_lda_indy, .cycles = 5 },

    // LDX
    [0xa2] = { .handler = _cpu_ldx_imm, .cycles = 2 },
    [0xa6] = { .handler = _cpu_ldx_zp, .cycles = 3 },
    [0xb6] = { .handler = _cpu_ldx_zpy, .cycles = 4 },
    [0xae] = { .handler = _cpu_ldx_abs, .cycles = 4 },
    [0xbe] = { .handler = _cpu_ldx_absy, .cycles = 4 },

    // LDY
    [0xa0] = { .handler = _cpu_ldy_imm, .cycles = 2 },
    [0xa4] = { .handler = _cpu_ldy_zp, .cycles = 3 },
    [0xb4] = { .handler = _cpu_ldy_zpx, .cycles = 4 },
    [0xac] = { .handler = _cpu_ldy_abs, .cycles = 4 },
    [0xbc] = { .handler = _cpu_ldy_absx, .cycles = 4 },

    // LSR
    [0x4a] = { .handler = _cpu_lsr_a, .cycles = 2 },
    [0x46] = { .handler = _cpu_lsr_zp, .cycles = 5 },
    [0x56] = { .handler = _cpu_lsr_zpx, .cycles = 6 },
    [0x4e] = { .handler = _cpu_lsr_abs, .cycles = 6 },
    [0x5e] = { .handler = _cpu_lsr_absx, .cycles = 7 },

    // NOP
    [0xea] = { .handler = _cpu_nop, .cycles = 2 },

    // ORA
    [0x09] = { .handler = _cpu_ora_imm, .cycles = 2 },
    [0x05] = { .handler = _cpu_ora_zp, .cycles = 3 },
    [0x15] = { .handler = _cpu_ora_zpx, .cycles = 4 },
    [0x0d] = { .handler = _cpu_ora_abs, .cycles = 4 },
    [0x1d] = { .handler = _cpu_ora_absx, .cycles = 4 },
    [0x19] = { .handler = _cpu_ora_absy, .cycles = 4 },
    [0x01] = { .handler = _cpu_ora_indx, .cycles = 6 },
    [0x11] = { .handler = _cpu_ora_indy, .cycles = 5 },

    // PHA
    [0x48] = { .handler = _cpu_pha, .cycles = 3 },

    // PHP
    [0x08] = { .handler = _cpu_php, .cycles = 3 },

    // PLA
    [0x68] = { .handler = _cpu_pla, .cycles = 4 },

    // PLP
    [0x28] = { .handler = _cpu_plp, .cycles = 4 },

    // ROL
    [0x2a] = { .handler = _cpu_rol_a, .cycles = 2 },
    [0x26] = { .handler = _cpu_rol_zp, .cycles = 5 },
    [0x36] = { .handler = _cpu_rol_zpx, .cycles = 6 },
    [0x2e] = { .handler = _cpu_rol_abs, .cycles = 6 },
    [0x3e] = { .handler = _cpu_rol_absx, .cycles = 7 },

    // ROR
    [0x6a] = { .handler = _cpu_ror_a, .cycles = 2 },
    [0x66] = { .handler = _cpu_ror_zp, .cycles = 5 },
    [0x76] = { .handler = _cpu_ror_zpx, .cycles = 6 },
    [0x6e] = { .handler = _cpu_ror_abs, .cycles = 6 },
    [0x7e] = { .handler = _cpu_ror_absx, .cycles = 7 },

    // RTI
    [0x40] = { .handler = _cpu_rti, .cycles = 6 },

    // RTS
    [0x60] = { .handler = _cpu_rts, .cycles = 6 },

    // SBC
    [0xe9] = { .handler = _cpu_sbc_imm, .cycles = 2 },
    [0xe5] = { .handler = _cpu_sbc_zp, .cycles = 3 },
    [0xf5] = { .handler = _cpu_sbc_zpx, .cycles = 4 },
    [0xed] = { .handler = _cpu_sbc_abs, .cycles = 4 },
    [0xfd] = { .handler = _cpu_sbc_absx, .cycles = 4 },
    [0xf9] = { .handler = _cpu_sbc_absy, .cycles = 4 },
    [0xe1] = { .handler = _cpu_sbc_indx, .cycles = 6 },
    [0xf1] = { .handler = _cpu_sbc_indy, .cycles = 5 },

    // SEC
    [0x38] = { .handler = _cpu_sec, .cycles = 2 },

    // SED
    [0xf8] = { .handler = _cpu_sed, .cycles = 2 },

    // SEI
    [0x78] = { .handler = _cpu_sei, .cycles = 2 },

    // STA
    [0x85] = { .handler = _cpu_sta_zp, .cycles = 3 },
    [0x95] = { .handler = _cpu_sta_zpx, .cycles = 4 },
    [0x8d] = { .handler = _cpu_sta_abs, .cycles = 4 },
    [0x9d] = { .handler = _cpu_sta_absx, .cycles = 5 },
    [0x99] = { .handler = _cpu_sta_absy, .cycles = 5 },
    [0x81] = { .handler = _cpu_sta_indx, .cycles = 6 },
    [0x91] = { .handler = _cpu_sta_indy, .cycles = 6 },

    // STX
    [0x86] = { .handler = _cpu_stx_zp, .cycles = 3 },
    [0x96] = { .handler = _cpu_stx_zpy, .cycles = 4 },
    [0x8e] = { .handler = _cpu_stx_abs, .cycles = 4 },

    // STY
    [0x84] = { .handler = _cpu_sty_zp, .cycles = 3 },
    [0x94] = { .handler = _cpu_sty_zpx, .cycles = 4 },
    [0x8c] = { .handler = _cpu_sty_abs, .cycles = 4 },

    // TAX
    [0xaa] = { .handler = _cpu_tax, .cycles = 2 },

    // TAY
    [0xa8] = { .handler = _cpu_tay, .cycles = 2 },

    // TSX
    [0xba] = { .handler = _cpu_tsx, .cycles = 2 },

    // TXA
    [0x8a] = { .handler = _cpu_txa, .cycles = 2 },

    // TXS
    [0x9a] = { .handler = _cpu_txs, .cycles = 2 },

    // TYA
    [0x98] = { .handler = _cpu_tya, .cycles = 2 },
};

static cpu_t _cpu;

void cpu_init() {
    memset(&_cpu, 0, sizeof(cpu_t));
}

void cpu_reset() {
    /** @todo */
}

void cpu_step() {
    uint8_t opcode = memory_read(_cpu.pc);
    cpu_instruction_t instr = _instr_table[opcode];

    if (instr.handler) {
        instr.handler();
    } 
}

uint8_t cpu_fetch_imm() {
    return memory_read(_cpu.pc++);
}

uint16_t cpu_fetch_abs() {
    uint16_t address = memory_read(_cpu.pc++);
    address |= memory_read(_cpu.pc++) << 8;
    return address;
}

uint16_t cpu_fetch_absx() {
    uint16_t address = memory_read(_cpu.pc++);
    address |= memory_read(_cpu.pc++) << 8;
    address += _cpu.x;
    return address;
}

uint16_t cpu_fetch_absy() {
    uint16_t address = memory_read(_cpu.pc++);
    address |= memory_read(_cpu.pc++) << 8;
    address += _cpu.y;
    return address;
}

uint16_t cpu_fetch_indx() {
    uint16_t address = memory_read(_cpu.pc++);
    address |= memory_read(_cpu.pc++) << 8;
    address += _cpu.x;
    return address;
}

uint16_t cpu_fetch_indy() {
    uint16_t address = memory_read(_cpu.pc++);
    address |= memory_read(_cpu.pc++) << 8;
    address += _cpu.y;
    return address;
}

uint8_t cpu_fetch_zp() {
    return memory_read(_cpu.pc++);
}

uint8_t cpu_fetch_zpx() {
    return (memory_read(_cpu.pc++) + _cpu.x) & 0xff;
}

uint8_t cpu_fetch_zpy() {
    return (memory_read(_cpu.pc++) + _cpu.y) & 0xff;
}

void cpu_set_flag(cpu_flag_t mask, uint8_t value) {
    _cpu.flags &= ~(1 << mask);
    _cpu.flags |= value != 0 << mask;
}

uint8_t cpu_get_flag(cpu_flag_t mask){
    return _cpu.flags & (1<<mask) != 0;
}


static void _cpu_adc_imm() {

    uint8_t operand = cpu_fetch_imm();
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_adc_zp() {
    uint8_t operand = memory_read(cpu_fetch_zp());
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_adc_zpx() {
    uint8_t operand = memory_read(cpu_fetch_zpx());
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_adc_abs() {
    uint8_t operand = memory_read(cpu_fetch_abs());
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_adc_absx() {
    uint8_t operand = memory_read(cpu_fetch_absx());
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_adc_absy() {
    uint8_t operand = memory_read(cpu_fetch_absy());
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_adc_indx() {
    uint8_t operand = memory_read(cpu_fetch_indx());
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_adc_indy() {
    uint8_t operand = memory_read(cpu_fetch_indy());
    uint8_t carry = ((uint16_t)operand + (uint16_t)_cpu.a) >> 8;

    _cpu.a += cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_and_imm() {
    _cpu.a &= cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_zp() {
    _cpu.a &= memory_read(cpu_fetch_zp());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_zpx() {
    _cpu.a &= memory_read(cpu_fetch_zpx());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_abs() {
    _cpu.a &= memory_read(cpu_fetch_abs());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_abs() {
    _cpu.a &= memory_read(cpu_fetch_abs());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_absx() {
    _cpu.a &= memory_read(cpu_fetch_absx());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_absy() {
    _cpu.a &= memory_read(cpu_fetch_absy());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_indx() {
    _cpu.a &= memory_read(cpu_fetch_indx());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_and_indy() {
    _cpu.a &= memory_read(cpu_fetch_indy());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_asl_a() {
    cpu_set_flag(CPU_FLAG_CARRY, _cpu.a >> 7);

    _cpu.a = _cpu.a >> 1;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_asl_zp() {
    uint16_t addr = cpu_fetch_zp();
    uint8_t data = memory_read(addr);

    data = data << 1;
    memory_write(addr, data);
    
    cpu_set_flag(CPU_FLAG_CARRY, data >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, data >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, data == 0);
}

static void _cpu_asl_zpx() {
    uint16_t addr = cpu_fetch_zpx();
    uint8_t data = memory_read(addr);

    data = data << 1;
    memory_write(addr, data);
    
    cpu_set_flag(CPU_FLAG_CARRY, data >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, data >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, data == 0);
}

static void _cpu_asl_abs() {
    uint16_t addr = cpu_fetch_abs();
    uint8_t data = memory_read(addr);

    data = data << 1;
    memory_write(addr, data);
    
    cpu_set_flag(CPU_FLAG_CARRY, data >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, data >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, data == 0);
}

static void _cpu_asl_absx() {
    uint16_t addr = cpu_fetch_absx();
    uint8_t data = memory_read(addr);

    data = data << 1;
    memory_write(addr, data);
    
    cpu_set_flag(CPU_FLAG_CARRY, data >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, data >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, data == 0);
}

static void _cpu_bcc() {
    uint8_t data = cpu_fetch_imm();

    if (cpu_get_flag(CPU_FLAG_CARRY) == 0)
        _cpu.pc = (int)_cpu.pc + (int8_t)data;
}

static void _cpu_bcs() {
    uint8_t data = cpu_fetch_imm();

    if (cpu_get_flag(CPU_FLAG_CARRY) == 1)
        _cpu.pc = (int)_cpu.pc + (int8_t)data;
}

static void _cpu_beq() {
    uint8_t data = cpu_fetch_imm();

    if (cpu_get_flag(CPU_FLAG_ZERO) == 1)
        _cpu.pc = (int)_cpu.pc + (int8_t)data;
}

static void _cpu_bit_zp() {
    uint8_t data = memory_read(cpu_fetch_zp());

    cpu_set_flag(CPU_FLAG_NEGATIVE, data>>7);
    cpu_set_flag(CPU_FLAG_OVERFLOW, data>>6 & 1);
    cpu_set_flag(CPU_FLAG_ZERO, (data & _cpu.a) == 0);
}

static void _cpu_bit_abs() {
    uint8_t data = memory_read(cpu_fetch_abs());

    cpu_set_flag(CPU_FLAG_NEGATIVE, data>>7);
    cpu_set_flag(CPU_FLAG_OVERFLOW, data>>6 & 1);
    cpu_set_flag(CPU_FLAG_ZERO, (data & _cpu.a) == 0);
}

static void _cpu_bmi() {
    uint8_t data = cpu_fetch_imm();

    if (cpu_get_flag(CPU_FLAG_NEGATIVE) == 1)
        _cpu.pc = (int)_cpu.pc + (int8_t)data;
}

static void _cpu_bne() {
    uint8_t data = cpu_fetch_imm();

    if (cpu_get_flag(CPU_FLAG_ZERO) == 0)
        _cpu.pc = (int)_cpu.pc + (int8_t)data;
}

static void _cpu_bpl() {
    uint8_t data = cpu_fetch_imm();

    if (cpu_get_flag(CPU_FLAG_ZERO) != 0)
        _cpu.pc = (int)_cpu.pc + (int8_t)data;
}

static void _cpu_brk() {
    //TODO
}

static void _cpu_bvc(){
    //TODO
}

static void _cpu_bvs() {
    uint8_t data = cpu_fetch_imm();

    if (cpu_get_flag(CPU_FLAG_OVERFLOW) == 1)
        _cpu.pc = (int)_cpu.pc + (int8_t)data;
}

static void _cpu_clc() {
    cpu_set_flag(CPU_FLAG_CARRY, 0);
}

static void _cpu_cld() {
    cpu_set_flag(CPU_FLAG_DECIMAL, 0);
}

static void _cpu_cli() {
    cpu_set_flag(CPU_FLAG_INTERRUPT, 0);
}

static void _cpu_clv() {
    cpu_set_flag(CPU_FLAG_OVERFLOW, 0);
}

static void _cpu_cmp_imm() {
    uint8_t result = _cpu.a - cpu_fetch_imm();

    //TODO Carry
    //cpu_set_flag(CPU_FLAG_CARRY, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_zp() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_zp());

    //TODO Carry
    //cpu_set_flag(CPU_FLAG_CARRY, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_zpx() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_zpx());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_zpy() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_zpy());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_abs() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_abs());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_absx() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_absx());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_absy() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_absy());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_indx() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_indx());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cmp_indy() {
    uint8_t result = _cpu.a - memory_read(cpu_fetch_indy());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cpx_imm() {
    uint8_t result = _cpu.x - cpu_fetch_imm();

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cpx_zp() {
    uint8_t result = _cpu.x - memory_read(cpu_fetch_zp());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cpx_abs() {
    uint8_t result = _cpu.x - memory_read(cpu_fetch_abs());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cpy_imm() {
    uint8_t result = _cpu.y - cpu_fetch_imm();

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cpy_zp() {
    uint8_t result = _cpu.y - memory_read(cpu_fetch_zp());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_cpy_abs() {
    uint8_t result = _cpu.y - memory_read(cpu_fetch_abs());

    //TODO carry
    //cpu_set_flag(cpu_flag_carry, result >> 7);
    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_dec_zp() {
    uint8_t address = cpu_fetch_zp();
    uint8_t result = memory_read(address)-1;

    memory_write(address, result);

    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_dec_zpx() {
    uint8_t address = cpu_fetch_zpx();
    uint8_t result = memory_read(address)-1;

    memory_write(address, result);

    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_dec_abs() {
    uint16_t address = cpu_fetch_abs();
    uint8_t result = memory_read(address)-1;

    memory_write(address, result);

    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_dec_absx() {
    uint16_t address = cpu_fetch_absx();
    uint8_t result = memory_read(address)-1;

    memory_write(address, result);

    cpu_set_flag(CPU_FLAG_NEGATIVE, result >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, result == 0);
}

static void _cpu_dex() {
    _cpu.x--;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.x >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.x == 0);
}

static void _cpu_dey() {
    _cpu.y--;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.y >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.y == 0);
}

static void _cpu_eor_imm() {
    uint8_t value = cpu_fetch_imm();

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_zp() {
    uint8_t value = memory_read(cpu_fetch_zp());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_zpx() {
    uint8_t value = memory_read(cpu_fetch_zpx());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_zpy() {
    uint8_t value = memory_read(cpu_fetch_zpy());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_abs() {
    uint8_t value = memory_read(cpu_fetch_abs());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_absx() {
    uint8_t value = memory_read(cpu_fetch_absx());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_absy() {
    uint8_t value = memory_read(cpu_fetch_absy());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_indx() {
    uint8_t value = memory_read(cpu_fetch_indx());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_eor_indy() {
    uint8_t value = memory_read(cpu_fetch_indy());

    _cpu.a = _cpu.a ^ value;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_inc_zp() {
    uint8_t address = cpu_fetch_zp();
    uint8_t value = memory_read(address) + 1;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_inc_zpx() {
    uint8_t address = cpu_fetch_zpx();
    uint8_t value = memory_read(address) + 1;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_inc_abs() {
    uint8_t address = cpu_fetch_abs();
    uint8_t value = memory_read(address) + 1;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_inc_absx() {
    uint8_t address = cpu_fetch_absx();
    uint8_t value = memory_read(address) + 1;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_inx() {
    _cpu.x++;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.x >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.x == 0);
}

static void _cpu_iny() {
    _cpu.y++;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.y >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.y == 0);
}

static void _cpu_jmp_abs() {
    _cpu.pc = cpu_fetch_abs();
}

static void _cpu_jmp_ind() {
    _cpu.pc = cpu_fetch_ind();
}

static void _cpu_jsr() {
    //TODO fix: pc is 16bit
    memory_write(_cpu.sp++, (_cpu.pc+1) & 0xff);
    memory_write(_cpu.sp++, _cpu.pc>>8);
    _cpu.pc = cpu_fetch_abs();
}

static void _cpu_lda_imm() {
    _cpu.a = cpu_fetch_imm();
}

static void _cpu_lda_zp() {
    _cpu.a = memory_read(cpu_fetch_zp());
}

static void _cpu_lda_zpx() {
    _cpu.a = memory_read(cpu_fetch_zp() + _cpu.x);

}

static void _cpu_lda_abs() {
    _cpu.a = memory_read(cpu_fetch_abs());
}

static void _cpu_lda_absx() {
    _cpu.a = memory_read(cpu_fetch_absx());
}

static void _cpu_lda_absy() {
    _cpu.a = memory_read(cpu_fetch_absy());
}

static void _cpu_lda_indx() {
    _cpu.a = memory_read(cpu_fetch_indx());
}

static void _cpu_lda_indy() {
    _cpu.a = memory_read(cpu_fetch_indy());
}

static void _cpu_lda_imm() {
    _cpu.a = cpu_fetch_imm();
}

static void _cpu_lda_zp() {
    _cpu.a = memory_read(cpu_fetch_zp());
}

static void _cpu_lda_zpx() {
    _cpu.a = memory_read(cpu_fetch_zp() + _cpu.x);
}

static void _cpu_lda_abs() {
    _cpu.a = memory_read(cpu_fetch_abs());
}

static void _cpu_lda_absx() {
    _cpu.a = memory_read(cpu_fetch_absx());
}

static void _cpu_lda_absy() {
    _cpu.a = memory_read(cpu_fetch_absy());
}

static void _cpu_lda_indx() {
    _cpu.a = memory_read(cpu_fetch_indx());
}

static void _cpu_lda_indy() {
    _cpu.a = memory_read(cpu_fetch_indy());
}

static void _cpu_ldx_imm() {
    _cpu.x = cpu_fetch_imm();
}

static void _cpu_ldx_zp() {
    _cpu.x = memory_read(cpu_fetch_zp());
}

static void _cpu_ldx_zpy() {
    _cpu.x = memory_read(cpu_fetch_zp() + _cpu.y);
}

static void _cpu_ldx_abs() {
    _cpu.x = memory_read(cpu_fetch_abs());
}

static void _cpu_ldx_absy() {
    _cpu.x = memory_read(cpu_fetch_absy());
}

static void _cpu_ldy_imm() {
    _cpu.y = cpu_fetch_imm();
}

static void _cpu_ldy_zp() {
    _cpu.y = memory_read(cpu_fetch_zp());
}

static void _cpu_ldy_zpx() {
    _cpu.y = memory_read(cpu_fetch_zp() + _cpu.x);
}

static void _cpu_ldy_abs() {
    _cpu.y = memory_read(cpu_fetch_abs());
}

static void _cpu_ldy_absx() {
    _cpu.y = memory_read(cpu_fetch_absx());
}

static void _cpu_lsr_a() {
    cpu_set_flag(CPU_FLAG_CARRY, _cpu.a & 0x01);
    _cpu.a = _cpu.a >> 1;
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_lsr_zp() {
    uint8_t addr = cpu_fetch_zp();
    uint8_t data = memory_read(addr);
    memory_write(addr, data>>1);
    cpu_set_flag(CPU_FLAG_CARRY, data & 0x01);
    cpu_set_flag(CPU_FLAG_NEGATIVE, 0);
    cpu_set_flag(CPU_FLAG_ZERO, (data>>1) == 0);
}

static void _cpu_lsr_zpx() {
    uint8_t addr = cpu_fetch_zpx();
    uint8_t data = memory_read(addr);
    memory_write(addr, data>>1);
    cpu_set_flag(CPU_FLAG_CARRY, data & 0x01);
    cpu_set_flag(CPU_FLAG_NEGATIVE, 0);
    cpu_set_flag(CPU_FLAG_ZERO, (data>>1) == 0);
}

static void _cpu_lsr_abs() {
    uint8_t addr = cpu_fetch_zpy();
    uint8_t data = memory_read(addr);
    memory_write(addr, data>>1);
    cpu_set_flag(CPU_FLAG_CARRY, data & 0x01);
    cpu_set_flag(CPU_FLAG_NEGATIVE, 0);
    cpu_set_flag(CPU_FLAG_ZERO, (data>>1) == 0);
}

static void _cpu_lsr_absx() {
    uint8_t addr = cpu_fetch_absx();
    uint8_t data = memory_read(addr);
    memory_write(addr, data>>1);
    cpu_set_flag(CPU_FLAG_CARRY, data & 0x01);
    cpu_set_flag(CPU_FLAG_NEGATIVE, 0);
    cpu_set_flag(CPU_FLAG_ZERO, (data>>1) == 0);
}

static void _cpu_nop() {
    // Do nothing
}

static void _cpu_ora_imm() {
    _cpu.a |= cpu_fetch_imm();

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_zp() {
    _cpu.a |= memory_read(cpu_fetch_zp());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_zpx() {
    _cpu.a |= memory_read(cpu_fetch_zpx());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_abs() {
    _cpu.a |= memory_read(cpu_fetch_abs());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_abs() {
    _cpu.a |= memory_read(cpu_fetch_abs());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_absx() {
    _cpu.a |= memory_read(cpu_fetch_absx());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_absy() {
    _cpu.a |= memory_read(cpu_fetch_absy());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_indx() {
    _cpu.a |= memory_read(cpu_fetch_indx());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_ora_indy() {
    _cpu.a |= memory_read(cpu_fetch_indy());

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_pha() {
    memory_write(_cpu.sp++, _cpu.a);
}

static void _cpu_php() {
    memory_write(_cpu.sp++, _cpu.flags);
}

static void _cpu_pla() {
    _cpu.a = memory_read(--_cpu.sp);

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
}

static void _cpu_plp() {
    _cpu.flags = memory_read(--_cpu.sp);
}

static void _cpu_rol_a() {
    uint8_t carry = _cpu.a >> 7;

    _cpu.a = (_cpu.a << 1) | cpu_get_flag(CPU_FLAG_CARRY);

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_rol_zp() {

    uint8_t address = cpu_fetch_zp();
    uint8_t value = memory_read(address);

    uint8_t carry = value >> 7;

    value = (value << 1) | cpu_get_flag(CPU_FLAG_CARRY);

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_rol_zpx() {

    uint8_t address = cpu_fetch_zpx();
    uint8_t value = memory_read(address);

    uint8_t carry = value >> 7;

    value = (value << 1) | cpu_get_flag(CPU_FLAG_CARRY);

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_rol_abs() {

    uint16_t address = cpu_fetch_abs();
    uint8_t value = memory_read(address);

    uint8_t carry = value >> 7;

    value = (value << 1) | carry;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_rol_absx() {

    uint16_t address = cpu_fetch_absx();
    uint8_t value = memory_read(address);

    uint8_t carry = value >> 7;

    value = (value << 1) | cpu_get_flag(CPU_FLAG_CARRY);

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_ror_a() {
    uint8_t carry = _cpu.a & 1;
    _cpu.a = (_cpu.a >> 1) | cpu_get_flag(CPU_FLAG_CARRY) << 7;

    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_ror_zp() {

    uint8_t address = cpu_fetch_zp();
    uint8_t value = memory_read(address);

    uint8_t carry = value & 1;
    value = (value >> 1) | cpu_get_flag(CPU_FLAG_CARRY) << 7;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_ror_zpx() {

    uint8_t address = cpu_fetch_zpx();
    uint8_t value = memory_read(address);

    uint8_t carry = value & 1;
    value = (value >> 1) | cpu_get_flag(CPU_FLAG_CARRY) << 7;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_ror_abs() {

    uint16_t address = cpu_fetch_abs();
    uint8_t value = memory_read(address);

    uint8_t carry = value & 1;
    value = (value >> 1) | cpu_get_flag(CPU_FLAG_CARRY) << 7;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_ror_absx() {

    uint16_t address = cpu_fetch_absx();
    uint8_t value = memory_read(address);

    uint8_t carry = value & 1;
    value = (value >> 1) | cpu_get_flag(CPU_FLAG_CARRY) << 7;

    memory_write(address, value);

    cpu_set_flag(CPU_FLAG_NEGATIVE, value >> 7);
    cpu_set_flag(CPU_FLAG_ZERO, value == 0);
    cpu_set_flag(CPU_FLAG_CARRY, carry);
}

static void _cpu_rti() {
    /** @todo */
}

static void _cpu_rts() {
    /** @todo */
}

static void _cpu_sbc_imm(){
    uint8_t val = cpu_fetch_imm();
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}

static void _cpu_sbc_zp(){
    uint8_t val = memory_read(cpu_fetch_zp());
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}

static void _cpu_sbc_zpx(){
    uint8_t val = memory_read(cpu_fetch_zpx());
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}

static void _cpu_sbc_abs(){
    uint8_t val = memory_read(cpu_fetch_abs());
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}

static void _cpu_sbc_absx(){
    uint8_t val = memory_read(cpu_fetch_absx());
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}

static void _cpu_sbc_absy(){
    uint8_t val = memory_read(cpu_fetch_absy());
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}

static void _cpu_sbc_indx(){
    uint8_t val = memory_read(cpu_fetch_indx());
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}

static void _cpu_sbc_indy(){
    uint8_t val = memory_read(cpu_fetch_indy());
    uint8_t prev = _cpu.a;
    _cpu.a = _cpu.a - val - ~cpu_get_flag(CPU_FLAG_CARRY);
    cpu_set_flag(CPU_FLAG_CARRY, prev >= val);
    cpu_set_flag(CPU_FLAG_NEGATIVE, _cpu.a & 0x80);
    cpu_set_flag(CPU_FLAG_ZERO, _cpu.a == 0);
    /** @todo overflow */
}


#endif // MODULE_CPU_ENABLE
