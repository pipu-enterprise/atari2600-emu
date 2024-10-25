#ifdef ATARI2600_CONF_CPU_ENABLE

#include "cpu.h"
#include "memory.h"

#include <string.h>

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
    // TODO
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


static _cpu_adc_imm() {
    uint
}

#endif // MODULE_CPU_ENABLE
