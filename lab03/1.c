#include <stdio.h>
#include <stdlib.h> //for malloc
 
FILE *input;
FILE *output;
 
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;
 
struct Node
{
    ElementType element;
    Position next;
};
 
List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
void PrintList(List L);
void Delete(ElementType X, List L);
Position Find(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void DeleteList(List L);
 
int main(int argc, char *argv[])
{
    char command;
    int key1, key2;
    FILE *input;
    Position header = NULL;
    Position tmp = NULL;
    if (argc == 1)
    {
        input = fopen("input.txt", "r");
        output = fopen("output.txt", "w");
    }
    else
    {
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
    }
    header = MakeEmpty(header);
    while (1)
    {
        command = fgetc(input);
        if (feof(input)) break;
        switch (command)
        {
            {
            case 'i':
                fscanf( input, "%d   %d", &key1, &key2);
                // duplication  check
                tmp = Find(key1, header);
                if (tmp != NULL)
                {
                    fprintf(output, "insertion(%d) failed : the key already exists in the list\n", key1);
                    break;
                }
                tmp = Find(key2, header);
                if (tmp == NULL)
                {
                    fprintf(output, "insertion(%d) failed : can not find the location to be inserted\n", key1);
                    break;
                }
                Insert(key1, header, tmp);
                break;
            case 'd':
                fscanf(input, "%d", &key1);
                Delete(key1, header);
                break;
            case 'f':
                fscanf(input, "%d", &key1);
                tmp = FindPrevious(key1, header);
                if (IsLast(tmp, header))
                    fprintf(output, "finding(%d) failed: element %d is not in the list\n", key1, key1);
                else
                {
                    if (tmp->element > 0)
                        fprintf(output, "key of the previous node of %d is %d \n", key1, tmp->element);
                    else
                        fprintf(output, "key of the previous node of %d is head\n", key1);
                }
                break;
            case 'p':
                PrintList(header);
                break;
            default:
                break;
            }
        }
    }
    DeleteList(header);
    fclose(input);
    fclose(output);
   
    return 0;
}
 
List MakeEmpty(List L)
{
    if (L != NULL)//Use deleteList to empty the whole list
    {
        DeleteList(L);
    }
    //give new space to the list
    L = (List)malloc(sizeof(struct Node));
    L->next = NULL;
    return L;
}
int IsEmpty(List L)
{
    //return the number as NULL
    return L->next == NULL;
}
int IsLast(Position P, List L)
{
    //Help to check if the number that is being search, is in the list
    return P->next == NULL;
}
void Insert(ElementType X, List L, Position P)
{
    Position tmp;
    tmp = (Position)malloc(sizeof(struct Node));
    //Changing the positions of the list
    tmp->element = X;
    tmp->next = P->next;
    P->next = tmp;
}
void PrintList(List L)
{
    Position p;
    p = L->next;
    while (p != NULL)
    {
        fprintf(output, "key: %d ", p->element);
        p = p->next;
    }
    fprintf(output, "\n");
}
void Delete(ElementType X, List L)
{
    Position p, tmp;
    p = FindPrevious(X, L);
    if (!IsLast(p, L))
    {
        tmp = p->next;
        p->next = tmp->next;
        free(tmp);
    }
    else if (IsLast(p, L))//Return NULL if the number does not exist
    {
        fprintf(output, "deletion(%d) failed: element %d is not in the list \n", X, X);
    }
}
 
Position Find(ElementType X, List L)
{
    Position p;
    //if is -1, put it in the position that is send automatically(Header)
    if (X == -1)
    {
        p = L;
    }
    else
    {
        p = L->next;
        while (p != NULL && p->element != X)
        {
            p = p->next;
        }
    }
    return p;
}

Position FindPrevious(ElementType X, List L)
{
    Position p;
    p = L;
    while (p->next != NULL && p->next->element != X)
    {
        p = p->next;
    }
    return p;
}

void DeleteList(List L)
{
    Position p, tmp;
 
    p = L->next;
    L->next = NULL;
    while (p != NULL)
    {
    	//replacing each place for NULL
        tmp = p->next;
        free(p);
        p = tmp;
    }
}


