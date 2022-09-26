#include "stack.h"

void StackCtor_(struct Stack *stack, int number, int line_ctor, const char* file_ctor, const char* func_name, const char *stack_name)
{
    *stack = 
    {
        (int*)calloc(number, sizeof(int)),
        number,
        0,
        STACK_INCREASE,

        #ifdef DEBUG_INFO
        line_ctor,
        file_ctor,
        0,
        NULL, 
        stack_name
        #endif//DEBUG_INFO
    };
}

void StackPush(struct Stack *stack, int number)
{
    #ifdef DEBUG_INFO
        stack->line_push = __LINE__;
        stack->file_push = __FILE__;
        Stack_OK(stack, number)
    #endif// DEBUG_INFO

    if(stack->size == stack->cappacity - 1)
    {
        StackResize(stack, STACK_INCREASE);
    }

    stack->data[stack->size] = number;
    stack->size++; 
}

int StackPop(Stack *stack)
{
    if(stack->size == (stack->cappacity / CAP_MULTIPLIER))
    {
        stack->capmode = STACK_DECREASE;
    }
    
    if(stack->size == (stack->cappacity / CAP_MULTIPLIER) / 2 - 1 && stack->capmode == STACK_DECREASE)
    {
        StackResize(stack, stack->capmode);
        stack->capmode = STACK_INCREASE;
    }

    stack->size--; 
    int temp = stack->data[stack->size];
    stack->data[stack->size] = POISON;
    return temp;
}

void StackResize(struct Stack *stack, bool mode)
{
    if(mode == STACK_DECREASE)
    {   
        stack->cappacity /= CAP_MULTIPLIER;
        stack->data = (int*)realloc(stack->data, sizeof(int) * stack->cappacity);
    }
    else 
    {
        stack->cappacity *= CAP_MULTIPLIER;
        stack->data = (int*)realloc(stack->data, stack->cappacity);
    }
}

int StackVerify(struct Stack *stack) //*********************************//
{
    int problem_code = 0;

    if(stack == NULL)
    {
        problem_code |= STACK_NULL;   
        
        return problem_code; 
    }
    {
        if(stack->size < 0)
        {
            problem_code |= NEGATIVE_SIZE;
        }
        if(stack->cappacity < 0)
        {
            problem_code |= NEGATIVE_CAPPACITY;
        }
        if(stack->cappacity < stack->size)
        {
            problem_code |= CAP_SMALLER_SIZE;
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
    if(problem_code &= NEGATIVE_CAPPACITY)
    {
        fprintf(logfile, "Stack cappacity cannot be negative. [stack.cappacity = %d]\n", stack->cappacity);
    }
    if(problem_code &= CAP_SMALLER_SIZE)
    {
        fprintf(logfile, "Stack cappacity cannot be smaller than size.\n   [stack.cappacity = %d]\n   [stack.size = %d]\n",
                                                                            stack->cappacity      ,    stack->size);
    }

}

void StackDump(struct Stack *stack, int problem_code) // ***********//
{
    FILE *logfile = fopen("log.txt", "a");

    fprintf(logfile, "=====================\n");
    fprintf(logfile, "StackPush() at %s (%d):\n"                "Stack[%p](%s) \"%s\" at main()\n", 
                    stack->file_push, stack->line_push,          stack, (problem_code ? "ERROR" : "ok"), stack->stackname);
    if(problem_code)
    {
        DecodeProblem(logfile, stack, problem_code);   
    }
    else 
    {
        fprintf(logfile, "%s(%d)\n", stack->file_ctor, stack->line_ctor);

        fprintf(logfile, "{\n");

        fprintf(logfile, "\tsize = %d\n\tcappacity = %d\n\tdata[%p]\n", stack->size, stack->cappacity, stack->data);
        
        fprintf(logfile, "\t{\n");
            StackPrint(stack, logfile);
        fprintf(logfile, "\t}\n");

        fprintf(logfile, "}\n");
    }
}

void StackPrint(struct Stack *stack, FILE *logfile)
{
    for(int i = 0; i < stack->cappacity; i++)
    {
        char in_stack = ' ';
        char isPOISONED[9] = "(POISON)";
        char notPOISONED[2] = " "; 
        char *situation = isPOISONED;

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
    for(int i = 0; i < stack->cappacity; i++)
    {
       stack->data[i] = POISON; 
    }

    stack->size = 0;
    stack->cappacity = 0;
} 