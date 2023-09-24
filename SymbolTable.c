#include "SymbolTable.h"



struct Symbol* head = NULL; /* Global variable to store the head of the symbol table */



struct Symbol *CreateSymbol(char *name, symbolType type, int value) {

    struct Symbol *new_symbol = malloc(sizeof(struct Symbol));

    new_symbol->name = name;

    new_symbol->type = type;

    new_symbol->value = value;

    new_symbol->next = NULL;

    return new_symbol;

}



void InsertSymbol(struct Symbol *symbol) {

    struct Symbol *current = head;

    if (head == NULL) {

        head = symbol;

    } else {

        while (current->next != NULL) {

            current = current->next;

        }

        current->next = symbol;

    }

}



void PrintSymbolTable() {

    struct Symbol *symbol = head;

    while (symbol != NULL) {

        printf("Name: %s, Type: %d, Value: %d\n", symbol->name, symbol->type, symbol->value);

        symbol = symbol->next;

    }

}
