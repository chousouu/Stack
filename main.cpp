#include "stack.h"

int main()
{
    int *StackInt = (int*)calloc(10, sizeof(int));

    struct Stack stack = 
    {
        StackInt, 
        1,
        0
    };

    Push(&stack, 5);
    Push(&stack, 3);
    Push(&stack, 67);
    Push(&stack, 91);
    Push(&stack, 5234);
    Push(&stack, 1);
    Push(&stack, 2);



    for(int i = 0; i < stack.current; i++)
    {
        printf("%d ", stack.stackptr[i]);
    }

    printf("\n\n");

    printf("%d %d %d %d %d %d %d", Pop(&stack), Pop(&stack), Pop(&stack), Pop(&stack), Pop(&stack), Pop(&stack), Pop(&stack));


    free(StackInt);
}