#include "stack.h"

void Push(struct Stack *stack, int number)
{
    if(stack->current == stack->cappacity - 1)
    {
        CapChange(stack, 0);
    }

    stack->stackptr[stack->current] = number;
    stack->current++; 
}


int Pop(Stack *stack)
{
    if(stack->current == (stack->cappacity / CAP_MULTIPLIER))
    {
        stack->capmode = 1;
    }
    
    if(stack->current == (stack->cappacity / CAP_MULTIPLIER) / 2 - 1 && stack->capmode == 1)
    {
        CapChange(stack, stack->capmode);
        stack->capmode = 0;
    }

    int temp = stack->stackptr[stack->current - 1];
    stack->current--; 
    return temp;
}

void CapChange(struct Stack *stack, bool mode)
{
    if(mode)
    {   
        stack->cappacity /= CAP_MULTIPLIER;
        stack->stackptr = (int*)realloc(stack->stackptr, stack->cappacity);
    }
    else 
    {
        stack->cappacity *= CAP_MULTIPLIER;
        stack->stackptr = (int*)realloc(stack->stackptr, stack->cappacity);
    }
}