#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <string.h>
#include "colors.h"
#include "utils.h"

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
    if(argc < 2 || argc > 4){
        puts(NEGRITO RED "ERRO" RESET);
        printf(RED "Número de argumentos de entrada inválido: %d \n\n"RESET,argc);
        return 0;
    }
    //permite o usuário digitar o comando de forma case insensitive
    char* arg1 = to_lower(argv[1]);

    //strcmp retorna 0 se forem iguais

    if(!strcmp(arg1,"help")){
        if(argc > 2){
            puts(RED "ERRO: Número de argumentos excessivo, esperado somente 'help'" RESET);
            free(arg1);
            return 0;
        }
        free(arg1);
        return 1;
    }
    else if(!strcmp(arg1,ZIP) || !strcmp(arg1,UNZIP)){
        if(argc == 2){
            puts(RED "ERRO: Esperado caminho do arquivo de entrada como terceiro argumento" RESET);
            free(arg1);
            return 0;
        }
        free(arg1);
        return 1;
    }
    else{
        puts(RED "ERRO: COMANDO DESCONHECIDO: " RESET);
        printf("%s\n",arg1);
        free(arg1);
        return 0;
    }
}

int main(int argc, int argv){
    if(!validar_argumentos_entrada(argc,argv)){
    }
}