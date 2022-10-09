#ifndef PARTIE5_H
#define PARTIE5_H
#include "partie4.h"

typedef struct block {
  Key* author;
  CellProtected* votes;
  unsigned char* hash;
  unsigned char* previous_hash;
  int nonce; //proof of work
}Block;

void ecrire_bloc(char* filename, Block* block);
Block* lire_bloc(char* filename);
char* hash_to_str(char* chaine);
void compute_proof_of_work(Block* B, int d);
void supprimer_bloc(Block* block);
void supprimer_bloc2(Block* b);
char* block_to_str(Block* block);
int verify_block(Block* B, int d);

#endif