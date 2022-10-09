#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "partie3.h"


CellKey* create_cell_key(Key* key){
  CellKey* LCK = (CellKey*)malloc(sizeof(CellKey));
  assert(LCK);
  LCK->data = key;
  LCK->next = NULL;
  return LCK;
}

void add_cell_key(CellKey** LCK, Key* key){ //double pointeur pour bouger la tete de la chaine
  if(!*LCK) {
    *LCK = create_cell_key(key);
    return;
  }
  CellKey* newlck = create_cell_key(key);
  newlck->next = *LCK;
  *LCK = newlck;
}

CellKey* read_public_keys(char* filename){
  FILE* f = fopen(filename, "r");
  assert(f);
  char buffer[256];
  char key_str[156];
  Key* pkey = NULL;
  CellKey* LCK = NULL;
  while(fgets(buffer,256,f)){
    if(sscanf(buffer, "%s", key_str) != 1) return NULL;
    pkey = str_to_key(key_str);
    add_cell_key(&LCK, pkey);
  }
  fclose(f);
  return LCK;
}


void print_list_keys(CellKey* LCK){ //fonction recursive pour afficher ds l'ordre
  if(!LCK) return;
  print_list_keys(LCK->next);
  printf("(%lx,%lx)\n", LCK->data->val, LCK->data->n);
}

void delete_cell_key(CellKey* c){
  free(c->data);
  free(c);
}

void delete_list_keys(CellKey* LCK){
  if(!LCK) return;
  CellKey* tmp = NULL;
  while(LCK) {
    tmp = LCK->next;
    delete_cell_key(LCK);
    LCK = tmp;
  }
}

CellProtected* create_cell_protected(Protected* pr){
  CellProtected* c = (CellProtected*)malloc(sizeof(CellProtected));
  assert(c);
  c->data = pr;
  c->next = NULL;
  return c;
}

void add_cell_protected(CellProtected** LCP, Protected* pr){
  if(!(*LCP)) {
    *LCP = create_cell_protected(pr);
    return;
  }
  CellProtected* newlcp = create_cell_protected(pr);
  newlcp->next = *LCP;
  *LCP = newlcp;
}

int nb_elem_list_protected(CellProtected* LCP){ //utilisee que dans la partie 4 pour compter le nb de decl
  int cpt = 0;
  while(LCP){
    cpt++;
    LCP = LCP->next;
  }
  return cpt;
}

CellProtected* read_protected(char* filename){
  FILE* f = fopen(filename, "r");
  assert(f);
  char buffer[256];
  Protected* pr = NULL;
  CellProtected* LCP = NULL;
  while(fgets(buffer,256,f)){
    pr = str_to_protected(buffer);
    add_cell_protected(&LCP, pr);
  }
  fclose(f);
  return LCP;
}

void print_list_protected(CellProtected* LCP){ //fonction recursive pour afficher ds l'ordre
  if(!LCP) return;
  print_list_protected(LCP->next);
  char* pr_str = protected_to_str(LCP->data);
  printf("%s\n", pr_str);
  free(pr_str);
}


void delete_cell_protected(CellProtected* c){
  if(!c) return;
  liberer_protected(c->data);
  free(c);
}

void delete_list_protected(CellProtected* LCP){
  if(!LCP) return;
  CellProtected* tmp = NULL;
  while(LCP) {
    tmp = LCP->next;
    delete_cell_protected(LCP);
    LCP = tmp;
  }
}

void check_signature(CellProtected** LCP){
  CellProtected* tmp = *LCP;
  CellProtected* prec = NULL;
  CellProtected* dlt = NULL;
  while(tmp){
    if(verify(tmp->data) != 1){
      char* sgn_str = signature_to_str((*LCP)->data->sgn);
      printf("cette declaration n'est pas valide\n"); 
      free(sgn_str);
      
      if(prec) prec->next = tmp->next;

      else *LCP = (*LCP)->next;

      dlt = tmp;
      tmp = tmp->next;
      delete_cell_protected(dlt);
    }
    else{
      printf("cette declaration est valide\n");
      prec = tmp;
      tmp = tmp->next;
    }
  }
}

