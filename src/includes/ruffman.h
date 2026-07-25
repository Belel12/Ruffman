#ifndef RUFFMAN_H

typedef struct ruffman_node Ruff_Node;
typedef struct ruffman_vector Ruff_Vector;

//A função já retorna o vetor heapfyzado, heapzado, heapado, hypado, sla
Ruff_Vector* string_to_heap(const char* string);

void destroy_RuffVector(Ruff_Vector* vetor);

//libera o vetor sem apagar os nos
void free_ruffVector(Ruff_Vector* vetor);

//retorna a raiz da árvore 
Ruff_Node* make_tree_from_heap(Ruff_Vector* heap);

size_t sizeofRuffNode();

void print_tree_inorder(Ruff_Node* raiz);

void print_heap(Ruff_Vector* heap);

#define RUFFMAN_H
#endif