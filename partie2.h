#ifndef PARTIE2_H
#define PARTIE2_H
#include "partie1.h"

typedef struct{
  long val;
  long n;
}Key;

typedef struct{
  long* content;
  int size;
}Signature;

typedef struct{
  Key* pKey;
  char* mess;
  Signature* sgn;
}Protected;

void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key* sKey, int low_size, int up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);
Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str (Signature* sgn);
Signature* str_to_signature(char* str);
Protected* init_protected(Key* pKey, char* mess, Signature* sgn);
int egalite_cles(Key* k1, Key* k2);
int verify(Protected* pr);
char* protected_to_str(Protected* pr);
Protected* str_to_protected(char* str);
void supprimer_signature(Signature* sgn);
void supprimer_protected(Protected* pr);
void generate_random_data(int nv, int nc);
Signature* copie_sgn(Signature* sgn);
void liberer_signature(Signature* sgn);
void liberer_protected(Protected*pr);


#endif