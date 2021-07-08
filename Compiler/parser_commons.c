
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"


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
