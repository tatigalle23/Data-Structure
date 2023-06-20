#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST *Tree;
typedef struct BST
{
    int value;
    struct BST *left;
    struct BST *right;
} BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    int key;

    Tree root = NULL;

    while (!feof(fin))
    {

        fscanf(fin, "%c", &cv);

        switch (cv)
        {
        case 'i':
            fscanf(fin, "%d", &key);
            //since the insertion function is recursive the printing must been done here
            if (findNode(root, key))
            {
                fprintf(fout, "insertion error: %d is already in the tree\n", key);
            }
            else{
                fprintf(fout, "insert %d \n", key);
                root = insertNode(root, key);                
            }
            break;
        case 'f':
            fscanf(fin, "%d", &key);
            if (findNode(root, key))
            {
                fprintf(fout, "%d is in the tree\n", key);
            }
            else
            {
                fprintf(fout, "finding error: %d is not in the tree\n", key);
            }
            break;
        case 'd':
            fscanf(fin, "%d", &key);
            if (findNode(root, key))
            {
                root = deleteNode(root, key);
                fprintf(fout, "delete %d\n", key);
            }
            else
            {
                fprintf(fout, "deletion error: %d is not in the tree\n", key);
            }
            break;
        case 'p':
            fscanf(fin, "%c", &cv);
            if (cv == 'i')
            {
                if (root == NULL)
                {
                    fprintf(fout, "tree is empty");
                }
                else
                {
                    printInorder(root);
                }
            }
            fprintf(fout, "\n");
            break;
        }
    }
    deleteTree(root);
    fclose(fin);
    fclose(fout);
}

Tree insertNode(Tree root, int key)
{
    //if the root is empty return a new space
    if (root == NULL)
    {
        root = (Tree)malloc(sizeof(struct BST));
        root->value = key;
        root->left = root->right = NULL;
    }
    //if is not empty it should start moving down the tree and adding childd depending the size of the value
    else if (key < root->value)
    {
        root->left = insertNode(root->left, key);        
    }
    else if (key > root->value)
    {
        root->right = insertNode(root->right, key);        
    }

    return root;
}
Tree deleteNode(Tree root, int key)
{
    
    if (root == NULL){
        return root;
    }
    //Depending of the size of the key it would move to left or right child
    if (key < root->value){
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->value){
        root->right = deleteNode(root->right, key);
    }
    //it means key is the root
    else
    {
        // root with only one child or no child case
        if (root->left == NULL)
        {
            Tree temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Tree temp = root->left;
            free(root);
            return temp;
        }

        // root with two children case, and get the minValue node
        Tree temp = root->right;
        while (temp && temp->left != NULL){
            temp = temp->left;
        }
        //copying the value to the root
        root->value = temp->value;
        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}
int findNode(Tree root, int key)
{
    //if the root is empty return false
    if (root == NULL)
    {
        return 0;
    }
    //if the root value is equal to the key then return true
    if (root->value == key)
    {
        return 1;
    }
    //we use recursive to search the key in the childs
    int exist1 = findNode(root->left, key);
    if (exist1)
    {
        return 1;
    }
    int exist2 = findNode(root->right, key);
    return exist2;
}
void printInorder(Tree root)
{
    //first should go to the left child and then to the right child
    if (root != NULL)
    {
        printInorder(root->left);
        fprintf(fout, "%d ", root->value);
        printInorder(root->right);
    }
}
void deleteTree(Tree root)
{
    //delete first the childs starting from the left and then go to the root
    if (root == NULL)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}