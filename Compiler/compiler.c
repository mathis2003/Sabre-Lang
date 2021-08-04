
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lexer.h"
#include "parser.h"
#include "code_gen.h"
#include "pretty_printer.h"

int main(int argc, char** argv) {
    
    
    /*---------------------------------------------------------------------------------------*/
    /* START BENCHMARK                                                                       */
    /*---------------------------------------------------------------------------------------*/

    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    /*---------------------------------------------------------------------------------------*/
    /* LEXING                                                                                */
    /*---------------------------------------------------------------------------------------*/
    
    TokenArr* tok_arr;
    
    tok_arr = lex_code(argv[1]);
    if (tok_arr == NULL) {
        printf("ERROR - UNABLE TO OPEN FILE: %s\n", argv[1]);
        return 1;
    }
    //print_tokens(tok_arr);
    
    /*---------------------------------------------------------------------------------------*/
    /* PARSING                                                                               */
    /*---------------------------------------------------------------------------------------*/
    
    struct Program* program_node_ptr = parse_tokens(tok_arr);
    
    print_parse_tree(program_node_ptr);

    
    //free_tokens(tok_arr);
    /*---------------------------------------------------------------------------------------*/
    /* CODE GEN                                                                              */
    /*---------------------------------------------------------------------------------------*/
    generate_code(program_node_ptr, argv[2]);
    
    print_generated_code(argv[2]);
    
    free_AST(program_node_ptr);
    /*---------------------------------------------------------------------------------------*/
    /* END BENCHMARK                                                                         */
    /*---------------------------------------------------------------------------------------*/
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n-----------------------\ncpu time used: %f\n-----------------------\n\n", cpu_time_used);
    
    
	return 0;
}
