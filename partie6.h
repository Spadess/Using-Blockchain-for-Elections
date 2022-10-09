#ifndef PARTIE6_H
#define PARTIE6_H
#include "partie5.h"

typedef struct block_tree_cell {
    Block* block ;
    struct block_tree_cell* father ;
    struct block_tree_cell* firstChild ;
    struct block_tree_cell* nextBro ;
    int height;
}CellTree;

CellTree* create_node(Block* b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_node(CellTree* node);
void print_tree(CellTree* cellT, int max_h);
void delete_node(CellTree* node);
void delete_node2(CellTree* node);
void delete_tree(CellTree* cellT);
void delete_tree2(CellTree* cellT);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected* fusion_lcp(CellProtected* l1, CellProtected* l2);
CellProtected* fusion_max_lcp(CellTree *tree);

#endif