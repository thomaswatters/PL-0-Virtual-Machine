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

int main(int argc, const char * argv[]) {
    FILE *file;
    int c;
    
    if (!(file = fopen("input.txt", "r" ))) {
        perror("input.txt");
        exit(1);
    }
    
    while ((c = fgetc(file)) != EOF) {
        
        // now do something with each character, c.
        
    }
    
    fclose(file);
}
