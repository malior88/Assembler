

/* Include necessary header files */

#include "SymbolTable.h"

#include "Decode.h"

#include <ctype.h>

#ifndef FirstPass_h

#define FirstPass_h

#define CUSTOM_EXTERNAL EXTERNAL

#define CUSTOM_CODE CODE

#define CUSTOM_DATA DATA

#define CUSTOM_ENTRY ENTRY




/* Function prototypes */



/* Check if a symbol with the given name already exists in the symbol table */

int CustomNotExistSymbol(struct Symbol* tail, char sname[]);



/* Count the number of data items (comma-separated) in the .data directive */

int CustomDcData(char line[], int index, struct Decode* dhead);



/* Count the number of characters in a string (inside the .string directive) */

int CustomDcString(char line[], int index, struct Decode* dhead);



/* Determine if the line contains .data or .string directive and process accordingly */

int CustomDataOrString(char line[], int index, struct Decode* dhead);



/* Check if the line contains a symbol, and if it does, update the symbol table accordingly */

int CustomIsSymbol(char line[], struct Symbol** temp, struct Symbol* tail, struct Decode* dhead);



/* Handle an external symbol, add it to the symbol table */

void CustomExternSymbol(struct Symbol* temp, char line[]);



/* Check if the line contains .entry or .extern directive and return the corresponding value */

int CustomEntryOrExtern(char line[]);



/* Convert an integer to its binary representation and return it as a string */

const char* CustomBinaryRepresentation(int n);



/* Parse the destination addressing mode and update the corresponding fields in the Decode struct */

int CustomDestinationMode(char line[], int index, struct Decode* dhead);



/* Parse the source addressing mode and update the corresponding fields in the Decode struct */

int CustomSourceMode(char line[], int index, struct Decode* dhead);



/* Check the opcode of the instruction and update the opcode field in the Decode struct */

int CustomCheckOpcode(char line[], int index, struct Decode* dhead);



/* Parse the instruction in the line and update the Decode struct with the binary representation of the instruction */

int CustomInstruction(char line[], int index, struct Decode* dhead);



/* Perform the first pass on the input file and process the symbols and instructions */

int CustomFirstCheck(int IJ, char* argv[], struct Symbol** shead, struct Decode** dhead, int* IC, int* DC);
