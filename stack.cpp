#include "stack.h"
static FILE *logfile = NULL;
int err_code = 0;

static long unsigned HashDataCounter(void *data, int len)
{
    char *datawalker = (char*)data;
    int hash_counter = 0;
    
    while (datawalker - (char*)data < len)
    {
        hash_counter += *datawalker++;
    }
    return hash_counter;
}

static int *PoisonStack(int start, int max)
{
    int* allocate =(int*)calloc(max + 1, sizeof(int));
    *allocate++ = DATA_LEFT_CANARY;
    
    allocate[max] = DATA_RIGHT_CANARY;

    for(int i = start; i < max; i++)
    {
        allocate[i] = POISON;
    }

    return allocate;
}

static int StackResize(struct Stack *stack, bool mode)
{    
    printf("ENTERED STACKRESIZE\n");
    if(mode == STACK_DECREASE)
    {   
        stack->capacity /= CAP_MULTIPLIER;    
    }
    else 
    {
        stack->capacity *= CAP_MULTIPLIER;
    }  
    printf("creating temp\n");
    int *temp = (int*)realloc(stack->data, sizeof(int) * stack->capacity); 
    printf("temp %p \n", temp);
    if(temp == NULL)
    {
        printf("Cannot allocate memory\n");
        return MEM_ALLOC_FAIL; 
    }

    stack->data = temp;
    printf("filling with poison\n");
    for(int i = stack->size; i < stack->capacity; i++)
    {
        stack->data[i] = POISON; 
    }
    return 0; // on success
}

void StackCtor_(struct Stack *stack, int capacity VAR_INFO)
{
    *stack = 
    {
        #ifdef CANARY_PROT
        .L_canary = STACK_LEFT_CANARY,
        #endif //CANARY_PROT

        .data = (int*)PoisonStack(0, capacity),
        
        .capacity = capacity,
        .size = 0,

        #ifdef CANARY_PROT
        .R_canary = STACK_RIGHT_CANARY,
        #endif //CANARY_PROT        
        
        #ifdef HASH_PROT
        .HashValueData = 0,
        #endif //HASH_PROT 

        #ifdef DEBUG_INFO
        .line_ctor = line,
        .file_ctor = file,
        .line_action = 0,
        .file_action = NULL, 
        .action_funcname = NULL,
        .stackname = name,
        #endif //DEBUG_INFO
    };

    stack->HashValueData = HashDataCounter(stack->data, stack->capacity * sizeof(int));
}

int StackPush(struct Stack *stack, int number) 
{
    Stack_OK(stack);
    printf("%d == %d\n",stack->size, stack->capacity - 1);
    if(stack->size == stack->capacity - 1)
    {
        printf("size = cap - 1\n");
        int err_resize = StackResize(stack, STACK_INCREASE);
        printf("err resize %d\n", err_resize);
        if(err_resize == MEM_ALLOC_FAIL)
        { 
            StackDump(stack, err_resize);
            return err_resize;
        }

        #ifdef CANARY_PROT
            *(stack->data - 1) = DATA_LEFT_CANARY;
            stack->data[stack->capacity] = DATA_RIGHT_CANARY;
        #endif// CANARY_PROT
    }

    stack->data[stack->size] = number;
    stack->size++;

    stack->HashValueData = HashDataCounter(stack->data, stack->capacity * sizeof(int));
    Stack_OK(stack);
    return 1; 
}

// StackTop. Does not remove top element/////

int StackPop(Stack *stack, int *error_code)
{
    printf("%d %d\n", stack->size, stack->data[stack->size]);
    Stack_OK(stack);
    if(stack->size < 1 && (stack->data[stack->size] == POISON))
    {
        printf("ENTERED\n");
        *error_code = POP_EMPTY_STACK;
        StackDump(stack, *error_code);
        return *error_code;
    }

    if(stack->size == (stack->capacity / 4 - 1))
    {
        int err_resize = StackResize(stack, STACK_DECREASE);
        if(err_resize  == MEM_ALLOC_FAIL)
        {
            *error_code = err_resize; 
            StackDump(stack, *error_code);
            return *error_code;
        }
        #ifdef CANARY_PROT
            *(stack->data - 1) = DATA_LEFT_CANARY;
            stack->data[stack->capacity] = DATA_RIGHT_CANARY;
        #endif//CANARY_PROT
    }

    stack->size--; 
    int temp = stack->data[stack->size];
    stack->data[stack->size] = POISON;

    stack->HashValueData = HashDataCounter(stack->data, stack->capacity * sizeof(int));

    Stack_OK(stack);
        
    return temp;
}

#ifdef DEBUG_INFO 
void GetActionInfo(struct Stack *stack VAR_INFO)
{
    stack->line_action = line; 
    stack->file_action = file;
    stack->action_funcname = name;
}
#endif// DEBUG_INFO

int StackVerify(struct Stack *stack) 
{
    int problem_code = 0;

    if(stack == NULL)
    {
        problem_code |= STACK_NULL;   
        
        return problem_code; 
    }
    else
    {
        IF_ERR(stack->size < 0, NEGATIVE_SIZE);
        
        IF_ERR(stack->capacity < 0, NEGATIVE_CAPACITY);

        IF_ERR(stack->capacity < stack->size, CAP_SMALLER_SIZE);
        
        IF_ERR(stack->data == NULL, MEM_ALLOC_FAIL);

        IF_ERR(stack->L_canary != STACK_LEFT_CANARY, S_LEFT_CANARY_DEAD);

        IF_ERR(stack->R_canary != STACK_RIGHT_CANARY, S_RIGHT_CANARY_DEAD);

        IF_ERR(*(stack->data - 1) != DATA_LEFT_CANARY, D_LEFT_CANARY_DEAD);

        IF_ERR(stack->data[stack->capacity] != DATA_RIGHT_CANARY, D_RIGHT_CANARY_DEAD);

        IF_ERR(stack->HashValueData != HashDataCounter(stack->data, stack->capacity * sizeof(int)), HASH_DEAD);
    }
//
    return problem_code;
}

void DecodeProblem(struct Stack *stack, int problem_code)  //Maybe macros? find best way for errors with %
{
    if(problem_code & STACK_NULL)
    {
        fprintf(logfile, "STACK IS NULL\n");
    }
    if(problem_code & NEGATIVE_SIZE)
    {
        fprintf(logfile, "Stack size cannot be negative. [stack.size = %d]\n", stack->size);
    }
    if(problem_code & NEGATIVE_CAPACITY)
    {
        fprintf(logfile, "Stack capacity cannot be negative. [stack.capacity = %d]\n", stack->capacity);
    }
    if(problem_code & CAP_SMALLER_SIZE)
    {
        fprintf(logfile, "Stack capacity cannot be smaller than size.\n   [stack.capacity = %d]\n   [stack.size = %d]\n",
                                                                            stack->capacity      ,    stack->size);
    }
    if(problem_code & POP_EMPTY_STACK)
    {
        fprintf(logfile, "ERROR : ZERO STACK->SIZE POP.\n"); // xz kak opisat
    }
    if(problem_code & MEM_ALLOC_FAIL)
    {
        fprintf(logfile, "Failed to allocate memory for stack.\n");
    }
    if(problem_code & S_LEFT_CANARY_DEAD)
    {
        fprintf(logfile, "Left canary attacked! (stack struct)\n");
    }
    if(problem_code & S_RIGHT_CANARY_DEAD)
    {
        fprintf(logfile, "Right canary attacked! (stack struct)\n");
    }
    if(problem_code & D_LEFT_CANARY_DEAD)
    {
        fprintf(logfile, "Left canary attacked! (data)\n");
    }
    if(problem_code & D_RIGHT_CANARY_DEAD)
    {
        fprintf(logfile, "Right canary attacked! (data)\n");
    }
    if(problem_code & HASH_DEAD)
    {
        fprintf(logfile, "HASH CHANGED\n");
        // #define FPRINT(...) fprintf(" sdfsdf %d %d", __VA_ARGS__)
    }
}

void StackDump(struct Stack *stack, int problem_code) // ***********// 
{
    logfile = fopen("log.txt", "a");
    
    if(logfile == NULL)
    {
        printf("Cannot open file\n");
    }
    else 
    {
        fprintf(logfile, "=====================\n");
        fprintf(logfile, "%s at %s (%d):\n"                         "Stack[%p](%s) \"%s\" at main()\n", 
  stack->action_funcname ,stack->file_action, stack->line_action,     stack, (problem_code ? "ERROR" : "ok"), stack->stackname);
     //   if(problem_code)
        {
            DecodeProblem(stack, problem_code); 

            fprintf(logfile, "%s(%d)\n", stack->file_ctor, stack->line_ctor);

            fprintf(logfile, "{\n");

            fprintf(logfile, "\tsize = %d\n\tcapacity = %d\n\tdata[%p]\n", stack->size, stack->capacity, stack->data);
            
            fprintf(logfile, "\t{\n");
                StackPrint(stack);
            fprintf(logfile, "\t}\n");
            
            fprintf(logfile, "}\n");
            
        }
            fclose(logfile);
    }

}

void StackPrint(struct Stack *stack)
{
    for(int i = -1; i <= stack->capacity; i++)
    {
        char        in_stack    =   ' ';
        const char *isPOISONED  = "(POISON)";
        const char *notPOISONED = " "; 
        const char *isCANARY    = "(CANARY)";
        const char *situation   = isPOISONED;

        if(*(stack->data + i) == DATA_LEFT_CANARY || *(stack->data + i) == DATA_RIGHT_CANARY)
        {
            in_stack = '#';
            situation = isCANARY;
        }
        else if(stack->data[i] != POISON)
        {
            in_stack = '*';
            situation = notPOISONED;
        }

        fprintf(logfile, "\t\t%c[%d] = %d%s\n", in_stack, i, *(stack->data + i), situation);
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