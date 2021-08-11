
#ifndef PRETTY_PRINTER_H
#define PRETTY_PRINTER_H

#include "../Lexer/lexer.h"
#include "../Parser/parser.h"

void print_tokens(TokenArr* tok_arr);
void print_parse_tree(struct Program* program_node);
void print_generated_code(char* output_file_name);

#endif
