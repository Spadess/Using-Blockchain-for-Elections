#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <openssl/sha.h>
#include "partie6.h"

CellTree* create_node(Block* b){
    CellTree* cellT = (CellTree*)malloc(sizeof(CellTree));
    assert(cellT);
    cellT->block = b;
    cellT->height = 0;
    cellT->father = NULL;
    cellT->firstChild = NULL;
    cellT->nextBro = NULL;
    return cellT; 
}

int update_height(CellTree* father, CellTree* child){
    if(!father || !child) return 0;
    if(father->height < (child->height +1)){
        father->height = child->height + 1;
        return 1;
    }
    return 0;
}

void add_child(CellTree* father, CellTree* child){
    if(!father || !child) return;

    child->father = father; //le fils a un pere 

    child->nextBro = father->firstChild; //le pere a un fils 
    father->firstChild = child;

    CellTree* father_tmp = father;
    CellTree* child_tmp = child;

    while(father_tmp){
        update_height(father_tmp,child_tmp);
        father_tmp = father_tmp->father;
        child_tmp = father_tmp;
    }
    update_height(father, child);
}

void print_node(CellTree* node){
    if(!node) return;
    printf("hauteur = %d, valeur hachee = %s\n", node->height, node->block->hash);
}

void print_tree(CellTree* cellT, int max_h){
    if(!cellT) return;
    for(int i = max_h; i > cellT->height; i--) printf(" ");
    print_node(cellT);
    print_tree(cellT->firstChild, max_h);
    print_tree(cellT->nextBro, max_h);
}


void delete_node2(CellTree* node){
    if(!node) return;
    supprimer_bloc2(node->block);
    free(node);
}


void delete_tree2(CellTree* cellT){
	if(!cellT) return;
    delete_tree2(cellT->nextBro);
    delete_tree2(cellT->firstChild);
    delete_node2(cellT);
}

void delete_node(CellTree* node){
    if(!node) return;
    supprimer_bloc(node->block);
    free(node);
}

void delete_tree(CellTree* cellT){
	if(!cellT) return;
    delete_tree(cellT->nextBro);
    delete_tree(cellT->firstChild);
    delete_node(cellT);
}


CellTree* highest_child(CellTree* cell){ //trouver le fils avec la plus grande hauteur
    if(!cell) return NULL;
    CellTree* maxchild = cell->firstChild;
    CellTree* bro_tmp = maxchild->nextBro;
    int max_h= maxchild->height;

    while(bro_tmp){
        if(max_h < bro_tmp->height){ //si la hauteur du noeud courant < que celui de son frere
            max_h = bro_tmp->height;
            maxchild = bro_tmp;
        }
        bro_tmp = bro_tmp->nextBro;
    }
    return maxchild;
}

CellTree* last_node(CellTree* tree){ //retourne le dernier bloc de cette plus longue chaine
    if(!tree) return NULL;

    if (!(tree->firstChild)) return tree; //si n'a plus de fils, c le dernier mallon de la chaine

    return last_node(highest_child(tree));
}

//une liste doublement chainee nous permettrait d'avoir acces directement au dernier element, donc une insertion a la queue
//est immediate ->Complexite O(1)
CellProtected* fusion_lcp(CellProtected* l1, CellProtected* l2){
  if(!l1 && !l2) return NULL;
  if (!l1) return l2;
  if (!l2) return l1;

  CellProtected* tmp = l1;
  while (tmp->next) tmp = tmp->next;
  tmp->next = l2;

  return l1;
}

CellProtected* fusion_max_lcp(CellTree *tree){ //prend la chaine la plus longue et fait une fusion des decl le long de cette chaine
  if(!tree) return NULL;

  CellProtected* res = (CellProtected*)malloc(sizeof(CellProtected));
  assert(res);
  res = tree->block->votes;
  CellTree* maxchild = highest_child(tree); 
  CellProtected* add = fusion_max_lcp(maxchild); //appels recursifs pour descendre le long de la chaine la plus longue
  fusion_lcp(res, add);

  return res;
}