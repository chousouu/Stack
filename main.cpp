#include "stack.h"

int main()
{
    Stack stack;
    StackCtor(stack, 12);  

    StackPush(&stack, 5);
    StackPush(&stack, 3);
    StackPush(&stack, 67);
    StackPush(&stack, 91);
    StackPush(&stack, 5234);
    StackPush(&stack, 1);
    StackPush(&stack, 5);
    StackPush(&stack, 3);
    StackPush(&stack, 67);
    StackPush(&stack, 91);
    StackPop(&stack);
    StackPush(&stack, 5234);
    StackPush(&stack, 1);
    StackPop(&stack);
    StackPush(&stack, 5);
    StackPush(&stack, 3);
    StackPush(&stack, 67);
    StackPush(&stack, 91);
    StackPush(&stack, 5234);
    StackPush(&stack, 1);
    StackPop(&stack);

    // StackDtor(&stack); ломает все??????????????
}