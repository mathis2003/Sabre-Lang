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
    
    /* INITIALIZE ENTRY POINT */{
        init_void_ptr_arr(&(entrypoint->decl_ptr_arr));
        init_void_ptr_arr(&(entrypoint->stmt_ptr_arr));
        entrypoint->imports.amount_of_imported_files = 0;
    }
    
    while (cur_token_ptr != NULL) {
        
        switch (cur_token_ptr->tok_type) {
            case TOK_OPEN_SQUARE_BRACKET: {
                struct Declaration* decl_ptr = parse_declaration(NULL);
                add_void_ptr_to_arr(&(entrypoint->decl_ptr_arr), (void*)decl_ptr);
                break;
            }
            case TOK_DOLLAR_SIGN:
            case TOK_NUMBER:
            case TOK_OPEN_PAREN: {
                struct Statement* stmt_ptr = parse_statement(NULL);
                add_void_ptr_to_arr(&(entrypoint->stmt_ptr_arr), (void*)stmt_ptr);
                break;
            }
            case TOK_IDENTIFIER: {
                if (str_equals_literal(&(cur_token_ptr->name_str), "import")) {
                    parse_imports(&(entrypoint->imports));
                } else {
                    struct Statement* stmt_ptr = parse_statement(NULL);
                    add_void_ptr_to_arr(&(entrypoint->stmt_ptr_arr), (void*)stmt_ptr);
                    break;
                }
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
        
        ret_fn_literal.imports.amount_of_imported_files = 0;
    }
    
    // parse function parameters
    if (get_tok_type(cur_token_ptr) == TOK_OPEN_CURLY) {
        struct DataType datatype;
        datatype.is_value = 1;
        datatype.type_enum_val = UNIT;
        ret_fn_literal.return_type = datatype;
    } else {
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
        
        ret_fn_literal.return_type = parse_data_type();
    }
    
    
    eat_token(TOK_OPEN_CURLY);
    
    while (cur_token_ptr->tok_type != TOK_CLOSE_CURLY) {
        switch (cur_token_ptr->tok_type) {
            case TOK_OPEN_SQUARE_BRACKET: {
                struct Declaration* decl_ptr = parse_declaration(surrounding_scope);
                add_void_ptr_to_arr(&(ret_fn_literal.decl_ptr_arr), (void*)decl_ptr);
                break;
            }
            case TOK_DOLLAR_SIGN:
            case TOK_EQUALS:
            case TOK_NUMBER:
            case TOK_OPEN_PAREN: {
                struct Statement* stmt_ptr = parse_statement(surrounding_scope);
                add_void_ptr_to_arr(&(ret_fn_literal.stmt_ptr_arr), (void*)stmt_ptr);
                break;
            }
            case TOK_IDENTIFIER: {
                if (str_equals_literal(&(cur_token_ptr->name_str), "import")) {
                    parse_imports(&(ret_fn_literal.imports));
                } else {
                    struct Statement* stmt_ptr = parse_statement(surrounding_scope);
                    add_void_ptr_to_arr(&(ret_fn_literal.stmt_ptr_arr), (void*)stmt_ptr);
                    break;
                }
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
    decl.type = parse_data_type();
    
    decl.is_initialized = 0;
    
    Declaration* ret_decl_ptr = add_declaration_to_bucket(&decl, ast_root_node->allocators.declaration_bucket);
    return ret_decl_ptr;
}

void parse_cfi(struct Statement* stmt) {
    ControlFlowIndicator cfi;
    next_token(); // skip over equals sign
    eat_token(TOK_CLOSE_ANGLE_BRACKET);
    eat_token(TOK_IDENTIFIER); // skip over "scope"
    eat_token(TOK_OPEN_SQUARE_BRACKET);
    
    if (cur_token_ptr->tok_type == TOK_NUMBER) {
        cfi.scope_obj.index = str_to_int(&(cur_token_ptr->name_str));
    } else {
        // NOOOOO!!!!! ERROR!!!!!
        printf("the index should be a number, instead got a token of type: %d", cur_token_ptr->tok_type);
        return;
    }
    next_token(); // skip over number
    
    
    eat_token(TOK_CLOSE_SQUARE_BRACKET);
    eat_token(TOK_DOT);
    
    cfi.scope_obj.start_lbl  = 0;
    cfi.scope_obj.end_lbl    = 0;
    cfi.scope_obj.exit_lbl   = 0;
    cfi.scope_obj.return_val = 0;
    
    if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        if (str_equals_literal(&(cur_token_ptr->name_str), "start")) {
            cfi.scope_obj.start_lbl = 1;
        } else if (str_equals_literal(&(cur_token_ptr->name_str), "end")) {
            cfi.scope_obj.end_lbl = 1;
        } else if (str_equals_literal(&(cur_token_ptr->name_str), "exit")) {
            cfi.scope_obj.exit_lbl = 1;
        } else {
            printf("can't lead control flow to: ");
            print_str_struct(&cur_token_ptr->name_str);
            printf("\n");
            return;
        }
    } else {
        // NOOOO!!!! ERROR!!!!
        printf("expected an accessible member of the scope but got a token of type: %d\n", cur_token_ptr->tok_type);
        return;
    }
    
    next_token(); // skip over "start", "end" or "exit"
    
    stmt->cfi = cfi;
    
}

struct Declaration* parse_declaration(struct FnLiteral* surrounding_scope) {
    eat_token(TOK_OPEN_SQUARE_BRACKET);
    Declaration decl;
    decl.identifier = cur_token_ptr->name_str;
    eat_token(TOK_IDENTIFIER);
    if (cur_token_ptr->tok_type == TOK_COLON) {
        decl.type = parse_data_type();
        /*check for initialization values*/
        if (cur_token_ptr->tok_type == TOK_EQUALS) {
            next_token(); // skip over equals sign
            
            decl.is_initialized = 1;
            
            if (decl.type.is_value) {
                decl.variable_assigned = 0;
                if (decl.type.type_enum_val == FN_PTR) {
                    decl.init_fn_ptr = parse_fn_literal(surrounding_scope);
                } else {
                    decl.init_expr = parse_expression();
                }
                
            } else {
                // it is a variable
                decl.variable_assigned = 1;
                if (cur_token_ptr->tok_type != TOK_IDENTIFIER) {
                    // NOOOO!!! PANICK!!!! ERROR!!!!
                    printf("expected an identifier as a variable name to be assigned to this variable\n");
                    return NULL;
                }
                decl.init_variable = cur_token_ptr->name_str;
                next_token(); // skip over name of assigned variable
            }
        } else if (cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET && peek_token(1)->tok_type == TOK_MINUS) {
            next_token(); // skip over "<-" operator
            next_token(); // skip over "<-" operator
            
            decl.is_initialized = 1;
            decl.variable_assigned = 0;
            
            if (decl.type.is_value) {
                // NOOOO!!! PANICK!!!! ERROR!!!!
                printf("can't put a value inside a value, the operator should be '=' instead of '<-'\n");
                return NULL;
            }
            
            if (decl.type.type_enum_val == FN_PTR) {
                decl.init_fn_ptr = parse_fn_literal(surrounding_scope);
            } else {
                decl.init_expr = parse_expression();
            }
            
            
            
        } else {
            decl.variable_assigned = 0;
            decl.is_initialized = 0;
            
        }
    } else {
        // inference type
        
        /*check for initialization values*/
        if (cur_token_ptr->tok_type == TOK_EQUALS) {
            next_token(); // skip over equals sign
            decl.is_initialized = 1;
            
            switch (get_tok_type(cur_token_ptr)) {
                case TOK_OPEN_PAREN: {
                    if (get_tok_type(peek_token(1)) == TOK_CLOSE_PAREN || get_tok_type(peek_token(2)) == TOK_COLON) {
                        decl.type.type_enum_val = FN_PTR;
                        decl.type.is_value = 1;
                        decl.init_fn_ptr = parse_fn_literal(surrounding_scope);
                        
                        decl.type.fn_type = parse_fn_literal_to_function_type(decl.init_fn_ptr);
                    } else {
                        //expression
                    }
                    break;
                }
                case TOK_OPEN_CURLY: {
                    decl.type.type_enum_val = FN_PTR;
                    decl.type.is_value = 1;
                    decl.init_fn_ptr = parse_fn_literal(surrounding_scope);
                    decl.type.fn_type = parse_fn_literal_to_function_type(decl.init_fn_ptr);
                    break;
                }
                case TOK_IDENTIFIER: {
                    decl.type.is_value = 0;
                    decl.init_variable = cur_token_ptr->name_str;
                    break;
                }
                case TOK_INVALID: {
                    // NOOOOO!!! PANICK!!!! ERROR!!!
                    printf("the expression in this declaration is incorrect\n");
                    return NULL;
                    break;
                }
                    
                default: {
                    decl.init_expr = parse_expression();
                    break;
                    
                }
            }
            
            
        } else if (cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET && peek_token(1)->tok_type == TOK_MINUS) {
            next_token(); // skip over "<-" operator
            next_token(); // skip over "<-" operator
            
            decl.is_initialized    = 1;
            decl.variable_assigned = 0;
            decl.type.is_value     = 1;
            
            switch (get_tok_type(cur_token_ptr)) {
                case TOK_OPEN_PAREN: {
                    if (get_tok_type(peek_token(1)) == TOK_CLOSE_PAREN || get_tok_type(peek_token(2)) == TOK_COLON) {
                        decl.type.type_enum_val = FN_PTR;
                        decl.init_fn_ptr = parse_fn_literal(surrounding_scope);
                        decl.type.fn_type = parse_fn_literal_to_function_type(decl.init_fn_ptr);
                    } else {
                        // expression
                    }
                    break;
                }
                case TOK_OPEN_CURLY: {
                    decl.type.type_enum_val = FN_PTR;
                    decl.init_fn_ptr = parse_fn_literal(surrounding_scope);
                    decl.type.fn_type = parse_fn_literal_to_function_type(decl.init_fn_ptr);
                    break;
                }
                case TOK_INVALID: {
                    // NOOOOO!!! PANICK!!!! ERROR!!!
                    printf("the expression in this declaration is incorrect\n");
                    return NULL;
                    break;
                }
                    
                default: {
                    decl.init_expr = parse_expression();
                    break;
                    
                }
            }
        }
    }
    
    
    
    eat_token(TOK_CLOSE_SQUARE_BRACKET);
    
    
    Declaration* ret_decl_ptr = add_declaration_to_bucket(&decl, ast_root_node->allocators.declaration_bucket);
    return ret_decl_ptr;
}


struct Statement* parse_statement(struct FnLiteral* surrounding_scope) {
    Statement stmt;
    
    // parse here
    if (cur_token_ptr->tok_type == TOK_NUMBER) {
        stmt.stmt_type = STMT_EXPR;
        stmt.expr = parse_expression();
        eat_token(TOK_SEMI_COLON);
    } else if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        if (str_equals_literal(&(cur_token_ptr->name_str), "if")) {
            // parse if expression
            stmt.stmt_type = STMT_EXPR;
            stmt.expr = parse_expression();
            eat_token(TOK_SEMI_COLON);
        } else {
            // also some expression for now
            stmt.stmt_type = STMT_EXPR;
            stmt.expr = parse_expression();
            eat_token(TOK_SEMI_COLON);
        }
    } else if (cur_token_ptr->tok_type == TOK_OPEN_PAREN) {
        stmt.stmt_type = STMT_EXPR;
        stmt.expr = parse_expression();
        eat_token(TOK_SEMI_COLON);
    } else if (cur_token_ptr->tok_type == TOK_EQUALS) {
        stmt.stmt_type = STMT_CFI;
        parse_cfi(&stmt);
    }
    
    
    struct Statement* ret_stmt_ptr = add_statement_to_bucket(&stmt, ast_root_node->allocators.statement_bucket);
    return ret_stmt_ptr;
}

void parse_imports(struct ImportList* import_list_ptr) {
    next_token(); // skips over "import"
    eat_token(TOK_OPEN_CURLY); // skips over "{"
    import_list_ptr->amount_of_imported_files = count_tokens_until_end_token_found(TOK_ASTERISK, TOK_CLOSE_CURLY);
    
    import_list_ptr->imported_files = malloc(import_list_ptr->amount_of_imported_files * sizeof(struct StringStruct));
    import_list_ptr->namespaces     = malloc(import_list_ptr->amount_of_imported_files * sizeof(struct StringStruct));
    
    // for now, don't parse the "as"- namespaces yet, just assume C headers
    for (int i = 0; i < import_list_ptr->amount_of_imported_files; i++) {
        eat_token(TOK_ASTERISK);
        if (cur_token_ptr->tok_type == TOK_STRING) {
            import_list_ptr->imported_files[i] = cur_token_ptr->name_str;
            //import_list_ptr->namespaces[i]     = {};
        } else {
            // NOOOOO!!!! ERROR!!!!!!
            return;
        }
        
        next_token(); // skips over imported file's name
    }
    eat_token(TOK_CLOSE_CURLY);
}

/*-------------------------------------------------------------------------------*/
/* DATA TYPE PARSING                                                             */
/*-------------------------------------------------------------------------------*/

struct DataType parse_data_type() {
    if (cur_token_ptr->tok_type == TOK_COLON) next_token(); // skip over colon if there is one
    
    // declare and init datatype
    struct DataType data_type;
    
    if (cur_token_ptr->tok_type == TOK_DOLLAR_SIGN) {
        // type is a "value"
        data_type.is_value = 1;
        next_token(); // skip over dollar_sign token
    } else {
        // type is a "variable"
        data_type.is_value = 0;
    }
    
    if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        // parse a non function type
        if (str_equals_literal(&(cur_token_ptr->name_str), "u8"))          data_type.type_enum_val = UINT_8;
        else if (str_equals_literal(&(cur_token_ptr->name_str), "u16"))    data_type.type_enum_val = UINT_16;
        else if (str_equals_literal(&(cur_token_ptr->name_str), "u32"))    data_type.type_enum_val = UINT_32;
        else if (str_equals_literal(&(cur_token_ptr->name_str), "Unit"))   data_type.type_enum_val = UNIT;
        else if (str_equals_literal(&(cur_token_ptr->name_str), "String")) data_type.type_enum_val = STRING;
        else {
            // ERROR!!!! PANICK!!!!
            printf("invalid datatype: ");
            print_str_struct(&(cur_token_ptr->name_str));
            printf("\n");
        }
        next_token(); // skip over type
    } else if (cur_token_ptr->tok_type == TOK_OPEN_PAREN) {
        // parse a function type
        data_type.type_enum_val = FN_PTR;
        data_type.fn_type = parse_function_type();
    }
    
    return data_type;
}

struct FunctionType* parse_function_type() {
    struct FunctionType* function_type_ptr = malloc(sizeof(struct FunctionType));
    /*----------------------------------------------------------------------------------------------------------*/
    {
        // TODO: memory allocations for function_type
        function_type_ptr->amount_of_fn_parameters = count_fn_parameters();
        function_type_ptr->parameter_types = malloc(function_type_ptr->amount_of_fn_parameters * sizeof(struct DataType));
    }
    /*----------------------------------------------------------------------------------------------------------*/
    {
        // TODO: actually parse parameter types
        next_token(); // skip over initial open parenthese
        for (int i = 0; i < function_type_ptr->amount_of_fn_parameters; i++) {
            function_type_ptr->parameter_types[i] = parse_data_type();
            if (function_type_ptr->amount_of_fn_parameters > i+1) eat_token(TOK_COMMA);
        }
        eat_token(TOK_CLOSE_PAREN);
    }
    /*----------------------------------------------------------------------------------------------------------*/
    {
        // TODO: parse return type
        eat_token(TOK_MINUS);
        eat_token(TOK_CLOSE_ANGLE_BRACKET);
        
        function_type_ptr->return_type = parse_data_type();
    }
    /*----------------------------------------------------------------------------------------------------------*/
    
    return function_type_ptr;
}

struct FunctionType* parse_fn_literal_to_function_type(struct FnLiteral* fn_literal) {
    struct FunctionType* function_type_ptr = malloc(sizeof(struct FunctionType));
    /*----------------------------------------------------------------------------------------------------------*/
    {
        // TODO: memory allocations for function_type
        function_type_ptr->amount_of_fn_parameters = fn_literal->param_decl_ptr_arr.size;
        function_type_ptr->parameter_types = malloc(function_type_ptr->amount_of_fn_parameters * sizeof(struct DataType));
    }
    /*----------------------------------------------------------------------------------------------------------*/
    {
        // TODO: actually parse parameter types
        for (int i = 0; i < function_type_ptr->amount_of_fn_parameters; i++) {
            function_type_ptr->parameter_types[i] = ((struct Declaration*)((fn_literal->param_decl_ptr_arr.void_ptrs)[i]))->type;
        }
    }
    /*----------------------------------------------------------------------------------------------------------*/
    {
        // TODO: parse return type
        function_type_ptr->return_type = fn_literal->return_type;
    }
    /*----------------------------------------------------------------------------------------------------------*/
    
    return function_type_ptr;
}

/*-------------------------------------------------------------------------------*/
/* EXPRESSION PARSING                                                            */
/*-------------------------------------------------------------------------------*/
struct Expression* parse_expression() {
    Expression* expr_ptr;
    
    
    if (cur_token_ptr->tok_type == TOK_NUMBER     ||
        cur_token_ptr->tok_type == TOK_BOOL       ||
        cur_token_ptr->tok_type == TOK_OPEN_PAREN ||
        cur_token_ptr->tok_type == TOK_STRING) {
        // parse boolean operand
        expr_ptr = parse_logic_expr();
        
    }
    else if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
        if (str_equals_literal(&(cur_token_ptr->name_str), "if")) {
            expr_ptr = parse_if_else_expr();
        } else if (str_equals_literal(&(cur_token_ptr->name_str), "Unit")) {
            // parse anonymous function call expression
            ;
        }  else {
            
            if (str_equals_literal(&(cur_token_ptr->name_str), "scope")) {
                if ((peek_token(6)->tok_type == TOK_OPEN_ANGLE_BRACKET && get_tok_type(peek_token(7)) == TOK_MINUS)) {
                    expr_ptr = parse_assignment(LEFT_HAND_RET, 1);
                } 
            } else {
                if ((peek_token(1)->tok_type == TOK_OPEN_ANGLE_BRACKET && get_tok_type(peek_token(2)) == TOK_MINUS)) expr_ptr = parse_assignment(LEFT_HAND_VARIABLE, 1);
                else if ((get_tok_type(peek_token(1)) == TOK_EQUALS && get_tok_type(peek_token(2)) != TOK_EQUALS)) expr_ptr = parse_assignment(LEFT_HAND_VALUE, 0);
                else expr_ptr = parse_logic_expr();
            }
            
        }
    } else if (cur_token_ptr->tok_type == TOK_DOLLAR_SIGN) {
        expr_ptr = parse_factor();
    }
    
    return expr_ptr;
}

struct Expression* parse_assignment(enum LeftHandSideType left_hand_side_type, char store_in_operator) {
    Expression assign_expr;
    assign_expr.expr_type = EXPR_ASSIGN;
    assign_expr.assignment.assigned_val_is_fn = 0;
    
    if (left_hand_side_type != LEFT_HAND_RET) {
        assign_expr.assignment.left_hand_side_enum = left_hand_side_type;
        assign_expr.assignment.variable_name = cur_token_ptr->name_str;
        next_token(); // skip over identifier
        
        if (store_in_operator) {
            next_token(); // skip over store-in operator
            next_token(); // skip over store-in operator
            
            if ((get_tok_type(cur_token_ptr) == TOK_OPEN_PAREN && get_tok_type(peek_token(1)) == TOK_CLOSE_PAREN) ||
                (get_tok_type(cur_token_ptr) == TOK_OPEN_PAREN && get_tok_type(peek_token(2)) == TOK_COLON)) {
                assign_expr.assignment.right_hand_side_is_variable = 1;
                assign_expr.assignment.assigned_val_is_fn = 1;
                assign_expr.assignment.assigned_fn_literal = parse_fn_literal(NULL);
            } else {
                assign_expr.assignment.right_hand_side_is_variable = 0;
                assign_expr.assignment.assigned_value = parse_expression();
            }
            
        } else {
            next_token(); // skip over equals sign
            if ((get_tok_type(cur_token_ptr) == TOK_OPEN_PAREN && get_tok_type(peek_token(1)) == TOK_CLOSE_PAREN) ||
                (get_tok_type(cur_token_ptr) == TOK_OPEN_PAREN && get_tok_type(peek_token(2)) == TOK_COLON)) {
                assign_expr.assignment.assigned_val_is_fn = 1;
                assign_expr.assignment.assigned_fn_literal = parse_fn_literal(NULL);
            } else {
                assign_expr.assignment.assigned_value = parse_expression();
            }
        }
        
    } else {
        // left hand side is scope object, so the operatpr has to be "<-"
        assign_expr.assignment.left_hand_side_enum = LEFT_HAND_RET;
        next_token(); // skip over "scope"
        eat_token(TOK_OPEN_SQUARE_BRACKET);
        
        if (cur_token_ptr->tok_type == TOK_NUMBER) {
            assign_expr.assignment.scope_object.index = str_to_int(&(cur_token_ptr->name_str));
        } else {
            // FUCKKKKK NOOOO ERROR!!!!!!!
            printf("index in scope[] is not a number but : %d\n", cur_token_ptr->tok_type);
            return NULL;
        }
        next_token(); // skip over index number
        eat_token(TOK_OPEN_SQUARE_BRACKET);
        eat_token(TOK_DOT);
        
        if (get_tok_type(cur_token_ptr) != TOK_IDENTIFIER || !str_equals_literal(&(cur_token_ptr->name_str), "ret")) {
            // FUCKKKKK NOOOO ERROR!!!!!!!
            printf("the object you're trying to access from scope is not valid: %d\n", cur_token_ptr->tok_type);
            return NULL;
        } else {
            next_token(); // skip over "ret"
        }
        
        assign_expr.assignment.scope_object.start_lbl    = 0;
        assign_expr.assignment.scope_object.end_lbl      = 0;
        assign_expr.assignment.scope_object.exit_lbl     = 0;
        assign_expr.assignment.scope_object.return_val   = 1;
        
        eat_token(TOK_OPEN_ANGLE_BRACKET);
        eat_token(TOK_MINUS);
        
        if ((get_tok_type(cur_token_ptr) == TOK_OPEN_PAREN && get_tok_type(peek_token(1)) == TOK_CLOSE_PAREN) ||
            (get_tok_type(cur_token_ptr) == TOK_OPEN_PAREN && get_tok_type(peek_token(2)) == TOK_COLON)) {
            assign_expr.assignment.assigned_fn_literal = parse_fn_literal(NULL);
        } else {
            assign_expr.assignment.assigned_value = parse_expression();
        }
    }
    
    
    Expression* expr_ptr = add_expression_to_bucket(&assign_expr, ast_root_node->allocators.expression_bucket);
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
        
        
        if (cur_token_ptr->tok_type == TOK_AMPERSAND && get_tok_type(peek_token(1)) == TOK_AMPERSAND) {
            parent_expr.expr_type = EXPR_LOGIC_AND;
            next_token();
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_PIPE && get_tok_type(peek_token(1)) == TOK_PIPE) {
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
    
    
    while (cur_token_ptr->tok_type == TOK_EQUALS             ||
           cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET ||
           cur_token_ptr->tok_type == TOK_CLOSE_ANGLE_BRACKET) {
        Expression parent_expr;
        Expression* right_expr;
        
        
        if (cur_token_ptr->tok_type == TOK_EQUALS && get_tok_type(peek_token(1)) == TOK_EQUALS) {
            parent_expr.expr_type = EXPR_COND_EQUALS;
            next_token();
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_OPEN_ANGLE_BRACKET && get_tok_type(peek_token(1)) == TOK_EQUALS) {
            parent_expr.expr_type = EXPR_COND_LOWER_EQUALS;
            next_token();
            next_token();
        } else if (cur_token_ptr->tok_type == TOK_CLOSE_ANGLE_BRACKET && get_tok_type(peek_token(1)) == TOK_EQUALS) {
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

struct Expression* parse_value_of_term() {
    next_token(); // skip over "$"
    
    Expression* ret_expr;
    
    struct Expression value_of_expr;
    value_of_expr.expr_type = EXPR_VALUE_OF;
    if (get_tok_type(cur_token_ptr) == TOK_IDENTIFIER) {
        value_of_expr.val_of_op.variable_name = cur_token_ptr->name_str;
    } else {
        // NOOOOO!!! PANICK!!!! ERROR!!!
        printf("expected an identifier after value-of operator $\n");
        return NULL;
    }
    
    next_token(); // skip over identifier
    
    ret_expr = add_expression_to_bucket(&value_of_expr, ast_root_node->allocators.expression_bucket);
    return ret_expr;
}

struct Expression* parse_factor() {
    Expression* ret_expr;
    // basically, the options here are: an expression between parentheses, a function call, or a literal, or a value-of operator and identifier
    if (cur_token_ptr->tok_type == TOK_IDENTIFIER) {
            struct Expression var_literal_expr;
            var_literal_expr.expr_type = EXPR_VAR_LITERAL;
            var_literal_expr.variable_literal = cur_token_ptr->name_str;
            ret_expr = add_expression_to_bucket(&var_literal_expr, ast_root_node->allocators.expression_bucket);
            next_token();
    }
    else if (cur_token_ptr->tok_type == TOK_NUMBER) {
        struct Expression number_literal_expr;
        number_literal_expr.expr_type = EXPR_NUM_LITERAL;
        number_literal_expr.int_literal = str_to_int(&cur_token_ptr->name_str);
        ret_expr = add_expression_to_bucket(&number_literal_expr, ast_root_node->allocators.expression_bucket);
        next_token();
    }
    else if (cur_token_ptr->tok_type == TOK_STRING) {
        struct Expression string_literal_expr;
        string_literal_expr.expr_type = EXPR_STR_LITERAL;
        string_literal_expr.string_literal = cur_token_ptr->name_str;
        ret_expr = add_expression_to_bucket(&string_literal_expr, ast_root_node->allocators.expression_bucket);
        next_token();
    }
    else if (cur_token_ptr->tok_type == TOK_OPEN_PAREN) {
        // parse parenthesized expression
        next_token();
        ret_expr = parse_expression();
        eat_token(TOK_CLOSE_PAREN);
    }
    else if (cur_token_ptr->tok_type == TOK_DOLLAR_SIGN) {
        ret_expr = parse_value_of_term();
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
    }
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

struct Token* peek_token(int offset) {
    if (cur_token_index + offset >= tok_arr_ptr->size) return NULL;
    return &(tok_arr_ptr->tokens[cur_token_index + offset]);
}

void eat_token(enum TokenType expected_type) {
    if (tok_arr_ptr->tokens[cur_token_index].tok_type != expected_type)  { (cur_token_ptr = NULL); }// error instead of return!!!
    next_token();
}

struct Token* cur_token() {
    if (cur_token_index >= tok_arr_ptr->size) return (cur_token_ptr = NULL); // exception!!!
    return (cur_token_ptr = &(tok_arr_ptr->tokens[cur_token_index]));
}

int count_tokens_until_end_token_found(int token_to_count, int end_token) {
    int count = 0;
    for (int i = 0; ((tok_arr_ptr->tokens[cur_token_index + i]).tok_type != end_token) && (i+cur_token_index < tok_arr_ptr->size); i++) {
        if (tok_arr_ptr->tokens[cur_token_index + i].tok_type == token_to_count) ++count;
    }
    return count;
}

int count_fn_parameters() {
    int i = 0;
    
    int open_parens_encountered = 1;
    ++i; // skip over initial open paren
    if (tok_arr_ptr->tokens[cur_token_index + i].tok_type == TOK_CLOSE_PAREN) return 0; // encountered an empty parameter list
    int amount_of_fn_params = 1; // there's at least one parameter
        
    while (open_parens_encountered) {
        if (open_parens_encountered > 1) {
            // this means we're in some nested function definition, skip it
            if (tok_arr_ptr->tokens[cur_token_index + i].tok_type == TOK_OPEN_PAREN)       ++open_parens_encountered;
            else if (tok_arr_ptr->tokens[cur_token_index + i].tok_type == TOK_CLOSE_PAREN) --open_parens_encountered;
            ++i;
        }
        else {
            if (tok_arr_ptr->tokens[cur_token_index + i].tok_type == TOK_OPEN_PAREN)       ++open_parens_encountered;
            else if (tok_arr_ptr->tokens[cur_token_index + i].tok_type == TOK_CLOSE_PAREN) open_parens_encountered = 0;
            else if (tok_arr_ptr->tokens[cur_token_index + i].tok_type == TOK_COMMA)       ++amount_of_fn_params;
            ++i;
        }
    }
    
    return amount_of_fn_params;
}
                 
enum TokenType get_tok_type(Token* tok) {
    if (tok == NULL) return TOK_INVALID;
    else return tok->tok_type;
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
