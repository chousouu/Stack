#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_INFO
#define CANARY_PROT
#define HASH_PROT

enum Options 
{
    CAP_MULTIPLIER = 2,
    STACK_DECREASE = 1, 
    STACK_INCREASE = 0,
};

enum ERRORS
{
    POP_EMPTY_STACK     = 1 << 0,
    NEGATIVE_SIZE       = 1 << 1,
    NEGATIVE_CAPACITY   = 1 << 2,
    CAP_SMALLER_SIZE    = 1 << 3 ,
    STACK_NULL          = 1 << 4,
    MEM_ALLOC_FAIL      = 1 << 5,
    S_LEFT_CANARY_DEAD  = 1 << 6,
    S_RIGHT_CANARY_DEAD = 1 << 7,
    D_LEFT_CANARY_DEAD  = 1 << 8,
    D_RIGHT_CANARY_DEAD = 1 << 9,
    HASH_DEAD           = 1 << 10,
};

enum HexConst 
{
    POISON             = 0x91DF00 ,
    STACK_LEFT_CANARY  = 0x5BE4   ,
    STACK_RIGHT_CANARY = 0x5AF1E91,
    DATA_LEFT_CANARY   = 0xDBE4   , 
    DATA_RIGHT_CANARY  = 0xDAF1E91, 
};

#ifdef CANARY_PROT
    #define  ON_CANARY_PROT(SIDE) int SIDE##_canary;
#else 
    #define ON_CANARY_PROT(SIDE) 
#endif //CANARY_PROT

#ifdef HASH_PROT
    #define  ON_HASH_PROT \
    unsigned long HashValueData;
    //unsigned long HashValueStruct;
#else 
    #define ON_HASH_PROT 
#endif //HASH_PROT

 
struct Stack
{
    ON_CANARY_PROT(L)

    int *data;
    int capacity;
    int size;
    
    ON_CANARY_PROT(R)

    ON_HASH_PROT

    #ifdef DEBUG_INFO
    int          line_ctor;
    const char  *file_ctor;
    int          line_action;
    const char  *file_action;
    const char  *action_funcname;
    const char  *stackname;
    #endif //DEBUG_INFO
};


#define IF_ERR(ERROR, ERRCODE) do { if((ERROR)) problem_code |= (ERRCODE); } while(0); 

#define FILL_POISON(arr, start, end)     \
        for(int i = start; i < end; i++) \
        {                                \
            arr[i] = POISON;             \
        }                                

#ifdef HASH_PROT
    #define CHECK_HASH stack->HashValueData = HashDataCounter(stack->data, stack->capacity * sizeof(int))
#else
    #define CHECK_HASH 
#endif

#ifdef DEBUG_INFO
    #define VAR_INFO , int line, const char* file, const char *name
    
    #define Stack_OK(stack) \
    err_code = StackVerify(stack); \
    StackDump(stack, err_code); \
    if(err_code) return err_code;

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