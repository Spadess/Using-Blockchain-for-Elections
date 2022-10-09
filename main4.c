#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "partie4.h"


int main(void){
  srand(time(NULL));
  generate_random_data(100, 5);
  CellKey* LV = read_public_keys("keys.txt");
  CellKey* LC = read_public_keys("candidates.txt");
  CellProtected* LP = read_protected("declarations.txt");
  Key* vainqueur = compute_winner(LP, LC, LV, 15, 300);
  char* str_vq = key_to_str(vainqueur);
  printf("\n\nle vainqueur des elections est: %s\n\n", str_vq);
  free(str_vq);

  delete_list_keys(LV);
  delete_list_keys(LC);
  delete_list_protected(LP);
  free(vainqueur);

  return 0;
}
