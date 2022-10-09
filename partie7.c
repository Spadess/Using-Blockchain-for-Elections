#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <dirent.h>
#include <openssl/sha.h>
#include "partie7.h"

//FAIRE UN MAKE CLEAN AVANT DE REEXECUTER LE MAIN

void submit_vote(Protected* pr){
  if(!pr) return;
  FILE* f = fopen("Pending_votes.txt", "a");
  assert(f);
  char* pr_str = protected_to_str(pr);
  fprintf(f, "%s\n", pr_str);
  free(pr_str);
  fclose(f);
}

void create_block(CellTree* tree, Key* author, int d){ //ajout de noeud dans l'arbre tree
  Block* b = (Block*)malloc(sizeof(Block));
  assert(b);
  
  CellProtected* lcp = read_protected("Pending_votes.txt"); //extraire les votes en attentes
  remove("Pending_votes.txt"); //supprimer les votes en attentes

  CellTree* lnode = last_node(tree);

  b->author = author;
  if(!lnode) b->previous_hash = NULL;
  
  else{ //si un arbre est deja initialise
    b->previous_hash = (unsigned char*)malloc(sizeof(unsigned char) * (SHA256_DIGEST_LENGTH+1));
    for(int i=0;i<SHA256_DIGEST_LENGTH;i++) b->previous_hash[i] = lnode->block->hash[i];
    b->previous_hash[SHA256_DIGEST_LENGTH] = '\0';
  }
  b->hash = NULL;
  b->nonce = 0;
  b->votes = lcp;
  
  compute_proof_of_work(b, d); //determiner le hash du bloc

  ecrire_bloc("Pending_block.txt", b); //ajout de bloc dans un fichier en attente

  CellTree* new_node = create_node(b); //creation du noeud correspond au bloc
  add_child(lnode, new_node);

  delete_list_protected(lcp);
}

void add_block(int d, char* name){
  Block* b = lire_bloc("Pending_block.txt"); //extraire le bloc 
  if(!b) return;

  if(verify_block(b, d)){ //si le bloc est valide
    printf("Pending block valide\n");
    FILE* f = fopen(name, "w"); //name provient de rand_filename
    assert(f);
    //printf("nom du fichier: %s\n", name);
    ecrire_bloc(name, b); //on sauvegarde ce bloc dans le repertoire /Blockchain/
    fclose(f);
  }
  else printf("Pending block non valide\n");

  remove("Pending_block.txt"); //supprimer le bloc en attente
  supprimer_bloc(b);
}

CellTree* init_tree(Key* author, int d){ //initiation d'un arbre
  CellTree* tree = NULL;
  Block* racine = (Block*)malloc(sizeof(Block));
  assert(racine);
  CellProtected* lcp = read_protected("Pending_votes.txt"); //extraire les votes
  racine->author = author;
  racine->hash = NULL;
  racine->previous_hash = NULL;
  racine->votes = lcp;
  racine->nonce = 0;

  remove("Pending_votes.txt"); //supprimer les declarations en attentes
  compute_proof_of_work(racine, d);
  ecrire_bloc("Pending_block.txt", racine); //sauvegarder le bloc dans un fichier en attente
  tree = create_node(racine);
  // print_tree(tree);
  
  delete_list_protected(lcp);
  return tree;
}


CellTree* read_tree(){
  DIR* rep = opendir ("./Blockchain/");
  struct dirent* dir;
  if(!rep) exit(1);

  int nb_blockfile = (nb_vote / nb_voteparbloc); //nb votes/nb de votes par bloc

  CellTree** tab_cellt = (CellTree**)calloc(nb_blockfile, sizeof(CellTree*));
  assert(tab_cellt);

  int index = 0;
  while ((dir = readdir(rep))){ //parcours du repertoire
    if (dir->d_type == DT_REG){ 
      char tmp[256], *path;
      strcpy(tmp, "./Blockchain/");
      path = strcat(tmp, dir->d_name);
      Block* b = lire_bloc(path);
      //char* str_b = block_to_str(b);
      //printf("%s\n\n", str_b);
      CellTree* cellT = create_node(b); // creation d'un noeud correspondant au bloc
  
      tab_cellt[index++] = cellT; //inserer le noeud ds le tableau
    }
  }
  closedir(rep);
  
  for (int i = 0; i < nb_blockfile; i++){ //liens father/child entres les noeuds du tableau tab_cellt en O(n2)
    for (int j = 0; j < nb_blockfile; j++){
      if (strcmp((char*)tab_cellt[i]->block->hash, (char*)tab_cellt[j]->block->previous_hash) == 0){
        add_child((tab_cellt[i]), tab_cellt[j]); 
      }
    }
  }

  CellTree* racine = NULL;
  for (int i = 0; i < nb_blockfile; i++){ //on determine la racine ds le tableau et on le renvoie
    if (!(tab_cellt[i]->father)) racine = tab_cellt[i]; 
  }
  free(tab_cellt);

  return racine; 
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
  CellTree* lnode = last_node(tree); //Extraire la liste des declarations
  CellProtected* lcp = NULL;

  while(lnode){
    lcp = fusion_lcp(lcp, lnode->block->votes);
    lnode = lnode->father;
  }
  check_signature(&lcp); //filtrage des declarations

  return compute_winner(lcp, candidates, voters, sizeC, sizeV); //determination du vainqueur
}

char* rand_filename(){
  char* buffer = (char*)malloc(sizeof(char) * 256);
  assert(buffer);
  char random_str[11];
  for (int i = 0; i < 10; i++) random_str[i] = rand() % 26 + 'a';
  random_str[10] = '\0';
  snprintf(buffer, 256, "%s%s.txt", "./Blockchain/", random_str);

  return buffer;
}