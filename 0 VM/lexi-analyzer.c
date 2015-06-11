//
//  lexi-analyzer.c
//  0 VM
//
//  Created by Darian Smalley on 6/3/15.
//  Copyright (c) 2015 Darian Smalley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define norw 15 /* number of reserved words */
#define imax 32767 /* maximum integer value */
#define cmax 11 /* maximum number of chars for idents */
#define nestmax 5 /* maximum depth of block nesting */
#define strmax 256 /* maximum length of strings */

static const int MAX_NAME_TABLE_SIZE = 256;
static const int MAX_NUM_LEN = 5;

typedef enum {
    nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    multsym = 6,  slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym =12,
    gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym =31,
    readsym = 32, elsesym = 33
} token_type;

typedef struct {
    int kind; /*const = 1, var = 2, proc = 3*/
    char name[10]; /*name up to 11*/
    int val; /*number (ASCII value)*/
    int level; /*L level*/
    int adr;   /*M address*/
} namerecord_t;
namerecord_t symbol_table[MAX_NAME_TABLE_SIZE];

/*list of reserved keywords*/
char *word[] = {"null","begin","call","cosnt","do","else","end","if","odd","procedure","read","then","var","while","write"};

/*internal representation of reserved keywords*/
int wsym[] = {nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym, oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};

/*list of special symbols*/
int ssym[256] = {0};

void parseInput(char inputStream[]) {
    //parse input char by char, giant ass switch statment
    int state = 0;
    int i = 0; //token end index
    int j = 0; //token begin index
    int ch = 0;
    
    /*
     TODO:
     two indicies, one points to start of current token substring, the other points to the end.
     iterate through input until a token delim is found.
     send token to be processed.
     */
    
    while ( (ch = inputStream[i]) != EOF) {
    
    }
    
    
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

void fillSsym() {
    ssym['+'] = plussym;
    ssym['-'] = minussym;
    ssym['*'] = multsym;
    ssym['/'] = slashsym;
    ssym['('] = lparentsym;
    ssym[')'] = rparentsym;
    ssym['='] = eqlsym;
    ssym[','] = commasym;
    ssym['.'] = periodsym;
    ssym['#'] = neqsym;
    ssym['<'] = lessym;
    ssym['>'] = gtrsym;
    ssym['$'] = leqsym;
    ssym['%'] = geqsym;
    ssym[';'] = semicolonsym;
    ssym[':'] = becomessym;
}

int main(int argc, const char * argv[]) {
    FILE *file;
    fillSsym();
    
    if (!(file = fopen("input.txt", "r" ))) {
        perror("input.txt");
        exit(1);
    }
    
    char* inputStream = fillInputStream(file);

    fclose(file);
    
    parseInput(inputStream);
}
