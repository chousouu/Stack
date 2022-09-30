#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_INFO
#define CANARY_PROT

enum Options 
{
    CAP_MULTIPLIER = 2,
    STACK_DECREASE = 1, 
    STACK_INCREASE = 0,
};

enum ERRORS
{
    POP_EMPTY_STACK    = 1,
    NEGATIVE_SIZE      = 2,
    NEGATIVE_CAPACITY  = 4,
    CAP_SMALLER_SIZE   = 8,
    STACK_NULL         = 16,
    MEM_ALLOC_FAIL     = 32,
    S_LEFT_CANARY_DEAD = 64,
    S_RIGHT_CANARY_DEAD = 128,

};

enum HexConst 
{
    POISON             = 0x91DF00 ,
    STACK_LEFT_CANARY  = 0x5BE4   ,
    STACK_RIGHT_CANARY = 0x5AF1E91,
    DATA_LEFT_CANARY   = 0xDBE4   , 
    DATA_RIGHT_CANARY  = 0xDAF1E91, 
};
  
 
struct Stack
{
    #ifdef CANARY_PROT
        int L_canary;
    #endif //CANARY_PROT
    int *data;
    #ifdef CANARY_PROT
        int R_canary;
    #endif //CANARY_PROT
    int capacity;
    int size;
    bool capmode;

    #ifdef DEBUG_INFO
    int          line_ctor;
    const char  *file_ctor;
    int          line_action;
    const char  *file_action;
    const char  *action_funcname;
    const char  *stackname;
    #endif //DEBUG_INFO
};


#define IF_ERR(ERROR, ERRCODE) if(ERROR) problem_code |= ERRCODE 

#ifdef DEBUG_INFO
    #define VAR_INFO , int line, const char* file, const char *name
    
    #define Stack_OK(stack) \
    err_code = StackVerify(stack); \
    StackDump(stack, err_code); \
    if(err_code) return err_code

    #define StackCtor(stack, X) StackCtor_(&stack, X, __LINE__, __FILE__, #stack)
    
    #define StackPush_(stack, X) \
    GetActionInfo(&stack, __LINE__, __FILE__, "StackPush()"); \
    StackPush(&stack, X)    

    #define StackPop_(stack, err) \
    GetActionInfo(&stack, __LINE__, __FILE__, "StackPop()"); \
    StackPop(&stack, &err)
#else 
    #define VAR_INFO 
    #define Stack_OK(stack) 
    #define StackCtor(stack, X)   StackCtor_(&stack, X)
    #define StackPush_(stack, X)  StackPush(&stack, X) 
    #define StackPop_(stack, err) StackPop(&stack, &err)    
#endif



void StackCtor_ (struct Stack *stack, int number VAR_INFO);
void GetActionInfo(struct Stack *stack VAR_INFO);
void DecodeProblem(struct Stack *stack, int problem_code);
int  StackPush  (struct Stack *stack, int number); 
int StackPop(Stack *stack, int *err_code);
void StackDump  (struct Stack *stack, int problem_code);
void StackDtor  (struct Stack *stack);
int  StackVerify(struct Stack *stack);
void StackPrint (struct Stack *stack);
#endif //STACK_H