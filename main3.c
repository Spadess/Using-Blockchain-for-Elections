#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "partie3.h"


int main(void){
  srand(time(NULL));
  generate_random_data(100, 5);
  CellKey* LCK = read_public_keys("keys.txt");
  //print_list_keys(LCK);
  CellProtected* LCP = read_protected("declarations.txt");
  print_list_protected(LCP);
  
  CellProtected* LCPfake = read_protected("fakedeclarations.txt");
  printf("\n\nNb de declarations avant filtrage avec check_signature: %d\n\n", nb_elem_list_protected(LCPfake));
  check_signature(&LCPfake);
  printf("\n\nNb de declarations apres filtrage avec check_signature: %d\n\n", nb_elem_list_protected(LCPfake));

  delete_list_keys(LCK);
  delete_list_protected(LCP);
  delete_list_protected(LCPfake);

  return 0;
}
