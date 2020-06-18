#include<stdio.h>
#include<stdlib.h>
#define NULL_VALUE -999999
#define INFINITY 999999
#define WHITE 1
#define GREY 2
#define BLACK 3
#include<iostream>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define sf scanf


using namespace std;

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
    data = 0; //set to NULL
}

//****************Queue class ends here************************


//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	int ** matrix ; 
	
	int *par,*col,*dist;
	int *inTime, *finTime;
	int time;

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
	void bfs(int source, bool self=true); 
	void dfs();
	void dfs_Visit(int u);
	void showMatrix();
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	matrix = 0 ;
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
	int i, j;
    matrix = new int*[nVertices];
    ffr(i,0,nVertices)
    {
        matrix[i] = new int[nVertices];
        ffr(j,0,nVertices)
            matrix[i][j] = 0; 
    }

}

void Graph::addEdge(int u, int v)
{
    //write your code here
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    if(u==v) return;
    
    matrix[u][v] = 1;
    if(!directed) matrix[v][u] = 1;
    nEdges++;

}

void Graph::removeEdge(int u, int v)
{
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;

    if(u == v)
	{
        
        return;
    }
    matrix[u][v] = 0;
    if(!directed) matrix[v][u] = 0;
    nEdges--;
}

bool Graph::isEdge(int u, int v)
{
    if(u<0 || u>=nVertices || v<0 || v>=nVertices)
	{
        throw -1;
    }
    if(matrix[u][v]) return true;
    else return false;
}

int Graph::getDegree(int u)
{
    if(u<0 || u>=nVertices)
	{
        
        throw -1;
    }
    int cnt = 0, i;
    ffr(i,0,nVertices)
	{
	
        if(matrix[u][i]){
            cnt++;
        }
    }
    return cnt;

}

bool Graph::hasCommonAdjacent(int u, int v)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices){
        throw -1;
    }
    int i;
	ffr(i,0,nVertices)
	{
        if(matrix[u][i] == matrix[v][i])
		{
            if(matrix[u][i] == 1)
			{
                return true;
            }

        }
    }
    return false;

}
void Graph::showMatrix()
{
	int i, j;
    ffr(i,0,nVertices)
	{
        ffr(j,0,nVertices)
		{
            pf("%3d ", matrix[i][j]);
        }
        pf("\n");
    }

}
void Graph::bfs(int source, bool self)
{
	if(source<0 || source >= nVertices) throw -1;
    
    int cur,v;
    int len;
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
    while( !q.empty() )
    {
        cur = q.dequeue();
        len = getDegree(cur);
        ffr(i,0,nVertices)
		{
            if(matrix[cur][i] == 1)
			{
                v = i;
                if(col[v] == WHITE)
				{
                    col[v] = GREY;
                    dist[v] = dist[cur]+1;
                    par[v] = cur;
                    q.enqueue(v);
                }
            }
        }
        col[cur] = BLACK;
    }
    if(self)
    {
    	
		pf("BFS running with source %d\n", source);
    	ffr(i,0,nVertices)
	
        pf("Vertex : %d, Color = %d, Distance = %d, Parent = %d\n",i,col[i],dist[i],par[i]);
    
	}
}


void Graph::dfs()
{

    if(!inTime) inTime = new int[nVertices];
    if(!finTime) finTime = new int[nVertices];
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
    pf("DFS running\n");
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
    int i;
    ffr(i,0,nVertices)
	{
        if(matrix[u][i] == 1)
		{
            int v = i;
            if(col[v] == WHITE){
                par[v] = u;
                dfs_Visit(v);
            }
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
	}
	catch(int i)
	{
		if(i==-1) throw -1;
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
        for(int j=0; j<nVertices;j++)
        {
            if(matrix[i][j]==1)
                pf(" %d", j);
        }
        pf("\n");
    }
}

Graph::~Graph()
{
    if(matrix) delete [] matrix;
    if(col) delete []col;
    if(par) delete []par;
    if(dist) delete []dist;
    if(inTime) delete []inTime;
    if(finTime) delete []finTime;
    
	nVertices = 0;
    nEdges = 0;
    time = 0;
    col = 0;
    par = 0;
    dist = 0;
    inTime = 0;
    finTime = 0;
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
