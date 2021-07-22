
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"


// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// SCOPE NODES ALLOCATOR
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------


// this function adds a ScopeBucket to the `next_bucket` member of the passed ScopeBucket
// this function should typically not be exposed to the programmer
struct ScopeBucket* add_new_scope_bucket(struct ScopeBucket* scope_bucket) {
    struct ScopeBucket* next_scope_bucket = malloc(sizeof(ScopeBucket));
    if (next_scope_bucket == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    
    // initialize new scope bucket
    next_scope_bucket->size        = 0;
    next_scope_bucket->capacity    = scope_bucket->capacity * 2;
    next_scope_bucket->scopes      = malloc(next_scope_bucket->capacity * sizeof(struct Scope));
    // check if we ran out of heap memory
    if (next_scope_bucket->scopes == NULL) {
        /* NO!!!! PANICK!!!! ERROR!!!!! */
        printf("FUCKKKKK!!!!!!");
        return NULL;
    }
    // haven't run out? great, go on
    next_scope_bucket->next_bucket = NULL;
    next_scope_bucket->prev_bucket = scope_bucket;
    
    // add new scope bucket to the end of the "linked list"
    scope_bucket->next_bucket = next_scope_bucket;
    
    return next_scope_bucket;
}


// this should be visible to the programmer
struct Scope* add_scope_to_bucket(struct Scope* scope_to_add, struct MainScopeBucket* scope_bucket) {
    // find last bucket
    ScopeBucket* cur_scope_bucket = scope_bucket->root_bucket;
    
    while (cur_scope_bucket->next_bucket != NULL) cur_scope_bucket = cur_scope_bucket->next_bucket;
    
    if (cur_scope_bucket->size >= cur_scope_bucket->capacity)
        cur_scope_bucket = add_new_scope_bucket(cur_scope_bucket);
    
    cur_scope_bucket->scopes[cur_scope_bucket->size] = *scope_to_add;
    cur_scope_bucket->size += 1;
    
    return &(cur_scope_bucket->scopes[(cur_scope_bucket->size - 1)]);
    
}


void init_main_scope_bucket(struct Program* program) {
    
    MainScopeBucket* scope_bucket_ptr = malloc(sizeof(MainScopeBucket));
    scope_bucket_ptr->root_bucket = malloc(sizeof(struct ScopeBucket));
    if (scope_bucket_ptr->root_bucket == NULL) {
        printf("FUCKKKKK!!!!!!");
        return;
    }
    
    scope_bucket_ptr->root_bucket->size        = 0;
    scope_bucket_ptr->root_bucket->capacity    = 100;
    scope_bucket_ptr->root_bucket->scopes      = malloc(scope_bucket_ptr->root_bucket->capacity * sizeof(struct Scope));
    if (scope_bucket_ptr->root_bucket->scopes == NULL) { return; /* NO!!!! PANICK!!!! ERROR!!!!! */}
    scope_bucket_ptr->root_bucket->next_bucket = NULL;
    scope_bucket_ptr->root_bucket->prev_bucket = NULL;
    
    program->allocators.scope_bucket = scope_bucket_ptr;

}

void free_scope_bucket(MainScopeBucket* scope_bucket) {
    
    // find last scope bucket in the linked list
    ScopeBucket* last_scope_bucket = scope_bucket->root_bucket;
    while (last_scope_bucket->next_bucket != NULL) last_scope_bucket = last_scope_bucket->next_bucket;
    
    // free scope buckets, starting with the last one
    while (last_scope_bucket->prev_bucket != NULL) {
        last_scope_bucket = last_scope_bucket->prev_bucket;
        free(last_scope_bucket->next_bucket->scopes);
        free(last_scope_bucket->next_bucket);
    }
    
    free(last_scope_bucket->scopes);
    free(last_scope_bucket);
    
    free(scope_bucket);
             
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
    
    // initialize new scope bucket
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
    
    // add new scope bucket to the end of the "linked list"
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
    
    // find last scope bucket in the linked list
    DeclarationBucket* last_declaration_bucket = declaration_bucket->root_bucket;
    while (last_declaration_bucket->next_bucket != NULL) last_declaration_bucket = last_declaration_bucket->next_bucket;
    
    // free scope buckets, starting with the last one
    while (last_declaration_bucket->prev_bucket != NULL) {
        last_declaration_bucket = last_declaration_bucket->prev_bucket;
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
    free(void_ptr_arr);
}
