#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "code_gen.h"

#include "string_commons.h"

typedef struct SwitchNamesPair {
    char *old_name, *new_name;
} SwitchNamesPair;

struct SwitchNamesPair global_switch_pair;
struct AssignDynArr    assign_dyn_arr;

void generate_code(struct Program* ast_root, char* file_name) {
    global_switch_pair.old_name = NULL;
    global_switch_pair.new_name = NULL;
    init_assign_dyn_arr(&assign_dyn_arr);
    
    FILE* fp = fopen(file_name, "w");
    if (fp == NULL) {
        // NOOO PANICK!!!!!
        printf("error: couldn't create/open file: %s", file_name);
        return;
    }
    write_entry_point(ast_root->entry_point, fp);
    fclose(fp);
}

void write_entry_point(struct EntryPoint* entry_point, FILE* fp) {
    write_import_arr(&(entry_point->imports), fp);
    fprintf(fp, "\n#include <stdint.h>\n");
    write_declarations_arr(entry_point->decl_ptr_arr, fp);
    
    fprintf(fp, "int main() {\n");
    for (int i = 0; i < assign_dyn_arr.size; i++) {
        write_assignment(&((assign_dyn_arr.assignments)[i]), fp);
    }
    write_statements_arr(entry_point->stmt_ptr_arr, fp);
    fprintf(fp, "\n\treturn 0;\n");
    fprintf(fp, "}");
}

void write_import_arr(struct ImportList* import_list, FILE* fp) {
    for (int i = 0; i < import_list->amount_of_imported_files; i++) {
        fprintf(fp, "\n#include <%s>\n", str_to_c_str(&(import_list->imported_files[i])));
    }
}

void write_declarations_arr(struct VoidPtrArr decl_ptr_arr, FILE* fp) {
    for (int i = 0; i < decl_ptr_arr.size; i++) {
        write_declaration(((struct Declaration*)((decl_ptr_arr.void_ptrs)[i])), fp);
    }
}

void write_statements_arr(struct VoidPtrArr stmt_ptr_arr, FILE* fp) {
    for (int i = 0; i < stmt_ptr_arr.size; i++) {
        write_statement(((struct Statement*)((stmt_ptr_arr.void_ptrs)[i])), fp);
    }
}

void write_declaration(struct Declaration* decl, FILE* fp) {
    char* anon_fn_name = NULL;
    if (decl->type.type_enum_val == FN_PTR) {
        if (decl->is_initialized) {
            anon_fn_name = generate_anon_fn_name();
            write_fn_literal(anon_fn_name, decl->init_fn_ptr, fp);
        }
        
        write_data_type(&(decl->type.fn_type->return_type), fp);
        if (decl->type.is_value == 0) fprintf(fp, "*");
        fprintf(fp, "(*%s)", str_to_c_str(&(decl->identifier)));
        fprintf(fp, "(");
        for (int i = 0; i < decl->type.fn_type->amount_of_fn_parameters; i++) {
            write_data_type(&(decl->type.fn_type->parameter_types[i]), fp);
            if (i < decl->type.fn_type->amount_of_fn_parameters-1) fprintf(fp, " , ");
        }
        fprintf(fp, ")");
        
        if (decl->is_initialized) {
            fprintf(fp, " = ");
            fprintf(fp, "&%s", anon_fn_name);
        }
    } else {
        write_data_type(&(decl->type), fp);
        if (decl->type.is_value == 0) fprintf(fp, "*");
        fprintf(fp, "%s", str_to_c_str(&(decl->identifier)));
        
        if (decl->is_initialized) {
            if (decl->type.is_value) {
                fprintf(fp, " = ");
                write_expression(decl->init_expr, fp);
            } else {
                // make another assignment and add to array to write at the start of main()
                struct Assignment assignment;
                assignment.variable_name = decl->identifier;
                assignment.left_hand_side_enum = LEFT_HAND_VARIABLE;
                if (decl->variable_assigned) {
                    assignment.right_hand_side_is_variable = 1;
                    assignment.assigned_variable = decl->init_variable;
                } else {
                    fprintf(fp, " = &(");
                    write_data_type(&(decl->type), fp);
                    fprintf(fp, "){0}");
                    assignment.right_hand_side_is_variable = 0;
                    assignment.assigned_value = decl->init_expr;
                }
                add_assignment_to_arr(&assign_dyn_arr, &assignment);
            }
        }
    }
    
    fprintf(fp, ";\n");
}

void write_data_type (DataType* data_type, FILE* fp) {
    switch (data_type->type_enum_val) {
        case UNIT: {
            fprintf(fp, "void ");
            break;
        }
        case UINT_8: {
            fprintf(fp, "uint8_t ");
            break;
        }
        case UINT_16: {
            fprintf(fp, "uint16_t ");
            break;
        }
        case UINT_32: {
            fprintf(fp, "uint32_t ");
            break;
        }
        case STRING: {
            fprintf(fp, "char* ");
            break;
        }
        case FN_PTR: {
            write_data_type(&(data_type->fn_type->return_type), fp);
            fprintf(fp, " (*) ");
            fprintf(fp, "(");
            for (int i = 0; i < data_type->fn_type->amount_of_fn_parameters; i++) {
                write_data_type(&(data_type->fn_type->parameter_types[i]), fp);
                if (i < data_type->fn_type->amount_of_fn_parameters-1) fprintf(fp, " , ");
            }
            fprintf(fp, ")");
            break;
        }
        case DATA_BOOL: {
           //<#statements#>
            break;
        }
            
            
        default:
            break;
    }
    
}

void write_statement(struct Statement* stmt, FILE* fp) {
    if (stmt->stmt_type == STMT_EXPR) {
        fprintf(fp, "\t");
        write_expression(stmt->expr, fp);
        fprintf(fp, ";\n");
    } else if (stmt->stmt_type == STMT_CFI) {
        
    }
}

void write_expression(struct Expression* expr, FILE* fp) {
    fprintf(fp, "(");
    switch (expr->expr_type) {
        case EXPR_ADD: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "+");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_SUB: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "-");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_MULT: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "*");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_DIV: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "/");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_IF_THEN_ELSE: {
            write_expression(expr->tern_op.left, fp);
            fprintf(fp, " ? ");
            write_expression(expr->tern_op.middle, fp);
            fprintf(fp, " : ");
            write_expression(expr->tern_op.right, fp);
            break;
        }
        case EXPR_FN_CALL: {
            
            write_expression(expr->fn_call.fn_ptr_expr, fp);
            
            fprintf(fp, "(");
            for (int i = 0; i < expr->fn_call.arg_ptr_arr.size; i++) {
                
                if(((Expression*)((expr->fn_call.arg_ptr_arr.void_ptrs)[i]))->expr_type == EXPR_FN_LITERAL) {
                    
                } else {
                    write_expression(((Expression*)((expr->fn_call.arg_ptr_arr.void_ptrs)[i])), fp);
                }
                if (i < expr->fn_call.arg_ptr_arr.size-1) fprintf(fp, " , ");
            }
            fprintf(fp, ")");
            break;
        }
        case EXPR_LOGIC_AND: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "&&");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_LOGIC_OR: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "||");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_COND_EQUALS: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "==");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_COND_GREATER_EQUALS: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, ">=");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_COND_LOWER_EQUALS: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "<=");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_COND_GREATER: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, ">");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_COND_LOWER: {
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "<");
            write_expression(expr->bin_op.right, fp);
            break;
        }
        case EXPR_VAR_LITERAL: {
            char* name = str_to_c_str(&(expr->variable_literal));
            if (global_switch_pair.old_name != NULL && !strcmp(global_switch_pair.old_name, name)) name = global_switch_pair.new_name;
            fprintf(fp, "%s", name);
            break;
        }
        case EXPR_NUM_LITERAL: {
            fprintf(fp, "%ld", expr->int_literal);
            break;
        }
        case EXPR_STR_LITERAL: {
            fprintf(fp, "\"%s\"", str_to_c_str(&(expr->string_literal)));
            break;
        }
        case EXPR_FN_LITERAL: {
            // do some stuff with anonymous function pointer or something
            break;
        }
        case EXPR_ASSIGN: {
            
            // should also include assigning function literals
            if (expr->assignment.left_hand_side_enum == LEFT_HAND_VALUE) {
                char* name = str_to_c_str(&(expr->assignment.variable_name));
                fprintf(fp, "%s", name);
            } else if (expr->assignment.left_hand_side_enum == LEFT_HAND_VARIABLE) {
                char* name = str_to_c_str(&(expr->assignment.variable_name));
                if (expr->assignment.right_hand_side_is_variable) fprintf(fp, "%s", name);
                else fprintf(fp, "*%s", name);
            } else {
                fprintf(fp, "__ret");
            }
            
            fprintf(fp, " = ");
            if (expr->assignment.assigned_val_is_fn) {
                // do some stuff with anonymous function pointer or something
            } else {
                write_expression(expr->assignment.assigned_value, fp);
            }
            break;
        }
        case EXPR_VALUE_OF: {
            fprintf(fp, "*%s", str_to_c_str(&(expr->val_of_op.variable_name)));
            break;
        }
            
        
        default: {
            printf("unkown expression");
            break;
        }
            
    }
    fprintf(fp, ")");
}

void write_assignment(struct Assignment* assignment, FILE* fp) {
    
    if (assignment->left_hand_side_enum == LEFT_HAND_VARIABLE) {
        if (assignment->right_hand_side_is_variable) {
            fprintf(fp, "\t%s = ", str_to_c_str(&(assignment->variable_name)));
            fprintf(fp, "%s", str_to_c_str(&(assignment->assigned_variable)));
        } else {
            fprintf(fp, "\t*%s = ", str_to_c_str(&(assignment->variable_name)));
            write_expression(assignment->assigned_value, fp);
        }
    } else {
        fprintf(fp, "\t%s = ", str_to_c_str(&(assignment->variable_name)));
        write_expression(assignment->assigned_value, fp);
    }
    
    fprintf(fp, ";\n");
}

void write_fn_literal(char* fn_name, struct FnLiteral* fn_ptr, FILE* fp) {
    write_declarations_arr(fn_ptr->decl_ptr_arr, fp);
    
    write_data_type(&(fn_ptr->return_type), fp);
    fprintf(fp, " %s ", fn_name);
    fprintf(fp, "(");
    for (int i = 0; i < fn_ptr->param_decl_ptr_arr.size; i++) {
        write_data_type(&(((struct Declaration*)(fn_ptr->param_decl_ptr_arr.void_ptrs[i]))->type), fp);
        fprintf(fp, " %s", str_to_c_str(&(((struct Declaration*)(fn_ptr->param_decl_ptr_arr.void_ptrs[i]))->identifier)));
        if (i < (fn_ptr->param_decl_ptr_arr.size)-1) fprintf(fp, " , ");
    }
    fprintf(fp, ")");
    fprintf(fp, " {\n\t");
    
    write_data_type(&(fn_ptr->return_type), fp);
    fprintf(fp, " __ret;\n\t");
    
    write_import_arr(&(fn_ptr->imports), fp);
    
    fprintf(fp, "\n");
    global_switch_pair.old_name = "this";
    global_switch_pair.new_name = fn_name;
    write_statements_arr(fn_ptr->stmt_ptr_arr, fp);
    global_switch_pair.old_name = NULL;
    global_switch_pair.new_name = NULL;
    
    fprintf(fp, "\n\treturn __ret;\n");
    
    fprintf(fp, "\n}\n");
}

/* ----------------------------------------------------------------------------- */



char* generate_anon_fn_name() {
    static int count = 0;
    
    char* prefix = malloc(5 * sizeof(char));
    prefix[0] = 'a'; prefix[1] = 'n'; prefix[2] = 'o'; prefix[3] = 'n'; prefix[4] = '\0';
    char count_str[10];
    sprintf(count_str, "%d", count);
    
    prefix = strcat(prefix, count_str);
    
    count++;
    
    return prefix;
    
}

void init_assign_dyn_arr(struct AssignDynArr* assign_dyn_arr) {
    assign_dyn_arr->size = 0;
    assign_dyn_arr->capacity = 100;
    assign_dyn_arr->assignments = malloc(assign_dyn_arr->capacity * sizeof(struct Assignment));
}

void add_assignment_to_arr(struct AssignDynArr* assign_dyn_arr, struct Assignment* assignment_to_add) {
    if (assign_dyn_arr->size >= assign_dyn_arr->capacity - 1) {
        assign_dyn_arr->capacity = 2 * assign_dyn_arr->capacity;
        assign_dyn_arr->assignments = realloc(assign_dyn_arr->assignments, assign_dyn_arr->capacity * sizeof(struct Assignment));
    }
    assign_dyn_arr->assignments[assign_dyn_arr->size] = *assignment_to_add;
    assign_dyn_arr->size += 1;
}

void free_assign_dyn_arr(struct AssignDynArr* assign_dyn_arr) {
    free(assign_dyn_arr->assignments);
    free(assign_dyn_arr);
}

