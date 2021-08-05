#ifndef CODE_GEN_H
#define CODE_GEN_H

void generate_code(struct Program* ast_root, char* file_name);
void write_entry_point(struct EntryPoint* entry_point, FILE* fp);
void write_import_arr(struct ImportList* import_list, FILE* fp);
void write_declarations_arr(struct VoidPtrArr decl_ptr_arr, FILE* fp);
void write_statements_arr(struct VoidPtrArr stmt_ptr_arr, FILE* fp);
void write_declaration(struct Declaration* decl, FILE* fp);
void write_data_type (DataType* data_type, FILE* fp);
void write_statement(struct Statement* stmt, FILE* fp);
void write_expression(struct Expression* expr, FILE* fp);
void write_fn_literal(char* fn_name, struct FnLiteral* fn_ptr, FILE* fp);

char* generate_anon_fn_name();


#endif
