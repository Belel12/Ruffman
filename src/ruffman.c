#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ruffman.h"

struct ruffman_node {
    unsigned long frequencia;
    char byte;
};

struct ruffman_vector {
    struct ruffman_node** vetor_nos;
    int tamanho;
    int qntd_nos;
};

//Cria um novo nó com frequência 1
Ruff_Node* new_RuffNode(char byte, unsigned long frequencia){
    Ruff_Node* newNode = (Ruff_Node*) malloc(sizeof(Ruff_Node));
    if(newNode == NULL){
        puts("ERRO AO CRIAR RUFF_NODE");
        return NULL;
    }
    newNode->byte = byte;
    newNode->frequencia = frequencia;
    return newNode;
}

Ruff_Vector* new_RuffVector(void){
    Ruff_Vector* newRH = (Ruff_Vector*) malloc(sizeof(Ruff_Vector));
    if(newRH == NULL){
        puts("ERRO AO ALOCAR HEAP");
        return NULL;
    }
    newRH->tamanho = 0;
    newRH->qntd_nos = 0;
    newRH->vetor_nos = NULL;
    return newRH;
}

void free_RuffVector(Ruff_Vector* vetor){
    if(vetor == NULL){
        return;
    }
    if(vetor->tamanho = 0){
        free(vetor);
        vetor = NULL;
        return;
    }

    //impa os nos alocados
    for(int i = 0;i < vetor->qntd_nos; i++){
        free(vetor->vetor_nos[i]);
    }
    free(vetor->qntd_nos);
    free(vetor);
    vetor = NULL;
    return;
}

int append_node(Ruff_Vector* vetor, Ruff_Node* no){
    if(vetor == NULL || no == NULL){
        puts("ERRO AO ACRESCENTAR NO AO VETOR: ELEMENTO NULL");
        return 0;
    }
    if(vetor->tamanho == 0){
        vetor->vetor_nos = (Ruff_Node**) malloc(sizeof(Ruff_Node*));
        if(vetor->vetor_nos == NULL){
            puts("ERRO AO ALOCAR ESPACO PARA NOVO NO (VETOR VAZIO)");
            return 0;
        }
        vetor->vetor_nos[vetor->qntd_nos] = no;
        vetor->tamanho++;
        vetor->qntd_nos++;
        return 1;
    }
    else{
        if(vetor->qntd_nos < vetor->tamanho){
            vetor->vetor_nos[vetor->qntd_nos] = no;
            if(vetor->vetor_nos[vetor->qntd_nos] == NULL){
                puts("ERRO AO ALOCAR ESPACO PARA NOVO NO");
                return 0;
            }
            vetor->qntd_nos++;
            return 1;
        }
        else{
            //dobra o tamanho do vetor dinamicamente, pique C++...acho
            unsigned int novo_tamanho = 2 * vetor->tamanho;
            Ruff_Node** tmp = realloc(vetor->vetor_nos,novo_tamanho * sizeof(Ruff_Node*));
            if(tmp == NULL){
                puts("ERRO AO AUMENTAR TAMANHO DO VETOR");
                return 0;
            }
            vetor->vetor_nos = tmp;
            vetor->tamanho = novo_tamanho;
            vetor->vetor_nos[vetor->qntd_nos] = no;
            vetor->qntd_nos++;
            return 1;
        }
    }
}

void heapify_RuffmanVector(Ruff_Vector* vetor){
    if(vetor == NULL){
        puts("NENHUM VETOR PARA ORDERNAR PARA HEAP");
        return;
    }
    if(vetor->tamanho == 0){
        puts("VETOR VAZIO, NADA PARA ORDENAR");
        return;
    }

    qsort(
        vetor->vetor_nos,
        vetor->qntd_nos,
        sizeof(Ruff_Node*),
        comparar_nosRuffman
    );

}

int comparar_nosRuffman(const void* no1, const void* no2){
    Ruff_Node* A = (Ruff_Node*) no1;
    Ruff_Node* B = (Ruff_Node*) no2;
    if(A->frequencia == B->frequencia) return 0;
    return (A->frequencia < B->frequencia)? -1 : 1;
}

Ruff_Vector* string_to_heap(const char* string){
    if(string == NULL){
        puts("ERRO AO GERAR HEAP: STRING NULL");
        return NULL;
    }

    Ruff_Vector* vetor = new_RuffVector();
    if(vetor == NULL){
        puts("ERRO AO CRIAR VETOR DURANTE PARSING DA STRING");
        return NULL;
    }

    //cada posicao representa um byte diferente
    int quantidade_bytes_possiveis = 256; //0-255
    unsigned long frequencias[quantidade_bytes_possiveis];
    //coloquei ulong pq vai que o arquivo e grande pra caramba

    //primeiro conta a frequência de cada byte
    for(unsigned long i = 0;i<strlen(string);i++){
        frequencias[string[i]]++;
    }

    //para cada posição que n tiver frequência 0, cria um novo no e adiciona ao heap
    for(int i = 0; i < quantidade_bytes_possiveis; i++){
        if(frequencias[i] != 0){
            Ruff_Node* no = new_RuffNode(i,frequencias[i]);
            if(no == NULL){
                printf("ERRO AO ADICIONAR NO DO BYTE %c DURANTE PARSING, ENCERRANDO OPERACAO\n",i);
                free_RuffVector(vetor);
                return NULL;
            }
            append_node(vetor,no);
        }
    }

    heapify_RuffmanVector(vetor);
    return vetor;
}
