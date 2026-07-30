#ifndef RUFFMAN_H

#define CURRENT_SERIALIZER_VERSION 1

typedef struct ruffman_node Ruff_Node;
typedef struct ruffman_vector Ruff_Vector;
typedef struct bytes_compactados BytesCompactados;

//A função já retorna o vetor heapfyzado, heapzado, heapado, hypado, sla
Ruff_Vector* bytes_to_heap(const unsigned char* string, long size);

void destroy_RuffVector(Ruff_Vector* vetor);

//libera o vetor sem apagar os nos
void free_ruffVector(Ruff_Vector* vetor);

//retorna a raiz da árvore 
Ruff_Node* make_tree_from_heap(Ruff_Vector* heap);

//imprime cada no folha da arvore
void print_tree_inorder(Ruff_Node* raiz);

void inspect_tree(Ruff_Node* raiz);

//imprime todos os nos do heap
void print_heap(Ruff_Vector* heap);

int serializar_arvore(FILE* arquivo,Ruff_Node* raiz);

Ruff_Node* desserializar_arvore(FILE* arquivo);

void tree_to_binary(Ruff_Node* root, BytesCompactados* array[],char* path_atual);

void free_BytesCompactados(BytesCompactados* ptr);

void tree_to_binary(Ruff_Node* root, BytesCompactados* array[],char* path_atual);

int write_compacted_data(
    BytesCompactados* bytes[], 
    unsigned char* file_data, 
    FILE* output_file,
    unsigned long file_data_size
);

int uncompact_data(
    Ruff_Node* root, 
    char* file_data, 
    FILE* output_file,
    long file_data_size, 
    long original_file_size
);

void free_RuffTree(Ruff_Node* root);

#define RUFFMAN_H
#endif