#include "Decode.h"



/* Initialize the decode entry */

void init_decode_entry(struct Decode *entry) {

  memset(entry->opcode, 0, sizeof(entry->opcode));

  memset(entry->funct, 0, sizeof(entry->funct));

  memset(entry->source_register, 0, sizeof(entry->source_register));

  memset(entry->destination_register, 0, sizeof(entry->destination_register));

  memset(entry->source_address, 0, sizeof(entry->source_address));

  memset(entry->destination_address, 0, sizeof(entry->destination_address));

}



/* Decode an instruction and update the decode entry */

void decode_instruction(struct Decode *entry, char *instruction) {

  int index = opcode(instruction);

  char newop[21] = "01000000000000000000";

  newop[19 - index] = '1';

  strcpy(entry->opcode, newop);

  strcpy(entry->funct, get_funct_binary(instruction));



  /* Update the source and destination registers */

  if (index == 0 || index == 2 || index == 5 || index == 9) {

    /* The instruction is an R-type instruction. */

    entry->source_register[0] = instruction[1];

    entry->destination_register[0] = instruction[2];

  } else if (index == 4) {

    /* The instruction is a LEA instruction. */

    entry->destination_register[0] = instruction[1];

    entry->source_address[0] = instruction[2];

  } else {

    /* The instruction is a J-type or I-type instruction. */

    entry->source_register[0] = instruction[1];

  }

}



/* Get the opcode type in decimal base */

int opcode(char op[]) {

  switch (op[0]) {

    case 'm': return 0; /* mov */

    case 'c': return 1; /* cmp */

    case 'a': return 2; /* add */

    case 's': return (op[1] == 'u') ? 2 : 15; /* sub or stop */

    case 'l': return 4; /* lea */

    case 'n': return 5; /* not */

    case 'i': return 5; /* inc */

    case 'd': return 5; /* dec */

    case 'j': return 9; /* jmp */

    case 'b': return 9; /* bne */

    case 'r': return 12; /* red */

    case 'p': return 13; /* prn */

    default: return 15; /* stop */

  }

}



/* Get the binary representation of the function number for the instructions */

const char* get_funct_binary(char op[]) {

  switch (op[0]) {

    case 'a':

    case 'c':

    case 'j':

    case 'm': return "1010";

    case 's':

    case 'n':

    case 'b': return "1011";

    case 'i':

    case 'd': return "1100";

    default: return "0000";

  }

}







