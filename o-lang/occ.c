/* Copyright(C) Midori Ado All right reserved. */
#include <stdio.h>
#include <stdlib.h>

void o_compile(FILE *fp){
    
}

int main(int argc, char *argv[]){
    if(argc>3){
        puts("OCC:[Error] Option is a lot!");
        return -1;
    }
    if(argv[1]=='h'){
        puts("OCC:[Help] OCC can reserve those commands");
        puts("o: Compile O-lang(.o)");
        puts("c: Compile C-lang(.c)");
        puts("h: Appear usage");
        return 0;
    }
    if(argv[1]=='o'){
        FILE *fp;
        fp=fopen(argv[2],"r");
        o_compile(fp);
    }
}