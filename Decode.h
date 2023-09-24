#ifndef DECODE_H

#define DECODE_H



#include <stdio.h>

#include <string.h>



struct Decode {

  char opcode[21]; /* The opcode of the instruction. */

  char funct[5]; /* The funct of the instruction. */

  char symbol[10]; /* The symbol of the instruction. */

  char source_register[5]; /* The source register of the instruction. */

  char destination_register[5]; /* The destination register of the instruction. */

  char source_address[3]; /* The source address of the instruction. */

  char destination_address[3]; /* The destination address of the instruction. */



  struct Decode* next; /* Pointer to the next node in the linked list. */

};



void init_decode_entry(struct Decode *entry);

void decode_instruction(struct Decode *entry, char *instruction);

int opcode(char op[]);

const char* get_funct_binary(char op[]);



#endif

