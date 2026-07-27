#include "utils.h"

#include <ctype.h>
#include <unistd.h>
#include <string.h>

char* to_lower(const char* string){
    if(string == NULL){
        return NULL;
    }
    char* new_string = calloc(strlen(string)+1,sizeof(char));
    for(size_t i = 0; i < strlen(string); i++){
        new_string[i] = tolower(string[i]);
    }
    return new_string; 
}

struct carregamento_options {
    char* message;
    atomic_uchar* flag_concluido;
};

//flag é obrigatória
LoadingOpt* new_LoadingOptions(char* message, atomic_uchar* flag_concluido){
    if(flag_concluido == NULL){
        return NULL;
    }
    LoadingOpt* newLO = (LoadingOpt*) malloc(sizeof(LoadingOpt));
    if(newLO == NULL){
        puts("ERRO AO CRIAR LoadingOpt");
        return NULL;
    }
    newLO->flag_concluido = flag_concluido;
    newLO->message= message;
    return newLO;
}

void free_LoadingOpt(LoadingOpt* ptr){
    if(!ptr) return;
    free(ptr->message);
    free(ptr);
}

void* tela_carregamento(void* load_options){
    if(load_options == NULL){
        puts("Erro ao iniciar tela de carregamento: Null load_options");
        return NULL;
    }
    LoadingOpt* options = (LoadingOpt*) load_options;
    char* loading_carrossel[] = {
        "|          |",
        "|>         |",
        "|-->       |",
        "|  -->     |",
        "|     -->  |",
        "|        --|",
    };

    int loading_carrosel_size = sizeof(loading_carrossel) / sizeof(char*);
    int stage = 0;
    while(!atomic_load(options->flag_concluido)){
        printf("\r%s %s",loading_carrossel[stage++ % loading_carrosel_size],options->message);
        fflush(stdout);
        sleep(1);
    }
    puts("\33[2K\r");
    return NULL;
}

//retorna o tamanho do arquivo em bytes
long tamanho_arquivo(FILE* arquivo){
    if(!arquivo){
        return 0;
    }
    long current = ftell(arquivo);
    fseek(arquivo,0,SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo,current,SEEK_SET);
    return tamanho;
}

//retorna 1 caso o arquivo exista
int arquivo_existe(const char* path){
    FILE* f = fopen(path,"r");
    int existe = (f == NULL)? 0 : 1;
    fclose(f);
    return existe;
}

//retorna um ponteiro para o nome do arquivo, ignorando diretorios do path
char* get_file_name(char* path){
    if(!path) return NULL;
    char* nome_isolado = strrchr(path,'/');
    return (nome_isolado)? nome_isolado+1 : path;
}
