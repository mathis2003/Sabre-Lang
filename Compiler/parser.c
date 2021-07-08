
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"

struct TokenArr* tok_arr_ptr;
struct Token* cur_token_ptr;
struct Program* ast_root_node;
struct Program* parse_tokens(struct TokenArr* tok_arr) {
    tok_arr_ptr = tok_arr;
    cur_token_ptr = cur_token();
    struct Program* result_node = malloc(sizeof(struct Program));
    if (result_node == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    ast_root_node = result_node;
    
    init_ast_allocators(result_node);
    
    
    parse_program_node(result_node);
    
    return result_node;
}

/*-------------------------------------------------------------------------------*/

void parse_program_node(struct Program* program_node) {
    program_node->entry_point = NULL;
    
    while (cur_token_ptr != NULL) {
        if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
            if (str_equals_literal(&(cur_token_ptr->name_str), "ENTRY_POINT")){
                struct EntryPoint* entrypoint = malloc(sizeof(EntryPoint));
                program_node->entry_point = entrypoint;
                
                eat_token(TOK_OPEN_PAREN);
                // args should be added to the symbol table, for now however, just skip over it
                eat_token(TOK_IDENTIFIER);
                eat_token(TOK_COLON);
                eat_token(TOK_IDENTIFIER);
                eat_token(TOK_OPEN_SQUARE_BRACKET);
                eat_token(TOK_CLOSE_SQUARE_BRACKET);
                eat_token(TOK_CLOSE_PAREN);
                
                entrypoint->scope = parse_scope();
                
            }
        }
        next_token();
    }
}

struct Scope* parse_scope() {
    // TODO: replace the malloc below with a custom allocator
    Scope ret_scope;
    // TODO: clean up initializing
    ret_scope.parent_scope = NULL;
    ret_scope.amount_of_nested_scopes = 0;
    ret_scope.amount_of_variables = 0;
    
    eat_token(TOK_OPEN_CURLY);
    // check for declarations/initializations of function pointers, integers, and other variables...
    // check for statements
    // nested scopes might be encountered in both statements as well as in initializations
    eat_token(TOK_CLOSE_CURLY);
    
    Scope* ret_scope_ptr = add_scope_to_bucket(&ret_scope, ast_root_node->allocators.scope_bucket);
    
    return ret_scope_ptr;
}


/*-------------------------------------------------------------------------------*/

int cur_token_index = 0;

struct Token* next_token() {
    if (cur_token_index >= tok_arr_ptr->size) return (cur_token_ptr = NULL); // exception!!!
    cur_token_index++;
    return (cur_token_ptr = &(tok_arr_ptr->tokens[cur_token_index]));
}

struct Token* peek_token() {
    if (cur_token_index >= tok_arr_ptr->size) return (cur_token_ptr = NULL); // exception!!!
    return &(tok_arr_ptr->tokens[cur_token_index + 1]);
}

void eat_token(enum TokenType expected_type) {
    if (tok_arr_ptr->tokens[cur_token_index].tok_type != expected_type) (cur_token_ptr = NULL); // error instead of return!!!
}

struct Token* cur_token() {
    if (cur_token_index >= tok_arr_ptr->size) return (cur_token_ptr = NULL); // exception!!!
    return (cur_token_ptr = &(tok_arr_ptr->tokens[cur_token_index]));
}


/*-------------------------------------------------------------------------------*/

// custom allocators are only made for the structs that can occur more than once. That is: scopes, statements
// the Program and the EntryPoint struct are malloc'd once, since they only occur once, and must therefore be separatly freed

void free_AST(struct Program* ast_root) {
    free(ast_root->entry_point);
    // free all nested scopes in global scope
    free(ast_root->global_scope);
    free(ast_root);
}


/*-------------------------------------------------------------------------------*/

void init_ast_allocators(Program* program_ptr) {
    init_main_scope_bucket(program_ptr);
    // also init other allocators here
}
