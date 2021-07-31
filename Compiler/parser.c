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
    
    ast_root_node = malloc(sizeof(Program));
    if (ast_root_node == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    
    parse_program_node(ast_root_node);
    
    return ast_root_node;
}

/*-------------------------------------------------------------------------------*/

void parse_program_node(Program* program_ptr) {
    
    // LATER ON WE'LL NEED TO LOOK WHETHER THIS IS THE MAIN MODULE OR NOT
    
    
    /* INIT PROGRAM NODE */ {
        init_ast_allocators(program_ptr);
        program_ptr->entry_point = parse_entry_point();
    }
    
}

struct EntryPoint* parse_entry_point() {
    struct EntryPoint* entrypoint = malloc(sizeof(EntryPoint));
    if (entrypoint == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    init_void_ptr_arr(&(entrypoint->decl_ptr_arr));
    init_void_ptr_arr(&(entrypoint->stmt_ptr_arr));
    
    while (cur_token_ptr != NULL) {
        switch (cur_token_ptr->tok_type) {
            case TOK_OPEN_ANGLE_BRACKET: {
                struct Declaration* decl_ptr = parse_declaration(NULL);
                add_void_ptr_to_arr(&(entrypoint->decl_ptr_arr), (void*)decl_ptr);
                break;
            }
            case TOK_NUMBER:
            case TOK_OPEN_PAREN:
            case TOK_IDENTIFIER: {
                struct Statement* stmt_ptr = parse_statement(NULL);
                add_void_ptr_to_arr(&(entrypoint->stmt_ptr_arr), (void*)stmt_ptr);
                eat_token(TOK_SEMI_COLON);
                break;
            }
            case TOK_EQUALS: {
                // control flow indicator found
                //<#statements#>
                break;
            }
                
            default: {
                // parse error
                break;
            }
                
        }
        
    }
    
    return entrypoint;
}

struct FnLiteral* parse_fn_literal(struct FnLiteral* surrounding_scope) {
    struct FnLiteral ret_fn_literal;
    /*Initialize ret_scope*/ {
        ret_fn_literal.parent_scope = surrounding_scope;
        
        init_void_ptr_arr(&(ret_fn_literal.param_decl_ptr_arr));
        init_void_ptr_arr(&(ret_fn_literal.decl_ptr_arr));
        init_void_ptr_arr(&(ret_fn_literal.stmt_ptr_arr));
    }
    
    // parse function parameters
    eat_token(TOK_OPEN_PAREN);
    if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        do {
            if (cur_token_ptr->tok_type == TOK_COMMA) next_token();
            struct Declaration* param_decl_ptr = parse_parameter_declaration();
            add_void_ptr_to_arr(&(ret_fn_literal.param_decl_ptr_arr), (void*)param_decl_ptr);
        } while (cur_token_ptr->tok_type == TOK_COMMA);
    }
    
    eat_token(TOK_CLOSE_PAREN);
    eat_token(TOK_MINUS);
    eat_token(TOK_CLOSE_ANGLE_BRACKET);
    
    // check return type
    if (str_equals_literal(&(cur_token_ptr->name_str), "u8")) {
        ret_fn_literal.return_type = UINT_8;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "u16")) {
        ret_fn_literal.return_type = UINT_16;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "u32")) {
        ret_fn_literal.return_type = UINT_32;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "String")) {
        ret_fn_literal.return_type = STRING;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "fn")) {
        ret_fn_literal.return_type = FN_PTR;
    } else if (str_equals_literal(&(cur_token_ptr->name_str), "Unit")) {
        ret_fn_literal.return_type = UNIT;
    } else {
        // parse error
    }
    
    
    eat_token(TOK_IDENTIFIER);
    
    
    eat_token(TOK_OPEN_CURLY);
    
    while (cur_token_ptr->tok_type != TOK_CLOSE_CURLY) {
        switch (cur_token_ptr->tok_type) {
            case TOK_OPEN_ANGLE_BRACKET: {
                struct Declaration* decl_ptr = parse_declaration(surrounding_scope);
                add_void_ptr_to_arr(&(ret_fn_literal.decl_ptr_arr), (void*)decl_ptr);
                break;
            }
            case TOK_NUMBER:
            case TOK_OPEN_PAREN:
            case TOK_IDENTIFIER: {
                struct Statement* stmt_ptr = parse_statement(surrounding_scope);
                add_void_ptr_to_arr(&(ret_fn_literal.stmt_ptr_arr), (void*)stmt_ptr);
                eat_token(TOK_SEMI_COLON);
                break;
            }
            case TOK_EQUALS: {
                // control flow indicator found
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

struct Declaration* parse_parameter_declaration() {
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
    
    decl.is_initialized = 0;
    
    Declaration* ret_decl_ptr = add_declaration_to_bucket(&decl, ast_root_node->allocators.declaration_bucket);
    return ret_decl_ptr;
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
                next_token();
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
                next_token();
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
                next_token();
                break;
            }
            case STRING: {
                if (cur_token_ptr->tok_type != TOK_STRING) {
                    printf("PARSE ERROR: WRONG TYPE FOR INITIALIZATION");
                    /*PARSE ERROR! WRONG TYPE FOR INITIALIZATION*/
                }
                decl.init_string = cur_token_ptr->name_str;
                next_token();
                break;
            }
            case FN_PTR: {
                if (cur_token_ptr->tok_type != TOK_OPEN_PAREN) {
                    /*PARSE ERROR! WRONG TYPE FOR INITIALIZATION*/
                    printf("PARSE ERROR: WRONG TYPE FOR INITIALIZATION");
                }
                decl.init_fn_ptr = parse_fn_literal(surrounding_scope);
                break;
            }
            
                
            default:
                break;
        }
        
    } else decl.is_initialized = 0;
    eat_token(TOK_CLOSE_ANGLE_BRACKET);
    
    Declaration* ret_decl_ptr = add_declaration_to_bucket(&decl, ast_root_node->allocators.declaration_bucket);
    return ret_decl_ptr;
}


struct Statement* parse_statement(struct FnLiteral* surrounding_scope) {
    Statement stmt;
    
    // parse here
    if (cur_token_ptr->tok_type == TOK_NUMBER) {
        stmt.stmt_type = STMT_EXPR;
        stmt.expr = parse_expression();
    } else if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        if (str_equals_literal(&(cur_token_ptr->name_str), "if")) {
            // parse if expression
            stmt.stmt_type = STMT_EXPR;
            stmt.expr = parse_expression();
        }
    } else if (cur_token_ptr->tok_type == TOK_OPEN_PAREN) {
        stmt.stmt_type = STMT_EXPR;
        stmt.expr = parse_expression();
    }
    
    
    struct Statement* ret_stmt_ptr = add_statement_to_bucket(&stmt, ast_root_node->allocators.statement_bucket);
    return ret_stmt_ptr;
}


/*-------------------------------------------------------------------------------*/
/* EXPRESSION PARSING                                                            */
/*-------------------------------------------------------------------------------*/
struct Expression* parse_expression() {
    Expression* expr_ptr;
    
    
    if (cur_token_ptr->tok_type == TOK_NUMBER || cur_token_ptr->tok_type == TOK_BOOL || cur_token_ptr->tok_type == TOK_OPEN_PAREN) {
        // parse boolean operand
        expr_ptr = parse_logic_expr();
        
    }
    else if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        if (str_equals_literal(&(cur_token_ptr->name_str), "if")) {
            expr_ptr = parse_if_else_expr();
        } else if (str_equals_literal(&(cur_token_ptr->name_str), "Unit")) {
            // parse anonymous function call expression
            ;
        }
    }
    //Expression* expr_ptr = add_expression_to_bucket(&expr, ast_root_node->allocators.expression_bucket);
    return expr_ptr;
}

struct Expression* parse_if_else_expr() {
    Expression if_else_expr;
    
    if_else_expr.expr_type = EXPR_IF_THEN_ELSE;

    eat_token(TOK_IDENTIFIER);
    if_else_expr.tern_op.left   = parse_expression();
    eat_token(TOK_IDENTIFIER);
    if_else_expr.tern_op.middle = parse_expression();
    eat_token(TOK_IDENTIFIER);
    if_else_expr.tern_op.right  = parse_expression();
    
    Expression* ret_expr_ptr = add_expression_to_bucket(&if_else_expr, ast_root_node->allocators.expression_bucket);
    
    return ret_expr_ptr;
}

struct Expression* parse_logic_expr() {
    Expression* left_expr = parse_bool_term();
    
    while (cur_token_ptr->tok_type == TOK_AMPERSAND || cur_token_ptr->tok_type == TOK_PIPE) {
        Expression parent_expr;
        Expression* right_expr;
        
        
        if (cur_token_ptr->tok_type == TOK_AMPERSAND && peek_token()->tok_type == TOK_AMPERSAND) {
            parent_expr.expr_type = EXPR_LOGIC_AND;
            next_token();
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_PIPE && peek_token()->tok_type == TOK_PIPE) {
            parent_expr.expr_type = EXPR_LOGIC_OR;
            next_token();
            next_token();
        } else {
            // parse error
        }
        
        right_expr = parse_bool_term();
        
        parent_expr.bin_op.left = left_expr;
        parent_expr.bin_op.right = right_expr;
        
        left_expr = add_expression_to_bucket(&parent_expr, ast_root_node->allocators.expression_bucket);
    }
    
    return left_expr;
}

struct Expression* parse_bool_term() {
    Expression* left_expr = parse_cond_term();
    
    while (cur_token_ptr->tok_type == TOK_EQUALS || cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET || cur_token_ptr->tok_type == TOK_CLOSE_ANGLE_BRACKET) {
        Expression parent_expr;
        Expression* right_expr;
        
        
        if (cur_token_ptr->tok_type == TOK_EQUALS && peek_token()->tok_type == TOK_EQUALS) {
            parent_expr.expr_type = EXPR_COND_EQUALS;
            next_token();
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET && peek_token()->tok_type == TOK_EQUALS) {
            parent_expr.expr_type = EXPR_COND_LOWER_EQUALS;
            next_token();
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_CLOSE_ANGLE_BRACKET && peek_token()->tok_type == TOK_EQUALS) {
            parent_expr.expr_type = EXPR_COND_GREATER_EQUALS;
            next_token();
            next_token();
        }
        else if (cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET) {
            parent_expr.expr_type = EXPR_COND_LOWER;
            next_token();
        }
        else if (cur_token_ptr->tok_type == TOK_CLOSE_ANGLE_BRACKET) {
            parent_expr.expr_type = EXPR_COND_GREATER;
            next_token();
        }
        
        right_expr = parse_cond_term();
        
        parent_expr.bin_op.left = left_expr;
        parent_expr.bin_op.right = right_expr;
        
        left_expr = add_expression_to_bucket(&parent_expr, ast_root_node->allocators.expression_bucket);
    }
    
    return left_expr;
    
}

struct Expression* parse_cond_term() {
    Expression* left_expr = parse_arith_term();
    
    while (cur_token_ptr->tok_type == TOK_PLUS || cur_token_ptr->tok_type == TOK_MINUS) {
        Expression parent_expr;
        Expression* right_expr;
        
        
        if (cur_token_ptr->tok_type == TOK_PLUS) {
            parent_expr.expr_type = EXPR_ADD;
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_MINUS) {
            parent_expr.expr_type = EXPR_SUB;
            next_token();
        } else {
            // parse error
            break;
        }
        
        right_expr = parse_arith_term();
        
        parent_expr.bin_op.left = left_expr;
        parent_expr.bin_op.right = right_expr;
        
        left_expr = add_expression_to_bucket(&parent_expr, ast_root_node->allocators.expression_bucket);
    }
    
    return left_expr;
}

struct Expression* parse_arith_term() {
    Expression* left_expr = parse_factor();
    
    while (cur_token_ptr->tok_type == TOK_ASTERISK || cur_token_ptr->tok_type == TOK_SLASH) {
        Expression parent_expr;
        Expression* right_expr;
        
        
        if (cur_token_ptr->tok_type == TOK_ASTERISK) {
            parent_expr.expr_type = EXPR_MULT;
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_SLASH) {
            parent_expr.expr_type = EXPR_DIV;
            next_token();
        } else {
            // parse error
            break;
        }
        
        right_expr = parse_factor();
        
        parent_expr.bin_op.left = left_expr;
        parent_expr.bin_op.right = right_expr;
        
        left_expr = add_expression_to_bucket(&parent_expr, ast_root_node->allocators.expression_bucket);
    }
    
    return left_expr;
}

struct Expression* parse_factor() {
    Expression* ret_expr;
    // basically, the options here are: an expression between parentheses, a function call, or a literal
    if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        if (peek_token()->tok_type == TOK_OPEN_PAREN) {
            // parse function call
            ;
        }
        else {
            // parse variable
            struct Expression var_literal_expr;
            var_literal_expr.expr_type = EXPR_VAR_LITERAL;
            var_literal_expr.variable_literal = cur_token_ptr->name_str;
            ret_expr = add_expression_to_bucket(&var_literal_expr, ast_root_node->allocators.expression_bucket);
            next_token();
        }
    }
    else if (cur_token_ptr->tok_type == TOK_NUMBER) {
        struct Expression number_literal_expr;
        number_literal_expr.expr_type = EXPR_NUM_LITERAL;
        number_literal_expr.int_literal = str_to_int(&cur_token_ptr->name_str);
        ret_expr = add_expression_to_bucket(&number_literal_expr, ast_root_node->allocators.expression_bucket);
        next_token();
    }
    else if (cur_token_ptr->tok_type == TOK_OPEN_PAREN) {
        // parse parenthesized expression
        next_token();
        ret_expr = parse_expression();
        eat_token(TOK_CLOSE_PAREN);
    }
    
    // check if there's an open paren following up, in that case: function call
    if (cur_token_ptr->tok_type == TOK_OPEN_PAREN) {
        ret_expr = parse_fn_call_expr(ret_expr);
        
    }
    
    return ret_expr;
}

struct Expression* parse_fn_call_expr(Expression* fn_ptr) {
    Expression fn_call_expr;
    
    fn_call_expr.expr_type            = EXPR_FN_CALL;
    fn_call_expr.fn_call.fn_ptr_expr  = fn_ptr;
    init_void_ptr_arr(&(fn_call_expr.fn_call.arg_ptr_arr));
    
    // parse arguments list
    next_token(); //skips over open paren
    if (cur_token_ptr->tok_type != TOK_CLOSE_PAREN) {
        
        while (1) {
            Expression* arg = parse_expression();
            add_void_ptr_to_arr(&(fn_call_expr.fn_call.arg_ptr_arr), (void*)arg);
            if (cur_token_ptr->tok_type == TOK_COMMA) next_token();
            else if (cur_token_ptr->tok_type == TOK_CLOSE_PAREN) break;
            else ; // NOOOO!!!! ERRRORRRRR!!!
        }
    } else ;
    next_token(); // skips over close paren
    
    Expression* ret_expr_ptr = add_expression_to_bucket(&fn_call_expr, ast_root_node->allocators.expression_bucket);
    
    return ret_expr_ptr;
}

/*-------------------------------------------------------------------------------*/

int cur_token_index = 0;

struct Token* next_token() {
    if (cur_token_index+1 >= tok_arr_ptr->size) return (cur_token_ptr = NULL); // exception!!!
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
    // free entrypoint
    free_void_ptr_arr(&ast_root->entry_point->decl_ptr_arr);
    free_void_ptr_arr(&ast_root->entry_point->stmt_ptr_arr);
    free(ast_root->entry_point);
    
    // free the space of FnLiterals, Declarations, Statements and Expressions
    free_fn_literal_bucket(ast_root->allocators.fn_literal_bucket);
    free_declaration_bucket(ast_root->allocators.declaration_bucket);
    free_statement_bucket(ast_root->allocators.statement_bucket);
    free_expression_bucket(ast_root->allocators.expression_bucket);
    
    free(ast_root);
}


/*-------------------------------------------------------------------------------*/

void init_ast_allocators(Program* program_ptr) {
    init_main_fn_literal_bucket(program_ptr);
    init_main_declaration_bucket(program_ptr);
    init_main_statement_bucket(program_ptr);
    init_main_expression_bucket(program_ptr);
    // also init expression allocator here
}
