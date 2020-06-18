#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

#define FALSE_VALUE 0
#define TRUE_VALUE 1

struct treeNode
{
    int item;
    struct treeNode * left; //points to left child
    struct treeNode * right; //points to right child
};

struct treeNode * root;

typedef struct treeNode nod;


void initializeTree()
{
    root = 0;
}


struct treeNode * searchItem(struct treeNode * node, int item)
{
    if(node==0) return 0;
    if(node->item==item) return node; 
    struct treeNode * t = 0;
    if(item < node->item)
        t = searchItem(node->left, item); 
    else
        t = searchItem(node->right, item); 
    return t;
};


struct treeNode * makeTreeNode(int item)
{
    struct treeNode * node ;
    node = (struct treeNode *)malloc(sizeof(struct treeNode));
    node->item = item;
    node->left = 0;
    node->right = 0;
    return node;
};

struct treeNode * insertItem(struct treeNode * node, int item)
{
    if(node==0)
    {
        struct treeNode * newNode = makeTreeNode(item);
        root = newNode;
        return newNode;
    }

    if(node->item==item) return 0; 

    if(item<node->item && node->left==0) 
    {
        struct treeNode * newNode = makeTreeNode(item);
        node->left = newNode;
        return newNode;
    }

    if(item>node->item && node->right==0) 
    {
        struct treeNode * newNode = makeTreeNode(item);
        node->right = newNode;
        return newNode;
    }

    if(item<node->item)
        return insertItem(node->left, item); 
    else
        return insertItem(node->right, item); 
}



int calcNodeHeight(struct treeNode * node) 
{
    if(node==0) return -1;
    int l, r;
    l = calcNodeHeight(node->left);
    r = calcNodeHeight(node->right);
    if(l>r) return l+1;
    else return r+1;
}


int calcHeight(int item) 
{
    struct treeNode * node = 0;
    node = searchItem(root, item);
    if(node==0) return -1; 
    else return calcNodeHeight(node);
}

//Task 1

int getSize(struct treeNode * node)
{
	nod *cur=node;
	if(cur==0) return 0;
	return getSize(cur->left)+getSize(cur->right)+1;
}

//Task 2a
int calcDepth(int item)
{
    
    nod *cur;
    cur = root;
    
    int depth=0;
    while(1)
    {
    	if(cur==0) return -9999999;
    	if(item<cur->item) cur=cur->left;
    	else if(cur->item<item) cur=cur->right;
    	else return depth;
    	depth++;
	}
}

//Task 2b
int calcNodeDepth(struct treeNode * node) 
{
    
    if(node==0) return -9999999;
    return calcDepth(node->item);
}



//Task 3a
int getMinItem() 
{
    
    nod *cur; int min;
    if(root==0) return -9999999;
    cur = root;
    
    while(1)
    {
    	if(cur==0) return min;
    	min=cur->item;
    	cur=cur->left; 
	}
}
//Task 3b
int getMaxItem() 
{
    
    nod *cur; int max;
    if(root==0) return -9999999;
    cur = root;
    
    while(1)
    {
    	if(cur==0) return max;
    	max=cur->item;
    	cur=cur->right; 
	}
    
}

//Task 4
int rangeSearch(struct treeNode * node, int lb, int rb) 
{
    
    if(node==0) return 0;
    if(lb > rb) return 0;
    
    int val = node->item;
    if(lb == rb && lb == val) return 1;
    //cout << lb << " " << val << " " << rb << endl;
    
    if(lb<val && rb<=val) return rangeSearch(node->left, lb, rb)+(rb==val);
    
    if(lb<val && val<rb) return rangeSearch(node->left, lb, val-1) + rangeSearch(node->right, val+1, rb) +1;
    
    if(val<=lb && val<rb) return rangeSearch(node->right, lb, rb)+(lb==val);
    
    return 0;
}


//Task 5
int deleteItem(struct treeNode * node, int item)
{
    
    if(node==0 || root ==0) return false;
    nod *prev=node, *cur=node, *del;
    
    if(cur==root && cur->item==item)
    {
    	del=cur;
    	if(cur->left==0)
    	{
    		
    		root=cur->right;
    		free(cur);
    		return 1;
		}
		if(cur->right==0) 
		{
			root=cur->left;
			free(cur); return 1;
		}
		prev=cur;
		cur=cur->right;
		while(cur->left!=0) 
		{
			prev=cur;
			cur=cur->left;
		}
		del->item=cur->item;
		if(prev==del) prev->right=cur->right;
		else prev->left=cur->right;
		free(cur);
		
		return 1;
	}
	
	
	
	while(cur!=0 && cur->item!=item)
	{
		
		prev=cur;
		if(item < cur->item) cur=cur->left;
		else if(cur->item < item) cur=cur->right;
		
	}
	
	
	if(cur==0) return 0;
	else
	{
		del=cur;
		
		if(cur->left==0) 
		{
			
			if(cur==prev->left) prev->left=cur->right;
			else prev->right=cur->right;
			
			free(cur); return 1;
		}
		if(cur->right==0) 
		{
			
			if(cur==prev->left) prev->left=cur->left;
			else prev->right=cur->left;
			free(cur); return 1;
		}
		prev=cur;
		cur=cur->right;
		while(cur->left!=0) 
		{
			prev=cur;
			cur=cur->left;
		}
		del->item=cur->item;
		if(prev==del) prev->right=cur->right;
		else prev->left=cur->right;
		free(cur);
		
		return 1;
	}
    
}


void printInOrder(struct treeNode * node, int height)
{
    if(node==0) return ;

    //print left sub-tree
    printInOrder(node->left, height-1);

    //print item
    for(int i=0;i<height;i++)printf("    ");
    printf("%03d\n",node->item);

    //print right sub-tree
    printInOrder(node->right, height-1);
}



int main()
{
	//freopen("BST.txt", "r", stdin);
    initializeTree();
    
    while(1)
    {
        //printf("1. Insert item. 2. Delete item. 3. Search item. \n");
        //printf("4. Print height of tree. 5. Print height of an item. \n");
        //printf("6. PrintInOrder. 7. exit.\n8. Range Search 9. Min 10. Max\n11. Size\n");

        int ch, a, b;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);
            insertItem(root, item);
        }
        else if(ch==2)
        {
            int item;
            scanf("%d", &item);
            ch=deleteItem(root, item);
            if(ch) printf("Deleted %d\n", item);
            else printf("Not Found\n");
        }
        else if(ch==3)
        {
            int item;
            scanf("%d", &item);
            struct treeNode * res = searchItem(root, item);
            if(res!=0) printf("Found.\n");
            else printf("Not found.\n");
        }
        else if(ch==4)
        {
            int height = calcNodeHeight(root);
            printf("Height of tree = %d\n", height);
        }
        else if(ch==5)
        {
            int item;
            scanf("%d", &item);
            int height = calcHeight(item);
            printf("Height of %d = %d\n", item, height);
        }
        else if(ch==6)
        {
            int h = calcNodeHeight(root);
            printf("\n--------------------------------\n");
            printInOrder(root, h);
            printf("--------------------------------\n");
        }
        else if(ch==7)
        {
            break;
        }
        else if(ch==8)
        {
        	scanf("%d %d", &a, &b);
        	printf("%d numbers in range %d - %d\n", rangeSearch(root,a,b), a, b);
		}
		else if(ch==9) printf("Min is %d\n", getMinItem());
		else if(ch==10) printf("Max is %d\n", getMaxItem());
		else if(ch==11) printf("Size is %d\n", getSize(root));
    }
	return 0;
}
