#include "stack.h"

int main()
{
    Stack stack;
    StackCtor(stack, 12);  

    int err_code = 0;


    StackPop_(stack, err_code);
    StackPush_(stack, 5);
    StackPop_(stack, err_code);
    StackPush_(stack, 5);
    StackPush_(stack, 3);  
    StackPush_(stack, 67);
    StackPop_(stack, err_code);
    StackPop_(stack, err_code);
    StackPop_(stack, err_code);
    StackPop_(stack, err_code);


    StackDtor(&stack); 
    printf("done");
}