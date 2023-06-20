#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node
{
    int vertex;
    int dist;
    int prev;
} Node;
typedef struct Graph
{
    int size;
    int **vertices;
    Node *nodes;
} Graph;
typedef struct Heap
{
    int Capacity;
    int Size;
    Node *Element;
} Heap;

Graph *createGraph(int size);
void deleteGraph(Graph *g);
void printShortestPath(Graph *g);
Heap *createMinHeap(int heapSize);
void deleteMinHeap(Heap *minHeap);
void insertToMinHeap(Heap *minHeap, int vertex, int distance);
Node deleteMin(Heap *minHeap);

int main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Graph *g;
    int size;
    fscanf(fin, "%d\n", &size);
    g = createGraph(size + 1);

    char tmp = 0;
    while (tmp != '\n' && tmp != EOF)
    {
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->vertices[node1][node2] = weight;
        tmp = fgetc(fin);
    }
    printShortestPath(g);
    deleteGraph(g);

    fclose(fin);
    fclose(fout);
    return 0;
}

Graph *createGraph(int size)
{
    //allocating and initializing the graph
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->size = size;
    g->vertices = (int **)malloc(sizeof(int *) * size);
    g->nodes = (Node *)malloc(sizeof(Node) * size);
    //initializing values of the nodes and allocating for vertices
    for (int i = 0; i < size; i++)
    {
        g->vertices[i] = (int *)malloc(sizeof(int) * size);
        g->nodes[i].vertex = i;
        g->nodes[i].dist = INF;
        g->nodes[i].prev = -1;
    }
    //initializing vertices as infinite
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            g->vertices[i][j] = INF;
        }
    }

    return g;  
}
void deleteGraph(Graph *g)
{
    //deleting vertices and then each value of the graph
    for (int i = 0; i < g->size; i++)
    {
        free(g->vertices[i]);
    }
    free(g->vertices);
    free(g->nodes);
    free(g);
}
void printShortestPath(Graph *g)
{
    //we first initialize the nodes of the graph in the position 1
    Heap *minHeap = createMinHeap(g->size);
    g->nodes[1].dist = 0;    
    g->nodes[1].prev = g->nodes[1].vertex;
    Node * currentNode = malloc(sizeof(Node));
    //distance 1, prev 0
    insertToMinHeap(minHeap, 1, 0);

    int k;
    while (minHeap->Size != 0){        
        Node currentNode = deleteMin(minHeap);
        k = currentNode.vertex;
        for (int i = 1; i < g->size; i++)
        {
            //distance+width<distance of i
            if ((g->nodes[k].dist + g->vertices[k][i]) < g->nodes[i].dist)
            {
                g->nodes[i].dist = g->nodes[k].dist + g->vertices[k][i];
                g->nodes[i].prev = k;
                insertToMinHeap(minHeap, i, g->nodes[i].dist);
            }
        }
    }
    for (int i = 2; i < g->size; i++)
    {
        if (g->nodes[i].dist == INF)
            fprintf(fout, "can not reach to node %d\n", g->nodes[i].vertex);
        else if (g->nodes[i].prev != g->nodes[1].vertex) //check where is the first node and then print
        {
            int p = i;
            for(int j=1;j<p;j++){
                fprintf(fout,"%d->",g->nodes[j].vertex);
            }
            fprintf(fout,"%d (cost: %d)\n",g->nodes[i].vertex, g->nodes[i].dist);
            
        }
        else
            fprintf(fout, "%d->%d (cost: %d)\n", g->nodes[i].prev, g->nodes[i].vertex, g->nodes[i].dist);
    }    
}
Heap *createMinHeap(int heapSize)
{
    //allocating and initializing the Heap
    Heap *tmp = malloc(sizeof(Heap));
    tmp->Size = 0;
    tmp->Capacity = heapSize;
    tmp->Element = malloc(sizeof(Node) * heapSize);
    tmp->Element[0].dist = 0;
    return tmp;
}
void deleteMinHeap(Heap *minHeap)
{
    free(minHeap->Element);
    free(minHeap);
}
void insertToMinHeap(Heap *minHeap, int vertex, int distance)
{
    //we insert a new vertex to the heap
    int i;
    //we use i/2 because we need the half of the heap element
    for (i = ++minHeap->Size; minHeap->Element[i / 2].dist > distance; i /= 2)
    {
        minHeap->Element[i] = minHeap->Element[i / 2];
    }
    //and then we initialize the distance and vertex from the heap, with the ones we recieve
    minHeap->Element[i].dist = distance;
    minHeap->Element[i].vertex = vertex;
}
Node deleteMin(Heap *minHeap)
{
    int i, child = 0;
    Node min;
    Node last;
    //initialize a node that contains the min element and the last element of the heap
    //so we can change the min element
    min.dist = minHeap->Element[1].dist;
    min.vertex = minHeap->Element[1].vertex;
    min.prev = minHeap->Element[1].prev;
    int size = minHeap->Size--;
    last.dist = minHeap->Element[size].dist;
    last.vertex = minHeap->Element[size].vertex;
    last.prev = minHeap->Element[size].prev;
    for (i = 1; i * 2 < minHeap->Size; i = child)
    {
        //we here compare left with right child
        child = i * 2;
        if (child != minHeap->Size && minHeap->Element[child + 1].dist < minHeap->Element[child].dist)
            child++;
        if (last.dist > minHeap->Element[child].dist)
            minHeap->Element[i] = minHeap->Element[child];
        else
            break;
    }
    minHeap->Element[i].dist = last.dist;
    minHeap->Element[i].vertex = last.vertex;
    minHeap->Element[i].prev = last.prev;    
    return min;
}