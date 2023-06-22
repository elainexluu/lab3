#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */
Instruction parse_instruction(uint32_t instruction_bits) {
  /* YOUR CODE HERE */
  Instruction instruction;
  // add x9, x20, x21   hex: 01 5A 04 B3, binary = 0000 0001 0101 1010 0000 0100 1011 0011
  // Opcode: 0110011 (0x33) Get the Opcode by &ing 0x1111111, bottom 7 bits
  instruction.opcode = instruction_bits & ((1U << 7) - 1);

  // Shift right to move to pointer to interpret next fields in instruction.
  instruction_bits >>= 7;

  switch (instruction.opcode) {
  // R-Type
  case 0x33:
    // instruction: 0000 0001 0101 1010 0000 0100 1, destination : 01001
    instruction.rtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0001 0101 1010 0000, func3 : 000
    instruction.rtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0001 0101 1010 0, src1: 10100
    instruction.rtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0001 0101, src2: 10101
    instruction.rtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.rtype.funct7 = instruction_bits & ((1U << 7) - 1);
    break;
  // cases for other types of instructions
  /* YOUR CODE HERE */

  // I-Type
  case 0x03:
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break;
  
  case 0x13:
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break;
  
  case 0x73:
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    break;
  
  ////////////////////////////////////////////////////////////////////////
  // U-Type
  case 0x37:
    instruction.utype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.utype.imm = instruction_bits & ((1U << 20) - 1);
    break;
  
  // UJ-Type
  case 0x6f:
    instruction.ujtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.ujtype.imm = instruction_bits & ((1U << 20) - 1);
    break;
  ////////////////////////////////////////////////////////////////////////
  // S-Type
  case 0x23:
    instruction.stype.imm5 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.stype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.stype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.stype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.stype.imm7 = instruction_bits & ((1U << 7) - 1);
    break;

  // SB-Type
  case 0x63:
    instruction.sbtype.imm5 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.sbtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    instruction.sbtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.sbtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.sbtype.imm7 = instruction_bits & ((1U << 7) - 1);
    break;

  #ifndef TESTING
  default:
    exit(EXIT_FAILURE);
  #endif
  }
  return instruction;
}

/************************Helper functions************************/
/* Here, you will need to implement a few common helper functions, 
 * which you will call in other functions when parsing, printing, 
 * or executing the instructions. */

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */
int sign_extend_number(unsigned int field, unsigned int n) {
  /* YOUR CODE HERE */
  // store the sign bit into var
  unsigned int msb = (field >> (n-1)) & 1;

  if (msb == 1) { // sign bit is negative
    // add leading 0s to create a 32-bit int
    // result gives higher order bits all 1s
    unsigned int append = (1 << (32-n)) - 1;
    // extend the higher order bits into 32-bit int 
    unsigned int extend = append << n;
    // bit mask to set the field number with the extended 32-bit int from above
    unsigned int mask = field | extend;
    return mask;

  } else { // if sign bit is positive (0)
    unsigned int set = (1 << n) - 1;
    unsigned int mask = field & set;
    return mask;
  }

  // return (int)field << (32 - n) >> (32 - n);


  // return 0;
}

/* Return the number of bytes (from the current PC) to the branch label using
 * the given branch instruction */
int get_branch_offset(Instruction instruction) {

  /* YOUR CODE HERE 
  // int extract_imm12 = instruction.sbtype.imm7 >> 6;
  // int extract_imm11 = instruction.sbtype.imm5 | 1;

  // extracting the 4 most right bits in imm5
  int imm5 = (instruction.sbtype.imm5 & 0xF) << 1;
  int imm7 = instruction.sbtype.imm7 << 5; // shift to align with imm5 for concat.

  imm12 = imm12 << 13;
  imm11 = imm11 << 12;

  int offset = (imm12 | imm11 | imm7 | imm5) << 1;

  offset = sign_extend_number(offset, 20);

  int imm12 = (instruction.sbtype.imm7 & 0x40) >> 6;
  int imm11 = (instruction.sbtype.imm5 & 0x3E) >> 2;
  int imm7 = (instruction.sbtype.imm7 & 0x01) << 4;
  int imm5 = (instruction.sbtype.imm5 & 0x0F) << 1;

  int offset = imm12 | imm11 | imm7 | imm5;
  offset = sign_extend_number(offset, 20);  // Sign-extend the offset to 32 bits
  */

  
  // WORKING CODE
  int imm1 = instruction.sbtype.imm5 & ((1 << (1)) - 1);
  int imm2 = (instruction.sbtype.imm5 >> 1) & ((1 << (4)) - 1);
  int imm3 = instruction.sbtype.imm7 & ((1 << (6)) - 1);
  int imm4 = (instruction.sbtype.imm7 >> 6) & ((1 << (1)) - 1);

  int offset = imm2|(imm3 << 4)|(imm1 << 10)|(imm4 << 11);
  int result = sign_extend_number(offset<< 1, 21);
  return result;
  //following the same logic from get_jump_offset

}

/* Returns the number of bytes (from the current PC) to the jump label using the
 * given jump instruction */
int get_jump_offset(Instruction instruction) {
  /* YOUR CODE HERE */

   // int bits19to12 = instruction.ujtype.imm & ((1 << 8) - 1);         //extracts first 7 bits -> 19:12 from GC
  // int bit11 = (instruction.ujtype.imm & (1 << 11) - 1);             //extracts the next bit, bit 11 
  // int bits1to10 = instruction.ujtype.imm & (((1 << 10) - 1) << 9);  //extracts next 10 bits -> 10:1 from GC
  // int bit20 = (instruction.ujtype.imm & (1 << 20));                 //extracts last bit, bit 20

  // int combine = (bit20|bits19to12|bit11|bits1to10) << 1;
  // int result = sign_extend_number(combine, 21);

  // printf("\n\nJUMP OFFSET = %d\n", result);
  // // 32 - 20  
  // // we will use this value and add it to PC which is a 32 bit value
  // // use sign_extend_number to scale accordingly
  
  // /* what I learned from the TA
  //   - need to add a 0 bit as the LSB since the range of imm starts at 1
  //   - sign extend at the end, not every single extracted partition
  //   - bits in scrambled order, which mixed up my naming convention for the bits
  //   - doesn't work but I think I'm close
  // */
  // return result;


  int imm1 = instruction.ujtype.imm & ((1 << (8)) - 1);
  int imm2 = (instruction.ujtype.imm >> 8) & ((1 << (1)) - 1);
  int imm3 = (instruction.ujtype.imm >> 9) & ((1 << (10)) - 1);
  int imm4 = (instruction.ujtype.imm >> 19) & ((1 << (1)) - 1);
  
  int offset = imm3|(imm2 << 10)|(imm1 << 11)|(imm4<<19);
  int result = sign_extend_number(offset << 1, 21);

  return result;  
  //this works!
}

/* Returns the number of bytes (from the current PC) to the base address using the
 * given store instruction */
int get_store_offset(Instruction instruction) {
  /* YOUR CODE HERE */

  // shift imm7 to align with imm5 in order to concatenate the bits
  int align_imm7 = instruction.stype.imm7 << 5;

  // concatenating imm7 and imm5 to get 1 immediate
  int offset = align_imm7 | instruction.stype.imm5;

  // Sign-extend the offset to 32 bits (PC is 32)
  offset = sign_extend_number(offset, 20);

  return offset;
}
/************************Helper functions************************/

void handle_invalid_instruction(Instruction instruction) {
  printf("Invalid Instruction: 0x%08x\n", instruction.bits);
}

void handle_invalid_read(Address address) {
  printf("Bad Read. Address: 0x%08x\n", address);
  exit(-1);
}

void handle_invalid_write(Address address) {
  printf("Bad Write. Address: 0x%08x\n", address);
  exit(-1);
}

