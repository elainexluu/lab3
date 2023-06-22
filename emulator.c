#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"
#include "utils.h"
#include "riscv.h"

void execute_rtype(Instruction, Processor *);
void execute_itype_except_load(Instruction, Processor *);
void execute_branch(Instruction, Processor *);
void execute_jal(Instruction, Processor *);
void execute_load(Instruction, Processor *, Byte *);
void execute_store(Instruction, Processor *, Byte *);
void execute_ecall(Processor *, Byte *);
void execute_lui(Instruction, Processor *);

void execute_instruction(uint32_t instruction_bits, Processor *processor,Byte *memory) {    
    Instruction instruction = parse_instruction(instruction_bits);
    switch(instruction.opcode) {
        case 0x33:
            execute_rtype(instruction, processor);
            break;
        case 0x13:
            execute_itype_except_load(instruction, processor);
            break;
        case 0x73:
            execute_ecall(processor, memory);
            break;
        case 0x63:
            execute_branch(instruction, processor);
            break;
        case 0x6F:
            execute_jal(instruction, processor);
            break;
        case 0x23:
            execute_store(instruction, processor, memory);
            break;
        case 0x03:
            execute_load(instruction, processor, memory);
            break;
        case 0x37:
            execute_lui(instruction, processor);
            break;
        default: // undefined opcode
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_rtype(Instruction instruction, Processor *processor) {
    switch (instruction.rtype.funct3){
        case 0x0:
            switch (instruction.rtype.funct7) {
                case 0x0:
                    // Add
                    processor->R[instruction.rtype.rd] =
                        ((sWord)processor->R[instruction.rtype.rs1]) +
                        ((sWord)processor->R[instruction.rtype.rs2]);
                    break;
                case 0x1:
                    // Mul
                    processor->R[instruction.rtype.rd] =
                        ((sWord)processor->R[instruction.rtype.rs1]) *
                        ((sWord)processor->R[instruction.rtype.rs2]);
                    break;
                case 0x20:
                    // Sub
                    processor->R[instruction.rtype.rd] =
                        ((sWord)processor->R[instruction.rtype.rs1]) -
                        ((sWord)processor->R[instruction.rtype.rs2]);
                    break;
                default:
                    handle_invalid_instruction(instruction);
                    exit(-1);
                    break;
            }
            break;
        /* YOUR CODE HERE */
	/* deal with other cases */
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
    // update PC
    processor->PC += 4;
}

void execute_itype_except_load(Instruction instruction, Processor *processor) {

    int result = (instruction.itype.imm  >> 5) & 0x3f;
    
    switch (instruction.itype.funct3) {  
      /* YOUR CODE HERE */
        case 0x0:
            //print_itype_except_load("addi", instruction, instruction.itype.imm);

            processor->R[instruction.itype.rd] =
                ((sWord)processor->R[instruction.itype.rs1]) +
                ((sWord)processor->R[instruction.itype.imm]);

            break;

        case 0x1:
            //print_itype_except_load("slli", instruction, instruction.itype.imm);

            processor->R[instruction.itype.rd] = 
                ((sWord)instruction.itype.rs1) <<
                (((sWord)instruction.itype.imm) & 0x1F);

            break;

        case 0x2:
            //print_itype_except_load("slti", instruction, instruction.itype.imm);

            processor->R[instruction.itype.rd] = 
                ( ((sWord)instruction.itype.rs1) < ((sWord)instruction.itype.imm) ) ? 1:0;

            break;

        case 0x4:
            //print_itype_except_load("xori", instruction, instruction.itype.imm);

            processor->R[instruction.itype.rd] =
                ((sWord)instruction.itype.rs1) ^
                ((sWord)instruction.itype.imm);

            break;

        case 0x5:
            switch (result) {
                case 0x00:
                    //print_itype_except_load("srli", instruction, instruction.itype.imm);

                    processor->R[instruction.itype.rd] = 
                        ((sWord)instruction.itype.rs1) >>
                        (((sWord)instruction.itype.imm) & 0x1F);

                    break;

                case 0x20:
                   // print_itype_except_load("srai", instruction, (instruction.itype.imm & 0x1F));
                   
                    processor->R[instruction.itype.rd] = 
                        ((sWord)instruction.itype.rs1) >>
                        (((sWord)instruction.itype.imm) & 0x1F);

                    break;

                default:
                    handle_invalid_instruction(instruction);
                break;
            }
            break;
        
        case 0x6:
            //print_itype_except_load("ori", instruction, instruction.itype.imm);

            processor->R[instruction.itype.rd] = 
                ((sWord)instruction.itype.rs1) |
                ((sWord)instruction.itype.imm);

            break;

        case 0x7:
            //print_itype_except_load("andi", instruction, instruction.itype.imm);

            processor->R[instruction.itype.rd] = 
                ((sWord)instruction.itype.rs1) &
                ((sWord)instruction.itype.imm);

            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

void execute_ecall(Processor *p, Byte *memory) {
    Register i;
    
    // syscall number is given by a0 (x10)
    // argument is given by a1
    switch(p->R[10]) {
        case 1: // print an integer
            printf("%d",p->R[11]);
            p->PC += 4;
            break;
        case 4: // print a string
            for(i=p->R[11];i<MEMORY_SPACE && load(memory,i,LENGTH_BYTE);i++) {
                printf("%c",load(memory,i,LENGTH_BYTE));
            }
            p->PC += 4;
            break;
        case 10: // exit
            printf("exiting the simulator\n");
            exit(0);
            break;
        case 11: // print a character
            printf("%c",p->R[11]);
            p->PC += 4;
            break;
        default: // undefined ecall
            printf("Illegal ecall number %d\n", p->R[10]);
            exit(-1);
            break;
    }
}

void execute_branch(Instruction instruction, Processor *processor) {
    switch (instruction.sbtype.funct3) {
        /* YOUR CODE HERE */
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_load(Instruction instruction, Processor *processor, Byte *memory) {
    switch (instruction.itype.funct3) {
        /* YOUR CODE HERE */
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

void execute_store(Instruction instruction, Processor *processor, Byte *memory) {
    switch (instruction.stype.funct3) {
        /* YOUR CODE HERE */
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_jal(Instruction instruction, Processor *processor) {
    /* YOUR CODE HERE */
}

void execute_lui(Instruction instruction, Processor *processor) {
    /* YOUR CODE HERE */
}

void store(Byte *memory, Address address, Alignment alignment, Word value) {
    /* YOUR CODE HERE */
}

Word load(Byte *memory, Address address, Alignment alignment) {
    if(alignment == LENGTH_BYTE) {
        return memory[address];
    } else if(alignment == LENGTH_HALF_WORD) {
        return (memory[address+1] << 8) + memory[address];
    } else if(alignment == LENGTH_WORD) {
        return (memory[address+3] << 24) + (memory[address+2] << 16)
               + (memory[address+1] << 8) + memory[address];
    } else {
        printf("Error: Unrecognized alignment %d\n", alignment);
        exit(-1);
    }
}
