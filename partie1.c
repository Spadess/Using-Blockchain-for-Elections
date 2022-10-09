#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include "partie1.h"

#define M_MAX 1000
#define A 10
#define N 3


int is_prime_naive(long p){
    for(int i = 2 ; i <= p/2 ; i++){
        if(p%i==0) return 0;
    }
    return 1;           
}


long modpow_naive(long a, long m, long n){
	long res = 1;
	for (int i=0; i<m; i++){
		res *= a;
		res = res%n;
	}
	return res;
}


long modpow(long a, long m, long n){
    long b;

    if (m == 0) return 1;
    
    if (m == 1) return a%n;

    b = modpow(a, m / 2, n);

    if (m % 2 == 0) // cas pair
        return (b * b) % n;

    else // cas impair
        return (a * b * b) % n;
}


int witness ( long a , long b , long d , long p ) {
    long x = modpow (a ,d , p ) ;
    if ( x == 1) return 0;
    for ( long i = 0; i < b ; i ++) {
        if ( x == p -1) return 0;
        x = modpow (x ,2 , p ) ;
    }
    return 1;
}

long rand_long ( long low , long up ) {
    return rand() % ( up - low +1 ) + low ;
}

int is_prime_miller ( long p , int k ) {
    if ( p == 2) {
        return 1;
    }
    if (!( p & 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!( d & 1) ) { //tant que d n’est pas impair
        d = d /2;
        b = b +1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a ;
    int i ;
    for ( i = 0; i < k ; i ++) {
        a = rand_long (2 , p -1) ;
        if ( witness (a ,b ,d , p ) ) {
            return 0;
        }
    }
    return 1;
}

long expo(int nb, int pow){
	long res = 1;
	for (int i = 0; i<pow; i++){
		res = res*nb;
	}return res;
}

long random_prime_number(int low_size, int up_size, int k){
	long low = expo(2,low_size);
	long up = expo(2,up_size)-1;

	long p = rand_long(low,up);
	
	while (!is_prime_miller(p,k)){
		p = rand_long(low,up);
	}
	if (is_prime_miller(p,k)){
		return p;
	}
    else return -1;
}


long extended_gcd(long s ,long t, long* u, long* v) {
    if(s == 0) {
    *u = 0;
    *v = 1;
    return t;
  }

  long uPrim, vPrim;
  long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
  *u = vPrim - (t/s) * uPrim;
  *v = uPrim;

  return gcd;
}


void generate_key_values(long p, long q, long* n, long* s, long* u){
    *n = p * q;
    long t = (p-1)*(q-1);
    long v;
    *s = rand()%t;
    while(extended_gcd(*s,t,u,&v)!=1){
        *s = rand()%t;
    }
}


long* encrypt(char* chaine, long s, long n){
    long* coded = (long*) malloc(strlen(chaine)*sizeof(long));
    int i = 0;
    for(i = 0; i < strlen(chaine) ; i++){
        coded[i] = modpow((long)chaine[i],s,n);        //On code chaque lettre du message
    }
    return coded;
}


char* decrypt(long* crypted, int size, long u, long n){
    char* decoded = (char*) malloc((size+1)*sizeof(char));

    for(int i = 0; i < size ; i++){
        decoded[i] = (char) modpow(crypted[i],u,n); 
    }
    decoded[size] = '\0';

    return decoded;
}

void print_long_vector(long* result, int size){
    printf("Vector: [");
    for (int i = 0; i < size ; i++){
        printf("%lx  ",result[i]);
    }
    printf("]\n");
}
