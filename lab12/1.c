#include <stdio.h>
#include <stdlib.h>
FILE *fin;
FILE *fout;

typedef struct BNode *BNodePtr;
struct BNode
{
    int order;
    int size;             /*	number	of	children	*/
    BNodePtr *child; /*	children pointers	*/
    int *key;            /*	keys	*/
    int is_leaf;
} BNode;

BNodePtr CreateTree(int	order);
void	Insert(BNodePtr root,	int	key);
int	Find(BNodePtr root,	int	key);
void	PrintTree(BNodePtr root);
void	DeleteTree(BNodePtr root);
BNodePtr Split (BNodePtr x, int i, BNodePtr y);
BNodePtr InsertNode(BNodePtr root, int a);

int main(int argc, char *argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root= CreateTree(order);
    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch (cv)
        {
        case 'i':
            fscanf(fin, "%d", &key);
            if(Find(root,key))
                fprintf(fout,"insert error: key %d is already in the tree!\n",key);
            else
                Insert(root, key);
            break;
        case 'f':
           fscanf(fin, "%d", &key);
        if(Find(root,key))
                fprintf(fout,"key %d found\n",key);
        else
            fprintf(fout,"finding error: key %d is not in the tree!\n",key);
        break;
        case 'p':
            if(root->size==1)//1 o 0
                fprintf(fout, "print error: tree is empty!");
            else
                PrintTree(root);
            fprintf(fout, "\n");
            break;
        }
    }
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

}

BNodePtr CreateTree(int order){
    //allocating new space for the tree (parents and child)
    BNodePtr b = (BNodePtr)malloc(sizeof(struct BNode));
    b->is_leaf = 1;
    b->size = 0;
    b->order = order;
    b->child = (BNodePtr*)malloc(2*order * sizeof(BNodePtr));
    b->key = (int*)malloc((2*order-1) * sizeof(int));
    for(int i = 0; i< order+1; i++){
       b->child[i] = NULL;
    }
    return b;
}


//to split the nodes and rearrange the tree
BNodePtr Split (BNodePtr x, int i, BNodePtr y) {
    int j;
    BNodePtr z = CreateTree(x->order);
    int T = x->order;
    z->size = T - 1;

    for (j = 0; j < T - 1; j++){
        z->key[j] = y->key[j+T];
    }

    if (y->is_leaf == 0)
    {
        for (j = 0; j < T; j++){
            z->child[j] = y->child[j+T];
        }
    }
    y->size = T - 1;
    for (j = x->size; j >= i+1; j--){
        x->child[j+1] = x->child[j];
    }
    x->child[i+1] = z;
    for (j = x->size-1; j >= i; j--){
        x->key[j+1] = x->key[j];
    }
    x->key[i] = y->key[T-1];
    x->size++;
   return x;
}
//To insert a new node if there is still space in the leafs
BNodePtr InsertNode(BNodePtr root, int a){
   int i = root->size-1;
    //check first is the root recieved is a leaf
    if(root->is_leaf){
        //then we search the correct place to put the value
        while(i >= 0 && root->key[i]> a){
            root->key[i+1] = root->key[i];
            i--;
        }
        root->key[i+1] = a;
        root->size++;
    }
    else
    {
        //if not it meas we have to re organize the tree so we use slip and then recursion to 
        //put the value in the correct spot
        while (i >= 0 && root->key[i] > a){
            i--;
        }       
        i = i + 1;
        if (root->child[i]->size == 2*root->order-1){
            root = Split(root, i, root->child[i]);

            if (a > root->key[i]){
                i = i + 1;
            }
        }
        root->child[i] = InsertNode(root->child[i], a);
    }
    return root;
}
void Insert(BNodePtr root, int key){
    BNodePtr r = root;
    //check that the order of the tree is correct then we insert the node
    //if not we have to rearrenge the tree
    if(r->size == 2*r->order -1){
        BNodePtr s = CreateTree(root->order);
        s->is_leaf = 0;
        s->child[0] = r;
        s= Split(s, 0, r);
        s = InsertNode(s, key);        
    }
    else{         
        r = InsertNode(r, key);
    }
}

int Find(BNodePtr root, int key){
    //Here we just search in each value of the tree the key inputed
    BNodePtr high = root;
    int flag = 1;
    while(1){
        int i;
        for(i = 0; i < high->size; i++){
            if(key == high->key[i]) return 1;
            else if(key < high->key[i]) break;
        }
        if(high->is_leaf) break;
        high = high->child[i];
    }
    return 0;
}

void PrintTree(BNodePtr a){
    //printing in traversal order with recursion
    int i = 0;    
    if(!a ){       
        return;
    }
    while(a->key[i]!=0){
        fprintf(fout,"%d ", a->key[i]);
        if (a->is_leaf < 0) {
            PrintTree(a->child[i]);
        }
        i++;
    }
}


void DeleteTree(BNodePtr a){
     int i;
     for (i = 0; i < a->size; i++) {
        free(a->child[i]);
     }

   for (i = 0; i <= a->size; i++) {
      if (a->is_leaf == 0) {
         DeleteTree(a->child[i]);
      }
   }
 free(a);
} 