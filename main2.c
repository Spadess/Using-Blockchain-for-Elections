#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "partie2.h"


int main(void){
  srand(time(NULL));

  //Testing Init Keys
  Key* pKey = malloc(sizeof(Key));
  Key* sKey = malloc(sizeof(Key));
  init_pair_keys(pKey, sKey, 3 ,7);
  printf("\nVoici les cles d'un voteur:\n");
  printf("pKey: (%lx, %lx)\n", pKey ->val, pKey ->n);
  printf("sKey: (%lx, %lx)\n", sKey ->val, sKey ->n);

  
  //Testing Key Serialization
  char* voteur = key_to_str(pKey);
  printf("\nkey to str: %s\n", voteur);
  Key* k = str_to_key(voteur);
  if(!k) {printf("erreur conversion str_to_key\n"); exit(1);}
  printf("str to key: (%lx, %lx)\n", k->val , k->n);

  //Testing signature
  //Candidate keys:
  Key* pKeyC = malloc(sizeof(Key));
  Key* sKeyC = malloc(sizeof(Key));
  init_pair_keys(pKeyC, sKeyC ,3 ,7);
  printf("\nVoici les cles d'un candidat:\n");
  printf("pKeyC: (%lx, %lx)\n", pKeyC->val, pKeyC->n);
  printf("sKeyC: (%lx, %lx)\n", sKeyC->val, sKeyC->n);
  
  //Declaration:
  char* mess = key_to_str(pKeyC);
  printf("\nSoit le voteur avec la cle: %s", voteur);
  printf("\nDisons que ce voteur %s vote pour le candidat %s\n", voteur, mess);
  
  
  Signature* sgn = sign(mess, sKey);
  printf ("\nVoici la signature associee a ce voteur:\n");
  char* sgn_str = signature_to_str(sgn);
  printf ("signature to str: %s\n", sgn_str);
  liberer_signature(sgn);
  sgn = str_to_signature(sgn_str);
  printf ("str to signature: ");
  print_long_vector(sgn->content, sgn->size);


  //Testing protected:
  Protected* pr = init_protected(pKey, mess, sgn);
  //Verification:
  char* pr_str = protected_to_str(pr);
  printf("\nVoici la declaration de vote associee a ce meme voteur:\n");
  printf("protected to str: %s\n", pr_str);
  liberer_protected(pr);
  pr = str_to_protected(pr_str);
  free(voteur);
  voteur = key_to_str(pr->pKey);
  free(sgn_str);
  sgn_str = signature_to_str(pr->sgn);
  
  printf("str to protected: %s %s %s \n", voteur, pr->mess, sgn_str);

  printf("\nVerification de la declaration de ce voteur:\n");
  if (verify(pr)) printf("\n\n->Cette Declaration est valide\n");
  else printf ("\n\n->Cette Declaration est non valide\n");

  free(k);
  k = str_to_key(pr->mess);
  k->n = 163;
  free(pr->mess);
  pr->mess = key_to_str(k);

  printf("\nVerification d'une version frauduleuse: ");
  printf("%s %s %s \n", mess, pr->mess, sgn_str);
  if (verify(pr)) printf("\n\n->Cette Declaration est valide\n");
  else printf ("\n\n->Cette Declaration est non valide\n\n");

  free(k);
  free(pKey);
  free(sKey);
  free(pKeyC);
  free(sKeyC);
  free(voteur);
  free(mess);
  free(sgn_str);
  free(pr_str);
  liberer_protected(pr);
  liberer_signature(sgn);
  
  return 0;
}