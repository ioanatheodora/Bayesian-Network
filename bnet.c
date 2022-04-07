#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bayes.h"

int main(int argc, char *argv[]){
    FILE *f_in = fopen("bnet.in", "r");
    FILE *f_out = fopen("bnet.out", "w");
    TGraph *bayes;


    if(strcmp(argv[argc - 1], "-c1") == 0){
       bayes = citire(f_in);
       cerinta1(bayes, f_out);
    }
    if(strcmp(argv[argc - 1], "-c2") == 0){
        bayes = citire(f_in);
        cerinta2(bayes, f_in, f_out);
        // destroyGraphAdjList(bayes);
    }

    fclose(f_in);
    fclose(f_out);
    return 0;
}