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

#define StackCtor(stack, X) StackCtor_(&stack, X, __LINE__, __FILE__, #stack);

#ifdef DEBUG_INFO
    #define Stack_OK(stack) \
    int problem_code = StackVerify(stack); \
    StackDump(stack, problem_code); \
    if(problem_code) return problem_code;
#endif

enum ERRORS
{
    POP_EMPTY_STACK    = 1,
    NEGATIVE_SIZE      = 2,
    NEGATIVE_CAPACITY  = 4,
    CAP_SMALLER_SIZE   = 8,
    STACK_NULL         = 16,
    MEM_ALLOC_FAIL     = 32,

};
const int POISON       = 0x91DF00;
 
struct Stack
{
    int *data;
    int capacity;
    int size;
    bool capmode;

    #ifdef DEBUG_INFO
    int         line_ctor;
    const char *file_ctor;
    int         line_action;
    const char *file_action;
    const char  *action_funcname;
    const char *stackname;
    #endif //DEBUG_INFO
};

#define StackPush_(stack, X) \
GetActionInfo(&stack, __LINE__, __FILE__, "StackPush()"); \
StackPush(&stack, X);

#define StackPop_(stack) \
GetActionInfo(&stack, __LINE__, __FILE__, "StackPop()"); \
StackPop(&stack);

#ifdef DEBUG_INFO
    #define VAR_INFO , int line, const char* file, const char *name
#else
    #define VAR_INFO
#endif



void GetActionInfo(struct Stack *stack VAR_INFO);
void DecodeProblem(struct Stack *stack, int problem_code);
void StackCtor_ (struct Stack *stack, int number VAR_INFO);
int  StackPush  (struct Stack *stack, int number); 
int  StackPop   (Stack *stack);
void StackDump  (struct Stack *stack, int problem_code);
void StackDtor  (struct Stack *stack);
int  StackVerify(struct Stack *stack);
void StackPrint (struct Stack *stack);
#endif //STACK_H