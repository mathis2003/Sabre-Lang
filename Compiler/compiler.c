
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "pretty_printer.h"

int main(int argc, char** argv) {
    
    TokenArr* tok_arr;
    
    
    tok_arr = lex_code(argv[1]);
    if (tok_arr == NULL) {
        printf("ERROR OCCURED DURING LEXING\n");
        return 1;
    }
    
    print_tokens(tok_arr);
    
    // PARSING...
    
    free_tokens(tok_arr);
    
	return 0;
}
