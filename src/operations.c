#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>
#include "ruffman.h"
#include "operations.h"
#include "utils.h"

/*Essas funções encapsulam as funções de compactação/descompactação
com a tela de carregamento*/

int encode(char* inputpath, char* outputpath){
    atomic_uchar flag_concluido;
    atomic_store(&flag_concluido,0);
    pthread_t thread_carregamento;
    LoadingOpt* lopt = new_LoadingOptions("COMPACTANDO ARQUIVO",&flag_concluido);
    pthread_create(&thread_carregamento,NULL,tela_carregamento,(void*) lopt);
    int sucess = __encode(inputpath,outputpath);
    atomic_store(&flag_concluido,1);
    pthread_join(thread_carregamento,NULL);
    return sucess;
}

int decode(char* inputpath, char* outputpath){
    atomic_uchar flag_concluido;
    atomic_store(&flag_concluido,0);
    pthread_t thread_carregamento;
    LoadingOpt* lopt = new_LoadingOptions("DESCOMPACTANDO ARQUIVO ARQUIVO",&flag_concluido);
    pthread_create(&thread_carregamento,NULL,tela_carregamento,(void*) lopt);
    int sucess = __decode(inputpath,outputpath);
    atomic_store(&flag_concluido,1);
    pthread_join(thread_carregamento,NULL);
    return sucess;
}

//TODO: alterar prints de erro nas funcoes para logs para n atrapalhar 
//a tela de carregamento


void generate_meta_data(FILE* file, char* original_file_name){
    if(!file) return NULL;
    const char* meta_string = (
        "#Aviso: Não altere esse arquivo manualmente\n"
        "original_file_name:%s\n"
        "current_serializer_version:%d\n"
    );

    fprintf(file,meta_string,original_file_name,CURRENT_SERIALIZER_VERSION);
}

int __encode(char* inputpath, char* outputpath){
    FILE* input_file = fopen(inputpath,"rb");
    char* file_name = get_file_name(inputpath);
    char* nome_arquivo_compactado = change_file_extension(file_name,"ruff");
    char* full_output_path = join_file_to_path(nome_arquivo_compactado,outputpath);
    FILE* output_file = fopen(full_output_path,"wb");
    free(nome_arquivo_compactado);
    free(full_output_path);
    if(!input_file || !output_file){
        puts("ERRO DURANTE COMPACTAÇÃO: NÃO FOI POSSÍVEL ABRIR OS ARQUIVOS NECESSÁRIOS");
        return 0;
    }

    long tamanho_arq = tamanho_arquivo(input_file);
    if(tamanho_arq < 0){
        puts("ERRO DURANTE COMPACTAÇÃO: ERRO AO CALCULAR TAMANHO DO ARQUIVO");
        return 0;
    }
    if(tamanho_arq == 0){
        puts("ARQUIVO VAZIO, NADA PARA COMPACTAR");
        return 1;
    }

    char bytes_arquivo[tamanho_arq];
    fread(bytes_arquivo,sizeof(char),tamanho_arq,input_file);

    Ruff_Vector* heap = string_to_heap(bytes_arquivo); if(!heap) return 0;
    Ruff_Node* arvore = make_tree_from_heap(heap); if(!arvore) return 0;
    BytesCompactados** bytes_compactados = (BytesCompactados**) calloc(sizeof(BytesCompactados*),256);
    tree_to_binary(arvore,bytes_compactados,NULL);
    generate_meta_data(output_file,file_name);
    fputs("tree:\n",output_file);
    serializar_arvore(output_file,arvore);
    fputs("\ndata:\n",output_file);
    if(!write_compacted_data(bytes_compactados,bytes_arquivo,output_file))
        return 0;
    fclose(input_file);
    fclose(output_file);
    destroy_RuffVector(heap);
    arvore = NULL;
    for(int i = 0; i < (sizeof(bytes_compactados) / sizeof(bytes_compactados[0])); i++){
        free_BytesCompactados(bytes_compactados[i]);
    }
    free(bytes_compactados);
    return 1;
    
}

int __decode(char* inputpath, char* outputpath){

}