#ifndef UTILS_H
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>

char* to_lower(const char* string);

typedef struct carregamento_options LoadingOpt;

LoadingOpt* new_LoadingOptions(char* message, atomic_uchar* flag_concluido);

void free_LoadingOpt(LoadingOpt* ptr);

//recebe um parâmetro do tipo LoadingOpt* obrigatoriamente
void* tela_carregamento(void* load_options);

long tamanho_arquivo(FILE* arquivo);

int arquivo_existe(const char* path);

char* get_file_name(char* path);

char* change_file_extension(char* file_name, char* new_extension);

char* join_file_to_path(char* file_name, char* path);

char get_os_path_separator();

long bytes_til_EOF(FILE* file);

int is_directory(char* path);

#define UTILS_H
#endif