#ifndef PRE_ASSEMBLER_H

#define PRE_ASSEMBLER_H

#define MAX_LINE_LENGTH 100

#define MAX_MACRO_COUNT 100



/* Structure to store macro names */

struct MacroNode {

    char name[MAX_LINE_LENGTH];

};



/* Function to insert a macro into the macro table */

void insertMacro(struct MacroNode macroTable[], int* macroCount, const char* name);



/* Function to check if a macro is defined in the macro table */

int isMacroDefined(struct MacroNode macroTable[], int macroCount, const char* name);



/* Function to perform pre-assembly and interpret macros */

void pre_assemble(const char* source_file, const char* output_file);



#endif  /* PRE_ASSEMBLER_H */
