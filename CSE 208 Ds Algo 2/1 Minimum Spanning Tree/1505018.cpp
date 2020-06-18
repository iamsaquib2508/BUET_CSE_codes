#include<bits/stdc++.h>
#define NULL_VALUE -999999
#define WHITE 1
#define GREY 2
#define BLACK 3
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define sf scanf
#define INF 1e8
#define ll long long int
#define MAX_HEAP_SIZE 100005



class node
{
public:
	int v, w;
	node(){
	}
	node (int vv, int ww)	
	{
		v=vv, w=ww;
	}
	void setinnode(int vv, int ww)
	{
		v=vv, w=ww;
	}
	bool operator ==(const node item)
	{
        return v==item.v && w==item.w;
    }
    bool operator =(const node item)
	{
        v=item.v; w=item.w;
    }
};


class Queue
{
    int queueInitSize ;
    int queueMaxSize;
    int * data;
    int length;
    int front;
    int rear;
public:
    Queue();
    ~Queue();
    void enqueue(int item);
    int dequeue(); 
    bool empty(); 
};

Queue::Queue()
{
    queueInitSize = 2 ;
    queueMaxSize = queueInitSize;
    data = new int[queueMaxSize] ; 
    length = 0 ;
    front = 0;
    rear = 0;
}


void Queue::enqueue(int item)
{
	if (length == queueMaxSize)
	{
		int * tempData ;
		queueMaxSize =  queueMaxSize + queueMaxSize ;
		tempData = new int[queueMaxSize] ;
		int i, j;
		j = 0;
		for( i = rear; i < length ; i++ )
		{
			tempData[j++] = data[i] ; 
		}
		for( i = 0; i < rear ; i++ )
		{
			tempData[j++] = data[i] ;
		}
		rear = 0 ;
		front = length ;
		delete[] data ; 
		data = tempData ; 
	}

	data[front] = item ; 
	front = (front + 1) % queueMaxSize ;
	length++ ;
}


bool Queue::empty()
{
	if(length == 0) return true ;
	else return false ;
}


int Queue::dequeue()
{
	if(length == 0) return NULL_VALUE ;
	int item = data[rear] ;
	rear = (rear + 1) % queueMaxSize ;
	length-- ;
	return item ;
}


Queue::~Queue()
{
    if(data) delete[] data; 
    data = 0; 
}

//****************Queue class ends here************************

//****************Dynamic ArrayList class based************************
class ArrayList
{
	node * list;
	int length ;
	int listMaxSize ;
	int listInitSize ;
public:
	ArrayList() ;
	~ArrayList() ;
	int searchItem(node item) ;
    void insertItem(node item) ;
	void removeItem(node item) ;
	void removeItemAt(int position);
	node getItem(int position) ;
	int getLength();
	bool empty();
	void printList();
} ;


ArrayList::ArrayList()
{
	listInitSize = 2 ;
	listMaxSize = listInitSize ;
	list = new node[listMaxSize] ;
	length = 0 ;
}

void ArrayList::insertItem(node newitem)
{
	node * tempList ;
	if (length == listMaxSize)
	{
		listMaxSize = 2 * listMaxSize ;
		tempList = new node[listMaxSize] ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; 
        }
        delete[] list ; 
        list = tempList ; 
	};

	list[length] = newitem ; 
	length++ ;
}

int ArrayList::searchItem(node item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

void ArrayList::removeItemAt(int position) 
{
	if ( position < 0 || position >= length ) return ;
	list[position] = list[length-1] ;
	length-- ;
}


void ArrayList::removeItem(node item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return ; 
	removeItemAt(position) ;
}


node ArrayList::getItem(int position)
{
	if(position < 0 || position >= length) return node(-1,-1) ;
	return list[position] ;
}

int ArrayList::getLength()
{
	return length ;
}

bool ArrayList::empty()
{
    if(length==0)return true;
    else return false;
}

void ArrayList::printList()
{
    int i;
    for(i=0;i<length;i++)
        //pf("%d ", list[i]);
    pf("Current size: %d, current length: %d\n", listMaxSize, length);
}

ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}

//******************ArrayList class ends here*************************

// ***************** Min Heap Start
class Item
{
public:
	int data; //actual data that is stored
	ll key; //key value of the data, heap is constructed based on key //priority
};


class MinHeap
{
public:
	Item * A; //stores heap Items, e.g., nodes
	int heapLength;
	int * map;

	MinHeap() //constructor
	{
		A = new Item[MAX_HEAP_SIZE];
		map = new int[MAX_HEAP_SIZE];
		heapLength=0;
	}

	~MinHeap() //destructor
	{
		if(map) delete [] map;
		if(A) delete [] A;
		map = 0; //set to NULL after deletion
		A = 0; //set to NULL after deletion
	}

	void initialize(int v[], int n)
	{
		heapLength = n;
		for(int i=0; i<n; i++) //nodes are stored from index 1 instead of 0 in the heap
		{
			A[i+1].data = v[i];
			A[i+1].key = INF;
			map[v[i]] = i+1; //map tracks which vertex is stored at which heap node
		}
	}

    void insertItem(int data, ll key)
    {
        if(heapLength==MAX_HEAP_SIZE) throw 1;
        
        heapLength++;
        Item it;
        it.data = data; it.key=key;
        A[heapLength]=it;
        buHeapify(heapLength);
        
    }

	Item removeMin() 
	{
	    if(heapLength<1) throw 1;
	    Item ret = A[1];
	    A[1].key=INF;
	    heapify(1);
	    heapLength--;
	    return ret;

	}

	void updateKey(int data, ll key)
	{
		int pos=map[data];
		if(A[pos].key>key)
		{
			A[pos].key=key;
			buHeapify(pos);
		}
		else if(A[pos].key<key)
		{
			A[pos].key=key;
			heapify(pos);
		}

	}


    //This function returns the key value of a data stored in heap
	ll getKey(int data)
	{
		int i = map[data];
		return A[i].key;
	}

	void heapify(int i)
	{
		int l,r,smallest;
		while(1)
		{
			l=i+i;      //left child index
			r=l+1;    //right child index
			smallest=i;

			if(l>heapLength && r>heapLength)
				break; //nothing to do, we are at bottom
			else if(r>heapLength)
				smallest = l;
			else if(l>heapLength)
				smallest = r;
			else if( A[l].key < A[r].key )
				smallest = l;
			else
				smallest = r;

			if(A[i].key <= A[smallest].key)
				break;	//we are done heapifying
            else
			{
                //swap nodes with smallest child, adjust map array accordingly
				Item t;
				t=A[i];
				A[i]=A[smallest];
				map[A[i].data]=i; //map[A[smallest].data]=i; 
				A[smallest]=t;
				map[A[smallest].data]=smallest; //map[A[i].data]=smallest;
				i=smallest;
			}

		}
	}
	void buHeapify(int i)
	{
		
		if(i==1 || A[i/2].key <=A[i].key) return; 
		Item t;
		t=A[i];
		A[i]=A[i/2];
		map[A[i].data]=i;
		A[i/2]=t;
		map[A[i/2].data]=i/2;
		buHeapify(i/2);
		
	}

    void printHeap()
    {
        printf("Heap length: %d\n", heapLength);
        for(int i=1;i<=heapLength;i++)
        {
            printf("(%d,%lld) ", A[i].data, A[i].key);
        }
        printf("\n");
    }

	bool Empty()
	{
		if(heapLength==0) return true;
		else return false;
	}
};
//  ********************* Min Heap End


//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	ArrayList  * adjList ;
	int *col,*par, *dist;
	int time;
	int *inTime, *finTime;

public:
	Graph(bool dir = false);
	~Graph();
	void setnVertices(int n);
	void addEdge(int u, int v, int w);
	void removeEdge(int u, int v, int c);
	bool isEdge(int u, int v, int c);
    int getDegree(int u);
    bool hasCommonAdjacent(int u, int v);
    int getDist(int u, int v);
    void printGraph();
	void bfs(int source, bool s=true); 
	void dfs(); 
	void dfs_Visit(int u);
	int mst();
    int sbmst(node *par);
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	adjList = 0 ;
	directed = dir ; 
	
	col = 0;
	dist = 0;
	par = 0;
	inTime = 0;
	finTime = 0;
}

void Graph::setnVertices(int n)
{
	nVertices = n ;
	if(adjList!=0) delete[] adjList ; 
	adjList = new ArrayList[nVertices+1] ;
	
    col = new int[nVertices+1];
    par = new int[nVertices+1];
    dist = new int[nVertices+1];
    inTime = new int[nVertices+1];
    finTime = new int[nVertices+1];
}

void Graph::addEdge(int u, int v, int w)
{
    if(u<0 || v<0 || u>nVertices || v>nVertices) return; 
    if(u == v){
        pf("Loop in graph not allowed");
        return;
    }
    
    nEdges++ ;
	adjList[u].insertItem(node(v,w)) ;
	if(!directed) adjList[v].insertItem(node(u,w)) ;
}

void Graph::removeEdge(int u, int v, int w)
{
    if(u<0 || v<0 || u>nVertices || v>nVertices) return;
    
	adjList[u].removeItem(node(v,w));
    if(!directed) adjList[v].removeItem(node(u,w));
    nEdges--;
}

bool Graph::isEdge(int u, int v, int c)
{
    
    if(u<0 || v<0 || u>nVertices || v>nVertices) throw -1;
    int pos = adjList[u].searchItem(node(v,c));
    if(pos != NULL_VALUE){
        return true;
    }
    else return false;
}

int Graph::getDegree(int u)
{
    if(u<0 || u>nVertices) throw -1;
    else return adjList[u].getLength();
    
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    
    if(u<0 || v<0 || u>nVertices || v>nVertices)
	{
        throw -1;
    }
    
	int sz=adjList[u].getLength(), t, i;
    ffr(i,0,sz)
    {
        node tmp = adjList[u].getItem(i);
        if(adjList[v].searchItem(tmp)!=NULL_VALUE)
        {
            return true;
        }
    }
    return false;
}
#define min(a,b) a<b? a:b
#define max(a,b) a>b? a:b
#define bug(i) pf("ok %d\n", i)

int minimum(int a, int b)
{
	if(a<b) return a; else return b;
}

int maximum(int a, int b)
{
	if(a>b) return a; else return b;
}

int Graph::mst()
{
	bool vis[nVertices+1];
    node parent[nVertices+1];
    MinHeap pq;
    int b=0;
    //bug(b++);
    for(int i=1; i<=nVertices; i++)
    {
        dist[i] = INF;
        vis[i] = 0;
        parent[i].v = -1;
        //pq.insertItem(i,INF);
    }

    dist[1] = 0; vis[1] = 1; parent[1].setinnode(-1,0);
    pq.insertItem(1,0);
    //pq.updateKey(1,0);
    int weight = 0;
    while(!pq.Empty())
    {
        Item item = pq.removeMin();
        vis[item.data] = 1;
        int sz=getDegree(item.data), i;
        
        ffr(i,0,sz)
        {
        	node tmp=adjList[item.data].getItem(i);
            int v = tmp.v, w=tmp.w;
            //pf("v %d w %d\n", v, w);
            if(!vis[v] && dist[v]>w)
            {
            	//bug(b++);
                dist[v] = w;
                parent[v].setinnode(item.data,w);
                //pq.updateKey(v,w);
                pq.insertItem(v,w);
            }
        }
    }
    for(int i=2; i<=nVertices; i++)
    {
        if(parent[i].v==-1)
        {
            printf("The Given Graph isn't connected\n");
            return 0;
        }
    }
    printf("Minimum Spanning Tree for the given Graph:\n");
    for(int i=2; i<=nVertices; i++)
    {
        printf("%d %d\n",min(i,parent[i].v),max(parent[i].v,i));
        weight+= parent[i].w;
    }
    printf("Weight of MST is %d \n",weight);
    sbmst(parent);
}

int Graph::sbmst(node *P)
{
    node parent[nVertices+1];
    int weight = INF, b=0;
    for(int i=2; i<=nVertices; i++)
    {
        int u = i, v = P[i].v, c = P[i].w;
        bool vis[nVertices+1];
        node tmp[nVertices+1];
        for(int j=1; j<=nVertices; j++)
        {
            dist[j] = INF;
            vis[j] = 0;
            tmp[j].v = -1;
            //parent[j].v=-1;
        }
        
        
        MinHeap pq;
        pq.insertItem(1,0);
        dist[1] = 0; vis[1] = 1; tmp[1].setinnode(-1,0);
        while(!pq.Empty())
        {
        	
            Item item = pq.removeMin();
            vis[item.data] = 1;
            //pf("%d visited hoilo for u %d\n", item.data, u);
            int sz=getDegree(item.data);
            
			
        	//bug(b++);
        	
        	int j;
            ffr(j,0,sz)
            {
                node temp=adjList[item.data].getItem(j);
            	int vv = temp.v, w=temp.w;
            
                if( (minimum(vv,item.data)==minimum(u,v) ) && (maximum(vv,item.data) == maximum(u,v) ) && (c==w) ) 
                {
                //	pf("cont\n" ); 
				continue;
				}
                
                if( (!vis[vv] ) && (dist[vv]>w ))
                {
                	
                    dist[vv] = w;
                    pq.insertItem(vv,w);
                    tmp[vv].setinnode(item.data,w);
                }
            }
            
        }
        int w = 0;
        for(int j=2; j<=nVertices; j++)
        {
        	
            if(tmp[j].v==-1)
            {
                w = INF ;
                break;
            }
            else w += tmp[j].w;
        }
        
        if(w<weight)
        {
            for(int j=2; j<=nVertices; j++)
            {
                parent[j] = tmp[j];
                weight = w;
            }
        }
    }
    for(int j=2; j<=nVertices; j++)
    {
    	
        if(parent[j].v==-1 || weight == INF)
        {
            printf("There is no Second Best MST\n");
            return 0;
        }
    }
    printf("Second Best MST is:\n");
    for(int i=2; i<=nVertices; i++)
    {
        printf("%d %d\n",min(i,parent[i].v),max(parent[i].v,i));
    }
    printf("Weight of Second Best MST is : %d\n",weight);

}
/*
void Graph::bfs(int source, bool self)
{
    
    
    if(source >= nVertices || source <0)
	{
        throw -1;
    }
    
    if(!col) col = new int[nVertices];
    if(!par) par = new int[nVertices];
    if(!dist) dist = new int[nVertices];
	
	int i;
    ffr(i,0,nVertices)
    {
        col[i] = WHITE ;
        par[i] = -1 ;
        dist[i] = INFINITY ;
    }
    Queue q ;
    col[source] = GREY;
    dist[source] = 0 ;
    q.enqueue(source) ;
    int len, cur, v;
    
    while( !q.empty() )
    {
        cur = q.dequeue();
        len = getDegree(cur);
        ffr(i,0,len)
		{
            v = adjList[cur].getItem(i);
            if(col[v] == WHITE)
			{
                col[v] = GREY;
                dist[v] = dist[cur]+1;
                par[v] = cur;
                q.enqueue(v);
            }
        }
        col[cur] = BLACK;
        
    }
    if(self==false) return;
    pf("BFS running with source %d\n", source);
    ffr(i,0,nVertices)
    {
        pf("Vertex : %d, Color = %d, Distance = %d, Parent = %d\n",i,col[i],dist[i],par[i]);
    }
    return ;
}

void Graph::dfs()
{
	if(nVertices==0) throw -1;
    inTime = new int[nVertices];
    finTime = new int[nVertices];
    if(!col) col = new int[nVertices];
    if(!par) par = new int[nVertices];
    int i;
    ffr(i,0,nVertices)
	{
        col[i] = WHITE;
        par[i] = -1;

    }
    time = 0;
    ffr(i,0,nVertices)
	{
        if(col[i] == WHITE){
            dfs_Visit(i);
        }
    }
    pf("DFS Running\n");
    ffr(i,0,nVertices)
	{
        pf("Vertex: %d, Start time: %d End time: %d\n",i, inTime[i], finTime[i]);
    }
 }


void Graph::dfs_Visit(int u)
{
    time++;
    inTime[u] = time;
    col[u] = GREY;
    int len = getDegree(u), i;
    ffr(i,0,len)
	{
        int v = adjList[u].getItem(i);
        if(col[v] == WHITE){
            par[v] = u;
            dfs_Visit(v);
        }
    }
    col[u] = BLACK;
    time++;
    finTime[u] = time;
 }

int Graph::getDist(int u, int v)
{
    try
    {
    	bfs(u,false);
	}catch(int re)
	{
		if(re==-1) throw -1;
	}
    
    int distance = dist[v];
    if(distance==INFINITY) throw 1;
    return distance;
}
void Graph::printGraph()
{
    pf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        pf("%d:", i);
        for(int j=0; j<adjList[i].getLength();j++)
        {
            pf(" %d", adjList[i].getItem(j));
        }
        pf("\n");
    }
}*/

Graph::~Graph()
{
    if(adjList) delete []adjList;
	if(col) delete []col;
	if(par) delete []par;
	if(dist) delete [] dist;
	
    adjList = 0;
    nVertices = 0 ;
	nEdges = 0 ;
	col = 0;
	dist = 0;
	par = 0;
}


//**********************Graph class ends here******************************
using namespace std;

int main()
{
   	freopen("Input.txt", "r", stdin);
    int n,e;
    int cc=1;
    while(cin >> n >> e)
    {
	//	scanf("%d %d",&n,&e);
        Graph g;
        g.setnVertices(n);
        int i;
        ffr(i,0,e)
        {
            int u,v,c;
            scanf("%d %d %d",&u,&v,&c);
            g.addEdge(u,v,c);
        }
        pf("Case %d:\n", cc++);
        g.mst();

    }
    return 0;
}

/*
9
14
1 2 4
1 8 8
2 3 8
2 8 11
3 4 7
3 6 4
3 9 2
4 5 9
4 6 14
5 6 10
6 7 2
7 8 1
7 9 6
8 9 7

*/
