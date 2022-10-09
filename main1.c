#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "partie1.h"


int main(void){
  //EXO 1
  
  clock_t temps_initial;
  clock_t temps_final;
  double temps_cpu = 0.0;
  int i = 3, nbpremier_max = 3, res;

    
  while(temps_cpu < 0.002) {
    temps_initial = clock();
    res = is_prime_naive(i);
    temps_final = clock();
    temps_cpu = (double)(temps_final - temps_initial)/CLOCKS_PER_SEC;
    if(is_prime_naive(res)) nbpremier_max = i;
    i+=2;
  }
  printf("\nLe plus grand nombre premier teste en moins de 2 milliemes de seconde: %d \n\n", nbpremier_max);
        
  printf("\nTracage des courbes de performance: modpow_naive vs modpow (cf fichier gnuplot)\n");
    
    
    //Courbes de performance: modpow_naive vs modpow

  FILE* mp = fopen("modpow.txt","w"); 
  long double temps_cpu1 = 0.0, temps_cpu2 = 0.0; //temps_cpu1: modpow naive et temps_cpu2: modpow
  int random = rand() % RAND_MAX;
  int mod = rand() % 1024;
  
  for(i=2;i<=100;i++) { 
    temps_initial = clock();
    for (int j = 0; j < 100000; j++) modpow_naive(random, i, mod);
    temps_final = clock();
    temps_cpu1 = (long double)(temps_final - temps_initial) / CLOCKS_PER_SEC;
        
    temps_initial = clock();
    for (int j = 0; j < 100000; j++) modpow(random, i, mod);
    temps_final = clock();
    temps_cpu2 = (long double)(temps_final - temps_initial) / CLOCKS_PER_SEC;
    
    fprintf(mp,"%d %Lf %Lf\n", i, temps_cpu1, temps_cpu2);
  }
    fclose(mp);

  //EXO 2

  srand ( time ( NULL ));
  //Generation de cle :
  long p = random_prime_number (3,7, 5000) ;
  long q = random_prime_number (3,7, 5000) ;
  while (p==q){
  q = random_prime_number (3,7, 5000) ;
  }
  long n, s, u;
  generate_key_values (p,q ,&n ,&s ,&u);
  //Pour avoir des cles positives :
  if (u <0){
    long t = (p -1) *(q -1);
    u = u+t; //on aura toujours s*u mod t = 1
  }
   
  //Afichage des cles en hexadecimal
  printf("\n\nGeneration de cles:");
  printf ("\ncle publique en hex = (%lx , %lx)\n",s,n);
  printf ("cle privee en hex = (%lx , %lx)\n",u,n);
      
  printf ("\ncle publique en dec = (%ld , %ld)\n",s,n);
  printf ("cle privee en dec = (%ld , %ld)\n",u,n);
  
  //Chiffrement
  printf("\nVoici un exemple d'un msg a chiffrer: ");
  char mess[13] = "Hello World!";
  int len = strlen(mess);
  long* crypted = encrypt(mess, s, n);
  
  printf("\nInitial message : %s\n", mess );
  printf("\nEncoded: ");
  print_long_vector(crypted, len);

  //Dechiffrement
  char* decoded = decrypt(crypted, len, u, n);
  printf("Decoded: %s\n", decoded);

  free(crypted);
  free(decoded);
  
  return 0;
}
