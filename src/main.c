#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include "colors.h"

//coloquei em variáveis para caso venha a 
//mudar de ideia e trocar o nome das operações
//não ter que mudar em vários lugares
#define ZIP "zip"
#define UNZIP "unzip"

const static char* help_message = (
    RED "===HUFFMAN===" RESET
    YELLOW "\n\n* A sintaxe básica do comando para se usar este programa é:\n" RESET
    NEGRITO "ruffman [" ZIP " || " UNZIP "] [path de entrada] [path de saída]\n" RESET
    "*"ZIP": compactar arquivo\n*"UNZIP": descompactar arquivo"
    YELLOW "\n\nCaso o path de saída não seja dado, por padrão será o diretório atual (path de entrada é obrigatório).\n"
    "Você pode acessar essa tela de ajuda sempre que quiser com:\n" RESET
    NEGRITO "ruffman help" RESET
);


int validar_argumentos_entrada(const int argc, const char* argv[]){
    if(argc == 2 && strcmp(argv[1],"help")){
        puts(RED NEGRITO "ERRO" RESET);
        printf(RED "Operação inválida: %s \n\n"RESET,argv[1]);
        return 0;
    }
    if(argc < 3 || argc > 4){
        puts(NEGRITO RED "ERRO" RESET);
        printf(RED "Número de argumentos de entrada inválido: %d \n\n"RESET,argc);
        return 0;
    }
    //strcmp retorna 0 se forem iguais
    if(strcmp(argv[1],ZIP) && strcmp(argv[1],UNZIP)){
        puts(RED NEGRITO "ERRO" RESET);
        printf(RED "Operação inválida: %s \n\n"RESET,argv[1]);
        return 0;
    }
    for(int i = 2;i<argc;i++){
        if( !strcmp(argv[i],ZIP) || !strcmp(argv[i],UNZIP) ){
            puts(RED NEGRITO "ERRO" RESET);
            puts(RED "Path de entrada ou saída não pode ter o mesmo nome de umas das operações"RESET);
            return 0;
        }
    }
    return 1;
}

int main(int argc, int argv){
    if(!validar_argumentos_entrada(argc,argv)){
        puts()
    }
}