#include "stack.h"

int main()
{
    Stack stack;
    StackCtor_(stack, 12);  

    int err_code = 0;
    StackTop_(stack, err_code);
    StackPop_(stack, err_code);
    StackPush_(stack, 0);
    StackTop_(stack, err_code);
    StackPop_(stack, err_code);
    StackPush_(stack, 1);
    StackPush_(stack, 2);
    StackPush_(stack, 3);
    StackPush_(stack, 4);
    StackPush_(stack, 5);
    StackPush_(stack, 6);
    StackPush_(stack, 7);
    StackPush_(stack, 8);
    StackPush_(stack, 9);
    StackPush_(stack, 10);
    StackPush_(stack, 11);
    StackPush_(stack, 12);
    StackPush_(stack, 13);
    for(int i = 0; i <= 13; i++)
    {
        StackPop_(stack, err_code);
    }
    
    StackDtor(&stack); 
    printf("done\n");
}