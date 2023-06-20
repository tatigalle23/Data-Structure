#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;
typedef struct AVLNode
{
    ElementType element;
    AVLTree left, right;
    int height;
} AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);
int Height(Position T); //check the Height and return it
int Max(int a, int b); //Check the max num of the height


int main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    AVLTree Tree = NULL;
    char cv;
    int key;

    while (!feof(fin))
    {
        fscanf(fin, "%c", &cv);
        switch (cv)
        {
        case 'i':
            fscanf(fin, "%d\n", &key);
            Tree = Insert(key, Tree);
            break;
        case 'd':
            fscanf(fin, "%d\n", &key);
            Tree = Delete(key, Tree);
            break;
        }
        PrintInorder(Tree);
        fprintf(fout, "\n");
    }
    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);

    return 0;
}

AVLTree Insert(ElementType X, AVLTree T)
{
    if (T == NULL)
    {//We create a new node to put the element
        
        T = malloc(sizeof(struct AVLNode));
        T->element = X;
        T->height = 0;
        T->left = T->right = NULL;
    }
    /*if the element is smaller than the root we go to left and if the height of the 2 child
    is bigger than 2 we rotate (we check if the element is smaller than the the left child alredy in if it is just rotate
    if not we double rotate)*/
    else if (X < T->element)
    {
        T->left = Insert(X, T->left); 
        if (Height(T->left) - Height(T->right) >= 2)
        {
            if (X < T->left->element)
            {
                T = SingleRotateWithLeft(T);
            }
            else
            {
                T = DoubleRotateWithLeft(T);
            }
        }
    }
    /*if the element is bigger than the root we go to right and if the height of the 2 childs
    is bigger than 2 we rotate (we check if the element is bigger than the the right child alredy in if it is just rotate
    if not we double rotate)*/
    else if (X > T->element)
    {
        T->right = Insert(X, T->right);
        if (Height(T->right) - Height(T->left) >= 2)
        {
            if (X > T->right->element)
            {
                T = SingleRotateWithRight(T);
            }
            else
            {
                T = DoubleRotateWithRight(T);
            }
        }
    }else{
        fprintf(fout,"insertion error: %d is already in the tree! \n", X);
    }
    //we adjust the height of the new node
    T->height = Max(Height(T->left), Height(T->right)) + 1;
    return T;

}
AVLTree Delete(ElementType X, AVLTree T)
{
    AVLTree p;
    if (T == NULL)
    {
        //if the node is empty it means it does not exist
        fprintf(fout,"deletion error: %d is not in the tree! \n",X);
        return NULL;
    }
    /*if bigger or smaller than the current element in the node we go right and try to find the element to delete 
    by recursion again we have to reorganize the tree by doing rotate or double rotate like in insert */
    else if (X > T->element) 
    {
        T->right = Delete(X,T->right);
        if (Height(T->right) - Height(T->left) == 2)
            if (Height(T->left) >= 0)
                T = SingleRotateWithRight(T);
            else
                T = DoubleRotateWithRight(T);
    }
    else if (X < T->element)
    {
        T->left = Delete(X,T->left);
        if (Height(T->right) - Height(T->left) == -2) 
            if (Height(T->right) <= 0)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
    }
    /*Here the element is found and start deleting its inorder succesor
    again we rebalance the tree*/
    else
    {
        if (T->right != NULL)
        { 
            p = T->right;
            while (p->left != NULL)
                p = p->left;
            T->element = p->element;
            T->right = Delete(p->element, T->right);
            if (Height(T->right) - Height(T->left) == 2)
                if (Height(T->left) >= 0)
                    T = SingleRotateWithRight(T);
                else
                    T = DoubleRotateWithRight(T);
        }
        else
            return (T->left);
    }
    
    T->height = Max(Height(T->left), Height(T->right)) + 1;
    return T;
}

Position SingleRotateWithLeft(Position node)
{
    //Function to reorganice the tree by a single rotation, return the new node
    Position node2;
    node2 = node->left;
    node->left = node2->right; 
    node2->right = node;
    node->height = Max(Height(node->left), Height(node->right)) + 1;
    node2->height = Max(Height(node2->left), node->height) + 1;
    return node2; 
}
Position SingleRotateWithRight(Position node)
{
    //Function to reorganice the tree by a single rotation, return the new node
    Position node2;
    node2 = node->right;
    node->right = node2->left;
    node2->left = node;
    node->height = Max(Height(node->left), Height(node->right)) + 1;
    node2->height = Max(Height(node->right), node->height) + 1;
    return node2;
}
Position DoubleRotateWithLeft(Position node)
{
    //similar to single rotate but with two rotation made between node and node2 (from singlerotate)
    node->left = SingleRotateWithRight(node->left);
    return SingleRotateWithLeft(node);
}
Position DoubleRotateWithRight(Position node)
{
    //similar to single rotate but with two rotation made between node and node2 (from singlerotate)
    node->right = SingleRotateWithLeft(node->right);
    return SingleRotateWithRight(node);
}
void PrintInorder(AVLTree T)
{
    //Here we just print the elemts first the left and the right
    if (T != NULL)
    {
        PrintInorder(T->left);
        fprintf(fout, "%d (%d) ", T->element, T->height);
        PrintInorder(T->right);
    }
}
void DeleteTree(AVLTree T)
{
   //delete each of the child and parents of the tree making it empty
   if (T != NULL)
    {
        DeleteTree(T->left);
        DeleteTree(T->right);
        free(T);
    }
    
}
int Height(Position T)
{
    //function to return the height of the node (so we can check the height of the node in the left or right)
    if (T == NULL)
    {
        return -1;
    }
    return T->height;
}
int Max(int a, int b)
{
    //we check the max between the height of the 2 nodes
    return (a > b) ? a : b;
}

