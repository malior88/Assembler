#ifndef SecondPass_h

#define SecondPass_h

#define MAX_LINE_LENGTH 100



#include "SymbolTable.h"

#include "Decode.h"

#include <ctype.h>



/* Function to handle the second pass of the assembler */

void CustomSecondPass(char* source_file, char* output_prefix, struct Symbol* shead, struct Decode* dhead);



/* Parse the instruction in the line and update the Decode struct with the binary representation of the instruction */

int CustomInstruction(char line[], int index, struct Decode* dhead);



#endif
