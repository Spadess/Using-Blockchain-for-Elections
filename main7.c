#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <openssl/sha.h>
#include "partie7.h"


int main(void){
  srand(time(NULL));
  generate_random_data(nb_vote, nb_candidat);

  CellKey* candidates = read_public_keys("candidates.txt");
  CellKey* voters = read_public_keys("keys.txt");
  CellProtected* decl = read_protected("declarations.txt");

  Key* pKey = (Key*)malloc(sizeof(Key));
  Key* sKey = (Key*)malloc(sizeof(Key));
  init_pair_keys(pKey, sKey, 3, 7);
 
  CellTree* tree = NULL;
  CellProtected* tmp = decl;
  int has_root = 0;

  int taille_block = 0;
  while(tmp){
    submit_vote(tmp->data);
    taille_block++;

    tmp = tmp->next;
    if (taille_block == 10 || !tmp){
      taille_block = 0;
      if (has_root == 0){
        has_root++;
        tree = init_tree(pKey, 2);
      }
      else create_block(tree, pKey, 2);
      
      char* tmp_name = rand_filename();
      add_block(2, tmp_name); //valider le bloc et ajoute ds le repertoire blockchain
      free(tmp_name);
    }
  }

  printf("\n\nTREE:\n");
  print_tree(tree, tree->height);

  CellTree* racine = read_tree();


  printf("\nlancement compute_winner_BT\n\n");
  Key* vainqueur = compute_winner_BT(racine, candidates, voters, 15, 300);
  char* str_vq = key_to_str(vainqueur);
  printf("\n\nle vainqueur des elections est: %s\n\n", str_vq);
  free(str_vq);


  //delete_tree(racine);
  delete_tree2(tree);
  
  delete_list_protected(decl);
  delete_list_keys(voters);
  delete_list_keys(candidates);
  free(vainqueur);
  free(pKey); 
  free(sKey);
  
  return 0;
}
