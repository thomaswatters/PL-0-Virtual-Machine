//
//  main.c
//  0 VM
//
//  Created by Darian Smalley on 5/23/15.
//  Copyright (c) 2015 Darian Smalley. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Instruction{
    int op; /*opcode*/
    int l; //Lexographical Level
    int m; //modifier
    int line_number; //line number
} IR;

static const int MAX_STACK_HEIGHT = 100;
static const int MAX_CODE_LENGTH = 500;
static const int MAX_LEXI_LEVELS = 3;

int CURRENT_CODE_LENGTH;
const char *opCodes[9] = {"LIT", "OPR", "LOD", "STO", "CAL", "INC","JMP","JPC","SIO"};
int sp = 0; //stack pointer
int bp = 1; //base pointer
int pc = 0; //program counter
IR ir;      //instruction register
IR code[MAX_CODE_LENGTH];
int stack[MAX_STACK_HEIGHT] = {0};
char stack_str[MAX_STACK_HEIGHT + 100][10]; //string version of stack
int str_top = 0; //points to the top of the string stack
int halt = 0;

/*
 *Find base L level down
 */
int base (int level,int b) {
    while(level > 0) {
        b = stack[b + 2];
        level--;
    }
    return b;
}

/*START OPR Arithmetic / Logic FUNCITONS*/

void opr00_RET () {
    sp = bp - 1 ;
    pc = stack[sp + 4];
    bp = stack[sp + 3];
}

void opr01_NEG ()  {
    stack[sp] = -stack[sp];
}

void opr02_ADD() {
    sp--;
    stack[sp] = stack[sp] + stack[sp + 1];
}

void opr03_SUB() {
    sp--;
    stack[sp] = stack[sp] - stack[sp + 1];
}

void opr04_MUL() {
    sp--;
    stack[sp] = stack[sp] * stack[sp + 1];
}

void opr05_DIV() {
    sp--;
    stack[sp] = stack[sp] / stack[sp + 1];
}

void opr06_ODD() {
    stack[sp] = stack[sp] % 2;
}

void opr07_MOD() {
    sp--;
    stack[sp] = stack[sp] % stack[sp + 1];
}

void opr08_EQL() {
    sp--;
    stack[sp] = stack[sp] == stack[sp + 1];
}

void opr09_NEQ() {
    sp--;
    stack[sp] = stack[sp] != stack[sp + 1];
}

void opr10_LSS() {
    sp--;
    stack[sp] = stack[sp] < stack[sp + 1];
}

void opr11_LEQ() {
    sp--;
    stack[sp] = stack[sp] <= stack[sp + 1];
}

void opr12_GTR() {
    sp--;
    stack[sp] = stack[sp] > stack[sp + 1];
}

void opr13_GEQ() {
    sp--;
    stack[sp] = stack[sp] >= stack[sp + 1];
}
/*END OPR FUNCTIONS*/

/* array of function pointers to the 14 arithmetic/logic operations available.*/
void (*opr_fp[14]) () = {opr00_RET, opr01_NEG, opr02_ADD, opr03_SUB, opr04_MUL, opr05_DIV, opr06_ODD, opr07_MOD, opr08_EQL, opr09_NEQ, opr10_LSS, opr11_LEQ, opr12_GTR, opr13_GEQ};

/*INSTRUCTION SET ARCHITECTURE*/

/*Push value M onto stack*/
void op01_LIT (int l, int m) {
    sp++;
    stack[sp] = m;
}

//arithmetic / logical operations
void op02_OPR (int l, int m) {
    //do opration specified by m
    (*opr_fp[m])();
}

/*Get value at offset M in frame L levels down and push it*/
void op03_LOD (int l, int m) {
    sp++;
    stack[sp] = stack[ base(l, bp) + m];
}

//Pop stack and insert value at offset M in frame L levels down
void op04_STO (int l, int m) {
    stack[ base(l, bp) + m] = stack[sp];
    sp--;
}

/*Call procedure at M (generates new stack frame)*/
void op05_CAL (int l, int m) {
    stack[sp + 1] = 0;             //return value (FV)
    stack[sp + 2] = base(l, bp);   //static link (SL)
    stack[sp + 3] = bp;            //dynamic link (DL)
    stack[sp + 4] = pc;            //return address (RA)
    bp = sp + 1;                   //bp points to FV in new AR
    pc = m;
}

//Allocate M locals on stack
void op06_INC (int l, int m) {
    sp += m;
}

//jupm to m
void op07_JMP (int l, int m) {
    pc = m;
}

//Pop stack and jump to M if value is equal to 0
void op08_JPC (int l, int m) {
    
    if ( stack[sp] == 0) {
        pc = m;
        sp--;
    }
    
}

//misc operations
void op09_SIO (int l, int m) {
    //pop stack and print out value
    if( m == 0) {
        printf("%d", stack[sp]);
        sp = sp - 1;
    }
    //read input from user and push it
    else if ( m == 1) {
        sp = sp + 1;
        //read value and store it in stack.data[sp]
    }
    //halt
    else { //m == 2
        halt = 1;
    }
}

/*END ISA*/

/* array of function pointers to the nine ISA operations available.*/
void (*op_fp[9]) (int lexi, int mod) = {op01_LIT, op02_OPR, op03_LOD, op04_STO, op05_CAL, op06_INC, op07_JMP, op08_JPC, op09_SIO};

void generateInst(IR *inst, char *terms[], int lineNumber) {
//    printf("%s", "converting from char to int...\n");
    int i = 0;

    inst->op = atoi(terms[i++]);
    inst->l = atoi(terms[i++]);
    inst->m = atoi(terms[i]);
    inst->line_number = lineNumber;
}

void printLine(char line[]) {
    int i = 0;
    printf("Print Line: ");
    while (line[i] != '\0') {
        printf("%c", line[i]);
        i++;
    }
}

void splitLine(char *line, char *terms[]) {
//    printf("clearLine line: %s", line);
    char *tmp = strdup(line);
    char *token;
    int i = 0;
    
    while(i < 3 ) {
        token = strsep(&tmp, " ") ;
//        printf ("\n line: %s\n token: %s\n", tmp, token);
        terms[i] = token;
        i++;
    }
}

void fillCode (FILE *file) {
    char line[8] ={0};
    int i = 0;
    char *terms[3]={0};
    
    while (fgets(line, sizeof(line), file) ) {
//        printLine(line);
        IR inst;
        
        //split input line using space a delim
        splitLine(line, terms);
        
        //generate instruction object using values in terms[]
        generateInst(&inst, terms, i);
        
        //set current code index to new inst obj
        code[i] = inst;
        
        //clear line in memory
        memset(&line[0], 0, sizeof(line));
        i++;
    }
    
    CURRENT_CODE_LENGTH = i;
    stack[0] = (int) &code;
}

void printCode(FILE *file) {
    const char *text = "Line   OP  L  M";
    fprintf(file, "%s\n", text);
    fprintf(file, "%s\n", "----------------");
    
    int i;
    
    for (i = 0; i < CURRENT_CODE_LENGTH; i++) {
//        fprintf(file, "%s\n", "New instruction");
        if ( i < 10)
            fprintf(file, " 0%d   %s  %d  %d\n",i, opCodes[code[i].op - 1], code[i].l, code[i].m);
        else
            fprintf(file, " %d   %s  %d  %d\n",i, opCodes[code[i].op - 1], code[i].l, code[i].m);
    }
    fprintf(file, "\n");
}

void printStack_Str(FILE *file) {
    int i;
    
    for ( i = 0; i <= str_top; i++) {
        fprintf(file, "%s ", stack_str[i]);
    }
}

void printInstructionTrace(FILE *file) {
    
    if ( ir.line_number < 10)
        fprintf(file, "0%d", ir.line_number);
    else
        fprintf(file, "%d", ir.line_number);
    
    fprintf(file, "   %s  %d  %d  %d  %d  %d     ", opCodes[ir.op - 1], ir.l, ir.m, pc, bp, sp);
    
    if( sp > 0)
        printStack_Str(file);
    
    fprintf(file, "\n");
}


int isDivider(int i) {
    return ( strcmp(stack_str[i], "|") == 0 ) ? 1: 0;
}

void matchStacks() {
    int i = 0; //stack_str index
    int j = 1; //stack index
    
    while ( j <= sp) {
        //skip AR dividers
        if( !isDivider(i) ) {
            char buf[50];
            
            // convert stack[j] to string and place in buffer
            sprintf(buf,"%d",stack[j]);
            
            //if the strings are different, copy buf to stack_str[i]
            if( strcmp(stack_str[i], buf) != 0) {
                strcpy(stack_str[i], buf);
            }
            
            //update indecies
            str_top = i;
            j++;
        }
        
        i++;
        
        //incase j is not incremented past sp
        if( i > MAX_STACK_HEIGHT) {
            printf("error@matchStacks(): i > MAX_STACK_HEIGHT");
            exit(1);
        }
        
    }
}

void removeTopAR() {
    int i = str_top;
    
    //find index of AR divider closest to top of stack
    while ( !isDivider(i) ) {
        //clear element
        memset(&stack_str[i], 0, sizeof(stack_str[i]));
        i--;
    }
    //set the divier to 0
    memset(&stack_str[i], 0, sizeof(stack_str[i]));
    
    str_top = i - 1;
}

void updateStack_str() {
    //check for instruction type
    
    //if CAL, handle with new bracket
    if (ir.op == 5) {
        //move index to next element, place a bracket to seperate activation records and update number_of_AR
        str_top++;
        strcpy(stack_str[str_top], "|");
        
        matchStacks();
        
    }
    //else if RET, handle by popping to last braket
    else if (ir.op == 2 && ir.m == 0) {
        removeTopAR();
    }
    //else, copy changes from stack to string
    else {
        matchStacks();
    }
}

void fetch() {
    //load next instruction in code to the instruction register
    ir = code[pc];
    pc++;
}

void execute() {
    
    //do current operation in instruction register
    (*op_fp[ir.op - 1]) (ir.l, ir.m);
    
    //update the string version of the stack
    updateStack_str();
}

int main(int argc, const char * argv[]) {
    
    FILE *file = fopen("mcode.txt", "r" );
    
//   stack_str = malloc((MAX_STACK_HEIGHT + 100) * sizeof(char));
    
    /* fopen returns 0, the NULL pointer, on failure */
    if ( file == 0 )
    {
        printf( "Could not open file\n" );
        exit(1);
    }

    fillCode( file );
    fclose( file );
    
    file = fopen("stacktrace.txt","w");
    
    if ( file == NULL) {
        printf("Could not open file\n");
        exit(1);
    }
    
    printCode( file );
    
    fprintf(file, "Line  OP  L  M  pc  bp  sp     stack    \n");
    fprintf(file, "----------------------------------------\n");
    fprintf(file, "Init Values:    0   1   0               \n");
    
    /*instruction cycle*/
    
    //while instruction is not halt
    while ( halt == 0) {
        fetch();
        execute();
        printInstructionTrace(file);
    }
    
    fclose( file );
}
