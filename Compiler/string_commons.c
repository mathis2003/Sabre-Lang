
#include "string_commons.h"

int str_equals_literal(struct StringStruct* str, char* str_literal) {
    int i;
    for (i = 0; i < str->str_length && str_literal[i] != '\0'; i++) {
        if (str->str_start[i] != str_literal[i]) return 0;
    }
    
    // have we gotten to the end of BOTH strings? if not -> return false
    if ((i != str->str_length) || (str_literal[i] != '\0')) return 0;
    
    return 1;
}
