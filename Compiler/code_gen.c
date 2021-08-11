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
int struct_definition_from_entry_point = 1;

FILE* header_file;

struct VoidPtrArr struct_types;
struct VoidPtrArr cur_param_decl_ptr_arr;
struct VoidPtrArr cur_decl_ptr_arr;
struct Declaration* function_return_declaration;

signed int is_primitive_type(struct StringStruct* str) {
    if (str_equals_literal(str, "Unit")) return 1;
    else if (str_equals_literal(str, "u8")) return 1;
    else if (str_equals_literal(str, "u16")) return 1;
    else if (str_equals_literal(str, "u32")) return 1;
    
    return 0;
}

signed int is_value(Expression* left_expr) {
    // this function determines whether the expression is a value or a variable.
    // it returns 1 if the expression is a value, 0 if it is a variable, and dies if the variable is not found
    if (left_expr->expr_type == EXPR_IDENT_LITERAL) {
        // look in parameters
        for (int i = 0; i < cur_param_decl_ptr_arr.size; i++) {
            Declaration* cur_decl = (Declaration*)(cur_param_decl_ptr_arr.void_ptrs[i]);
            if (str_equals_str(&(left_expr->identifier_literal), &(cur_decl->identifier))) {
                // found the right declaration of the variable/value in question
                if (cur_decl->type.is_value) return 1;
                else return 0;
            }
        }
            
        // look in return declaration
        if (function_return_declaration != NULL) {
            if (str_equals_str(&(left_expr->identifier_literal), &(function_return_declaration->identifier))) {
                // found the right declaration of the variable/value in question
                if (function_return_declaration->type.is_value) return 1;
                else return 0;
            }
        }
        
        
        // look in declarations made in function body
        for (int i = 0; i < cur_decl_ptr_arr.size; i++) {
            Declaration* cur_decl = (Declaration*)(cur_decl_ptr_arr.void_ptrs[i]);
            if (str_equals_str(&(left_expr->identifier_literal), &(cur_decl->identifier))) {
                // found the right declaration of the variable/value in question
                if (cur_decl->type.is_value) return 1;
                else return 0;
            }
        }
        
    } else if (left_expr->expr_type == EXPR_MEMBER_ACCESS) {
        // look at struct member's return type
    } else if (left_expr->expr_type == EXPR_IF_THEN_ELSE) {
        // look at first expressions return type
    } else if (left_expr->expr_type == EXPR_FN_CALL) {
        // look at return type
    }
    
    // in case of any other expression (add, sub, logic or, ...) the returned thing is a value
    return 1;
    
}

void generate_code(struct Program* ast_root, char* file_name, char* header_file_name) {
    global_switch_pair.old_name = NULL;
    global_switch_pair.new_name = NULL;
    struct_types.void_ptrs = NULL;
    init_assign_dyn_arr(&assign_dyn_arr);
    
    FILE* fp = fopen(file_name, "w");
    if (fp == NULL) {
        // NOOO PANICK!!!!!
        printf("error: couldn't create/open file: %s", file_name);
        return;
    }
    
    header_file = fopen(header_file_name, "w");
    if (header_file == NULL) {
        // NOOO PANICK!!!!!
        printf("error: couldn't create/open file: %s", header_file_name);
        return;
    }
    fprintf(header_file, "#include <stdint.h>\n");
    fprintf(header_file, "#include <stdio.h>\n");
    fprintf(fp, "#include \"%s\"\n", header_file_name);
    
    write_entry_point(ast_root->entry_point, fp);
    fclose(fp);
}

void write_entry_point(struct EntryPoint* entry_point, FILE* fp) {
    struct_types                     = entry_point->struct_type_ptr_arr;
    cur_param_decl_ptr_arr.size      = 0;
    cur_decl_ptr_arr                 = entry_point->decl_ptr_arr;
    function_return_declaration      = NULL;
    
    write_import_arr(&(entry_point->imports), fp);
    fprintf(fp, "\n#include <stdint.h>\n");
    
    // struct definitions
    struct_types = entry_point->struct_type_ptr_arr;
    
    struct_definition_from_entry_point = 1;
    write_struct_definitions(entry_point->struct_type_ptr_arr, fp);
    
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
        if ((import_list->is_local_file_flags[i])) {
            fprintf(fp, "\n#include \"%s\"\n", str_to_c_str(&(import_list->imported_files[i])));
        } else {
            fprintf(fp, "\n#include <%s>\n", str_to_c_str(&(import_list->imported_files[i])));
        }
    }
}

void write_struct_definitions(struct VoidPtrArr struct_type_ptr_arr, FILE* fp) {
    for (int i = 0; i < struct_type_ptr_arr.size; i++) {
        write_struct_definition(((struct StructType*)((struct_type_ptr_arr.void_ptrs)[i])), fp);
    }
}

void write_struct_definition(struct StructType* struct_type, FILE* fp) {
    fprintf(fp, "typedef struct %s {\n", str_to_c_str(&(struct_type->struct_name)));
    
    for (int i = 0; i < struct_type->amount_of_decls; i++) {
        struct Declaration* decl = struct_type->decl_ptr_arr[i];
        
        fprintf(fp, "\t");
        if (decl->type.is_fn_ptr) {
            write_data_type(&(decl->type.fn_type->return_type), fp);
            fprintf(fp, "(*%s)", str_to_c_str(&(decl->identifier)));
            fprintf(fp, "(");
            for (int i = 0; i < decl->type.fn_type->amount_of_fn_parameters; i++) {
                write_data_type(&(decl->type.fn_type->parameter_types[i]), fp);
                if (i < decl->type.fn_type->amount_of_fn_parameters-1) fprintf(fp, " , ");
            }
            fprintf(fp, ")");
            
        } else {
            write_data_type(&(decl->type), fp);
            fprintf(fp, "%s", str_to_c_str(&(decl->identifier)));
        }
        
        fprintf(fp, ";\n");
    }
    
    fprintf(fp, "} %s;\n", str_to_c_str(&(struct_type->struct_name)));
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
    if (decl->type.is_fn_ptr) {
        if (decl->is_initialized) {
            anon_fn_name = generate_anon_fn_name();
            write_fn_literal(anon_fn_name, &(decl->init_expr->fn_ptr_literal), fp);
        }
        
        write_data_type(&(decl->type.fn_type->return_type), fp);
        //if (decl->type.is_value == 0) fprintf(fp, "*");
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
        fprintf(fp, "%s", str_to_c_str(&(decl->identifier)));
        
        if (decl->is_initialized) {
            if (decl->type.is_value) {
                fprintf(fp, " = ");
                write_expression(decl->init_expr, fp);
            } else {
                // make another assignment and add to array to write at the start of main()
                struct Assignment assignment;
                {
                    Expression* left_expr = malloc(sizeof(Expression));
                    left_expr->expr_type = EXPR_IDENT_LITERAL;
                    left_expr->identifier_literal = decl->identifier;
                    assignment.left = left_expr;
                }
                
                fprintf(fp, " = &(");
                
                {
                    if (decl->type.is_fn_ptr) {
                        write_data_type(&(decl->type.fn_type->return_type), fp);
                        fprintf(fp, " (*) ");
                        fprintf(fp, "(");
                        for (int i = 0; i < decl->type.fn_type->amount_of_fn_parameters; i++) {
                            write_data_type(&(decl->type.fn_type->parameter_types[i]), fp);
                            if (i < decl->type.fn_type->amount_of_fn_parameters-1) fprintf(fp, " , ");
                        }
                        fprintf(fp, ")");
                    } else {
                        if (str_equals_literal(&(decl->type.type_name), "Unit")) {
                            fprintf(fp, "void ");
                        } else if (str_equals_literal(&(decl->type.type_name), "u8")) {
                            fprintf(fp, "uint8_t ");
                        } else if (str_equals_literal(&(decl->type.type_name), "u16")) {
                            fprintf(fp, "uint16_t ");
                        } else if (str_equals_literal(&(decl->type.type_name), "u32")) {
                            fprintf(fp, "uint32_t ");
                        } else {
                            fprintf(fp, "%s ", str_to_c_str(&(decl->type.type_name)));
                        }
                    }
                }
                
                fprintf(fp, "){0}");
                
                if (is_value(decl->init_expr) == 1) assignment.arrow_operator = 1;
                else assignment.arrow_operator = 0;
                
                assignment.assigned_expr = decl->init_expr;
            
                add_assignment_to_arr(&assign_dyn_arr, &assignment);
            }
        } else {
            if (is_primitive_type(&(decl->type.type_name)) == 0 && decl->type.is_value == 0) {
                fprintf(fp, " = &(");
                fprintf(fp, "%s", str_to_c_str(&(decl->type.type_name)));
                fprintf(fp, "){0}");
            }
        }
    }
    
    fprintf(fp, ";\n");
}

void write_data_type (DataType* data_type, FILE* fp) {
    if (data_type->is_fn_ptr) {
        write_data_type(&(data_type->fn_type->return_type), fp);
        fprintf(fp, " (*) ");
        fprintf(fp, "(");
        for (int i = 0; i < data_type->fn_type->amount_of_fn_parameters; i++) {
            write_data_type(&(data_type->fn_type->parameter_types[i]), fp);
            if (i < data_type->fn_type->amount_of_fn_parameters-1) fprintf(fp, " , ");
        }
        fprintf(fp, ")");
    } else {
        if (str_equals_literal(&(data_type->type_name), "Unit")) {
            fprintf(fp, "void ");
        } else if (str_equals_literal(&(data_type->type_name), "u8")) {
            fprintf(fp, "uint8_t ");
        } else if (str_equals_literal(&(data_type->type_name), "u16")) {
            fprintf(fp, "uint16_t ");
        } else if (str_equals_literal(&(data_type->type_name), "u32")) {
            fprintf(fp, "uint32_t ");
        } else {
            fprintf(fp, "%s ", str_to_c_str(&(data_type->type_name)));
        }
    }
    
    if (data_type->is_value == 0) fprintf(fp, "*");
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
    switch (expr->expr_type) {
        case EXPR_ADD: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "+");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_SUB: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "-");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_MULT: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "*");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_DIV: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "/");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_IF_THEN_ELSE: {
            fprintf(fp, "(");
            write_expression(expr->tern_op.left, fp);
            fprintf(fp, " ? ");
            write_expression(expr->tern_op.middle, fp);
            fprintf(fp, " : ");
            write_expression(expr->tern_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_FN_CALL: {
            fprintf(fp, "(");
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
            fprintf(fp, ")");
            break;
        }
        case EXPR_LOGIC_AND: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "&&");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_LOGIC_OR: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "||");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_COND_EQUALS: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "==");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_COND_GREATER_EQUALS: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, ">=");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_COND_LOWER_EQUALS: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "<=");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_COND_GREATER: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, ">");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_COND_LOWER: {
            fprintf(fp, "(");
            write_expression(expr->bin_op.left, fp);
            fprintf(fp, "<");
            write_expression(expr->bin_op.right, fp);
            fprintf(fp, ")");
            break;
        }
        case EXPR_IDENT_LITERAL: {
            char* name = str_to_c_str(&(expr->identifier_literal));
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
        case EXPR_ASSIGN: {
            fprintf(fp, "(");
            // should also include assigning function literals
            if (expr->assignment.arrow_operator) {
                fprintf(fp, "*");
            } else if (is_value(expr->assignment.left) == 0) {
                // it's a variable to which a value is assigned
                if (is_value(expr->assignment.assigned_expr)) fprintf(fp, "*");
            }
            
            write_expression(expr->assignment.left, fp);
            
            fprintf(fp, " = ");
            
            write_expression(expr->assignment.assigned_expr, fp);
            /*
            if (expr->assignment.assigned_val_is_fn_literal) {
                // do some stuff with anonymous function pointer or something
                write_fn_literal(str_to_c_str(&(expr->assignment.left->identifier_literal)), expr->assignment.assigned_fn_literal, fp);
            } else {
                write_expression(expr->assignment.assigned_expr, fp);
            }
            */
            fprintf(fp, ")");
            break;
        }
        case EXPR_VALUE_OF: {
            fprintf(fp, "(");
            fprintf(fp, "*%s", str_to_c_str(&(expr->val_of_op.variable_name)));
            fprintf(fp, ")");
            break;
        }
            
        case EXPR_MEMBER_ACCESS: {
            write_expression(expr->member_access_op.parent, fp);
            if (is_value(expr->member_access_op.parent)) {
                fprintf(fp, ".");
            } else {
                fprintf(fp, "->");
            }
            
            write_expression(expr->member_access_op.member, fp);
            break;
        }
            
        case EXPR_FN_LITERAL: {
            char* anon_fn_name = NULL;
            anon_fn_name = generate_anon_fn_name();
            write_fn_literal(anon_fn_name, &(expr->fn_ptr_literal), header_file);
            fprintf(fp, "%s", anon_fn_name);
            break;
        }
            
        case EXPR_CHAR_LITERAL: {
            fprintf(fp, "\'%c\'", expr->char_literal);
            break;
        }
            
        case EXPR_BOOL_LITERAL: {
            fprintf(fp, "%d", (expr->bool_literal) ? 1 : 0);
            break;
        }
            
        
        default: {
            printf("unkown expression");
            break;
        }
            
    }
    //fprintf(fp, ")");
}

void write_assignment(struct Assignment* assignment, FILE* fp) {
    
    // should also include assigning function literals
    if (assignment->arrow_operator) {
        fprintf(fp, "*");
    } else if (is_value(assignment->left) == 0) {
        // it's a variable to which a value is assigned
        if (is_value(assignment->assigned_expr)) fprintf(fp, "*");
    }
    
    write_expression(assignment->left, fp);
    
    fprintf(fp, " = ");
    
    write_expression(assignment->assigned_expr, fp);
    /*
    if (assignment->assigned_val_is_fn_literal) {
        // do some stuff with anonymous function pointer or something
        write_fn_literal(str_to_c_str(&(assignment->left->identifier_literal)), assignment->assigned_fn_literal, fp);
    } else {
        write_expression(assignment->assigned_expr, fp);
    }
    */
    
    fprintf(fp, ";\n");
}

void write_fn_literal(char* fn_name, struct FnLiteral* fn_ptr, FILE* fp) {
    struct VoidPtrArr old_struct_types = struct_types;
    struct VoidPtrArr old_cur_param_decl_ptr_arr = cur_param_decl_ptr_arr;
    struct VoidPtrArr old_cur_decl_ptr_arr = cur_decl_ptr_arr;
    struct Declaration* old_function_return_declaration = function_return_declaration;
    
    struct_types                     = fn_ptr->struct_type_ptr_arr;
    cur_param_decl_ptr_arr           = fn_ptr->param_decl_ptr_arr;
    cur_decl_ptr_arr                 = fn_ptr->decl_ptr_arr;
    function_return_declaration      = fn_ptr->return_variable;
    
    printf("heyyyy\n");
    write_declarations_arr(fn_ptr->decl_ptr_arr, fp);
    
    write_data_type(&(fn_ptr->return_variable->type), fp);
    fprintf(fp, " %s ", fn_name);
    fprintf(fp, "(");
    for (int i = 0; i < fn_ptr->param_decl_ptr_arr.size; i++) {
        write_data_type(&(((struct Declaration*)(fn_ptr->param_decl_ptr_arr.void_ptrs[i]))->type), fp);
        fprintf(fp, " %s", str_to_c_str(&(((struct Declaration*)(fn_ptr->param_decl_ptr_arr.void_ptrs[i]))->identifier)));
        
        if (i < (fn_ptr->param_decl_ptr_arr.size)-1) fprintf(fp, " , ");
    }
    fprintf(fp, ")");
    fprintf(fp, " {\n\t");
    
    write_data_type(&(fn_ptr->return_variable->type), fp);
    fprintf(fp, " %s;", str_to_c_str(&(fn_ptr->return_variable->identifier)));
    //fprintf(fp, " __ret;\n\t");
    
    write_import_arr(&(fn_ptr->imports), fp);
    
    fprintf(fp, "\n");
    global_switch_pair.old_name = "this";
    global_switch_pair.new_name = fn_name;
    write_statements_arr(fn_ptr->stmt_ptr_arr, fp);
    global_switch_pair.old_name = NULL;
    global_switch_pair.new_name = NULL;
    
    //fprintf(fp, "\n\treturn __ret;\n");
    fprintf(fp, "\n\treturn %s;\n", str_to_c_str(&(fn_ptr->return_variable->identifier)));
    
    fprintf(fp, "\n}\n");
    
    
    struct_types = old_struct_types;
    cur_param_decl_ptr_arr = old_cur_param_decl_ptr_arr;
    cur_decl_ptr_arr = old_cur_decl_ptr_arr;
    function_return_declaration = old_function_return_declaration;
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
