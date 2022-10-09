#ifndef PARTIE7_H
#define PARTIE7_H
#include "partie6.h"
#define nb_vote 100
#define nb_candidat 5
#define nb_voteparbloc 10

void submit_vote(Protected* pr);
void create_block(CellTree* tree, Key* author, int d);
void add_block(int d, char* name);
CellTree* read_tree();
CellTree* init_tree(Key* author, int d);
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);
char* rand_filename();


#endif