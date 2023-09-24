
#ifndef SYMBOL_H

#define SYMBOL_H


#include <stdio.h>

#include <stdlib.h>



typedef enum symbolType {

    CODE,

    DATA,

    EXTERNAL,

    ENTRY

} symbolType;



struct Symbol {

    char *name;

    symbolType type;

    int value;

    struct Symbol *next;

};



struct Symbol *CreateSymbol(char *name, symbolType type, int value);

void InsertSymbol(struct Symbol *symbol);

void PrintSymbolTable();



#endif
