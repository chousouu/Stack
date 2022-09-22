#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int CAP_MULTIPLIER = 2;

struct Stack
{
    int *stackptr;
    int cappacity;
    int current;
    bool capmode;
};

void Push(struct Stack *stack, int number); 
int Pop(Stack *stack);
void CapChange(struct Stack *stack, bool mode); // 1 decrease ; 0 increase
#endif //STACK_H    