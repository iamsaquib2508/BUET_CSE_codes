#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define Infinity 1e9
#define NULL_VALUE 1e8

using namespace std;

class Edge{

public:
    int vert;
    double wt;
    Edge(){
        vert = 0;
        wt = 0;
    }
    Edge(int v, double w){
        vert = v;
        wt = w;
    }

    ~Edge(){
        vert = 0;
        wt = 0;
    }

};

//****************Dynamic ArrayList class based************************
class ArrayList
{
	Edge * list;
	int length ;
	int listMaxSize ;
	int listInitSize ;
public:
	ArrayList() ;
	~ArrayList() ;
	int searchItem(Edge item) ;
    void insertItem(Edge item) ;
	void removeItem(Edge item) ;
	void removeItemAt(int item);
	Edge getItem(int position) ;
	int getLength();
	bool empty();
	void printList();
	int getWeight(int v);
} ;


ArrayList::ArrayList()
{
	listInitSize = 2 ;
	listMaxSize = listInitSize ;
	list = new Edge[listMaxSize] ;
	length = 0 ;
}

void ArrayList::insertItem(Edge newitem)
{
	Edge * tempList ;
	if (length == listMaxSize)
	{
		
		listMaxSize = 2 * listMaxSize ;
		tempList = new Edge[listMaxSize] ;
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

int ArrayList::searchItem(Edge item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if(list[i].vert == item.vert) return i;
	}
	return NULL_VALUE;
}

void ArrayList::removeItemAt(int position)
{
	if ( position < 0 || position >= length ) return ;
	list[position] = list[length-1] ;
	length-- ;
}


void ArrayList::removeItem(Edge item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return ; 
	removeItemAt(position) ;
}


Edge ArrayList::getItem(int position)
{
    Edge nulledge;

	if(position < 0 || position >= length) return nulledge ;
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
        printf("%d (%lf)", list[i].vert,list[i].wt);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);
}
int ArrayList::getWeight(int v){

    for(int i = 0; i < length; i++){
        if(list[i].vert == v){
            return list[i].wt;
        }
    }
    cout << "Search Finished, Edge doesn't exist" << endl;
    return -1;
}


ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}

//******************ArrayList class ends here*************************

//******************MinHeap class ends here***************
class Edges{

public:
    int u;
    int v;
    double w;
    Edges()
	{
        u = 0;
        v = 0;
        w = 0;
    }
    void operator = (Edges o2){
        u = o2.u;
        v = o2.v;
        w = o2.w;
        
    }
};


//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	ArrayList  * adjList;
	int *color,*parent, *dist;
	int time;
	int *inTime, *finalTime;
	//define other variables required for bfs such as color, parent, and dist
	//you must use pointers and dynamic allocation

public:
	Graph(bool dir = false);
	~Graph();
	void setnVertices(int n);
	void addEdge(int u, int v,int wt);
	void removeEdge(int u, int v);
	bool isEdge(int u, int v);
    int getDegree(int u);
    bool hasCommonAdjacent(int u, int v);
    int getDist(int u, int v);
    void printGraph();
    void MST_PRIM(int root);
    void Bellman_Ford(Edges *EdgeArray, int source);

};

void Graph::Bellman_Ford(Edges *EdgeArray, int source){

    double *d = new double[nVertices+1];
	int i, j, u, v;
    int *par = new int[nVertices+1];
    
    ffr(i,0,nVertices) //here
	{
        d[i] = Infinity;
        par[i] = i;
    }
    //here
    d[i] = Infinity;
        par[i] = i;
        
    d[source] = 0;
    
    ffr(i,1,nVertices)
	{
        ffr(j,0,nEdges)
		{
            u = EdgeArray[j].u;
            v = EdgeArray[j].v;
			
            if(d[v] > d[u] + EdgeArray[j].w)
			{
                d[v] = d[u] + EdgeArray[j].w;
                par[v]=u;
            }
        }
    }
    
    ffr(j,0,nEdges)
	{
        u = EdgeArray[j].u;
        v = EdgeArray[j].v;
        if(d[v] > d[u] + EdgeArray[j].w)
		{
            pf("Contains negative cycle\n");
            return;
        }
    }
    
    ffr(i,0,nVertices) //here
	{
        pf("Node %d Parent %d Weight %.3lf\n", i, par[i], d[i] );
    }
    //pf("Node %d Parent %d Weight %.3lf\n", i, par[i], d[i] ); //here
    
	return;
}

Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	adjList = 0 ;
	directed = dir ;

}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;
	if(adjList!=0) delete[] adjList ; //delete previous list
	adjList = new ArrayList[nVertices] ;
}

void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        printf("%d:", i+1);
        for(int j=0; j<adjList[i].getLength();j++)
        {
            printf(" %d (%d) ", adjList[i].getItem(j).vert+1, adjList[i].getItem(j).wt);
        }
        printf("\n");
    }
}
void Graph::addEdge(int u, int v,int w)
{
    if(u<0 || v<0 || u>nVertices || v>nVertices) return; //vertex out of range
    if(u == v){
        printf("Loop in graph not allowed");
        return;
    }
    nEdges++ ;
    /*Edge e1(u,w), e2(v,w);
	adjList[u-1].insertItem(e2) ;
	if(!directed) adjList[v-1].insertItem(e1) ;*/
}

void Graph::removeEdge(int u, int v)
{
    if(u<0 || v<0 || u>nVertices || v>nVertices) return;
    int weight = adjList[u].getWeight(v);
    Edge e1(u,weight),e2(v,weight);
    adjList[u].removeItem(e2);
    if(!directed) adjList[v].removeItem(e1);
    nEdges--;
}


Graph::~Graph()
{
    //if(adjList) delete []adjList;
    adjList = 0;
    nVertices = 0 ;
	nEdges = 0 ;
	//if(parent) delete []parent;
	//if(dist) delete [] dist;
	//if(color) delete []color;
	color = 0;
	dist = 0;
	parent = 0;
    //cout << "Destroying Graph" << endl;
}
//**********************Graph class ends here******************************

int main()
{
	//freopen("Test.txt", "r", stdin);
    int n, m;
    Graph g = new Graph(true);

    
    scanf("%d %d", &n, &m);
    
    g.setnVertices(n);
    Edges *EdgeArray = new Edges[m];
    
    int u, v, wt, i;
    ffr(i,0,m)
	{
        scanf("%d %d %d", &u, &v, &wt);
        
        
        g.addEdge(u,v,wt);
        EdgeArray[i].u = u; EdgeArray[i].v = v; EdgeArray[i].w = wt;
        
    }
    
    int source;
    cout << "The Source? : " << endl;
    scanf("%d",&source);
    g.Bellman_Ford(EdgeArray,source);
    return 0;
}
