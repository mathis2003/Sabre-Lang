
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "pretty_printer.h"

int main(int argc, char** argv) {
    
    TokenArr* tok_arr;
    
    
    tok_arr = lex_code(argv[1]);
    if (tok_arr == NULL) {
        printf("ERROR OCCURRED DURING LEXING\n");
        return 1;
    }
    
    print_tokens(tok_arr);
    
    // PARSING...
    struct Program* program_node_ptr = parse_tokens(tok_arr);
    
    print_parse_tree(program_node_ptr);

    free_tokens(tok_arr);
    
	return 0;
}
