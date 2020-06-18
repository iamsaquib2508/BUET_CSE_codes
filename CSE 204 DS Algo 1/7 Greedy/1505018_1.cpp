#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define MAX_HEAP_SIZE 10000

using namespace std;

char CODE[26][30];
char CHARS[26];
int SCHARS[26];

class node
{
public:		
	int lsz, rsz, *l, *r;
	double key;
	node()
	{
		l=new int[30];
		r=new int[30];
		lsz=rsz=0;
	}
	void merge(node a, node b)
	{
		int i;
		key=a.key+b.key;
		ffr(i,0,a.lsz) l[lsz++]=a.l[i];
		ffr(i,0,a.rsz) l[lsz++]=a.r[i];
		
		ffr(i,0,b.lsz) r[rsz++]=b.l[i];
		ffr(i,0,b.rsz) r[rsz++]=b.r[i];
		
		ffr(i,0,lsz) CODE[l[i]][SCHARS[l[i]]++]='0';
		ffr(i,0,rsz) CODE[r[i]][SCHARS[r[i]]++]='1';
		return;
	}
};

class MinHeap
{
public:
	node *A;
	int length;
	MinHeap()
	{
		A = new node[MAX_HEAP_SIZE];
		length=0;
	}	
	~MinHeap()
	{
		if(A) delete []A;
		A=0;
	}
	
	void push(node item)
	{
		length++;
		A[length]=item;
		buheap(length);
	}
	
	node pop()
	{
		node ret=A[1];
		A[1]=A[length--];
		heapify(1);
		return ret;
	}
	
	void heapify(int i)
	{
		int r, l, smallest;
		while(1)
		{
			l=i+i;
			r=l+1;
			smallest=i;
			
			if(l>length && r> length) break;
			else if(r>length) smallest=l;
			else if(l>length) smallest=r;
			else if(A[l].key < A[r].key) smallest=l;
			else smallest=r;
			
			if(A[i].key<=A[smallest].key) break;
			else
			{
				node t;
				t=A[i]; A[i]= A[smallest]; A[smallest]=t; i=smallest;
			}
		}
	}
	
	void buheap(int i)
	{
		while(1)
		{
			if(i==1 || A[i/2].key<=A[i].key) break;
			node t;
			t=A[i]; A[i]= A[i/2]; A[i/2]=t; i/=2;
		}
	}
	
	
	
};

int main()
{
	int i, j, k, n;
	cin >> n;
	char c;
	MinHeap heap;
	ffr(i,0,n)
	{
		cin >> c >> k;
		CHARS[i]=c;
		SCHARS[i]=0;
		node a;
		a.key=k;
		a.l[a.lsz++]=i;
		heap.push(a);
	}
	
	while(heap.length>1)
	{
		node L, R, niu;
		L=heap.pop(); R=heap.pop();
		niu.merge(L,R);
		heap.push(niu);
	}
	
	ffr(i,0,n)
	{
		int sz=SCHARS[i];
		k=sz/2;
		ffr(j,0,k)
		{
			c=CODE[i][j];
			CODE[i][j]=CODE[i][sz-1-j];
			CODE[i][sz-1-j]=c;
		}
		pf("\n%c %s", CHARS[i], CODE[i]);
	}
	
	return 0;
	
}


/*
4
a 23
b 45
c 19
d 52

out:
a 101
b 11
c 100
d 0

6
a 45
b 13
c 12
d 16
e 9
f 5

out:
a 0
b 101
c 100
d 111
e 1101
f 1100
*/
