#include "stack.h"

int main()
{
    Stack stack;
    StackCtor(stack, 12);  


    #ifdef DEBUG_INFO
    StackPop_(stack);
    StackPush_(stack, 5);
    StackPop_(stack);
    StackPush_(stack, 5);
    StackPush_(stack, 3);  
    StackPush_(stack, 67);
    StackPop_(stack);
    StackPop_(stack);
    StackPop_(stack);
    StackPop_(stack);

//    StackPop(&stack);
//     StackPush(&stack, 3);
//     StackPush(&stack, 67);
//     StackPush(&stack, 91);
//     StackPush(&stack, 5234);
//     StackPush(&stack, 1);
//     StackPop(&stack);


    StackDtor(&stack); 
    printf("done");
    #endif //DEBUG_INFO
}