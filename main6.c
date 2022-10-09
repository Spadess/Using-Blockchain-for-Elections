#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <openssl/sha.h>
#include "partie6.h"


int main(void){
  // creation d'un tableau de blocs
  Block** tab_block = (Block**)malloc(sizeof(Block*) * 20);
  for(int i = 0; i < 20; i++) tab_block[i] = NULL;

  generate_random_data(100, 5);
  for(int i = 0; i < 20; i++){
    tab_block[i] = lire_bloc("blocktest.txt");
    // printf("%s\n", block_to_str(tab_block[i]));
  }

  // Creation d'un tableau de noeud
  CellTree** tab_cellt = (CellTree**)malloc(sizeof(CellTree*) * 20);
  for (int i = 0; i < 20; i++) tab_cellt[i] = create_node(tab_block[i]);

  add_child(tab_cellt[0], tab_cellt[1]);
  add_child(tab_cellt[1], tab_cellt[2]);
  add_child(tab_cellt[2], tab_cellt[3]);
  add_child(tab_cellt[3], tab_cellt[4]);
  add_child(tab_cellt[4], tab_cellt[5]);
  add_child(tab_cellt[5], tab_cellt[6]);
  add_child(tab_cellt[6], tab_cellt[7]);
  add_child(tab_cellt[7], tab_cellt[8]);
  add_child(tab_cellt[8], tab_cellt[9]);
  add_child(tab_cellt[9], tab_cellt[10]);
  add_child(tab_cellt[0], tab_cellt[11]);
  add_child(tab_cellt[11], tab_cellt[12]);
  add_child(tab_cellt[12], tab_cellt[13]);
  add_child(tab_cellt[13], tab_cellt[14]);
  add_child(tab_cellt[14], tab_cellt[15]);
  add_child(tab_cellt[15], tab_cellt[16]);
  add_child(tab_cellt[16], tab_cellt[17]);
  add_child(tab_cellt[17], tab_cellt[18]);
  add_child(tab_cellt[18], tab_cellt[19]);

  printf("Voici une representation de l'arbre de blocs:\n\n");
  print_tree(tab_cellt[0], tab_cellt[0]->height);

  CellTree* high_child = highest_child(tab_cellt[0]);
  printf("\nplus grand fils de l'arbre:\n");
  print_node(high_child);
  printf("\ndernier noeud:\n");
  CellTree* last = last_node(tab_cellt[0]);
  print_node(last);
  delete_tree(tab_cellt[0]);
  free(tab_cellt);
  free(tab_block);

  return 0;
}
