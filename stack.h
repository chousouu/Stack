#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_INFO

enum Options 
{
    CAP_MULTIPLIER = 2,
    STACK_DECREASE = 1, 
    STACK_INCREASE = 0,
};

#define StackCtor(stack, X) StackCtor_(&stack, X, __LINE__, __FILE__, __func__, #stack);

#ifdef DEBUG_INFO
    #define Stack_OK(stack, X) \
    StackDump(stack, StackVerify(stack));
#endif

enum ERRORS
{
    STACK_NULL         = 1,
    NEGATIVE_SIZE      = 2,
    NEGATIVE_CAPPACITY = 4,
    CAP_SMALLER_SIZE   = 8,
    
};
const int POISON       = 0x91DF00;
 
struct Stack
{
    int *data;
    int cappacity;
    int size;
    bool capmode;

    #ifdef DEBUG_INFO
    int line_ctor;
    int line_push;
    const char *file_ctor;
    const char *file_push;
    const char *stackname;
    #endif //DEBUG_INFO
};


void StackCtor_(struct Stack *stack, int number, int line_ctor, const char* file_ctor, const char* func_name, const char *stack_name);
void StackPush(struct Stack *stack, int number); 
int StackPop(Stack *stack);
void StackResize(struct Stack *stack, bool mode); 
void StackPrint(struct Stack *stack);
void StackDump(struct Stack *stack, int problem_code);
void StackDtor(struct Stack *stack);
int StackVerify(struct Stack *stack);
void StackPrint(struct Stack *stack, FILE *logfile);
#endif //STACK_H