#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <openssl/sha.h>
#include "partie5.h"


int main(void){
  srand(time(NULL));
  generate_random_data(100, 5);

  Key* k = str_to_key("(10a5,124f),(5dd,124f)");
  CellProtected* lcp = read_protected("declarations.txt");
  Block* block_write = (Block*)malloc(sizeof(Block));
  block_write->author = k; 
  block_write->hash = (unsigned char*)strdup("test"); 
  block_write->previous_hash = (unsigned char*)strdup("test");
  block_write->votes = lcp; block_write->nonce = 0;
  ecrire_bloc("blocktest.txt", block_write);

  ////
  Block* block_read = lire_bloc("blocktest.txt");
  clock_t temps_initial;
  clock_t temps_final;
  long double temps_cpu;
  FILE* f = fopen("computePOW.txt","w"); 
  int d = 0;
  while(d<4){
    temps_initial = clock();
    compute_proof_of_work(block_read, d);
    temps_final = clock();
    temps_cpu = ((double)(temps_final - temps_initial))/CLOCKS_PER_SEC;
    fprintf(f,"%d %Lf\n", d, temps_cpu);
    printf("temps de calcul du hash valide pour d = %d: %.2Lfs\n\n", d, temps_cpu);
    //if (temps_cpu > 20) break;
    d++;
  }

  supprimer_bloc(block_read);
  supprimer_bloc(block_write);
  fclose(f);

  return 0;
}
