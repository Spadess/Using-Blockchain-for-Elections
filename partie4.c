#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "partie4.h"

HashCell* create_hashcell(Key* key){
  if(!key) return NULL;
  HashCell* hc = (HashCell*)malloc(sizeof(HashCell));
  assert(hc);
  hc->key = (Key*)malloc(sizeof(Key));
  assert(hc->key);
  hc->key->val = key->val;
  hc->key->n = key->n;
  hc->val = 0;
  return hc;
}

int hash_function(Key* key, int size){ //methode de Knuth
  float A = (sqrt(5) - 1) / 2.0;
  return (int)floor(size * ((key->val * A) - floor(key->val * A)));
}

int find_position(HashTable* t, Key* key){ //chercher si une cle existe ds le tableau
  if(!t || !key) return 0;
  int i = 0, res = hash_function(key, t->size);
  for(i=0;i<t->size;i++){
    int pos = (res + i) % t->size; //probing lineaire: h(k)+i
    //printf("valeur: %d\n", pos);
    if(t->tab[pos]){
      if(egalite_cles(t->tab[pos]->key, key)) return pos;
    }
  } 
  return hash_function(key, t->size); //la cle n'exsite pas
}

HashTable* create_hashtable(CellKey* keys, int size){
  if(!keys) return NULL;
  int i;
  HashTable* t = (HashTable*)malloc(sizeof(HashTable));
  t->tab = (HashCell**)malloc(sizeof(HashCell*) * size);
  assert(t); assert(t->tab);
  t->size = size;
  for(i=0;i<t->size;i++) t->tab[i] = NULL;

  while(keys){
    int res = hash_function(keys->data, t->size);
    for(i=0;i<t->size;i++){
      int pos = (res + i) % t->size; //probing lineaire: h(k)+i
      if(!(t->tab[pos])){
        printf("insertion a la position: %d\n", pos);
        t->tab[pos] = create_hashcell(keys->data);
        break;
      }
      else printf("collision: %d\n", pos);
    }

    keys = keys->next;
  }
  
  return t;
}

void print_hashtable(HashTable* t){
  if(!t) return;
  int i;
  for(i=0;i<t->size;i++){
    if(t->tab[i]) printf("(%lx,%lx)\n", t->tab[i]->key->val, t->tab[i]->key->n);
  }
}

int nb_elem_hashtable(HashTable* t){
  int i, cpt = 0;
  for(i= 0;i<t->size;i++){
    if(t->tab[i]) cpt++;
  }
  return cpt;
}

void affiche_resultats(HashTable* t){
  if(!t) return;
  int i;
  printf("\n\n");
  for(i=0;i<t->size;i++){
    if(t->tab[i]){
      char* str = key_to_str(t->tab[i]->key);
      printf("le candidat %s a obtenu %d votes\n", str, t->tab[i]->val);
      free(str);
    } 
  }
}

void delete_hashtable(HashTable* t){
  if(!t) return;
  int i;
  for(i=0;i<t->size;i++){
    if(t->tab[i]){
      free((t->tab[i])->key);
      free(t->tab[i]);
    } 
  }
  free(t->tab);
  free(t);
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
  check_signature(&decl); //filtrage de la liste des signatures
  HashTable* tv = create_hashtable(voters, sizeV);
  HashTable* tc = create_hashtable(candidates, sizeC);
  HashCell* leader = NULL;

  printf("\n\nApres la verification des declarations, on a:\n");
  printf("%d voteurs participants\n", nb_elem_hashtable(tv));
  printf("%d candidats participants\n", nb_elem_hashtable(tc));
  printf("%d declarations autorisees et valides\n", nb_elem_list_protected(decl));
  
  while(decl){
    Key* vote = str_to_key(decl->data->mess);
    int voter_index = find_position(tv, decl->data->pKey);
    int candidate_index = find_position(tc, vote);
    
    if(egalite_cles(tv->tab[voter_index]->key, decl->data->pKey)){ //si la personne a le droit de voter
      if(!(tv->tab[voter_index]->val)){ //et si cette personne n'a pas encore vote
        if(egalite_cles(tc->tab[candidate_index]->key, vote)){ //et si la personne sur qui porte le vote est un candidat de l'election
            (tc->tab[candidate_index]->val)++; //incrementer le nb de votes pour le candidat vote
            
          //mise a jour du leader actuel de l'election
          if(!leader || leader->val < tc->tab[candidate_index]->val) leader = tc->tab[candidate_index];
        }
      }
      tv->tab[voter_index]->val = 1; //la personne qui vient de voter ne pourra plus voter
    }
    free(vote);
    decl = decl->next;
  }

  //annonce des resultats de l'election
  Key* vainqueur = (Key*)malloc(sizeof(Key));
  assert(vainqueur);
  if(leader){
    vainqueur->val = leader->key->val;
    vainqueur->n = leader->key->n;
  }

  affiche_resultats(tc);
  delete_hashtable(tv);
  delete_hashtable(tc);
  return vainqueur;
}