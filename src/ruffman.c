#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ruffman.h"

struct ruffman_node {
    unsigned long frequencia;
    char byte;
    
    struct ruffman_node* esq;
    struct ruffman_node* dir;

    bool is_folha;
};

struct ruffman_vector {
    struct ruffman_node** vetor_nos;
    int tamanho;
    int qntd_nos;
};

//Cria um novo nó com frequência 1
Ruff_Node* new_RuffNode_folha(char byte, unsigned long frequencia){
    Ruff_Node* newNode = (Ruff_Node*) malloc(sizeof(Ruff_Node));
    if(newNode == NULL){
        puts("ERRO AO CRIAR RUFF_NODE FOLHA");
        return NULL;
    }
    newNode->byte = byte;
    newNode->frequencia = frequencia;
    newNode->is_folha = true;
    newNode->esq = NULL;
    newNode->dir = NULL;
    return newNode;
}

Ruff_Node* new_RuffNode_interno(Ruff_Node* esq, Ruff_Node* dir,unsigned long frequencia){
    Ruff_Node* newRN = (Ruff_Node*) malloc(sizeof(Ruff_Node));
    if(newRN == NULL){
        puts("ERRO AO CRIAR RUFF_NODE INTERNO");
        return NULL;
    }

    newRN->esq = esq;
    newRN->dir = dir;
    newRN->frequencia = frequencia;
    newRN->is_folha = false;
    return newRN;
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

//apaga os nos apontados pelo vetor junto
void destroy_RuffVector(Ruff_Vector* vetor){
    if(vetor == NULL){
        puts("VETOR JA E NULL");
        return;
    }

    //limpa os nos alocados
    for(int i = 0;i < vetor->qntd_nos; i++){
        free(vetor->vetor_nos[i]);
    }
    free(vetor->vetor_nos);
    free(vetor);
    return;
}

//libera o vetor sem apagar os nos
void free_ruffVector(Ruff_Vector* vetor){
    if(vetor == NULL){
        puts("VETOR JA E NULL");
        return;
    }

    free(vetor->vetor_nos);
    free(vetor);

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

int comparar_nosRuffman(const void* no1, const void* no2){
    Ruff_Node* A = *(Ruff_Node**) no1;
    Ruff_Node* B = *(Ruff_Node**) no2;
    if(A->frequencia == B->frequencia) return 0;
    return (A->frequencia < B->frequencia)? -1 : 1;
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

    //inicializa o vetor
    for(int i = 0; i < quantidade_bytes_possiveis; i++){
        frequencias[i] = 0;
    }

    //conta a frequência de cada byte
    for(unsigned long i = 0;i<strlen(string);i++){
        frequencias[(int)string[i]]++;
    }

    //para cada posição que n tiver frequência 0, cria um novo no e adiciona ao heap
    for(int i = 0; i < quantidade_bytes_possiveis; i++){
        if(frequencias[i] != 0){
            Ruff_Node* no = new_RuffNode_folha(i,frequencias[i]);
            if(no == NULL){
                printf("ERRO AO ADICIONAR NO DO BYTE %c DURANTE PARSING, ENCERRANDO OPERACAO\n",i);
                destroy_RuffVector(vetor);
                return NULL;
            }
            append_node(vetor,no);
        }
    }

    heapify_RuffmanVector(vetor);
    return vetor;
}

//copia os elementos de um heap para outro
void heap_concat(Ruff_Vector* heap_origem, Ruff_Vector* heap_destino){
    if(heap_origem == NULL || heap_destino == NULL){
        puts("ERRO heap_concat: HEAP NULL");
        return;
    }

    for(int i = 0; i < heap_origem->qntd_nos; i++){
        append_node(heap_destino,heap_origem->vetor_nos[i]);
    }
}

//não apaga o nó em si, apenas o remove do vetor
//e move os nos da frente uma posicao para tras
void remove_node_from_heap(Ruff_Vector* heap,int position){
    if(heap == NULL){
        puts("ERRO AO REMOVER NO: HEAP NULL");
        return;
    }

    if(position < 0 || position > heap->qntd_nos-1){
        puts("ERRO AO REMOVER NO: POSICAO INVALIDA");
        return;
    }

    heap->vetor_nos[position] = NULL;

    for(position += 1;position < heap->qntd_nos;position++){
        heap->vetor_nos[position-1] = heap->vetor_nos[position];
    }
    heap->vetor_nos[--position] = NULL;
    heap->qntd_nos--;

}

//retorna a raiz da árvore 
Ruff_Node* make_tree_from_heap(Ruff_Vector* heap){
    if(heap == NULL || heap->qntd_nos == 0){
        puts("ERRO AO CRIAR ÁRVORE: HEAP NULL OU VAZIA");
        return NULL;
    }
    if(heap->qntd_nos == 1){
        Ruff_Node* no_unico = heap->vetor_nos[0];
        Ruff_Node* raiz = new_RuffNode_interno(no_unico,NULL,no_unico->frequencia);
        return raiz;
    }
    //crio um novo heap para não alterar o original
    //visto que esse terá nós da própria árvore
    //e será destruído depois
    Ruff_Vector* heap_temp = new_RuffVector();
    heap_concat(heap,heap_temp);

    while(heap_temp->qntd_nos > 1){
        Ruff_Node* no1 = heap_temp->vetor_nos[0];
        Ruff_Node* no2 = heap_temp->vetor_nos[1];
        Ruff_Node* ligacao = new_RuffNode_interno(no1,no2,no1->frequencia + no2->frequencia);
        
        heap_temp->vetor_nos[0] = ligacao;
        remove_node_from_heap(heap_temp,1);

        heapify_RuffmanVector(heap_temp);
    }

    Ruff_Node* raiz = heap_temp->vetor_nos[0];
    free_ruffVector(heap_temp);
    return raiz;
    

}
