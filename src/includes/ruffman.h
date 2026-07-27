#ifndef RUFFMAN_H

#define CURRENT_SERIALIZER_VERSION 1

typedef struct ruffman_node Ruff_Node;
typedef struct ruffman_vector Ruff_Vector;

//A função já retorna o vetor heapfyzado, heapzado, heapado, hypado, sla
Ruff_Vector* string_to_heap(const char* string);

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

#define RUFFMAN_H
#endif