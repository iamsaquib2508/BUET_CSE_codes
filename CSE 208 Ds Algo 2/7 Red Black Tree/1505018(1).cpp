/*  *****************************************            DONATE BLOOD, SAVE LIFE!               ***************************************  */




#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define ll long long int
#define pb push_back
#define pii pair<int,int>
#define plolo pair<ll,ll>
#define mm(a,b) memset(a,b,sizeof(a))
#define pf printf

using namespace std;

#define black 0
#define red 1

class node
{
public:
	int val, col;
	node *par, *left, *right;
	void init()
	{
		par=NULL; left=NULL; right=NULL;
	}
	setv(int k)
	{
		val=k;
	}
};

node *root;
void Del_fix(node *x);
void Insert_fix(node *x);
void print(node *x);

void Left_Rotate(node* x)
{
	if(x==NULL) return;
 	node *y = x->right ;  		   // set y
	x->right = y->left;
	if(y->left)
	y->left->par = x;
 	y->par = x->par;
 	if ( x->par = NULL)	   // x is the root
	       root = y;
	else if (x = x->par->left )   // check whether x is the left child of p[x]
		x->par->left = y;
	else 
		x->par->right = y;
	y->left = x;
 	x->par = y;
}

void Right_Rotate(node *x)
{
	if(x==NULL) return;
 	node *y = x->left ;  		   // set y
	x->left = y->right;
	if(y->right)
	y->right->par = x;
 	y->par = x->par;
 	if ( x->par = NULL)	   // x is the root
	       root = y;
	else if (x = x->par->left )   // check whether x is the left child of p[x]
		x->par->left = y;
	else 
		x->par->right = y;
	y->right = x;
 	x->par = y;
}


node* SUCCESSOR(node *cur)
{
	
	if(cur->right==NULL) return NULL;
	cur=cur->right;
	while(cur->left)
	{
		cur=cur->left;
	}
	return cur;
}

void Delete(node *z)
{
	node *y, *x;
 	if ( z->left ==  NULL || z->right == NULL)
		y =  z;	 
	else y =  SUCCESSOR(z)	;
	if (y->left != NULL)
			x = y->left ;
 	else x = z->right;
 	if(x!=NULL)
 	x->par = y->par;
	if ( y->par == NULL)
			root =  x;
	else if ( y == y->par->left )
			y->par->left = x;
	else y->par->right =  x;
	if(y==z) 
	{
		//free[z];
		y=NULL; z=NULL; return;
	}
	if (y != z)  
		z->val = y->val;	// swap z and y
	if (y->col = black)
	{
		Del_fix(x);
		y=NULL;
	}
	
}

void Del_fix(node *x)
{
	node *w;
	while (x->col == black && x != root )
	{
	
		if (x == x->par->left )
		{
		
			w = x->par->right;		 // w is x’s sibling
			if ( w->col == red)  		 	// case 1
			{
			
				w->col = black ;
				x->par->col =  red;
				Left_Rotate(x->par);
				w = x->par->right;
			}
			if (w->left->col== black && w->right->col== black )     	// case 2
			{
			
				w->col= red ;
				x = x->par;
			}
			else 
			{
			
				if (w->right->col == black) // case 3
				{
				
					w->left->col = black;
					w->col = red;
					Right_Rotate(w);
					w =  x->par->right;
				}
				w->col= x->par->col;	// case 4
				x->par->col = black;
				w->right->col = black;
				Left_Rotate(x->par);
				x = root;
			}
		}
		//else same as then clause of line 2 with left and right exchanged->
	}
	x->col = black;
}

#define hello cout << "bug\n";
void Insert(int k) //prepare a node first
{
	node *z=new node[1], *y=new node[1], *x=new node[1];
	z->init(); x->init(); y->init(); z->setv(k);
	
 	y = NULL;
	x = root;
	
	while (x != NULL)
	{
 		y = x;
 		if (z->val < x->val) x = x->left;
		 else x = x->right;
	}
	z->par = y;
	if (y == NULL)  root= z;
	else if ( z->val < y->val ) y->left = z;
	else y->right = z;
	z->left = NULL;  z->right = NULL;
 	z->col = red;
 	
	Insert_fix(z);
}

void Insert_fix(node *z)
{
	node *y=new node[1];
	int qw;
	//cout << z->par << endl;
 	while (z->par!=NULL && z->par->col == red )
	{
		//if(z->par->par==NULL) break;
		if (z->par == z->par->par->left )
		{
		
 			y = z->par->par->right; 
 			
			if (y->col == red) 	    // case 1
			{
			
 				z->par->col = black;
 				y->col =  black;
		 		z->par->par->col = red;
				z = z->par->par;
			}
			else
			{
			 
 				if (z == z->par->right)	// case 2
 				{
				
					z = z->par;
					Left_Rotate(z);
				}
				z->par->col = black;	 // case 3
 				z->par->par->col = red;
				Right_Rotate( z->par->par);
			}
	
		}
		//if(z->par->par==NULL) break;
		else
		{
		
 			y = z->par->par->left; 
 			
			if (y->col == red) 	    // case 1
			{
			
 				z->par->col = black;
 				y->col =  black;
		 		z->par->par->col = red;
				z = z->par->par;
			}
			else
			{
			 
 				if (z == z->par->left)	// case 2
 				{
				
					z = z->par;
					Right_Rotate(z);
				}
				z->par->col = black;	 // case 3
 				z->par->par->col = red;
				Left_Rotate( z->par->par);
			}
	
		}
	}
	root->col = black;
}

void print(node *z)
{
	if(z==NULL) return;
	print(z->left);
	cout << z->val << " " ;
	if(z->col==red) cout << "red\n";
	else cout << "black\n";
	print(z->right);
	
	return;
}

node* min()
{
	if(root==NULL) return NULL;
	node *cur;
	cur=root;
	while(cur->left) cur=cur->left;
	return cur;
}

node* max()
{
	if(root==NULL) return NULL;
	node *cur;
	cur=root;
	while(cur->right) cur=cur->right;
	return cur;
}

node* search(int a)
{
	if(root==NULL) return NULL;
	node* cur=root;
	while(cur)
	{
		if(a<cur->val) cur=cur->left;
		else if(cur->val<a) cur=cur->right;
		else return cur;
	}
	return NULL;
}


int main()
{
	root=NULL;
	node *pt;
	while(1)
	{
		pf("1. Insert 2. Delete 3. Max 4. Min 5. Search 6. Print 7.Exit\n");
		int op, a;
		cin >> op;
		if(op==1)
		{
			cin >> a;
			Insert(a);
		}
		else if(op==2)
		{
			cin >> a;
			pt=search(a);
			if(pt==NULL) pf("Not in tree\n");
			else Delete(pt);
		}
		else if(op==3)
		{
			pt=max();
			if(pt==NULL) pf("Empty tree\n");
			else pf("Max: %d\n", pt->val);
		}
		else if(op==4)
		{
			pt=min();
			if(pt==NULL) pf("Empty tree\n");
			else pf("Min: %d\n", pt->val);
		}
		else if(op==5)
		{
			cin >> a;
			pt=search(a);
			if(pt==NULL) pf("Not in tree\n");
			else pf("Found %d\n", pt->val);
		}
		else if(op==6)
		{
			print(root);
		}
		else if(op==7)
		{
			break;
		}
	}
	

	return 0;
	
}
