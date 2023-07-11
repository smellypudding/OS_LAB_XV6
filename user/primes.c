#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int primes(int p[2]){
   int prime;
   int n;
   close(p[1]);
   if(read(p[0], &prime, sizeof(int))!=sizeof(int)){
    fprintf(2, "failed when reading\n");
    exit(1);
   }
   fprintf(0, "prime %d\n", prime);
   
   int is_not_end = read(p[0], &n, sizeof(int));
   if(is_not_end){
    int p1[2];
    pipe(p1);
    if(fork()==0){
        primes(p1);
    }
    else{
        close(p1[0]);
        if(n%prime){
            write(p1[1], &n, sizeof(int));
        }
        while(read(p[0], &n, sizeof(int))){
            if(n%prime){
                write(p1[1], &n, 4);
            }
        }
        close(p[0]);
        close(p1[1]);
        wait(0);
    }
   }
   exit(0);
}

    

int
main(void){
    int p[2];
    pipe(p);
    if(fork()==0){
        primes(p);
    }
    else{
        close(p[0]);
        for(int i=2;i<=35;i++){
            if(write(p[1], &i, sizeof(int))!=sizeof(int)){
                fprintf(2, "failed when writing\n");
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
    return 0;
}