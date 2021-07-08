
#ifndef PRETTY_PRINTER_H
#define PRETTY_PRINTER_H

#include "lexer.h"
#include "parser.h"

void print_tokens(TokenArr* tok_arr);
void print_parse_tree(struct Program* program_node);

#endif
