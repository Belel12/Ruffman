#include "utils.h"
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

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

struct carregamento_options {
    char* message;
    unsigned char flag_concluido;
};

//flag é obrigatória
LoadingOpt* new_LoadingOptions(char* message, unsigned char* flag_concluido){
    if(flag_concluido == NULL){
        return NULL;
    }
    LoadingOpt* newLO = (LoadingOpt*) malloc(sizeof(LoadingOpt));
    newLO->flag_concluido = flag_concluido;
    newLO->message= message;
    return newLO;
}
void* tela_carregamento(void* load_options){
    if(load_options == NULL){
        puts("Erro ao iniciar tela de carregamento: Null load_options");
        return;
    }
    LoadingOpt* options = (LoadingOpt*) load_options;
    char* loading_carrossel[] = {
        "|          |",
        "|>         |",
        "|->        |",
        "|  -->     |",
        "|     -->  |",
        "|        --|",
    };

    int loading_carrosel_size = sizeof(loading_carrossel) / sizeof(char*);
    int stage = 0;
    while(!options->flag_concluido){
        printf("%s %s",loading_carrossel[stage++ % loading_carrosel_size],options->message);
        sleep(1);
    }
    return ;
}
