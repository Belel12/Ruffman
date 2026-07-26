#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "utils.h"
//#include "operations.h"

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

//retorna a string da operação a ser realizada
//TODO: validar se caminho de entrada == caminho de saida
char* validar_operacao_entrada(const int argc, const char* argv[]){
    if(argc < 2 || argc > 4){
        puts(NEGRITO RED "ERRO" RESET);
        printf(RED "Número de argumentos de entrada inválido: %d \n\n"RESET,argc);
        return NULL;
    }
    //permite o usuário digitar o comando de forma case insensitive
    char* arg1 = to_lower(argv[1]);

    //strcmp retorna 0 se forem iguais

    if(!strcmp(arg1,"help")){
        if(argc > 2){
            puts(RED "ERRO: Número de argumentos excessivo, esperado somente 'help'" RESET);
            free(arg1);
            return NULL;
        }
        return arg1;
    }
    else if(!strcmp(arg1,ZIP) || !strcmp(arg1,UNZIP)){
        if(argc == 2){
            puts(RED "ERRO: Esperado caminho do arquivo de entrada como terceiro argumento" RESET);
            free(arg1);
            return NULL;
        }
        return arg1;
    }
    else{
        puts(RED "ERRO: COMANDO DESCONHECIDO: " RESET);
        printf("%s\n",arg1);
        free(arg1);
        return NULL;
    }
}

int main(int argc, char* argv){
    char* operacao = validar_operacao_entrada(argc,argv);
    if (operacao == NULL){
        printf("\nUse 'ruffman help' para verificar o uso do programa\n");
        return 1;
    }
    //caso seja apenas comando de help
    if (!strcmp(operacao,"help")){
        free(operacao);
        puts(help_message);
        return 0;
    }
    FILE* arquivo_entrada = fopen(argv[2],"rb");
    if(arquivo_entrada == NULL){
        free(operacao);
        puts(
            RED "ERRO: ARQUIVO NÃO ENCONTRADO, "
            "VERIFIQUE SE O ARQUIVO EXISTE OU SE O CAMINHO ESTÁ CORRETO"RESET);
        return 1;
    }
    //caso o caminho de saída não seja especificado
    //o caminho padrão será o diretório atual
    char* output_path = (argc == 3) ? "./" : argv[3];
    int sucess = 0;
 
    if(!strcmp(operacao,ZIP)){
        sucess = encode(arquivo_entrada,output_path);
    }
    else{
        sucess = decode(arquivo_entrada,output_path);
    }
    fclose(arquivo_entrada);

    if(sucess){
        FILE* validacao_saida = fopen(output_path,"r");
        if(validacao_saida == NULL){
            puts(
                RED "ERRO: ARQUIVO DE SAÍDA NÃO FOI CRIADO, "
                "VERIFIQUE SE O DIRETÓRIO EXISTE E QUE VOCÊ TENHA PERMISSÃO DE ESCRITA" RESET
            );
        }
        fclose(validacao_saida);
    }
    else{
        puts(RED "OCORREU UM ERRO DURANTE O PROCESSAMENTO DO ARQUIVO"RESET);
    }

    return sucess;

}