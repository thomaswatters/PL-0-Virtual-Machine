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
    
    if (argc > 1)
        file = fopen(argv[1], "r" );
    else
        file = fopen("input.txt", "r" );
    
    /* fopen returns 0, the NULL pointer, on failure */
    if ( file == 0 )
    {
        printf( "Could not open file\n" );
        exit(1);
    }
    
    
}
