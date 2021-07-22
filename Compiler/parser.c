/*
 * REFACTORING:
 *---------------------------------------------------
 *---------------------------------------------------
 * MALLOC ERRORS:
 *---------------------------------------------------
 * EXIT IMMEDIATELY + ERROR THAT MALLOC DID NOT WORK
 *---------------------------------------------------
 *---------------------------------------------------
 * SYNTAX ERRORS:
 *---------------------------------------------------
 * EXIT IMMEDIATELY + ERROR THAT SYNTAX IS WRONG
 *---------------------------------------------------
 *---------------------------------------------------

 *
 */
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "string_commons.h"

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
        // program is made of declarations, scopes, statements and an entrypoint
        
        if (cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET) {
            // parse declaration
        }
        else if (str_equals_literal(&(cur_token_ptr->name_str), "ENTRY_POINT")) {
            EntryPoint* entrypoint = parse_entry_point();
            program_node->entry_point = entrypoint;
        }
        else {
            // parse error
        }
        next_token();
    }
}

struct EntryPoint* parse_entry_point() {
    eat_token(TOK_IDENTIFIER);
    struct EntryPoint* entrypoint = malloc(sizeof(EntryPoint));
    init_void_ptr_arr(&(entrypoint->decl_ptr_arr));
    init_void_ptr_arr(&(entrypoint->stmt_ptr_arr));
    //entrypoint->scope = parse_scope(NULL);
    eat_token(TOK_OPEN_CURLY);
    
    while (cur_token_ptr->tok_type != TOK_CLOSE_CURLY) {
        switch (cur_token_ptr->tok_type) {
            case TOK_OPEN_ANGLE_BRACKET: {
                struct Declaration* decl_ptr = parse_declaration(NULL);
                add_void_ptr_to_arr(&(entrypoint->decl_ptr_arr), (void*)decl_ptr);
                break;
            }
            case TOK_IDENTIFIER: {
                // statement found
                //<#statements#>
                break;
            }
                
            default: {
                // parse error
                break;
            }
                
        }
    }
    
    eat_token(TOK_CLOSE_CURLY);
    
    return entrypoint;
}

struct FnLiteral* parse_fn_literal(struct FnLiteral* surrounding_scope) {
    struct FnLiteral ret_fn_literal;
    /*Initialize ret_scope*/ {
        ret_fn_literal.parent_scope = surrounding_scope;
        init_void_ptr_arr(&(ret_fn_literal.decl_ptr_arr));
        init_void_ptr_arr(&(ret_fn_literal.stmt_ptr_arr));
    }
        
    eat_token(TOK_OPEN_CURLY);
    
    while (cur_token_ptr->tok_type != TOK_CLOSE_CURLY) {
        switch (cur_token_ptr->tok_type) {
            case TOK_OPEN_ANGLE_BRACKET: {
                struct Declaration* decl_ptr = parse_declaration(surrounding_scope);
                add_void_ptr_to_arr(&(ret_fn_literal.decl_ptr_arr), (void*)decl_ptr);
                break;
            }
            case TOK_IDENTIFIER: {
                // statement found
                //<#statements#>
                break;
            }
                
            default: {
                // parse error
                break;
            }
                
        }
    }
    
    eat_token(TOK_CLOSE_CURLY);
    
    struct FnLiteral* ret_fn_literal_ptr = add_fn_literal_to_bucket(&ret_fn_literal, ast_root_node->allocators.fn_literal_bucket);
    
    return ret_fn_literal_ptr;
}

struct Declaration* parse_declaration(struct FnLiteral* surrounding_scope) {
    eat_token(TOK_OPEN_ANGLE_BRACKET);
    Declaration decl;
    decl.identifier = cur_token_ptr->name_str;
    eat_token(TOK_IDENTIFIER);
    eat_token(TOK_COLON);
    // replace beneath with a switch statement over the possible data types
    if (str_equals_literal(&(cur_token_ptr->name_str), "u8")) {
        decl.type = UINT_8;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "u16")) {
        decl.type = UINT_16;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "u32")) {
        decl.type = UINT_32;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "String")) {
        decl.type = STRING;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "fn")) {
        decl.type = FN_PTR;
    } else {
        // parse error
    }
    
    eat_token(TOK_IDENTIFIER);
    
    /*check for initialization values*/
    if (cur_token_ptr->tok_type == TOK_EQUALS) {
        next_token();
        decl.is_initialized = 1;
        switch (decl.type) {
            case UINT_8: {
                if (cur_token_ptr->tok_type != TOK_NUMBER) {
                    printf("PARSE ERROR: WRONG TYPE FOR INITIALIZATION");
                    /*PARSE ERROR! WRONG TYPE FOR INITIALIZATION*/
                }
                decl.init_int_val = str_to_int(&cur_token_ptr->name_str);
                if (decl.init_int_val > 256) {
                    printf("PARSE ERROR: initialization value is too big to store in a u8");
                    // PARSE ERROR! initialization value is too big to store in a u8
                }
                break;
            }
            case UINT_16: {
                if (cur_token_ptr->tok_type != TOK_NUMBER) {
                    printf("PARSE ERROR: WRONG TYPE FOR INITIALIZATION");
                    /*PARSE ERROR! WRONG TYPE FOR INITIALIZATION*/
                }
                decl.init_int_val = str_to_int(&cur_token_ptr->name_str);
                if (decl.init_int_val > 65536) {
                    printf("PARSE ERROR: initialization value is too big to store in a u16");
                    // PARSE ERROR! initialization value is too big to store in a u16
                }
                break;
            }
            case UINT_32: {
                if (cur_token_ptr->tok_type != TOK_NUMBER) {
                    printf("PARSE ERROR: WRONG TYPE FOR INITIALIZATION");
                    /*PARSE ERROR! WRONG TYPE FOR INITIALIZATION*/
                }
                decl.init_int_val = str_to_int(&cur_token_ptr->name_str);
                if (decl.init_int_val > 4294967296) {
                    printf("PARSE ERROR: initialization value is too big to store in a u32");
                    // PARSE ERROR! initialization value is too big to store in a u8
                }
                break;
            }
            case STRING: {
                if (cur_token_ptr->tok_type != TOK_STRING) {
                    printf("PARSE ERROR: WRONG TYPE FOR INITIALIZATION");
                    /*PARSE ERROR! WRONG TYPE FOR INITIALIZATION*/
                }
                decl.init_string = cur_token_ptr->name_str;
                break;
            }
            case FN_PTR: {
                if (cur_token_ptr->tok_type != TOK_IDENTIFIER) {
                    /*PARSE ERROR! WRONG TYPE FOR INITIALIZATION*/
                    printf("PARSE ERROR: WRONG TYPE FOR INITIALIZATION");
                }
                eat_token(TOK_OPEN_PAREN);
                // parse anonymous function
                break;
            }
            
                
            default:
                break;
        }
        next_token();
    } else decl.is_initialized = 0;
    
    eat_token(TOK_CLOSE_ANGLE_BRACKET);
    
    Declaration* ret_decl_ptr = add_declaration_to_bucket(&decl, ast_root_node->allocators.declaration_bucket);
    return ret_decl_ptr;
}

struct Statement* parse_statement (struct FnLiteral* surrounding_scope) {
    
    Statement* ret_stmt_ptr;
    return ret_stmt_ptr;
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
    if (tok_arr_ptr->tokens[cur_token_index].tok_type != expected_type)  { (cur_token_ptr = NULL); }// error instead of return!!!
    next_token();
}

struct Token* cur_token() {
    if (cur_token_index >= tok_arr_ptr->size) return (cur_token_ptr = NULL); // exception!!!
    return (cur_token_ptr = &(tok_arr_ptr->tokens[cur_token_index]));
}


/*-------------------------------------------------------------------------------*/

// custom allocators are only made for the structs that can occur more than once. That is: scopes, statements and declarations
// the Program and the EntryPoint struct are malloc'd once, since they only occur once, and must therefore be separatly freed

void free_AST(struct Program* ast_root) {
    free(ast_root->entry_point);
    free_void_ptr_arr(&ast_root->decl_ptr_arr);
    // might need to free allocators here too, although i'm not sure rn, perhaps the nodes are necessary or smth
    // free all nested scopes in global scope
    
    
    free(ast_root);
}


/*-------------------------------------------------------------------------------*/

void init_ast_allocators(Program* program_ptr) {
    init_main_fn_literal_bucket(program_ptr);
    init_main_declaration_bucket(program_ptr);
    // also init other allocators here
}
