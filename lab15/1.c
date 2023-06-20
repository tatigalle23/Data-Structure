#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl *HashTable;
typedef struct HashTbl
{
    int TableSize;
    List *TheLists;
} HashTbl;

HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);

int main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char solution_str[20];
    int solution, TableSize;

    fscanf(fin, "%s", solution_str);
    if (!strcmp(solution_str, "linear"))
        solution = 1;
    else if (!strcmp(solution_str, "quadratic"))
        solution = 2;
    else
    {
        fprintf(fout, "error: invalid hashing solution!");
        return 0;
    }
    fscanf(fin, "%d", &TableSize);

    HashTable H = createTable(TableSize);

    char cv;
    int key;

    while (!feof(fin))
    {
        fscanf(fin, "%c", &cv);
        switch (cv)
        {
        case 'i':
            fscanf(fin, "%d", &key);
            Insert(H, key, solution);
            break;
        case 'f':
            fscanf(fin, "%d", &key);
            int result = Find(H, key, solution);
            if (result)
                fprintf(fout, "%d is in the table\n", key);
            else
                fprintf(fout, "%d is not in the table\n", key);
            break;
        case 'd':
            fscanf(fin, "%d", &key);
            Delete(H, key, solution);
            break;
        case 'p':
            printTable(H);
            break;
        }
    }
    deleteTable(H);

    return 0;
}
// allocating memorie and initialzing values for the hashing table
HashTable createTable(int TableSize)
{
    HashTable H = (HashTbl *)malloc(sizeof(HashTbl));
    H->TableSize = TableSize;
    H->TheLists = (int *)malloc(TableSize * sizeof(int));
    for (int i = 0; i < TableSize; i++)
    {
        H->TheLists[i] = 0;
    }
    return H;
}
void Insert(HashTable H, ElementType Key, int solution)
{
    int Hash;
    // first we need to check that the value is nor in the table already
    int exists = Find(H, Key, solution);
    Hash = Key % H->TableSize;
    if (exists)
        fprintf(fout, "insertion error: %d already exists at address %d\n", Key, exists);
    // we check if we can put the value into the table
    else if (H->TheLists[Hash])
    {
        // now depending of the solution we are going to insert the value
        if (solution == 1)
        {
            int i = 1;
            while (i)
            {
                // we know check if the Hash have one more space and then we input the key in there
                // in the space next to the Hash as Linear
                if (H->TheLists[Hash + i])
                    i++;
                else
                {
                    if (Hash + i < H->TableSize)
                    {
                        H->TheLists[Hash + i] = Key;
                        fprintf(fout, "insert %d into address %d\n", Key, Hash + i);
                    }
                    else
                        fprintf(fout, "insertion error: table is full\n");
                    i = 0;
                }
            }
        }
        else if (solution == 2)
        {
            // here we check the same but as quadratic and add the keys in a quadratic way
            int i = 1;
            while (i)
            {
                if (H->TheLists[Hash + (i * i)])
                    i++;
                else
                {
                    if (Hash + (i * i) <= H->TableSize)
                    {
                        H->TheLists[Hash + (i * i)] = Key;
                        fprintf(fout, "insert %d into address %d\n", Key, Hash + (i * i));
                    }
                    else
                        fprintf(fout, "insertion error: table is full\n");
                    i = 0;
                }
            }
        }
    }
    else
    {
        H->TheLists[Hash] = Key;
        fprintf(fout, "insert %d into address %d\n", Key, Hash);
    }
}
void Delete(HashTable H, ElementType Key, int solution)
{
    // we first find the position that the key is and then we delete the key in that position
    // so the key in that position=0
    int pos = Find(H, Key, solution);
    if (pos)
    {
        H->TheLists[pos] = 0;
        fprintf(fout, "delete %d\n", Key);
    }
    else
        fprintf(fout, "deletion error: %d is not in the table\n", Key);
}
int Find(HashTable H, ElementType Key, int solution)
{
    // we first do a way of sorting to search in the Hash
    int Hash = Key % H->TableSize;
    if (H->TheLists[Hash] != Key)
    {
        if (solution == 1)
        {
            // here we need to search in a Linear way
            int i = 1;
            while (i)
            {
                if (H->TheLists[Hash + i] != Key && Hash + i < H->TableSize)
                    i++;
                else if (H->TheLists[Hash + i] != Key && Hash + i >= H->TableSize)
                    return 0;
                else
                    return Hash + i;
            }
        }
        // here we are searching in a quadratic way
        // so we are going to start moving by i*i instead or just adding one i at the time
        else if (solution == 2)
        {
            int i = 1;
            while (i)
            {
                if (H->TheLists[Hash + i] != Key && Hash + (i * i) < H->TableSize)
                    i++;
                else if (H->TheLists[Hash + i] != Key && Hash + (i * i) >= H->TableSize)
                    return 0;
                else
                    return Hash + (i * i);
            }
        }
    }
    else if (H->TheLists[Hash] == Key)
        return (Hash);
}
void printTable(HashTable H)
{
    // printing of the table
    for (int i = 0; i < H->TableSize; i++)
        fprintf(fout, "%d ", H->TheLists[i]);
    fprintf(fout, "\n");
}
void deleteTable(HashTable H)
{
    // freeing the list and the Hash
    free(H->TheLists);
    free(H);
}