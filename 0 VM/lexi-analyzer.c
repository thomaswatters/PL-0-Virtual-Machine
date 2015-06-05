//
//  lexi-analyzer.c
//  0 VM
//
//  Created by Darian Smalley on 6/3/15.
//  Copyright (c) 2015 Darian Smalley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

static const int MAX_ID_LEN = 11;
static const int MAX_NUM_LEN = 5;

void parseInput(char inputStream[]) {
    //parse input char by char
}

char* fillInputStream(FILE* file) {
    static char inputStream[267];
    int c;
    int i = 0;
    
    while ((c = fgetc(file)) != EOF) {
        inputStream[i] = c;
    }
    
    return inputStream;
}

int main(int argc, const char * argv[]) {
    FILE *file;
    
    
    if (!(file = fopen("input.txt", "r" ))) {
        perror("input.txt");
        exit(1);
    }
    
    char* inputStream = fillInputStream(file);

    fclose(file);
    
    parseInput(inputStream);
}
