//
//  lexi-analyzer.c
//  0 VM
//
//  Created by Darian Smalley on 6/3/15.
//  Copyright (c) 2015 Darian Smalley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int isTokenDelim(char ch) {
    //if ssym[ch] is not zero then ch is a special symbol
    if ( ssym[ch] != 0) {
        return 1;
    }
    else {
        return 0;
    }
}

//C passes by value and makes a copy so that changs made in this functions local scope will not effect the caller
void processToken(int start,int end, char stream[] ) {
    int i = 0;
    int len = end - start;
    char *token;
    
    strncpy ( token, stream, len );
    
    for (i = 0; i < norw; i++) {
        //reserved word found
        if ( strcmp ( word[i], token ) == 0) {
            //DO STUFF!
        }
    }
    
}

void tokenizeInput(char inputStream[]) {
    int i = 0; //token being index
    int j = 0; //token end index
    int ch = 0;
    
    /*
     TODO:
     two indicies, one points to start of current token substring, the other points to the end.
     iterate through input until a token delim is found.
     send token to be processed.
     */
    
    /*tokens are seperated by either whitespace or special symbols and operators*/
    while ( (ch = inputStream[j]) != EOF) {
        //ch is whitespace
        if ( isspace(ch) != 0 ) {
            processToken(i,j,inputStream);
            //print white space to files using j
            i = j + 1;
            j++;
        }
        //ch is special symbol
        else if ( isTokenDelim(ch) ) {
            processToken(i,j,inputStream);
            //process special sym
            //print symbol to files using j
            i = j + 1;
            j++;
        }
        //ch is the next character in a token
        else {
            //dont forget the error checking, damnit
            j++;
        }
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

int main(int argc, const char * argv[]) {
    FILE *file;
    fillSsym();
    
    if (!(file = fopen("input.txt", "r" ))) {
        perror("input.txt");
        exit(1);
    }
    
    char* inputStream = fillInputStream(file);

    fclose(file);
    
    tokenizeInput(inputStream);
}
