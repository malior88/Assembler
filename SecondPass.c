#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "SecondPass.h"

#include "Decode.h"

#include "Pre_assembler.h"



/* Function to write the object file */

void writeObjectFile(FILE* object_fp, int instruction_counter, int data_counter, struct Decode* dhead);



/* Function to write the externals file */

void writeExternalsFile(FILE* externals_fp, struct Symbol* shead, struct Decode* dhead);



/* Function to write the entries file */

void writeEntriesFile(FILE* entries_fp, struct Symbol* shead);



/* Function to handle the second pass of the assembler */

void CustomSecondPass(char* source_file, char* output_prefix, struct Symbol* shead, struct Decode* dhead) {

    FILE* source_fp;

    FILE* object_fp;

    FILE* externals_fp;

    FILE* entries_fp;

    char line[MAX_LINE_LENGTH];

    int IC = 0; // Initialize IC to 0 for instruction counter

    int DC = 0; // Initialize DC to 0 for data counter



    /* Open the source file for reading */

    source_fp = fopen(source_file, "r");

    if (source_fp == NULL) {

        printf("Error opening source file: %s\n", source_file);

        exit(EXIT_FAILURE);

    }



    /* Loop to read each line from the source file */

    while (fgets(line, sizeof(line), source_fp) != NULL) {

        // Skip empty lines and comments

        if (strlen(line) == 0 || line[0] == ';' || line[0] == '\n')

            continue;



        /* Remove the newline character at the end of the line*/

        line[strcspn(line, "\n")] = '\0';



        /* Skip the first field if it's a symbol */

        char* instruction = line;

        if (line[0] != ' ')

            instruction = strchr(line, ' ') + 1;



        /* Check if it's an extern, data, or string directive*/

        if (strncmp(instruction, ".extern", 7) == 0 || strncmp(instruction, ".data", 5) == 0 || strncmp(instruction, ".string", 7) == 0)

            continue;



        /* Check if it's an entry directive */

        if (strncmp(instruction, ".entry", 6) == 0) {

            /* Mark appropriate symbols as entry in the symbol table */

            char* symbol_name = strchr(instruction, ' ') + 1;

            struct Symbol* temp = shead;

            while (temp != NULL) {

                if (strcmp(temp->name, symbol_name) == 0) {

                    temp->type = ENTRY;

                    break;

                }

                temp = temp->next;

            }

            continue;

        }



        /* Otherwise, it's an instruction, so decode and encode the operands */

        int instruction_length = CustomInstruction(instruction, 0, dhead);

        IC += instruction_length;

    }



    /* Close the source file */

    fclose(source_fp);



    /* Create and open output files for writing */

    char object_file_name[MAX_LINE_LENGTH];

    char externals_file_name[MAX_LINE_LENGTH];

    char entries_file_name[MAX_LINE_LENGTH];



    snprintf(object_file_name, sizeof(object_file_name), "%s.ob", output_prefix);

    snprintf(externals_file_name, sizeof(externals_file_name), "%s.ext", output_prefix);

    snprintf(entries_file_name, sizeof(entries_file_name), "%s.ent", output_prefix);



    object_fp = fopen(object_file_name, "w");

    externals_fp = fopen(externals_file_name, "w");

    entries_fp = fopen(entries_file_name, "w");



    if (object_fp == NULL || externals_fp == NULL || entries_fp == NULL) {

        printf("Error opening output files.\n");

        exit(EXIT_FAILURE);

    }



    /* Write the object file, externals file, and entries file*/

    writeObjectFile(object_fp, IC, DC, dhead);

    writeExternalsFile(externals_fp, shead, dhead);

    writeEntriesFile(entries_fp, shead);



    /* Close the output files */

    fclose(object_fp);

    fclose(externals_fp);

    fclose(entries_fp);

}



/* Function to write the object file */

void writeObjectFile(FILE* object_fp, int instruction_counter, int data_counter, struct Decode* dhead) {

    /* Write the header of the object file */

    fprintf(object_fp, "%d %d\n", instruction_counter, data_counter);



    /* Loop through the decoded instructions and write them to the object file */

    struct Decode* current = dhead;

    while (current != NULL) {

        fprintf(object_fp, "%s\n", current->opcode);

        current = current->next;

    }

}



/* Function to write the externals file */

void writeExternalsFile(FILE* externals_fp, struct Symbol* shead, struct Decode* dhead) {

    /* Loop through the symbol table and check for external symbols */

    struct Symbol* current_symbol = shead;

    while (current_symbol != NULL) {

        if (current_symbol->type == EXTERNAL) {

            /* Loop through the decoded instructions to find references to this external symbol */

            struct Decode* current_decode = dhead;

            int address = 100; // Starting address in memory

            while (current_decode != NULL) {

                if (strcmp(current_decode->source_address, current_symbol->name) == 0) {

                    /* Write the address to the externals file*/

                    fprintf(externals_fp, "%s %d\n", current_symbol->name, address);

                }

                current_decode = current_decode->next;

                address += 4; /* Assuming each instruction takes 4 memory words */

            }

        }

        current_symbol = current_symbol->next;

    }

}



/* Function to write the entries file */

void writeEntriesFile(FILE* entries_fp, struct Symbol* shead) {

    /* Loop through the symbol table and check for entry symbols*/

    struct Symbol* current = shead;

    while (current != NULL) {

        if (current->type == ENTRY) {

            /* Write the entry symbol and its value to the entries file */

            fprintf(entries_fp, "%s %d\n", current->name, current->value);

        }

        current = current->next;

    }

}

