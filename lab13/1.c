#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue
{
    int *key;
    int first;
    int rear;
    int qsize;
    int max_queue_size;
} Queue;

typedef struct _Graph
{
    int size;
    int *node;
    int **matrix;
} Graph;

Graph *CreateGraph(int *nodes, int n);
void InsertEdge(Graph *G, int a, int b);
void PrintGraph(Graph *G);
void DeleteGraph(Graph *G);
void Topsort(Graph *G);
Queue *MakeNewQueue(int X);
int IsEmpty(Queue *Q);
int Dequeue(Queue *Q);
void Enqueue(Queue *Q, int X);
void DeleteQueue(Queue *Q);
void countInput(int *n, char *str);
void parseInput(int *arr, char *str, int n);

int main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char numbers[100], inputs[150];
    fgets(numbers, 100, fin);
    int n = 0;
    countInput(&n, numbers);
    int *nodes = (int *)malloc(sizeof(int) * n);
    parseInput(nodes, numbers, n);

    Graph *G = CreateGraph(nodes, n);

    fgets(inputs, 100, fin);
    int len = strlen(inputs), i, j;
    for (i = 0; i < len; i += 4)
    {
        int a = inputs[i] - '0', b = inputs[i + 2] - '0';
        InsertEdge(G, a, b);
    }
    PrintGraph(G);
    Topsort(G);
    DeleteGraph(G);

    fclose(fin);
    fclose(fout);
    return 0;
}

void countInput(int *n, char *str)
{
    int len = strlen(str), i;
    for (i = 0; i < len; i++)
    {
        if (0 <= str[i] - '0' && str[i] - '0' < 10)
        {
            (*n)++;
        }
    }
}
void parseInput(int *arr, char *str, int n)
{
    int len = strlen(str), i;
    int cnt = 0;
    for (i = 0; i < len; i++)
    {
        if (0 <= str[i] - '0' && str[i] - '0' < 10)
        {
            arr[cnt++] = str[i] - '0';
        }
    }
}
Graph *CreateGraph(int *nodes, int n)
{
    // sorting nodes
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (nodes[i] > nodes[j])
            {
                int temp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp;
            }
        }
    }
    // allocating space and initializing the nodes
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->size = n;
    g->node = nodes;
    g->matrix = (int **)malloc(sizeof(int *) * n);
    // initializng the matriz
    for (int i = 0; i < n; i++)
    {
        g->matrix[i] = malloc(sizeof(int) * n);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            g->matrix[i][j] = 0;
        }
    }
    return g;
}
void InsertEdge(Graph *G, int a, int b)
{
    // initializing the position in the matriz with 0 or 1
    int x = 0;
    int y = 0;

    while (G->node[x] != a)
    {
        x++;
    }
    while (G->node[y] != b)
    {
        y++;
    }
    G->matrix[x][y] = 1;
}
void PrintGraph(Graph *G)
{
    // printing up part
    fprintf(fout, "   ");
    for (int i = 0; i < G->size; i++)
    {
        fprintf(fout, "%d  ", G->node[i]);
    }
    fprintf(fout, "\n");
    for (int i = 0; i < G->size; i++)
    {
        // printing left part
        fprintf(fout, "%d  ", G->node[i]);
        // printing matrix
        for (int j = 0; j < G->size; j++)
        {
            fprintf(fout, "%d  ", G->matrix[i][j]);
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}
void DeleteGraph(Graph *G)
{
    free(G->node);
    free(G->matrix);
    free(G);
}
void Topsort(Graph *G)
{
    Queue *Q = MakeNewQueue(G->size);
    int *Indegree = malloc(sizeof(int) * G->size);
    // checking if the value in the matrix is 1 or 0, and putting the count in a new arr
    for (int i = 0; i < G->size; i++)
    {
        int n = 0;
        for (int j = 0; j < G->size; j++)
        {
            if (G->matrix[j][i] == 1)
            {
                n++;
            }
        }
        Indegree[i] = n;
    }
    // if the value in a x position in the new array is 0 we enqueue
    for (int k = 0; k < G->size; k++)
    {
        if (Indegree[k] == 0)
        {
            Enqueue(Q, k);
        }
    }
    // we first check that the queue is not empty and then we insert a new value at the end and then print it
    while (!IsEmpty(Q))
    {
        int a = Dequeue(Q);
        fprintf(fout, "%d ", G->node[a]);
        for (int b = 0; b < G->size; b++)
        {
            if (G->matrix[a][b] == 1)
            {
                Indegree[b]--;
                if (Indegree[b] == 0)
                {
                    Enqueue(Q, b);
                }
            }
        }
    }
    fprintf(fout, "\n");
}
Queue *MakeNewQueue(int X)
{
    // initialize and allocate for the new queue
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->max_queue_size = X;
    q->qsize = 0;
    q->key = malloc(sizeof(int) * X);
    q->first = 0;
    q->rear = -1;
    return q;
}
int IsEmpty(Queue *Q)
{
    if (Q->rear < Q->first)
    {
        return 1;
    }
    return 0;
}
// inserting a new element at the end of the queue
int Dequeue(Queue *Q)
{
    if (IsEmpty(Q))
    {
        return -1;
    }
    int deq = Q->key[Q->first % Q->max_queue_size];
    Q->first++;
    Q->qsize--;
    return deq;
}
// poping the element at the front of the queue
void Enqueue(Queue *Q, int X)
{
    if (Q->qsize == Q->max_queue_size)
    {
        return;
    }
    Q->key[(++Q->rear) % Q->max_queue_size] = X;
    Q->qsize++;
}
void DeleteQueue(Queue *Q)
{
    free(Q->key);
    free(Q);
}
