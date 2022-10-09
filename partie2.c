#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "partie2.h"

void init_key(Key* key, long val, long n){
    if(!key) return;
    key->val = val;
    key->n = n;
}

void init_pair_keys(Key* pKey, Key* sKey, int low_size, int up_size){
    /*On déclare les variables qui iront dans les clés*/
    long u;
    long s; 
    long n;

    /*Variables pour la génération de clés*/
    long p = random_prime_number(low_size,up_size,5000);        //test de miller répété 5000 fois
    long q = random_prime_number(low_size,up_size,5000);
    while ( p == q ) {
        q = random_prime_number (3 ,7 , 5000);
    }

    generate_key_values (p ,q ,&n ,&s ,&u) ;               //On génère les valeurs des clés selon le protocole RSA
    //Pour avoir des cles positives :
    if (u <0) {
        long t = (p -1) *( q -1) ;                      //(lignes 33 à 36 directement tirés du main fourni dans l'énoncé de la partie 1)
        u = u + t ; //on aura toujours s*u mod t = 1
    }

    /*On peut maintenant intialiser les valeurs de nos deux clés*/
    init_key(pKey,s,n);
    init_key(sKey,u,n);

}

char* key_to_str(Key* key){
    if(!key) return NULL;
    char buffer[256];
    sprintf(buffer,"(%lx,%lx)%c",key->val,key->n,'\0');

    return strdup(buffer);    
}

    

Key* str_to_key(char* str){
    long val;
    long n;
    if(sscanf(str,"(%lx,%lx)",&val,&n) != 2) return NULL;
    Key* cle_res =(Key*) malloc(sizeof(Key));
    init_key(cle_res,val,n);

    return cle_res;
}

Signature* init_signature(long* content, int size){
	Signature* sign = (Signature*)(malloc(sizeof(Signature)));
	sign->content = content;
	sign->size = size;
	return sign;
}

Signature* copie_sgn(Signature* sgn){
	Signature* sign = (Signature*) malloc(sizeof(Signature));
	sign->content = (long*) malloc(sgn->size *sizeof(long));
	for(int i = 0 ; i < sgn->size; i++){
		(sign->content)[i] = (sgn->content)[i];
	}
	sign->size = sgn->size;
	return sign;
}

Signature* sign(char* mess, Key* sKey){
	long* coded = encrypt(mess,sKey->val,sKey->n);
	return init_signature(coded,strlen(mess));
}

char* signature_to_str(Signature* sgn){
	char* result = (char*) malloc (10*sgn->size*sizeof(char));
	result [0]= '#' ;
	int pos = 1;
	char buffer[156];
	for (int i = 0; i < sgn->size; i ++){
		sprintf(buffer,"%lx", sgn->content[i]);
		for (int j = 0; j < strlen(buffer) ; j ++){
			result [pos] = buffer [j];
			pos = pos +1;
		}
		result[pos] = '#';
		pos = pos +1;
	}
	result[pos] = '\0';
	result = realloc(result,(pos+1)*sizeof(char));
	return result;
}

Signature* str_to_signature(char* str){
	int len = strlen (str) ;
	long * content = (long*)malloc(sizeof(long)*len);
	int num = 0;
	char buffer [256];
	int pos = 0;
	for (int i = 0; i < len; i++){
		if (str[i] != '#'){
			buffer[pos] = str[i];
			pos = pos+1;
		}else{
			if (pos != 0){
				buffer[pos] = '\0';
				sscanf(buffer, "%lx", &(content[num]));
				num = num + 1;
				pos = 0;
			}
		}
	}
	content = realloc(content,num*sizeof(long));
	return init_signature(content,num);
}

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
	Protected* p = (Protected*)(malloc(sizeof(Protected)));
	p->pKey = (Key*)malloc(sizeof(Key));
	init_key(p->pKey,pKey->val,pKey->n);			//On fait une deepcopy de la clé
	p->mess = strdup(mess); 
	p->sgn = copie_sgn(sgn);

	return p;
}

int verify(Protected* pr){ //verifie que la signature contenue dans pr correspond bien au mess et a la personne contenus dans pr.
  if(!pr) return 0;
  char* check = decrypt(pr->sgn->content, pr->sgn->size, pr->pKey->val, pr->pKey->n);
  Key* k1 = str_to_key(check);
  Key* k2 = str_to_key(pr->mess);
  printf("checking: (%lx,%lx) vs (%lx,%lx): ", k1->val, k1->n, k2->val, k2->n);
  if(egalite_cles(k1, k2) != 1) {
    free(k1); free(k2); free(check);
    return 0;
  }
  free(k1); free(k2); free(check);
  return 1;
}


char* protected_to_str(Protected* protected){
	char* cle = key_to_str(protected->pKey);
	char* signature = signature_to_str(protected->sgn);
	int size = strlen(cle)+strlen(protected->mess)+strlen(signature)+3;
	char * res = (char*) malloc(size*sizeof(char));

	sprintf(res,"%s %s %s",cle,protected->mess,signature);
	free(cle);
	free(signature);

	return res;
}

Protected* str_to_protected(char* chaine){
	char cleS[256];
	char message[256];
	char signature[256];

	if(sscanf(chaine,"%s %s %s",cleS,message,signature)!=3) return NULL;

	Signature* sgn = str_to_signature(signature);
	Key* cle = str_to_key(cleS);
	Protected* pr = init_protected(cle,message,sgn);
	liberer_signature(sgn);
	free(cle);
	return pr;
}

void liberer_signature(Signature* sgn){
	free(sgn->content);
	free(sgn);
}
void liberer_protected(Protected*pr){
	free(pr->mess);
	free(pr->pKey);
	liberer_signature(pr->sgn);
	free(pr);
}

int egalite_cles(Key* k1, Key* k2){
  if(!k1 || !k2) return 0;
  return ((k1->val == k2->val) && (k1->n == k2->n));
}


void generate_random_data(int nv, int nc){
  Key* pKey1 = NULL;
  Key* pKey2 = NULL;
  Key* sKey = NULL;
  char* sgn_str = NULL;
  Signature* sgn = NULL;
  char bufferkeys[256]="";
  char buffercand[256]="";
  char skey_str[100]="";
  int i, random , currentline = 0;
  FILE* fkeys = fopen("keys.txt", "w+");
  FILE* fcand = fopen("candidates.txt", "w+");
  FILE* fdecl = fopen("declarations.txt","w+");
  assert(fkeys); assert(fcand); assert(fdecl);

  for(i=0;i<nv;i++){ //generer tous les voteurs
    pKey1 = malloc(sizeof(Key));
    sKey = malloc(sizeof(Key)); 
    init_pair_keys(pKey1, sKey, 3 ,7);
    fprintf(fkeys, "(%lx,%lx) (%lx,%lx)\n", pKey1->val, pKey1->n, sKey->val, sKey->n);
    free(pKey1);
    free(sKey);
  }

  for(i=0;i<nc;i++){ //generer les candidats aleatoirement sans repetition
    do{
      currentline = 0;
      rewind(fcand);
      rewind(fkeys);
      random = rand() % nv;
      while(currentline!=random) {
        fgets(bufferkeys, 256, fkeys);
        currentline++;
      }
      fgets(bufferkeys, 256, fkeys);
      pKey1 = str_to_key(bufferkeys);

      while(!feof(fcand)){
        fgets(buffercand, 256, fcand);
        pKey2 = str_to_key(buffercand); 
        if(egalite_cles(pKey1, pKey2)){
          free(pKey2);
          break;
        } 
        free(pKey2);
      }
      if(feof(fcand)) break;
      free(pKey1);
    }while(1);
    
    fprintf(fcand, "(%lx,%lx)\n", pKey1->val, pKey1->n);
    free(pKey1);
  }

  rewind(fkeys);
  for(i=0;i<nv;i++){ //generer les declarations de votes
    currentline = 0;
    rewind(fcand);
    fgets(bufferkeys, 256, fkeys);
    if(sscanf(bufferkeys, "%*s %s", skey_str) !=1){ 
      printf("erreur lecture generate_random_data\n");
      return;
    } 
    sKey = str_to_key(skey_str);
    pKey1 = str_to_key(bufferkeys);
    random = rand() % nc;
    while(currentline!=random) {
      fgets(buffercand, 256, fcand);
      currentline++;
    }
    fgets(buffercand, 256, fcand);
    sgn = sign(buffercand, sKey);
    sgn_str = signature_to_str(sgn);
    liberer_signature(sgn);
    buffercand[strlen(buffercand)-1] = '\0';
    fprintf(fdecl, "(%lx,%lx) %s %s\n", pKey1->val, pKey1->n, buffercand, sgn_str);
    free(sgn_str);
    free(pKey1);
    free(sKey);
  }
  
  
  fclose(fkeys);
  fclose(fcand);
  fclose(fdecl);
}
