#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "Pre_assembler.h"



/* Function to insert a macro into the macro table */

void insertMacro(struct MacroNode macroTable[], int* macroCount, const char* name) {

    if (*macroCount >= MAX_MACRO_COUNT) {

        printf("Maximum macro count reached.\n");

        return;

    }

    strcpy(macroTable[*macroCount].name, name);

    (*macroCount)++;

}



/* Function to check if a macro is defined in the macro table*/

int isMacroDefined(struct MacroNode macroTable[], int macroCount, const char* name) {

    int i;

    for (i = 0; i < macroCount; i++) {

        if (strcmp(macroTable[i].name, name) == 0) {

            return 1;

        }

    }

    return 0;

}



/* Function to perform pre-assembly and interpret macros*/

void pre_assemble(const char* source_file, const char* output_file) {

    FILE* source;

    FILE* output;



    char line[MAX_LINE_LENGTH];

    struct MacroNode macroTable[MAX_MACRO_COUNT];

    int macroCount = 0;

    int active_macro = 0;



    /* Open the source file for reading */

    source = fopen(source_file, "r");

    if (source == NULL) {

        printf("Error opening source file.\n");

        return;

    }



    /* Open the output file for writing */

    output = fopen(output_file, "w");

    if (output == NULL) {

        printf("Error creating output file.\n");

        fclose(source);

        return;

    }



    /* Read each line from the source file */

    while (fgets(line, sizeof(line), source)) {

        if (line[0] == '\n') {

            continue;

        }



        char* token = strtok(line, " \t\n");



        /* Check if the token is a defined macro */

        if (isMacroDefined(macroTable, macroCount, token)) {

            /* Macro name detected, replace with lines from macro table */

             int i;

            for (i = 0; i < macroCount; i++) {

                if (strcmp(macroTable[i].name, token) == 0) {

                    fprintf(output, "%s", macroTable[i].name);  /*Write macro name to output file*/

                    break;

                }

            }

            continue;

        }



        /* Check if the token indicates the start of a macro definition*/

        if (strcmp(token, "mcro") == 0) {

            // Start of macro definition

            if (active_macro) {

                printf("Error: Nested macro definitions not allowed.\n");

                fclose(source);

                fclose(output);

                return;

            }



            char* macro_name = strtok(NULL, " \t\n");



            if (isMacroDefined(macroTable, macroCount, macro_name)) {

                printf("Error: Macro name conflicts with an existing macro.\n");

                fclose(source);

                fclose(output);

                return;

            }



            insertMacro(macroTable, &macroCount, macro_name);

            active_macro = 1;

            continue;

        }



        /* Check if the token indicates the end of a macro definition*/

        if (strcmp(token, "endmcro") == 0) {

            /* End of macro definition */

            if (!active_macro) {

                printf("Error: Unmatched 'endmcro' token.\n");

                fclose(source);

                fclose(output);

                return;

            }



            active_macro = 0;

            continue;

        }



        if (active_macro) {

            /* Line inside a macro, add it to the macro table */

            fprintf(output, "%s", line);

            continue;

        }



        /* Not a macro, write line to output file */

        fprintf(output, "%s", line);

    }



    /* Close the files */

    fclose(source);

    fclose(output);



    printf("Pre-assembling completed. Output saved to %s\n", output_file);

}

