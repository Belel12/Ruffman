#include "ruffman_tree.h"
#include <stdbool.h>


struct ruffman_tree_node {
    const unsigned long frequencia;
    struct ruffman_tree_node* no_esq;
    struct ruffman_tree_node* no_dir;
    const bool is_folha;
    const unsigned char caractere;
};

typedef struct ruffman_tree_node* RuffNode_t;

