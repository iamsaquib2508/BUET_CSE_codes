#include<stdio.h>
#include<stdlib.h>
#define NULL_VALUE -999999
#define INFINITY 999999
#define WHITE 1
#define GREY 2
#define BLACK 3
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define sf scanf

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
		queueMaxSize = 2 * queueMaxSize ;
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
	int * list;
	int length ;
	int listMaxSize ;
	int listInitSize ;
public:
	ArrayList() ;
	~ArrayList() ;
	int searchItem(int item) ;
    void insertItem(int item) ;
	void removeItem(int item) ;
	void removeItemAt(int position);
	int getItem(int position) ;
	int getLength();
	bool empty();
	void printList();
} ;


ArrayList::ArrayList()
{
	listInitSize = 2 ;
	listMaxSize = listInitSize ;
	list = new int[listMaxSize] ;
	length = 0 ;
}

void ArrayList::insertItem(int newitem)
{
	int * tempList ;
	if (length == listMaxSize)
	{
		listMaxSize = 2 * listMaxSize ;
		tempList = new int[listMaxSize] ;
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

int ArrayList::searchItem(int item)
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


void ArrayList::removeItem(int item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return ; 
	removeItemAt(position) ;
}


int ArrayList::getItem(int position)
{
	if(position < 0 || position >= length) return NULL_VALUE ;
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
        pf("%d ", list[i]);
    pf("Current size: %d, current length: %d\n", listMaxSize, length);
}

ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}

//******************ArrayList class ends here*************************

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
	void addEdge(int u, int v);
	void removeEdge(int u, int v);
	bool isEdge(int u, int v);
    int getDegree(int u);
    bool hasCommonAdjacent(int u, int v);
    int getDist(int u, int v);
    void printGraph();
	void bfs(int source, bool s=true); 
	void dfs(); 
	void dfs_Visit(int u);
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
	adjList = new ArrayList[nVertices] ;
}

void Graph::addEdge(int u, int v)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return; 
    if(u == v){
        pf("Loop in graph not allowed");
        return;
    }
    
    nEdges++ ;
	adjList[u].insertItem(v) ;
	if(!directed) adjList[v].insertItem(u) ;
}

void Graph::removeEdge(int u, int v)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return;
    
	adjList[u].removeItem(v);
    if(!directed) adjList[v].removeItem(u);
    nEdges--;
}

bool Graph::isEdge(int u, int v)
{
    
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) throw -1;
    
    
	
    int pos = adjList[u].searchItem(v);
    if(pos != NULL_VALUE){
        return true;
    }
    else return false;
}

int Graph::getDegree(int u)
{
    if(u<0 || u>=nVertices) throw -1;
    else return adjList[u].getLength();
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    
    if(u<0 || v<0 || u>=nVertices || v>=nVertices)
	{
        throw -1;
    }
	int sz=adjList[u].getLength(), t, i;
    ffr(i,0,sz)
	{
        t = adjList[u].getItem(i);
        if(adjList[v].searchItem(t) != NULL_VALUE){
            return true;
        }
    }
    return false;


}


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
}

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



int main()
{
	
	//freopen("inp.txt", "r", stdin);
    int n;
    Graph g;
    pf("Enter number of vertices: ");
    sf("%d", &n);
    g.setnVertices(n);

    while(1)
    {
        pf("1. Add edge. 2. Remove edge 3. Is edge \n");
        pf("4. GetDegree. 5. Print Graph  6. Has Common Adjacent Vertex\n");
        pf("7. GetDistance 8. DFS 9. BFS 10. Exit\n");

        int ch;
        sf("%d",&ch);
        if(ch==1)
        {
            int u, v;
            sf("%d %d", &u, &v);
            g.addEdge(u, v);
        }
        else if(ch==2)
        {
            int u,v;
            sf("%d %d", &u, &v);
            g.removeEdge(u,v);

        }
        else if(ch==3)
        {
            int u,v;
            sf("%d %d", &u, &v);
            bool k;
            try
            {
            	k=g.isEdge(u,v);
			}
			catch(int i)
			{
				if(i==-1) pf("Invalid query\n");
				continue;
			}
            
            
            if(k) pf("The vertices %d and %d have common edges\n",u,v);
            else pf("The vertices %d and %d don't have edges\n",u,v);

        }
        else if(ch == 4){
            int u;
            sf("%d",&u);
            int t;
			try
			{
				t=g.getDegree(u);;
			} 
			catch(int i)
			{
				if(i==-1) pf("Vertex %d does not exist\n", u);
				continue;
			}
            pf("The degree of the vertex %d : %d\n",u,t);

        }
        else if(ch==5)
        {
            g.printGraph();
        }
        else if(ch==6)
        {
            int u,v;
            sf("%d %d", &u, &v);
            bool t;
            try
			{
            	t = g.hasCommonAdjacent(u,v);
			}catch(int i)
			{
				if(i==-1) pf("Invalid query\n");
				continue;
			}
			
            if(t) pf("The vertices %d and %d have common adjacent Vertex\n",u,v);
            else pf("The vertices %d and %d don't have common adjacent Vertex\n",u,v);
        }
        else if(ch == 7){
            int u, v, k;
            sf("%d %d", &u, &v);
            try
            {
            	k=g.getDist(u,v);
			}catch(int er)
			{
				if(er==-1) pf("Invalid query\n");
				else if(er==1) pf("%d and %d are not in the same component\n", u, v);
				continue;
			}
            
            pf("The shortest distance between the vertex %d and %d is: %d\n",u,v,k);
        }
        else if(ch == 8)
		{
            try
            {
            	g.dfs();
			}catch(int er)
			{
				if(er==-1) pf("No edges in the graph\n");
			}
        }
        else if(ch == 9)
		{
            int x;
            sf("%d", &x);
            try
            {
            	g.bfs(x);
			}catch(int er)
			{
				if(er==-1) pf("%d vertex is not the graph\n", x);
			}
        }
        else if(ch==10) break;
        
    }
    return 0;

}
