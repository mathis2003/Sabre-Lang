
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"


// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// SCOPE NODES ALLOCATOR
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------


// this function adds a FnLiteralBucket to the `next_bucket` member of the passed FnLiteralBucket
// this function should typically not be exposed to the programmer
struct FnLiteralBucket* add_new_fn_literal_bucket(struct FnLiteralBucket* fn_literal_bucket) {
    struct FnLiteralBucket* next_fn_literal_bucket = malloc(sizeof(FnLiteralBucket));
    if (next_fn_literal_bucket == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    
    // initialize new fn_literal bucket
    next_fn_literal_bucket->size             = 0;
    next_fn_literal_bucket->capacity         = fn_literal_bucket->capacity * 2;
    next_fn_literal_bucket->fn_literals      = malloc(next_fn_literal_bucket->capacity * sizeof(struct FnLiteral));
    // check if we ran out of heap memory
    if (next_fn_literal_bucket->fn_literals == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    // haven't run out? great, go on
    next_fn_literal_bucket->next_bucket = NULL;
    next_fn_literal_bucket->prev_bucket = fn_literal_bucket;
    
    // add new fn_literal bucket to the end of the "linked list"
    fn_literal_bucket->next_bucket = next_fn_literal_bucket;
    
    return next_fn_literal_bucket;
}


// this should be visible to the programmer
struct FnLiteral* add_fn_literal_to_bucket(struct FnLiteral* fn_literal_to_add, struct MainFnLiteralBucket* fn_literal_bucket) {
    // find last bucket
    FnLiteralBucket* cur_fn_literal_bucket = fn_literal_bucket->root_bucket;
    
    while (cur_fn_literal_bucket->next_bucket != NULL) cur_fn_literal_bucket = cur_fn_literal_bucket->next_bucket;
    
    if (cur_fn_literal_bucket->size >= cur_fn_literal_bucket->capacity)
        cur_fn_literal_bucket = add_new_fn_literal_bucket(cur_fn_literal_bucket);
    
    cur_fn_literal_bucket->fn_literals[cur_fn_literal_bucket->size] = *fn_literal_to_add;
    cur_fn_literal_bucket->size += 1;
    
    return &(cur_fn_literal_bucket->fn_literals[(cur_fn_literal_bucket->size - 1)]);
    
}


void init_main_fn_literal_bucket(struct Program* program) {
    
    MainFnLiteralBucket* fn_literal_bucket_ptr = malloc(sizeof(MainFnLiteralBucket));
    fn_literal_bucket_ptr->root_bucket = malloc(sizeof(struct FnLiteralBucket));
    if (fn_literal_bucket_ptr->root_bucket == NULL) {
        printf("FUCKKKKK!!!!!!");
        return;
    }
    
    fn_literal_bucket_ptr->root_bucket->size             = 0;
    fn_literal_bucket_ptr->root_bucket->capacity         = 100;
    fn_literal_bucket_ptr->root_bucket->fn_literals      = malloc(fn_literal_bucket_ptr->root_bucket->capacity * sizeof(struct FnLiteral));
    if (fn_literal_bucket_ptr->root_bucket->fn_literals == NULL) { return; /* NO!!!! PANICK!!!! ERROR!!!!! */}
    fn_literal_bucket_ptr->root_bucket->next_bucket      = NULL;
    fn_literal_bucket_ptr->root_bucket->prev_bucket      = NULL;
    
    program->allocators.fn_literal_bucket = fn_literal_bucket_ptr;

}

void free_fn_literal_bucket(MainFnLiteralBucket* fn_literal_bucket) {
    
    // find last fn_literal bucket in the linked list
    FnLiteralBucket* last_fn_literal_bucket = fn_literal_bucket->root_bucket;
    while (last_fn_literal_bucket->next_bucket != NULL) last_fn_literal_bucket = last_fn_literal_bucket->next_bucket;
    
    // free fn_literal buckets, starting with the last one
    while (last_fn_literal_bucket->prev_bucket != NULL) {
        last_fn_literal_bucket = last_fn_literal_bucket->prev_bucket;
        free(last_fn_literal_bucket->next_bucket->fn_literals);
        free(last_fn_literal_bucket->next_bucket);
    }
    
    free(last_fn_literal_bucket->fn_literals);
    free(last_fn_literal_bucket);
    
    free(fn_literal_bucket);
             
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// DATA DECLARATION NODES ALLOCATOR
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

// this function adds a DeclarationBucket to the `next_bucket` member of the passed DeclarationBucket
// this function should typically not be exposed to the programmer
struct DeclarationBucket* add_new_declaration_bucket(struct DeclarationBucket* declaration_bucket) {
    struct DeclarationBucket* next_declaration_bucket = malloc(sizeof(DeclarationBucket));
    if (next_declaration_bucket == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    
    // initialize new declaration bucket
    next_declaration_bucket->size         = 0;
    next_declaration_bucket->capacity     = declaration_bucket->capacity * 2;
    next_declaration_bucket->declarations = malloc(next_declaration_bucket->capacity * sizeof(struct Declaration));
    // check if we ran out of heap memory
    if (next_declaration_bucket->declarations == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    // haven't run out? great, go on
    next_declaration_bucket->next_bucket  = NULL;
    next_declaration_bucket->prev_bucket  = declaration_bucket;
    
    // add new declaration bucket to the end of the "linked list"
    declaration_bucket->next_bucket = next_declaration_bucket;
    
    return next_declaration_bucket;
}


// this should be visible to the programmer
struct Declaration* add_declaration_to_bucket(struct Declaration* declaration_to_add, struct MainDeclarationBucket* declaration_bucket) {
    // find last bucket
    struct DeclarationBucket* cur_declaration_bucket = declaration_bucket->root_bucket;
    
    while (cur_declaration_bucket->next_bucket != NULL) cur_declaration_bucket = cur_declaration_bucket->next_bucket;
    
    if (cur_declaration_bucket->size >= cur_declaration_bucket->capacity)
        cur_declaration_bucket = add_new_declaration_bucket(cur_declaration_bucket);
    
    cur_declaration_bucket->declarations[cur_declaration_bucket->size] = *declaration_to_add;
    cur_declaration_bucket->size += 1;
    
    return &(cur_declaration_bucket->declarations[(cur_declaration_bucket->size - 1)]);
    
}


void init_main_declaration_bucket(struct Program* program) {
    
    struct MainDeclarationBucket* declaration_bucket_ptr = malloc(sizeof(MainDeclarationBucket));
    declaration_bucket_ptr->root_bucket = malloc(sizeof(struct DeclarationBucket));
    if (declaration_bucket_ptr->root_bucket == NULL) {
        printf("FUCKKKKK!!!!!!");
        return;
    }
    
    declaration_bucket_ptr->root_bucket->size         = 0;
    declaration_bucket_ptr->root_bucket->capacity     = 100;
    declaration_bucket_ptr->root_bucket->declarations = malloc(declaration_bucket_ptr->root_bucket->capacity * sizeof(struct Declaration));
    if (declaration_bucket_ptr->root_bucket->declarations == NULL) { return; /* NO!!!! PANICK!!!! ERROR!!!!! */}
    declaration_bucket_ptr->root_bucket->next_bucket  = NULL;
    declaration_bucket_ptr->root_bucket->prev_bucket  = NULL;
    
    program->allocators.declaration_bucket = declaration_bucket_ptr;

}

void free_declaration_bucket(MainDeclarationBucket* declaration_bucket) {
    
    // find last declaration bucket in the linked list
    DeclarationBucket* last_declaration_bucket = declaration_bucket->root_bucket;
    while (last_declaration_bucket->next_bucket != NULL) last_declaration_bucket = last_declaration_bucket->next_bucket;
    
    // free declaration buckets, starting with the last one
    while (last_declaration_bucket->prev_bucket != NULL) {
        last_declaration_bucket = last_declaration_bucket->prev_bucket;
        // don't forget to free the parameter types if declaration was a function
        for (int i = 0; i < last_declaration_bucket->size; i++) {
            /*if (last_declaration_bucket->next_bucket->declarations[i].type == FN_PTR &&
                last_declaration_bucket->next_bucket->declarations[i].fn_type.amount_of_parameters > 0) {
                free(last_declaration_bucket->next_bucket->declarations[i].fn_type.parameter_types);
            }*/
        }
        free(last_declaration_bucket->next_bucket->declarations);
        free(last_declaration_bucket->next_bucket);
    }
    
    free(last_declaration_bucket->declarations);
    free(last_declaration_bucket);
    
    free(declaration_bucket);
}


// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// STATEMENT NODES ALLOCATOR
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

// this function adds a StatementBucket to the `next_bucket` member of the passed StatementBucket
// this function should typically not be exposed to the programmer
struct StatementBucket* add_new_statement_bucket(struct StatementBucket* statement_bucket) {
    struct StatementBucket* next_statement_bucket = malloc(sizeof(StatementBucket));
    if (next_statement_bucket == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    
    // initialize new statement bucket
    next_statement_bucket->size             = 0;
    next_statement_bucket->capacity         = statement_bucket->capacity * 2;
    next_statement_bucket->statements       = malloc(next_statement_bucket->capacity * sizeof(struct Statement));
    // check if we ran out of heap memory
    if (next_statement_bucket->statements == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    // haven't run out? great, go on
    next_statement_bucket->next_bucket = NULL;
    next_statement_bucket->prev_bucket = statement_bucket;
    
    // add new statement bucket to the end of the "linked list"
    statement_bucket->next_bucket = next_statement_bucket;
    
    return next_statement_bucket;
}


// this should be visible to the programmer
struct Statement* add_statement_to_bucket(struct Statement* statement_to_add, struct MainStatementBucket* statement_bucket) {
    // find last bucket
    StatementBucket* cur_statement_bucket = statement_bucket->root_bucket;
    
    while (cur_statement_bucket->next_bucket != NULL) cur_statement_bucket = cur_statement_bucket->next_bucket;
    
    if (cur_statement_bucket->size >= cur_statement_bucket->capacity)
        cur_statement_bucket = add_new_statement_bucket(cur_statement_bucket);
    
    cur_statement_bucket->statements[cur_statement_bucket->size] = *statement_to_add;
    cur_statement_bucket->size += 1;
    
    return &(cur_statement_bucket->statements[(cur_statement_bucket->size - 1)]);
    
}


void init_main_statement_bucket(struct Program* program) {
    
    MainStatementBucket* statement_bucket_ptr = malloc(sizeof(struct MainStatementBucket));
    statement_bucket_ptr->root_bucket = malloc(sizeof(struct StatementBucket));
    if (statement_bucket_ptr->root_bucket == NULL) {
        printf("FUCKKKKK!!!!!!");
        return;
    }
    
    statement_bucket_ptr->root_bucket->size            = 0;
    statement_bucket_ptr->root_bucket->capacity        = 100;
    statement_bucket_ptr->root_bucket->statements      = malloc(statement_bucket_ptr->root_bucket->capacity * sizeof(struct Statement));
    if (statement_bucket_ptr->root_bucket->statements == NULL) { return; /* NO!!!! PANICK!!!! ERROR!!!!! */}
    statement_bucket_ptr->root_bucket->next_bucket     = NULL;
    statement_bucket_ptr->root_bucket->prev_bucket     = NULL;
    
    program->allocators.statement_bucket = statement_bucket_ptr;

}

void free_statement_bucket(MainStatementBucket* statement_bucket) {
    
    // find last statement bucket in the linked list
    StatementBucket* last_statement_bucket = statement_bucket->root_bucket;
    while (last_statement_bucket->next_bucket != NULL) last_statement_bucket = last_statement_bucket->next_bucket;
    
    // free statement buckets, starting with the last one
    while (last_statement_bucket->prev_bucket != NULL) {
        last_statement_bucket = last_statement_bucket->prev_bucket;
        free(last_statement_bucket->next_bucket->statements);
        free(last_statement_bucket->next_bucket);
    }
    
    free(last_statement_bucket->statements);
    free(last_statement_bucket);
    
    free(statement_bucket);
             
}



// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// EXPRESSION NODES ALLOCATOR
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

// this function adds an ExpressionBucket to the `next_bucket` member of the passed ExpressionBucket
// this function should typically not be exposed to the programmer
struct ExpressionBucket* add_new_expression_bucket(struct ExpressionBucket* expression_bucket) {
    struct ExpressionBucket* next_expression_bucket = malloc(sizeof(ExpressionBucket));
    if (next_expression_bucket == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    
    // initialize new expression bucket
    next_expression_bucket->size              = 0;
    next_expression_bucket->capacity          = expression_bucket->capacity * 2;
    next_expression_bucket->expressions       = malloc(next_expression_bucket->capacity * sizeof(struct Expression));
    // check if we ran out of heap memory
    if (next_expression_bucket->expressions == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    // haven't run out? great, go on
    next_expression_bucket->next_bucket = NULL;
    next_expression_bucket->prev_bucket = expression_bucket;
    
    // add new expression bucket to the end of the "linked list"
    expression_bucket->next_bucket = next_expression_bucket;
    
    return next_expression_bucket;
}


// this should be visible to the programmer
struct Expression* add_expression_to_bucket(struct Expression* expression_to_add, struct MainExpressionBucket* expression_bucket) {
    // find last bucket
    ExpressionBucket* cur_expression_bucket = expression_bucket->root_bucket;
    
    while (cur_expression_bucket->next_bucket != NULL) cur_expression_bucket = cur_expression_bucket->next_bucket;
    
    if (cur_expression_bucket->size >= cur_expression_bucket->capacity)
        cur_expression_bucket = add_new_expression_bucket(cur_expression_bucket);
    
    cur_expression_bucket->expressions[cur_expression_bucket->size] = *expression_to_add;
    cur_expression_bucket->size += 1;
    
    return &(cur_expression_bucket->expressions[(cur_expression_bucket->size - 1)]);
    
}


void init_main_expression_bucket(struct Program* program) {
    
    MainExpressionBucket* expression_bucket_ptr = malloc(sizeof(struct MainExpressionBucket));
    expression_bucket_ptr->root_bucket = malloc(sizeof(struct ExpressionBucket));
    if (expression_bucket_ptr->root_bucket == NULL) {
        printf("FUCKKKKK!!!!!!");
        return;
    }
    
    expression_bucket_ptr->root_bucket->size             = 0;
    expression_bucket_ptr->root_bucket->capacity         = 100;
    expression_bucket_ptr->root_bucket->expressions      = malloc(expression_bucket_ptr->root_bucket->capacity * sizeof(struct Expression));
    if (expression_bucket_ptr->root_bucket->expressions == NULL) { return; /* NO!!!! PANICK!!!! ERROR!!!!! */}
    expression_bucket_ptr->root_bucket->next_bucket     = NULL;
    expression_bucket_ptr->root_bucket->prev_bucket     = NULL;
    
    program->allocators.expression_bucket = expression_bucket_ptr;

}

void free_expression_bucket(MainExpressionBucket* expression_bucket) {
    
    // find last expression bucket in the linked list
    ExpressionBucket* last_expression_bucket = expression_bucket->root_bucket;
    while (last_expression_bucket->next_bucket != NULL) last_expression_bucket = last_expression_bucket->next_bucket;
    
    // free expression buckets, starting with the last one
    while (last_expression_bucket->prev_bucket != NULL) {
        last_expression_bucket = last_expression_bucket->prev_bucket;
        free(last_expression_bucket->next_bucket->expressions);
        free(last_expression_bucket->next_bucket);
    }
    
    free(last_expression_bucket->expressions);
    free(last_expression_bucket);
    
    free(expression_bucket);
             
}


// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// VOID POINTER ARRAY ALLOCATOR
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

void init_void_ptr_arr(struct VoidPtrArr* void_ptr_arr) {
    void_ptr_arr->size = 0;
    void_ptr_arr->capacity = 10;
    void_ptr_arr->void_ptrs = malloc(void_ptr_arr->capacity * sizeof(void*));
}

void add_void_ptr_to_arr(struct VoidPtrArr* void_ptr_arr, void* ptr_to_add) {
    if (void_ptr_arr->size >= void_ptr_arr->capacity) {
        void_ptr_arr->capacity *= 2;
        void_ptr_arr->void_ptrs = realloc(void_ptr_arr->void_ptrs, void_ptr_arr->capacity * sizeof(void*));
    }
    void_ptr_arr->void_ptrs[void_ptr_arr->size] = ptr_to_add;
    void_ptr_arr->size += 1;
}

void free_void_ptr_arr(struct VoidPtrArr* void_ptr_arr) {
    free(void_ptr_arr->void_ptrs);
}
