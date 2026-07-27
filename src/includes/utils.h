#ifndef UTILS_H
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>

char* to_lower(const char* string);

typedef struct carregamento_options LoadingOpt;
LoadingOpt* new_LoadingOptions(char* message, atomic_uchar* flag_concluido);
//recebe um parâmetro do tipo LoadingOpt* obrigatoriamente
void* tela_carregamento(void* load_options);

size_t tamanho_arquivo(FILE* arquivo);

int arquivo_existe(const char* path);

char* get_file_name(char* path);

#define UTILS_H
#endif