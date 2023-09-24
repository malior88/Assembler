#include "Main.h"



int main(int argc, char* argv[]) {

    if (argc < 2) {

        printf("Usage: %s input_file.asm\n", argv[0]);

        return 1;

    }



    /* Perform pre-assembly to handle macros (if any) */

    char output_prefix[FILENAME_MAX] = {0};

    snprintf(output_prefix, sizeof(output_prefix), "%s", "output");

    /* Set the desired output file prefix */

    pre_assemble(argv[1], output_prefix);



    /* Initialize the symbol table and decode list */

    struct Symbol* symbol_head = NULL;

    struct Decode* decode_head = NULL;



    /* Perform the first pass */

    int IC = 0; /* Instruction Counter */

    int DC = 0; /* Data Counter */

    int first_pass_result = CustomFirstCheck(1, argv, &symbol_head, &decode_head, &IC, &DC);

    if (first_pass_result != 1) {

        printf("First pass failed. Error code: %d\n", first_pass_result);

        /* Clean up and free resources (e.g., symbol table and decode data) */

        return 1;

    }



    /* Perform the second pass */

    CustomSecondPass(argv[1], output_prefix, symbol_head, decode_head);



    /* Clean up and free resources (e.g., symbol table and decode data) */

    



    return 0;

}

