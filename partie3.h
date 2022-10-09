#ifndef PARTIE3_H
#define PARTIE3_H
#include "partie2.h"

typedef struct CellKey {
  Key* data;
  struct CellKey* next;
}CellKey;

typedef struct cellProtected {
  Protected* data ;
  struct cellProtected* next ;
}CellProtected;

CellKey* create_cell_key(Key* key);
void add_cell_key(CellKey** LCK, Key* key);
CellKey* read_public_keys(char* filename);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* LCK);
CellProtected* create_cell_protected(Protected* pr);
void add_cell_protected(CellProtected** LCP, Protected* pr);
CellProtected* read_protected(char* filename);
void print_list_protected(CellProtected* LCP);
void delete_cell_protected(CellProtected* c);
void delete_list_protected(CellProtected* LCP);
int nb_elem_list_protected(CellProtected* LCP);
void check_signature(CellProtected** LCP);


#endif