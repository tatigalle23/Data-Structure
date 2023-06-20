#include <stdio.h>
#include <time.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet
{
    int size_maze;
    int *ptr_arr;
} DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int num, i;
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);
    sets = (DisjointSets *)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets *)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(sets, maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *Sets, DisjointSets *maze_print, int num)
{
    Sets->size_maze = (num * num);
    Sets->ptr_arr = (int *)malloc(sizeof(int) * (Sets->size_maze + 1));
    for (int i = 1; i <= Sets->size_maze; i++)
    {
        Sets->ptr_arr[i] = 0;
    }
    maze_print->size_maze = (Sets->size_maze * 2);
    maze_print->ptr_arr = (int *)malloc(sizeof(int) * (maze_print->size_maze + 1));
    for (int i = 1; i <= maze_print->size_maze; i++)
    {
        maze_print->ptr_arr[i] = -1;
    }
}

int find(DisjointSets *Sets, int i)
{
    // fingding number of the member of the set
    while (Sets->ptr_arr[i] > 0)
    {
        i = Sets->ptr_arr[i];
    }
    return i;
}

void Union(DisjointSets *Sets, int i, int j)
{
    // finding the 2 roots for checking the cells
    int r1 = find(Sets, i);
    int r2 = find(Sets, j);

    // union of elements
    if (Sets->ptr_arr[r2] < Sets->ptr_arr[r1])
        Sets->ptr_arr[r1] = r2;
    else
    {
        if (Sets->ptr_arr[r1] == Sets->ptr_arr[r2])
            Sets->ptr_arr[r1]--;
        Sets->ptr_arr[r2] = r1;
    }
}

void createMaze(DisjointSets *Sets, DisjointSets *maze_print, int num)
{

    int wall, cell1, cell2;
    int size = maze_print->size_maze + 1;
    int floor = size - num;

    while (find(Sets, 1) != find(Sets, Sets->size_maze))
    {
        while (1)
        {
            wall = rand() % size; // gettin size for wall

            if (wall == 0)
                wall = 1;
            if (maze_print->ptr_arr[wall] == 0) // check if the wall its already break
                continue;
            else if (wall >= floor) // to not break the floor
                continue;
            else if (wall <= Sets->size_maze && wall % num == 0) // to not break the wall
                continue;
            else
                break;
        }

        // for wall
        if (wall <= Sets->size_maze)
        {
            cell1 = wall;
            cell2 = wall + 1;
        }
        // for the roof
        else
        {
            cell1 = wall - Sets->size_maze;
            cell2 = cell1 + num;
        }

        if (find(Sets, cell1) != find(Sets, cell2))
        {
            Union(Sets, cell1, cell2);
        }
        maze_print->ptr_arr[wall] = 0; // break the wall
    }
}

void printMaze(DisjointSets *Sets, DisjointSets *maze_print, int num)
{
    // printing upper roof
    fprintf(fout, "+");
    for (int i = 1; i <= num; i++)
    {
        fprintf(fout, "---+");
    }
    fprintf(fout, "\n    ");
    int i = 1, size = Sets->size_maze + 1;
    for (; i <= Sets->size_maze; i++)
    {
        if (maze_print->ptr_arr[i] < 0)
        {                             // less than 0 is a wall
            if (i == Sets->size_maze) // to break the last wall
                fprintf(fout, "   ");
            else
            {
                fprintf(fout, "|   ");
            }
            if ((i % num) == 0)
            {
                // print floor
                int j = 1;
                fprintf(fout, "\n");
                fprintf(fout, "+");
                for (; j <= num; j++)
                {
                    if (maze_print->ptr_arr[size] < 0) // checking if the roof is broken or not
                        fprintf(fout, "---+");
                    else
                        fprintf(fout, "   +");
                    size++;
                }
                if (i < Sets->size_maze) // printing right wall
                    fprintf(fout, "\n|   ");
                else
                    fprintf(fout, "\n   ");
            }
        }
        // broken wall
        else
        {
            if (i % num == 0)
                fprintf(fout, "   \n");
            else
                fprintf(fout, "    ");
        }
    }
    fprintf(fout, "\n");
}

void freeMaze(DisjointSets *Sets, DisjointSets *maze_print)
{
    free(Sets);
    free(maze_print);
}
