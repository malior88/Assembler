#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "SymbolTable.h"

#include "Decode.h"

#include "FirstPass.h"

#define MAX 256



/* Check if a symbol with the given name already exists in the symbol table */

int CustomNotExistSymbol(struct Symbol* tail, char sname[]) {



    struct Symbol* tailf = tail;

    while (tailf != NULL) {

        if (!strcmp(tailf->name, sname)) {

            return 0; // Symbol already exists

        }

        tailf = tailf->next;

    }

    return 1; // Symbol does not exist

}



/* Count the number of data elements in a .data directive */

int CustomDcData(char line[], int index, struct Decode* dhead) {

    int dccounter = 1;

    while (line[index] != '\0') {

        if (line[index] == 44) // ',' character represents a new data element

            dccounter++;

        index++;

    }

    return dccounter;

}



/* Count the number of characters in a .string directive (including the null terminator) */

int CustomDcString(char line[], int index, struct Decode* dhead) {

    int dccounter = 0;

    while (isspace(line[index]))

        index++;

    if (line[index] == 34) { // Double quote (") represents the start of a string

        index = index + 1;

        while (line[index] != 34 && line[index] != '\n') { // Continue until the ending double quote is found

            index++;

            dccounter++;

        }

    }

    return dccounter + 1; // Return the length of the string, including the null terminator

}



/* Check if the directive is .data or .string and return the number of data elements or characters in the string */

int CustomDataOrString(char line[], int index, struct Decode* dhead) {

    int aindex = 0;

    char att[MAX];

    if (line[index] == '\n')

        return 0;

    memset(att, '\0', MAX);

    while (isspace(line[index]))

        index++;

    while (!isspace(line[index]) && line[index] != '\n') {

        att[aindex] = line[index];

        aindex++;

        index++;

    }

    if (!strcmp(".data", att))

        return CustomDcData(line, index, dhead);

    if (!strcmp(".string", att))

        return CustomDcString(line, index, dhead);

    return 0;

}



/* Extract and store an external symbol from the line */

void CustomExternSymbol(struct Symbol* temp, char line[]) {

    char sname[MAX];

    int index = 0, sindex = 0;

    memset(sname, '\0', MAX);

    while (isspace(line[index]))

        index++;

    while (!isspace(line[index]))

        index++;

    while (isspace(line[index]))

        index++;

    while (!isspace(line[index]) && line[index] != '\n') {

        sname[sindex] = line[index];

        sindex++;

        index++;

    }

    temp = (struct Symbol*)malloc(sizeof(struct Symbol));

    strcpy(temp->name, sname);

    temp->value = 0;

    temp->type = CUSTOM_EXTERNAL;

}



/* Check if the directive is .entry or .extern and return the corresponding value */

int CustomEntryOrExtern(char line[]) {

    int aindex = 0, index = 0;

    char att[MAX];

    if (line[index] == '\n')

        return 0;

    memset(att, '\0', MAX);

    while (isspace(line[index]))

        index++;

    while (!isspace(line[index]) && line[index] != '\n') {

        att[aindex] = line[index];

        aindex++;

        index++;

    }

    if (!strcmp(".entry", att))

        return 2; // Return 2 for .entry

    if (!strcmp(".extern", att))

        return 1; // Return 1 for .extern

    return 0;

}



/* Convert an integer to its binary representation as a string */

const char* CustomBinaryRepresentation(int n) {

    int i;

    char* p;

    static char binary[5] = "0000";

    for (i = 3; n > 0; i--) {

        binary[i] = n % 2 + 48; // Convert binary digits to characters ('0' or '1')

        n = n / 2;

    }

    p = binary;

    return p;

}



/* Parse the destination addressing mode and update the corresponding fields in the CustomDecode struct */

int CustomDestinationMode(char line[], int index, struct Decode* dhead) {

    char num[MAX];

    int nindex = 0, i;

    memset(num, '\0', MAX);

    while (isspace(line[index]))

        index++;

    if (line[index] == '#') { // Immediate address

        strcpy(dhead->destination_address, "00");

        strcpy(dhead->destination_register, "0000");

    }

    else if (line[index] == 'r') { // Direct register address

        index++;

        while (line[index] >= '0' && line[index] <= '9') {

            num[nindex] = line[index];

            index++;

            nindex++;

        }

        i = atoi(num);

        if (i < 16) {

            strcpy(dhead->destination_register, CustomBinaryRepresentation(i));

            strcpy(dhead->destination_address, "11");

        }

        return index;

    }

    else { // Direct address or index address

        strcpy(dhead->destination_register, "0000");

        if (line[index] == 'r') {

            index++;

            while (line[index] >= '0' && line[index] <= '9') {

                num[nindex] = line[index];

                index++;

                nindex++;

            }

            i = atoi(num);

            if (i < 16) {

                strcpy(dhead->destination_register, CustomBinaryRepresentation(i));

            }

        }

        else {

            while (line[index] != '[' && line[index] != ',' && !isspace(line[index])) {

                num[nindex] = line[index];

                index++;

                nindex++;

            }

            strcpy(dhead->symbol, num);

        }

        if (line[index] == '[') { // Index address

            memset(num, '\0', MAX);

            nindex = 0;

            strcpy(dhead->destination_address, "10");

            index++;

            if (line[index] == 'r') {

                index++;

                while (line[index] >= '0' && line[index] <= '9') {

                    num[nindex] = line[index];

                    index++;

                    nindex++;

                }

                i = atoi(num);

                if (i < 16) {

                    strcpy(dhead->destination_register, CustomBinaryRepresentation(i));

                }

            }

            strcpy(dhead->destination_address, "10");

        }

        else {

            strcpy(dhead->destination_address, "01"); // Direct address

        }

        return index;

    }

    return 0;

}



/* Parse the source addressing mode and update the corresponding fields in the CustomDecode struct */

int CustomSourceMode(char line[], int index, struct Decode* dhead) {

    char num[MAX];

    int nindex = 0, i;

    memset(num, '\0', MAX);

    while (isspace(line[index]))

        index++;

    if (line[index] == '#') { /* Immediate address */

        strcpy(dhead->source_address, "00");

        strcpy(dhead->source_register, "0000");

        while (!isspace(line[index]) && line[index] != '\0' && line[index] != '\n')

            index++;

    }

    else if (line[index] == 'r') { /* Direct register address */

        index++;

        while (line[index] >= '0' && line[index] <= '9') {

            num[nindex] = line[index];

            index++;

            nindex++;

        }

        i = atoi(num);

        if (i < 16) {

            strcpy(dhead->source_register, CustomBinaryRepresentation(i));

            strcpy(dhead->source_address, "11");

        }

        while (!isspace(line[index]) && line[index] != '\0' && line[index] != '\n')

            index++;

    }

    else { /* Direct address or index address */

        strcpy(dhead->source_register, "0000");

        if (line[index] == 'r') {

            index++;

            while (line[index] >= '0' && line[index] <= '9') {

                num[nindex] = line[index];

                index++;

                nindex++;

            }

            i = atoi(num);

            if (i < 16) {

                strcpy(dhead->source_register, CustomBinaryRepresentation(i));

            }

            while (!isspace(line[index]) && line[index] != '\0' && line[index] != '\n')

                index++;

        }

        else {

            while (line[index] != '[' && line[index] != ',' && !isspace(line[index])) {

                num[nindex] = line[index];

                index++;

                nindex++;

            }

            strcpy(dhead->symbol, num);

        }

        if (line[index] == '[') { /* Index address */

            memset(num, '\0', MAX);

            nindex = 0;

            strcpy(dhead->source_address, "10");

            index++;

            if (line[index] == 'r') {

                index++;

                while (line[index] >= '0' && line[index] <= '9') {

                    num[nindex] = line[index];

                    index++;

                    nindex++;

                }

                i = atoi(num);

                if (i < 16) {

                    strcpy(dhead->source_register, CustomBinaryRepresentation(i));

                }

                while (!isspace(line[index]) && line[index] != '\0' && line[index] != '\n')

                    index++;

            }

            else {

                while (line[index] != ']' && !isspace(line[index])) {

                    num[nindex] = line[index];

                    index++;

                    nindex++;

                }

            }

        }

        else {

            strcpy(dhead->source_address, "01"); /* Direct address */

        }

        return index;

    }

    return 0;

}



/* Check the opcode of the instruction and update the opcode field in the CustomDecode struct */

int CustomCheckOpcode(char line[], int index, struct Decode* dhead) {

    char att[MAX];

    int op; // Declare 'op' variable

    memset(att, '\0', MAX);

    while (isspace(line[index]))

        index++;

    while (!isspace(line[index])) {

        att[index] = line[index];

        index++;

    }

    op = opcode(att); // Use the 'op' variable to store the numeric opcode

    if (op == 15)

        return 0;

    strcpy(dhead->opcode, CustomBinaryRepresentation(op));

    return 1;

}



/* Parse the instruction in the line and update the CustomDecode struct with the binary representation of the instruction */

int CustomInstruction(char line[], int index, struct Decode* dhead) {

    if (!CustomCheckOpcode(line, index, dhead)) {

        printf("Error: Unknown command in line\n");

        return 0;

    }

    index = index + 4;

    index = CustomSourceMode(line, index, dhead);

    index = CustomDestinationMode(line, index, dhead);

    return 1;

}



/* Parse the line and check if it represents a symbol */



int CustomIsSymbol(char line[], struct Symbol** temp, struct Symbol* tail, struct Decode* dhead) {

    char sname[MAX];

    int index = 0;

    int sindex = 0;

    int sourceindex = 0;



    memset(sname, '\0', MAX);

    

    while (isspace(line[index]))

        index++;



    while (!isspace(line[index]) && line[index] != ':') {

        sname[sindex] = line[index];

        sindex++;

        index++;

    }



    if (strlen(sname) && line[index] == ':') {

        *temp = (struct Symbol*)malloc(sizeof(struct Symbol));

        if (*temp == NULL) {

            printf("Error: Memory allocation failed.\n");

            return 0;

        }



        strcpy((*temp)->name, sname);

        (*temp)->value = 0;

        (*temp)->type = CUSTOM_CODE;



        index = index + 1;



        if (!CustomDestinationMode(line, index, dhead)) {

            free(*temp);

            return 0;

        }



        index = index + 4;



        while (line[index] != '\0' && line[index] != '\n') {

            if (isspace(line[index])) {

                index++;

                continue;

            }



            if (!CustomInstruction(line, index, dhead)) {

                free(*temp);

                return 0;

            }



            index = index + 19;



            while (line[index] != '\0' && line[index] != '\n' && !isspace(line[index]))

                index++;

        }

    } else {

        *temp = (struct Symbol*)malloc(sizeof(struct Symbol));

        if (*temp == NULL) {

            printf("Error: Memory allocation failed.\n");

            return 0;

        }



        strcpy((*temp)->name, sname);

        (*temp)->value = 0;

        (*temp)->type = CUSTOM_DATA;



        CustomDataOrString(line, index, dhead);



        while (line[index] != '\0' && line[index] != '\n') {

            if (isspace(line[index])) {

                index++;

                continue;

            }



            if (line[index] == '.')

                sourceindex = CustomEntryOrExtern(line + index);



            index++;

        }



        if (sourceindex == 1)

            (*temp)->type = CUSTOM_EXTERNAL;



        if (sourceindex == 2)

            (*temp)->type = CUSTOM_ENTRY;



        if (sourceindex)

            CustomExternSymbol(*temp, line);

    }



    return 1;

}







/* Perform the first pass on the input file and process the symbols and instructions */

int CustomFirstCheck(int IJ, char* argv[], struct Symbol** shead, struct Decode** dhead, int* IC, int* DC) {

    FILE* file;

    char line[MAX];

    int counter = 100;

    file = fopen(argv[IJ], "r");

    if (file == NULL) {

        printf("Error: File could not be opened.\n");

        return 0;

    }



    while (fgets(line, MAX, file)) {

        if (!CustomIsSymbol(line, &*shead, *shead, *dhead)) {

            fclose(file);

            return 0;

        }

        (*IC)++;

    }

    (*IC)--;

    *DC = counter;

    return 1;

}
