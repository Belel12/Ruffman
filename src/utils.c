#include "utils.h"
#include <stdlib.h>
#include <ctype.h>

char* to_lower(const char* string){
    if(string == NULL){
        return NULL;
    }
    char* new_string = calloc(strlen(string),sizeof(char));
    for(int i = 0; i < strlen(string); i++){
        new_string[i] = tolower(string[i]);
    }
    return new_string; 
}