#ifndef PARTIE4_H
#define PARTIE4_H
#include "partie3.h"

typedef struct hashcell {
  Key* key;
  int val;
}HashCell ;

typedef struct hashtable {
  HashCell** tab;
  int size ;
}HashTable ;

void check_signature(CellProtected** LCP);
HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
void print_hashtable(HashTable* t);
void affiche_resultats(HashTable* t);
int nb_elem_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates,CellKey* voters, int sizeC, int sizeV);


#endif