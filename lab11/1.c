#include <stdio.h>
#include <stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct
{
    int Capacity;
    int Size;
    int *Elements;
} Heap;

Heap *CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap *heap);
void PrintHeap(Heap *heap);
int IsFull(Heap *heap);

int main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    Heap *maxHeap;
    int heapSize, key, max_element;

    while (!feof(fin))
    {
        fscanf(fin, "%c", &cv);
        switch (cv)
        {
        case 'n':
            fscanf(fin, "%d", &heapSize);
            maxHeap = CreateHeap(heapSize);
            break;
        case 'i':
            fscanf(fin, "%d", &key);
            Insert(maxHeap, key);
            break;
        case 'd':
            max_element = DeleteMax(maxHeap);
            if (max_element != -INF)
            {
                fprintf(fout, "max element: %d deleted \n", max_element);
            }
            break;
        case 'p':
            PrintHeap(maxHeap);
            break;
        case 'f':
            fscanf(fin, "%d", &key);
            if (Find(maxHeap, key))
                fprintf(fout, "%d is in the heap\n", key);
            else
                fprintf(fout, "finding error: %d is not in the heap\n", key);
            break;
        }
    }
    return 0;
}

Heap *CreateHeap(int heapSize){
    //allocate the space for the heap depending in the size that is given
    Heap* heap = malloc(sizeof(Heap));
    heap->Capacity = heapSize;
    heap->Size = 0;
    heap->Elements = malloc(sizeof(int) * (heapSize ));
    heap->Elements[0] = 0xFFFFFFF;
    return heap;

}
void Insert(Heap *heap, int value)
{
    //checking capacity of the heap
    if(IsFull(heap)==1) {
        fprintf(fout,"insertion Error: heap is full\n");
        return;
    }
    //search for the value in the heap
    if(Find(heap, value) == 1) {
        fprintf(fout,"insert error: %d is in already in the heap\n", value);
        return;
    }
    //if the size is less than 2 we have to arrenge the positions again
    if(heap->Size < 2) {
        fprintf(fout,"insert %d\n", value);
        heap->Elements[++ heap->Size] = value;
        if(heap->Size == 2 && heap->Elements[1] < heap->Elements[2]){
           //here we swap the positions
            int tmp = heap->Elements[1];
            heap->Elements[1] = heap->Elements[2];
            heap->Elements[2]= tmp;
        }
        return;
    }
    //here we put the parent as the floor
    int i;
    for(i = ++heap->Size; heap->Elements[i / 2] < value; i /= 2) {
        heap->Elements[i] = heap->Elements[i / 2];
    }
    //here we just insert the value
    heap->Elements[i] = value;
    fprintf(fout,"insert %d\n", value);
}
int Find(Heap *heap, int value){
    //we search in all the heap with the for, checking one by one the values
    int i;
    for(i = 1; i < heap->Size + 1; i ++) {
        if(heap->Elements[i] == value) {
            return 1;
        }
    }
    return 0;

}
int DeleteMax(Heap *heap){
    //if the size is 0= empty, we return -INF
   if(heap->Size == 0) {
        fprintf(fout,"delete error: heap is empty\n");        
        return -INF;
    }
    int i, child;
    int MaxElement, LastElement;
    MaxElement = heap->Elements[1];
    LastElement = heap->Elements[heap->Size --];
    //if the left child if different than the heap size and the heap elemenet in the position 
    //of the child+1 is less than the elemet of the positions in the child we just add 1 to the
    //child value
    for(i = 1; i * 2 <= heap->Size; i = child) {
        child = i * 2;
        if(child != heap -> Size && heap->Elements[child + 1] > heap->Elements[child]) {
            child ++;
        }
        //if the last element is less than the element in the child positions we put the current
        //position to be the same value as the element in the child position
        if(LastElement < heap->Elements[child]) {
            heap->Elements[i] = heap->Elements[child];
        }
        else {
            break;
        }
    }
    //we just return the max element that is the element in the top position
    heap->Elements[i] = LastElement;
    return MaxElement;
}
void PrintHeap(Heap *heap){
    //heap is empty if the heap size is 0
    if(heap->Size == 0) {
        fprintf(fout,"print error: heap is empty\n");
        return;
    } else {
        //print each element
        for(int i = 1; i <= heap->Size; i ++) {
            fprintf(fout,"%d ", heap->Elements[i]);
        }
    }
    fprintf(fout,"\n");
}
int IsFull(Heap *heap){
    //checking the capacity of the heap
    if(heap->Size >= heap->Capacity) {
        return 1;
    }else return 0;
}
