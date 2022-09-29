#include "stack.h"
static int *PoisonStack(int start, int max)
{
    int* allocate =(int*)calloc(max, sizeof(int));

    for(int i = start; i < max; i++)
    {
        allocate[i] = POISON;
    }

    return allocate;
}

static void StackResize(struct Stack *stack, bool mode)
{
    // TODO: run verificator
    if(mode == STACK_DECREASE)
    {   
        stack->capacity /= CAP_MULTIPLIER;    
    }
    else 
    {
        stack->capacity *= CAP_MULTIPLIER;
    }  

    stack->data = (int*)realloc(stack->data, sizeof(int) * stack->capacity); // FILL WITH POISON?

    if(stack->data == NULL)
    {
        printf("Cannot allocate memory\n");
        return; 
    }

    for(int i = stack->size; i < stack->capacity; i++)
    {
        stack->data[i] = POISON; 
    }
}

void StackCtor_(struct Stack *stack, int capacity VAR_INFO)
{
    *stack = 
    {
        .data = (int*)PoisonStack(0, capacity),// TODO: poison
        capacity,
        .size = 0,
        .capmode = STACK_INCREASE,

        #ifdef DEBUG_INFO
        line_ctor,
        file_ctor,
        .line_push = 0,
        .file_push = NULL, 
        stack_name
        #endif//DEBUG_INFO
    };
}

void StackPush(struct Stack *stack, int number)
{
    #ifdef DEBUG_INFO
        stack->line_push = __LINE__; // remove
        stack->file_push = __FILE__;
        Stack_OK(stack) // StackDump (__LINE__, __FILE__, stack, number);

        // int err = Verify
        // StackDump (err)
        // if (err)
        // {
        //      return jopa;
        // }

    #endif// DEBUG_INFO

    if(stack->size == stack->capacity - 1)
    {
        StackResize(stack, STACK_INCREASE);
    }

    stack->data[stack->size] = number;
    stack->size++; 

    return; // 0;
}

// StackTop. Does not remove top element

int StackPop(Stack *stack) //, int *ErrCode) //*****************************
{
    #ifdef DEBUG_INFO
        Stack_OK(stack)
    #endif DEBUG_INFO
    
    if(stack->size == (stack->capacity / CAP_MULTIPLIER))
    {
        stack->capmode = STACK_DECREASE;
    }
    
    if(stack->size == (stack->capacity / CAP_MULTIPLIER) / 2 - 1 && stack->capmode == STACK_DECREASE)
    {
        StackResize(stack, stack->capmode);
        stack->capmode = STACK_INCREASE;
    }

    stack->size--; 
    int temp = stack->data[stack->size];
    stack->data[stack->size] = POISON;
    return temp;
}



int StackVerify(struct Stack *stack) //*********************************//
{
    int problem_code = 0;

    if(stack == NULL)
    {
        problem_code |= STACK_NULL;   
        
        return problem_code; 
    }
    else
    {
        if(stack->size < 0)
        {
            problem_code |= NEGATIVE_SIZE;
        }
        if(stack->capacity < 0)
        {
            problem_code |= NEGATIVE_CAPACITY;
        }
        if(stack->capacity < stack->size)
        {
            problem_code |= CAP_SMALLER_SIZE;
        }
        if(stack->size == 0 && stack->data[0] == POISON) // будет вызываться всегда для первого элемента
        {
            problem_code |= POP_EMPTY_STACK;
        }
    }

    return problem_code;
}

void DecodeProblem(FILE *logfile, struct Stack *stack, int problem_code) //*****************************//
{
    if(problem_code &= STACK_NULL)
    {
        fprintf(logfile, "STACK IS NULL\n");
    }
    if(problem_code &= NEGATIVE_SIZE)
    {
        fprintf(logfile, "Stack size cannot be negative. [stack.size = %d]\n", stack->size);
    }
    if(problem_code &= NEGATIVE_CAPACITY)
    {
        fprintf(logfile, "Stack capacity cannot be negative. [stack.capacity = %d]\n", stack->capacity);
    }
    if(problem_code &= CAP_SMALLER_SIZE)
    {
        fprintf(logfile, "Stack capacity cannot be smaller than size.\n   [stack.capacity = %d]\n   [stack.size = %d]\n",
                                                                            stack->capacity      ,    stack->size);
    }

}

void StackDump(struct Stack *stack, int problem_code) // ***********//
{
    FILE *logfile = fopen("log.txt", "a");
    
    if(logfile == NULL)
    {
        printf("Cannot open file\n");
    }
    else 
    {
        fprintf(logfile, "=====================\n");
        fprintf(logfile, "StackPush() at %s (%d):\n"                "Stack[%p](%s) \"%s\" at main()\n", 
                        stack->file_push, stack->line_push,          stack, (problem_code ? "ERROR" : "ok"), stack->stackname);
       // if(problem_code)
        {
            DecodeProblem(logfile, stack, problem_code); 

            fprintf(logfile, "\t{\n");
                StackPrint(stack, logfile);
            fprintf(logfile, "\t}\n");

            fprintf(logfile, "%s(%d)\n", stack->file_ctor, stack->line_ctor);

            fprintf(logfile, "{\n");

            fprintf(logfile, "\tsize = %d\n\tcapacity = %d\n\tdata[%p]\n", stack->size, stack->capacity, stack->data);
            
            fprintf(logfile, "}\n");
            
            fclose(logfile);
        }
    }

}

void StackPrint(struct Stack *stack, FILE *logfile)
{
    for(int i = 0; i < stack->capacity; i++)
    {
        char        in_stack    = ' ';
        const char *isPOISONED  = "(POISON)";
        const char *notPOISONED = " "; 
        const char *situation   = isPOISONED;

        if (stack->data[i] != POISON)
        {
            in_stack = '*';
            situation = notPOISONED;
        }

        fprintf(logfile, "\t\t%c[%d] = %d%s\n", in_stack, i, stack->data[i], situation);
     }    
}



void StackDtor(struct Stack *stack)
{
    for(int i = 0; i < stack->capacity; i++)
    {
       stack->data[i] = POISON; 
    }

    stack->size = 0;
    stack->capacity = 0;
    free (stack->data);
} 