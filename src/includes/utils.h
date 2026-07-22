#ifndef UTILS_H

char* to_lower(const char* string);

typedef struct carregamento_options LoadingOpt;
LoadingOpt* new_LoadingOptions(char* message, unsigned char* flag_concluido);
//recebe um parâmetro do tipo LoadingOpt* obrigatoriamente
void* tela_carregamento(void* load_options);

#define UTILS_H
#endif