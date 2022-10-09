#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <openssl/sha.h>
#include "partie5.h"

void supprimer_bloc(Block* b){
  if(!b) return;
  free(b->author);
  free(b->hash);
  free(b->previous_hash);
  while (b->votes){
    CellProtected* tmp = b->votes;
    b->votes = b->votes->next;
    delete_cell_protected(tmp);
  }
  free(b);
}

void supprimer_bloc2(Block* b){ //version modifiee de la consigne
  if(!b) return;
  free(b->hash);
  free(b->previous_hash);
  free(b);
}


void ecrire_bloc(char* filename, Block* block){
  if(!block) return;
  FILE* f = fopen(filename, "w+");
  assert(f);
  
  fprintf(f, "(%lx,%lx)\n", block->author->val, block->author->n); //cles auteur
  fprintf(f, "%s\n%s\n%d\n", (char*)block->hash, (char*)block->previous_hash, block->nonce);//les 2 hash et le nonce
  
  CellProtected* temp = block->votes;
  while(temp){ //ecriture des decl
    char* str_pr = protected_to_str(temp->data);
    fprintf(f, "%s\n", str_pr);
    free(str_pr);
    temp = temp->next;
  }
  fclose(f);
}

Block* lire_bloc(char* filename){
  FILE* f = fopen(filename, "r");
  assert(f);
  Block* block = (Block*)malloc(sizeof(Block));
  assert(block);
  char buffer[512];
  
  fgets(buffer, 512, f); //lecture auteur
  block->author = str_to_key(buffer);

  fgets(buffer, 512, f); //lecture hash
  unsigned char* hash = NULL;
  buffer[strlen(buffer)-1] = '\0';

  hash = (unsigned char *)malloc(sizeof(unsigned char) * (SHA256_DIGEST_LENGTH+1));
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) hash[i] = buffer[i];
  hash[SHA256_DIGEST_LENGTH] = '\0';
  block->hash = hash;
  
  fgets(buffer, 512, f); //lecture previous hash
  buffer[strlen(buffer)-1] = '\0';
  unsigned char *previous_hash = NULL;
  previous_hash = (unsigned char *)malloc(sizeof(unsigned char) * (SHA256_DIGEST_LENGTH+1));
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) previous_hash[i] = buffer[i];
  previous_hash[SHA256_DIGEST_LENGTH] = '\0';
  block->previous_hash = previous_hash;

  int nonce;
  fgets(buffer,256,f); //lecture nonce
  if(sscanf(buffer, "%d", &nonce) !=1) return NULL;
  block->nonce = nonce;
  
  Protected* pr = NULL;
  block->votes = NULL;
  while(fgets(buffer,256,f)){ //lecture des declarations
    pr = str_to_protected(buffer);
    if(!pr) break;
    add_cell_protected(&(block->votes), pr);
  }
  
  fclose(f);
  return block;
}

char* block_to_str(Block *block){
  char buffer[65536]; //taille maximale d'un tableau de char pour ne pas avoir de problemes de stockage
  char *key_str = key_to_str(block->author);
  sprintf(buffer, "%s\n%s\n", key_str, block->previous_hash);
  free(key_str);
  
  CellProtected* tmp = block->votes;
  while(tmp){
    char *pr_str = protected_to_str(tmp->data);
    sprintf(buffer + strlen(buffer), "%s\n", pr_str);
    free(pr_str);
    tmp = tmp->next;
  }
  char str_nonce[256];
  sprintf(str_nonce, "%d", block->nonce);
  strcat(buffer, str_nonce);
  return strdup(buffer); 
}

char* hash_to_str(char* str){
  char* res = (char *)malloc(sizeof(char) * 256);
  unsigned char* hash = SHA256((unsigned char*)str, strlen(str), 0);
  
  for(int i=0; i<SHA256_DIGEST_LENGTH;i++) sprintf(res + 2 * i, "%02x", hash[i]); //format hex avec du padding
  
  return res;
}

void compute_proof_of_work(Block* b, int d){
  if (!b) return;
  char* zero_str = (char*)malloc(sizeof(char) * (d + 1));
  for(int i = 0; i < d; i++) zero_str[i] = '0';
  zero_str[d] = '\0';
  int nonce = 0;
  while(1){
    b->nonce = nonce;
    char* block_str = block_to_str(b); //conversiondu bloc en str
    char* new_hash = hash_to_str(block_str); //conversion du hash en hex
    free(block_str);
    //printf("nonce = %d hash: %s\n", nonce, new_hash);
    free(b->hash);
    b->hash = (unsigned char*)new_hash;
    if(strncmp(zero_str, (char*)b->hash, d) == 0){ //comparaison du nb de 0
      printf("nonce = %d hash: %s\n", nonce, new_hash);
      free(zero_str);
      return; //on sort des que strncmp vaut 0->bloc valide
    }
    nonce += 1;
  }
}

int verify_block(Block* b, int d){
  char* zero_str = (char*)malloc(sizeof(char) * d);
  for (int i = 0; i < d; i++) zero_str[i] = '0';
  if(strncmp(zero_str, (char*)b->hash, d) == 0){ //comparaison du nb de 0
    free(zero_str);
    return 1;
  }
  else{
    free(zero_str);
    return 0;
  }
}