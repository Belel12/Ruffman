#ifndef RUFFMAN_H

typedef struct ruffman_node Ruff_Node;
typedef struct ruffman_vector Ruff_Vector;

//A função já retorna o vetor heapfyzado, heapzado, heapado, hypado, sla
Ruff_Vector* string_to_heap(const char* string);

void free_RuffVector(Ruff_Vector* vetor);

#define RUFFMAN_H
#endif