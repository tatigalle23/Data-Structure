#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack
{
    int *key;
    int top;
    int max_stack_size;
} Stack;

Stack *CreateStack(int max);
void Push(Stack *S, int X);
int Pop(Stack *S);
int Top(Stack *S);
void DeleteStack(Stack *S);
int IsEmpty(Stack *S);
int IsFull(Stack *S);

void main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Stack *stack;
    char input_str[101];
    int max = 20, i = 0, a, b, result, error_flag = 0;

    fgets(input_str, 101, fin);
    stack = CreateStack(max);

    fprintf(fout, "top numbers: ");

    while (input_str[i] != '#')
    {
        // check if is a number
        if (isdigit(input_str[i]))
        {
            // First check the stack status to change the error flag
            if (IsFull(stack))
            {
                error_flag = ISFULL;
            }
            else if (IsEmpty(stack))
            {
                error_flag = ISEMPTY;
            }
            else
            {
                error_flag = 0;
            }
            //-'0' to change it to int
            Push(stack, input_str[i] - '0');
        }
        // check if is a operator
        else
        {
            // since we evaluate from left to right b pop first
            b = Pop(stack);
            a = Pop(stack);

            switch (input_str[i])
            {
            case '*':
                result = a * b;
                break;
            case '+':
                result = a + b;
                break;
            case '-':
                result = a - b;
                break;
            case '/':
                // check if the division is by 0 to change the error flag
                if (b == 0)
                {
                    error_flag = DIVIDEZERO;
                }
                else
                {
                    result = a / b;
                }
                break;
            case '%':
                // check if the division is by 0 to change the error flag
                if (b == 0)
                {
                    error_flag = DIVIDEZERO;
                }
                else
                {
                    result = a % b;
                }
                break;
            }
            // pushing the result into the stack
            Push(stack, result);
        }
        int t = Top(stack);
        fprintf(fout, "%d ", t);
        result = t;
        i++;
    }
    if (error_flag == ISFULL)
    {
        fprintf(fout, "\nerror: invalid postfix expression, stack is full!\n");
    }
    else if (error_flag == ISEMPTY)
    {
        fprintf(fout, "\nerror: invalid postfix expression, stack is empty!\n");
    }
    else if (error_flag == DIVIDEZERO)
    {
        fprintf(fout, "\nerror: invalid postix expression, divide by zero!\n");
    }
    else
    {
        if (stack->top + 1 > 1)
        {
            fprintf(fout, "\nerror: invalid postfix expression, %d elements are left\n", stack->top + 1);
        }
        else
        {
            fprintf(fout, "\nevaluation result: %d\n", result);
        }
    }
    fclose(fin);
    fclose(fout);
    DeleteStack(stack);
}

Stack *CreateStack(int max)
{
    Stack *s;
    s = malloc(sizeof(Stack));
    s->key = malloc(sizeof(int) * max);
    s->max_stack_size = max;
    s->top = -1;
    return s;
}
void Push(Stack *S, int X)
{
    // Check if the stack is full first and then push
    if (!IsFull(S))
    {
        S->key[++(S->top)] = X;
    }
}
int Pop(Stack *S)
{
    // Check is if empty first and the pop
    if (!IsEmpty(S))
    {
        return S->key[S->top--];
    }
}
int Top(Stack *S)
{
    // put at the top for the result
    if (!IsEmpty(S))
    {
        return S->key[S->top];
    }
}
void DeleteStack(Stack *S)
{
    if (S)
    {
        if (S->key)
        {
            free(S->key);
        }
        free(S);
    }
}
int IsEmpty(Stack *S)
{
    if (S->top == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int IsFull(Stack *S)
{
    if (S->top == S->max_stack_size - 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
