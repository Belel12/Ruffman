#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "ruffman.h"



struct ruffman_node {
    unsigned long frequencia;
    struct ruffman_node* esq;
    struct ruffman_node* dir;
    unsigned char byte;
    bool is_folha;
};

struct ruffman_vector {
    struct ruffman_node** vetor_nos;
    int tamanho;
    int qntd_nos;
};

struct bytes_compactados{
    unsigned int n_bits;
    char* bytes;
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

Ruff_Node* new_RuffNode_interno(
    Ruff_Node* esq, 
    Ruff_Node* dir,
    unsigned long frequencia
){
    Ruff_Node* newRN = (Ruff_Node*) malloc(sizeof(Ruff_Node));
    if(newRN == NULL){
        puts("ERRO AO CRIAR RUFF_NODE INTERNO");
        return NULL;
    }

    newRN->byte = '\0';
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
            Ruff_Node** tmp = realloc(
                vetor->vetor_nos,novo_tamanho * sizeof(Ruff_Node*)
            );
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

    //para cada posição que n tiver frequência 0, 
    //cria um novo no e adiciona ao heap
    for(int i = 0; i < quantidade_bytes_possiveis; i++){
        if(frequencias[i] != 0){
            Ruff_Node* no = new_RuffNode_folha(i,frequencias[i]);
            if(no == NULL){
                printf(
                    "ERRO AO ADICIONAR NO DO BYTE %c DURANTE PARSING, "
                    "ENCERRANDO OPERACAO\n",i
                );
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
        Ruff_Node* ligacao = new_RuffNode_interno(
            no1,no2,no1->frequencia + no2->frequencia
        );
        
        heap_temp->vetor_nos[0] = ligacao;
        remove_node_from_heap(heap_temp,1);

        heapify_RuffmanVector(heap_temp);
    }

    Ruff_Node* raiz = heap_temp->vetor_nos[0];
    free_ruffVector(heap_temp);
    return raiz;
    

}

//imprime uma versão detalhada de todos os nós da árvore no percurso em-ordem
void inspect_tree(Ruff_Node* raiz){

    if(raiz == NULL){
        return;
    }
    inspect_tree(raiz->esq);
    
    printf("freq=%lu byte=%d folha=%d esq=%p dir=%p\n",
       raiz->frequencia,
       raiz->byte,
       raiz->is_folha,
       (void*)raiz->esq,
       (void*)raiz->dir);
    

    inspect_tree(raiz->dir);
}

void print_tree_inorder(Ruff_Node* raiz){
    if(raiz == NULL){
        return;
    }

    print_tree_inorder(raiz->esq);
    if(raiz->is_folha){
        printf("%c | freq: %lu\n",raiz->byte,raiz->frequencia);
    }
    print_tree_inorder(raiz->dir);

}

void print_heap(Ruff_Vector* heap){
    if(heap == NULL || heap->qntd_nos == 0){
        puts("HEAP VAZIO, NADA PARA IMPRIMIR");
        return;
    }

    for(int i = 0; i < heap->qntd_nos; i++){
        Ruff_Node* atual = heap->vetor_nos[i];
        printf("pos %d:%c | freq: %lu\n",i,atual->byte,atual->frequencia);
    }
}

//serializa a arvore em pre-ordem utilizando recursão
//cada nó serializado é uma string estilo csv contendo:
//"folha?;byte;frequencia"
int serializar_arvore(FILE* arquivo,Ruff_Node* raiz){
    if(arquivo == NULL || raiz == NULL){
        return 0;
    }

    fwrite(raiz,sizeof(Ruff_Node),1,arquivo);

    serializar_arvore(arquivo,raiz->esq);

    serializar_arvore(arquivo,raiz->dir);

    return 1;

}

//desserializa a arvore
//ja pressupoem que o ponteiro do arquivo esta na posicao da linha da arvore
Ruff_Node* desserializar_arvore(FILE* arquivo){
    if(arquivo == NULL){
        puts("ERRO AO DESSERIALIZAR ARVORE: ARQUIVO NULL");
        return NULL;
    }

    Ruff_Node* no = (Ruff_Node*) malloc(sizeof(Ruff_Node));

    if(!fread(no,sizeof(Ruff_Node),1,arquivo)){
        puts("ERRO AO LER ARQUIVO DURANTE DESSERIALIZAÇÃO");
        return NULL;
    }

    if(no->is_folha){
        return no;
    }
    else{
        //so tenta ler se o no possuia um filho antes
        if(no->esq != NULL) no->esq = desserializar_arvore(arquivo);
        if(no->dir != NULL) no->dir = desserializar_arvore(arquivo);
        return no;
    }

}

BytesCompactados* new_BytesCompactados(){
    BytesCompactados* newBC = (BytesCompactados*) malloc(sizeof(BytesCompactados));
    if(!newBC) return NULL;

    newBC->n_bits = 0;
    newBC->bytes = NULL;
}

void free_BytesCompactados(BytesCompactados* ptr){
    if(!ptr) return;
    free(ptr->bytes);
    free(ptr);
}

/*pega o caminho percorrido da raíz até o nó e transforma em 
bytes*/
BytesCompactados* tree_path_to_binary(char* tree_path){
    if(!tree_path) return NULL;

    BytesCompactados* bc = new_BytesCompactados();
    size_t tamanho_string = strlen(tree_path);
    int tamanho_vetor = (int) ceil((double)(tamanho_string/(sizeof(char)*8)));

    char* bytes = (char*) calloc(tamanho_vetor,sizeof(char));
    if(!bytes) return NULL;

    bc->bytes = bytes;

    char* temp = bytes-1;

    for(int i = 0; i < tamanho_string; i++,bc->n_bits++){
        if(i % 8 == 0) temp++;

        *temp <<= 1;

        if(tree_path[i] == '0')
            *temp |= 0b00000000;
        else if(tree_path[i] == '1')
            *temp |= 0b00000001;
        else{
            free(bytes);
            return NULL;
        }
    }

    return bc;
    
}

/*organiza um vetor do tipo BytesCompactados [256] onde 
cada posicao corresponde a um byte possivel, e o ponteiro
dessa posicao aponta para o caminho dele na árvore já 
compactado em binario. O terceiro argumento serve
apenas para a chamada recursiva interna, passe NULL*/
void tree_to_binary(Ruff_Node* root, BytesCompactados* array[],char* path_atual){
    if(
        !root 
        || !array 
        || sizeof(array) / sizeof(BytesCompactados*) < 256
    ){
        free(path_atual);
        return;
    }
    if(root->is_folha){
        if(!path_atual) return;

        array[root->byte] = tree_path_to_binary(path_atual);
        free(path_atual);
        return;
    }
    else{
        //contabiliza o espaço pro \0 e pro novo caractere
        unsigned int novo_tamanho = (path_atual) ? strlen(path_atual) + 2 : 2;
        if(root->esq != NULL){
            char* path_esquerda = (char*) calloc(novo_tamanho,1);
            if(path_atual)
                strcpy(path_esquerda,path_atual);
            strcpy(strrchr(path_esquerda,'\0'),"0");
            tree_to_binary(root->esq,array,path_esquerda);
        }
        if(root->dir != NULL){
            char* path_direita = (char*) calloc(novo_tamanho,1);
            if(path_atual)
                strcpy(path_direita,path_atual);
            strcpy(strrchr(path_direita,'\0'),"1");
            tree_to_binary(root->esq,array,path_direita);
        }
        //libera o caminho intermediario
        free(path_atual);
        return;
    }
    
}

/*para cada caractere recebido em file_data,
essa função escreve os bits que representam
o caminho da raiz da árvore até o byte correspondente*/
int write_compacted_data(
    BytesCompactados* bytes[], 
    char* file_data, 
    FILE* output_file
){
    if(!bytes || !file_data || !output_file) return 0;

    int bits_written_buffer = 0;
    char buffer = '\0';
    unsigned char bits_filter[] = {
        0b10000000,
        0b01000000,
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001
    };

    for(unsigned long i = 0; i < strlen(file_data);i++){
        

        BytesCompactados* byte = bytes[(unsigned char)file_data[i]];
        if(!byte) return 0;

        char* bits_ptr = byte->bytes;
        int bits_written_ptr = 0;
        for(int j = 0; j < byte->n_bits; j++){
            buffer <<= 1;
            buffer |= (*bits_ptr | bits_filter[j % 8]);
            bits_written_buffer++;
            bits_written_ptr++;
            if(bits_written_buffer == 8){
                fwrite(&buffer,sizeof(char),1,output_file);
                bits_written_buffer = 0;
                buffer = '\0';
            }
            if(bits_written_ptr == 8){
                bits_ptr++;
                bits_written_ptr = 0;
            }
        }
    }
    return 1;
}