#!/bin/python

import argparse
import sys

JUMP_OPCODE = 15
MEM_ACCESS_OPCODES = [10, 11, 12, 13]

def get_mnemonic(opcode, jc):
    if opcode != JUMP_OPCODE:
        return ['SL', 'SR', 'AND', 'OR', 'REV', 'XOR', 'ADD', 'SUB', 'MUL', 'DIV', 'LH', 'SH', 'LW', 'SW', 'STU'][opcode]
    else:
        return ['J', 'JGT', 'JEQ', 'JLT', 'JLE', 'JNE', 'JGE', 'JAL'][jc]

def get_register_name(reg_num):
    return '$' + ['zero', 'jc', 'sp', 'fp', 'v0', 'v1', 'ra', 'a0', 'a1', 'a2', 'a3', 'a4',
            't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7', 't8', 't9', 't10',
            't11', 't12', 't13', 't14', 't15', 't16', 't17', 't18', 't19'][reg_num]

def get_format_string(opcode, has_immediate):
    if opcode == JUMP_OPCODE:
        if has_immediate:
            return '{mne} {addr}'
        else:
            return '{mne} {ri}'
    elif opcode in MEM_ACCESS_OPCODES:
        if has_immediate:
            return '{mne} {rd} {imm}({rs})'
        else:
            return '{mne} {rd} {ri}({rs})'
    else:
        if has_immediate:
            return '{mne} {rd} {rs} {imm}'
        else:
            return '{mne} {rd} {rs} {ri}'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('hex_file', help='path to a Logisim hex file produced by Ida.jar')
    parser.add_argument('-l', '--no-line-nums', action='store_true', help='disable line numbers in the output')
    args = parser.parse_args()

    with open(args.hex_file) as f:
        if f.readline().strip() != 'v2.0 raw':
            sys.exit('Unexpected file format.')

        instructions = [int(x, 16) for x in f.read().split()]

        pc = 0

        for instruction in instructions:
            opcode = (instruction >> 28) & 0b1111
            has_immediate = (instruction >> 27) & 1
            rd = (instruction >> 22) & 0b11111
            rs = (instruction >> 17) & 0b11111
            ri = instruction & 0b11111
            jc = (instruction >> 24) & 0b111
            imm = instruction & 0b11111111111111111 # 17 ones
            addr = instruction & 0b111111111111111111111111 # 24 ones

            format_string = get_format_string(opcode, has_immediate)
            line = format_string.format(
                mne=get_mnemonic(opcode, jc),
                rd=get_register_name(rd),
                rs=get_register_name(rs),
                ri=get_register_name(ri),
                imm='0x%x' % imm,
                addr='0x%06x' % addr
            )

            if not args.no_line_nums:
                line = ('%06x: ' % pc) + line

            print(line)

            pc += 1

if __name__ == '__main__':
    main()

