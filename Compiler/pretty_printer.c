
// INCLUDES
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "string_commons.h"

/* ----------------------------------TOKENS---------------------------------- */

void print_tokens(TokenArr* tok_arr) {
    
    // Print header
    
    printf("\n\n----------------\n|    TOKENS   |\n----------------\n\n");
    
    // Print body
    
    for (int i = 0; i < tok_arr->size; i++) {
        printf("TOKEN:\n");
        printf("\t-file: %s\n", tok_arr->tokens[i].file_name);
        printf("\t-line: %d\n", tok_arr->tokens[i].line);
        printf("\t-type: ");
        switch (tok_arr->tokens[i].tok_type) {
            case TOK_IDENTIFIER: {
                printf("TOK_IDENTIFIER\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_NUMBER: {
                printf("TOK_NUMBER\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_STRING: {
                printf("TOK_STRING\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_CHAR: {
                printf("TOK_CHAR\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_COMMA: {
                printf("TOK_COMMA\n");
                break;
            }
            
            case TOK_DOT: {
                printf("TOK_DOT\n");
                break;
            }
            
            case TOK_SEMI_COLON: {
                printf("TOK_SEMI_COLON\n");
                break;
            }
                
            case TOK_COLON: {
                printf("TOK_COLON\n");
                break;
            }
            
            case TOK_PLUS: {
                printf("TOK_PLUS\n");
                break;
            }
            
            case TOK_MINUS: {
                printf("TOK_MINUS\n");
                break;
            }
            
            case TOK_ASTERISK: {
                printf("TOK_ASTERISK\n");
                break;
            }
            
            case TOK_SLASH: {
                printf("TOK_SLASH\n");
                break;
            }
            
            case TOK_DOLLAR_SIGN: {
                printf("TOK_DOLLAR_SIGN\n");
                break;
            }
            
            case TOK_AT_SIGN: {
                printf("TOK_AT_SIGN\n");
                break;
            }
            
            case TOK_AMPERSAND: {
                printf("TOK_AMPERSAND\n");
                break;
            }
            
            case TOK_PIPE: {
                printf("TOK_PIPE\n");
                break;
            }
            
            case TOK_EQUALS: {
                printf("TOK_EQUALS\n");
                break;
            }
            
            case TOK_OPEN_PAREN: {
                printf("TOK_OPEN_PAREN\n");
                break;
            }
            
            case TOK_CLOSE_PAREN: {
                printf("TOK_CLOSE_PAREN\n");
                break;
            }
            
            case TOK_OPEN_CURLY: {
                printf("TOK_OPEN_CURLY\n");
                break;
            }
            
            case TOK_CLOSE_CURLY: {
                printf("TOK_CLOSE_CURLY\n");
                break;
            }
            
            case TOK_OPEN_SQUARE_BRACKET: {
                printf("TOK_OPEN_SQUARE_BRACKET\n");
                break;
            }
            
            case TOK_CLOSE_SQUARE_BRACKET: {
                printf("TOK_CLOSE_SQUARE_BRACKET\n");
                break;
            }
            
            case TOK_OPEN_ANGLE_BRACKET: {
                printf("TOK_OPEN_ANGLE_BRACKET\n");
                break;
            }
            
            case TOK_CLOSE_ANGLE_BRACKET: {
                printf("TOK_CLOSE_ANGLE_BRACKET\n");
                break;
            }
                
            default:
                break;
        }
        
    }
}


/* ----------------------------------AST---------------------------------- */


#define PRINT_NODE(node_name, tree_level) \
for (int i = 0; i < tree_level; i++) printf("    "); \
printf("%s", node_name);

void print_fn_literal(struct FnLiteral* fn_literal, int tree_level);
void print_import_list(struct ImportList* import_list, int tree_level);
void print_declaration(struct Declaration* decl, int tree_level);
void print_struct_types(struct VoidPtrArr* struct_type_ptr_arr, int tree_level);

void print_expression(Expression* expr, int tree_level) {
    PRINT_NODE("-EXPRESSION:\n", tree_level);
    switch (expr->expr_type) {
        case EXPR_ADD: {
            PRINT_NODE("type: add\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_SUB: {
            PRINT_NODE("type: sub\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_MULT: {
            PRINT_NODE("type: mult\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_DIV: {
            PRINT_NODE("type: div\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_LOGIC_AND: {
            PRINT_NODE("type: logic and\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_LOGIC_OR: {
            PRINT_NODE("type: logic or\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_COND_EQUALS: {
            PRINT_NODE("type: equals condition\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_COND_GREATER_EQUALS: {
            PRINT_NODE("type: greater or equals condition\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_COND_LOWER_EQUALS: {
            PRINT_NODE("type: lower or equals condition\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_COND_GREATER: {
            PRINT_NODE("type: greater condition\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        case EXPR_COND_LOWER: {
            PRINT_NODE("type: lower condition\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->bin_op.left, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->bin_op.right, tree_level+1);
            break;
        }
        
            
        /* also put function call, if expression, ... here */
        
        case EXPR_IF_THEN_ELSE: {
            PRINT_NODE("type: if-then-else\n", tree_level+1);
            PRINT_NODE("left:\n", tree_level+1);
            print_expression(expr->tern_op.left, tree_level+1);
            PRINT_NODE("middle:\n", tree_level+1);
            print_expression(expr->tern_op.middle, tree_level+1);
            PRINT_NODE("right:\n", tree_level+1);
            print_expression(expr->tern_op.right, tree_level+1);
            break;
        }
        
        case EXPR_FN_CALL: {
            PRINT_NODE("type: function call\n", tree_level+1);
            PRINT_NODE("function pointer:\n", tree_level+1);
            print_expression(expr->fn_call.fn_ptr_expr, tree_level+1);
            PRINT_NODE("argument list:\n", tree_level+1);
            if (expr->fn_call.arg_ptr_arr.size < 1) {
                PRINT_NODE("", tree_level+2);
                printf("Empty\n");
            }
            else {
                for (int i = 0; i < expr->fn_call.arg_ptr_arr.size; i++) {
                    print_expression((Expression*)(expr->fn_call.arg_ptr_arr.void_ptrs[i]), tree_level+2);
                }
            }
            break;
        }
            
        case EXPR_ASSIGN: {
            PRINT_NODE("type: assignment\n", tree_level+1);
            PRINT_NODE("left variable:\n", tree_level+1);
            if ((expr->assignment.left_hand_side_enum == LEFT_HAND_VARIABLE) ||(expr->assignment.left_hand_side_enum == LEFT_HAND_VALUE)) {
                PRINT_NODE("", tree_level+2);
                print_str_struct(&(expr->assignment.variable_name));
                printf("\n");
            } else {
                // left hand side is scope
                printf("\n");
                PRINT_NODE("scope:\n", tree_level+2);
                PRINT_NODE("index:\n", tree_level + 3);
                PRINT_NODE("", tree_level + 4);
                printf("%d\n", expr->assignment.scope_object.index);
            }
            PRINT_NODE("assigned:\n", tree_level+1);
            print_expression(expr->assignment.assigned_value, tree_level+1);
            break;
        }
        
        case EXPR_VAR_LITERAL: {
            PRINT_NODE("type: variable value\n", tree_level+1);
            PRINT_NODE("variable: ", tree_level+1);
            print_str_struct(&(expr->variable_literal));
            printf("\n");
            break;
        }
        case EXPR_NUM_LITERAL: {
            PRINT_NODE("type: number literal\n", tree_level+1);
            PRINT_NODE("number: ", tree_level+1);
            printf("%ld\n", expr->int_literal);
            break;
        }
        case EXPR_STR_LITERAL: {
            PRINT_NODE("type: string literal\n", tree_level+1);
            PRINT_NODE("string: ", tree_level+1);
            printf("%s\n", str_to_c_str(&(expr->string_literal)));
            break;
        }
        case EXPR_VALUE_OF: {
            PRINT_NODE("type: value of ($)\n", tree_level+1);
            PRINT_NODE("variable: ", tree_level+1);
            print_str_struct(&(expr->val_of_op.variable_name));
            printf("\n");
            break;
        }
            
        default: {
            PRINT_NODE("", tree_level + 1);
            printf("couldn't match expression type: %d\n", expr->expr_type);
            break;
        }
            
    }
    printf("\n");
}

void print_cfi(struct ControlFlowIndicator* cfi_ptr, int tree_level) {
    PRINT_NODE("Control Flow Indicator:\n", tree_level);
    PRINT_NODE("index: ", tree_level + 1);
    printf("%d\n", cfi_ptr->scope_obj.index);
    PRINT_NODE("label: ", tree_level + 1);
    if (cfi_ptr->scope_obj.start_lbl) {
        printf("start\n");
    } else if (cfi_ptr->scope_obj.end_lbl) {
        printf("end\n");
    } else if (cfi_ptr->scope_obj.exit_lbl) {
        printf("exit\n");
    }
}

void print_statements(struct VoidPtrArr* stmt_ptr_arr, int tree_level) {
    for (int i = 0; i < stmt_ptr_arr->size; i++) {
        printf("\n");
        PRINT_NODE("-STATEMENT:\n", tree_level);
        switch (((struct Statement*)(stmt_ptr_arr->void_ptrs[i]))->stmt_type) {
            case STMT_EXPR: {
                print_expression(((struct Statement*)(stmt_ptr_arr->void_ptrs[i]))->expr, tree_level+1);
                break;
            }
            case STMT_CFI: {
                print_cfi(&(((struct Statement*)(stmt_ptr_arr->void_ptrs[i]))->cfi) , tree_level+1);
                break;
            }
            
            default:
                break;
                
        }
    }
}

void print_data_type(struct DataType* data_type_ptr, int tree_level) {
    PRINT_NODE("Type:\n", tree_level);
    if (data_type_ptr->is_value) { PRINT_NODE("$\n", tree_level); }
    switch (data_type_ptr->type_enum_val) {
        case UINT_8: {
            PRINT_NODE("u8\n", tree_level+1);
            break;
        }
        case UINT_16: {
            PRINT_NODE("u16\n", tree_level+1);
            break;
        }
        case UINT_32: {
            PRINT_NODE("u32\n", tree_level+1);
            break;
        }
        case STRING: {
            PRINT_NODE("String\n", tree_level+1);
            break;
        }
        case FN_PTR: {
            PRINT_NODE("function\n", tree_level+1);
            PRINT_NODE("parameters:\n", tree_level+2);
            struct FunctionType* function_type = data_type_ptr->fn_type;
            for (int i = 0; i < function_type->amount_of_fn_parameters; i++) {
                print_data_type(&(function_type->parameter_types[i]), tree_level+3);
            }
            PRINT_NODE("return type:\n", tree_level+2);
            print_data_type(&(function_type->return_type), tree_level+3);
            break;
        }
        case UNIT: {
            PRINT_NODE("Unit\n", tree_level+1);
            break;
        }
            
        default:
            break;
    }
}

void print_init_val(struct Declaration* decl_ptr, int tree_level) {
    PRINT_NODE("init val:\n", tree_level);
    
    if (decl_ptr->type.is_value) {
        if (decl_ptr->type.type_enum_val == FN_PTR) {
            print_fn_literal(decl_ptr->init_fn_ptr, tree_level + 1);
        } else print_expression(decl_ptr->init_expr, tree_level);
    } else {
        if (decl_ptr->variable_assigned) {
            PRINT_NODE("", tree_level+1);
            print_str_struct(&(decl_ptr->init_variable));
        } else {
            if (decl_ptr->type.type_enum_val == FN_PTR) {
                print_fn_literal(decl_ptr->init_fn_ptr, tree_level + 1);
            } else print_expression(decl_ptr->init_expr, tree_level);
        }
    }
}

void print_declarations(struct VoidPtrArr* decl_ptr_arr, int tree_level) {
    for (int i = 0; i < decl_ptr_arr->size; i++) {
        print_declaration(((struct Declaration*)(decl_ptr_arr->void_ptrs[i])), tree_level);
    }
}

void print_declaration(struct Declaration* decl, int tree_level) {
    printf("\n");
    PRINT_NODE("-DECLARATION:\n", tree_level);
    PRINT_NODE("Name: ", tree_level + 1);
    print_str_struct(&(decl->identifier));
    printf("\n");
    print_data_type(&(decl->type), tree_level+1);
    if (decl->is_initialized) {
        print_init_val(decl, tree_level+1);
    }
}



void print_fn_literal(struct FnLiteral* fn_literal, int tree_level) {
    
    PRINT_NODE("Parameter(s): \n", tree_level + 1);
    for (int i = 0; i < fn_literal->param_decl_ptr_arr.size; i++) {
        PRINT_NODE("Name: ", tree_level + 2);
        print_str_struct(&((struct Declaration*)(fn_literal->param_decl_ptr_arr.void_ptrs[i]))->identifier);
        printf("\n");
        print_data_type(&(((struct Declaration*)(fn_literal->param_decl_ptr_arr.void_ptrs[i]))->type), tree_level+2);
        
    }
    
    PRINT_NODE("Return Type:\n", tree_level + 1);
    print_data_type(&(fn_literal->return_type), tree_level+1);

    print_import_list(&(fn_literal->imports), tree_level + 1);
    print_struct_types(&(fn_literal->struct_type_ptr_arr), tree_level+1);
    print_declarations(&(fn_literal->decl_ptr_arr), tree_level+1);
    print_statements(&(fn_literal->stmt_ptr_arr), tree_level+1);
}

void print_struct_type(struct StructType* struct_type, int tree_level) {
    PRINT_NODE("-STRUCT DEFINITION:\n", tree_level);
    PRINT_NODE("Struct Name: ", tree_level+1);
    print_str_struct(&(struct_type->struct_name));
    printf("\n");
    
    for (int i = 0; i < struct_type->amount_of_decls; i++) {
        print_declaration((struct_type->decl_ptr_arr)[i], tree_level+1);
    }
}

void print_struct_types(struct VoidPtrArr* struct_type_ptr_arr, int tree_level) {
    for (int i = 0; i < struct_type_ptr_arr->size; i++) {
        print_struct_type(((struct StructType*)(struct_type_ptr_arr->void_ptrs[i])), tree_level);
    }
}

void print_import_list(struct ImportList* import_list, int tree_level) {
    printf("\n");
    PRINT_NODE("-IMPORTS:\n", tree_level);
    for (int i = 0; i < import_list->amount_of_imported_files; i++) {
        PRINT_NODE("import file name: ", tree_level+1);
        print_str_struct(&(import_list->imported_files[i]));
        printf("\n");
    }
}

void print_entry_point(struct EntryPoint* entry_point) {
    print_import_list(&(entry_point->imports), 2);
    print_struct_types(&(entry_point->struct_type_ptr_arr), 2);
    print_declarations(&(entry_point->decl_ptr_arr), 2);
    print_statements(&(entry_point->stmt_ptr_arr), 2);
}

void print_parse_tree(Program* program_node) {
    
    // Print header
    printf("\n\n---------------\n|     AST     |\n---------------\n\n");
    
    // Print body
    
    printf("-PROGRAM:\n");
    /// print entry point
    if (program_node->entry_point == NULL) printf("NO ENTRY POINT => INVALID PROGRAM\n");
    else {
        printf("\n");
        PRINT_NODE("-ENTRY_POINT:\n", 1);
        print_entry_point(program_node->entry_point);
    }
}


/* ----------------------------------OUTPUT CODE---------------------------------- */


void print_generated_code(char* output_file_name) {
    printf("\n\n---------------\n| OUTPUT CODE |\n---------------\n\n");
    FILE* fp = fopen(output_file_name, "r");
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }
    
    fclose(fp);
}
