#ifndef UTILS_H
#include <stdatomic.h>

char* to_lower(const char* string);

typedef struct carregamento_options LoadingOpt;
LoadingOpt* new_LoadingOptions(char* message, atomic_uchar* flag_concluido);
//recebe um parâmetro do tipo LoadingOpt* obrigatoriamente
void* tela_carregamento(void* load_options);

#define UTILS_H
#endif