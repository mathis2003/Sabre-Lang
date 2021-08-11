#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Parser/parser.h"
#include "code_gen.h"

#include "../Commons/string_commons.h"

signed int is_primitive_type(struct StringStruct* str) {
    if (str_equals_literal(str, "Unit")) return 1;
    else if (str_equals_literal(str, "u8")) return 1;
    else if (str_equals_literal(str, "u16")) return 1;
    else if (str_equals_literal(str, "u32")) return 1;
    
    return 0;
}

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

