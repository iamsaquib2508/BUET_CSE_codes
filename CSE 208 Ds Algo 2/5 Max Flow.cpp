#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define mm(a,b) memset(a,b,sizeof(a))
#define pf printf
#define ll long long int
#define pb push_back
#define MX 1000

using namespace std;

vector<int> G[MX];
int par[MX];
int A[MX][MX];
int Or[MX][MX];

void dfs(int s, bool vis[])
{
	int i, sz=G[s].size();
	vis[s]=true;
	ffr(i,0,sz)
	{
		int v=G[s][i];
		if(!vis[v] && A[s][v]>0) 
		{
			dfs(v,vis);
			
		}
	}
	return;
}

bool pathexists(int s, int t)
{
    int i;
    bool vis[MX];
    mm(vis,0);
    queue<int> q;
    q.push(s);
    vis[s]=true;
    par[s]=-1;
    while(!q.empty())
    {
        int u=q.front(), sz=G[u].size(), v; q.pop();
        ffr(i,0,sz)
        {
            v=G[u][i];
            if(vis[v]) continue;
            if(A[u][v]>0)
            {
            	par[v]=u;
                if(v==t) return true;
                q.push(v);
                vis[v]=true;
            }
        }

    }
    return false;
}

int sendflow(int s, int t)
{
    int cflow=1e6, i, sz, u, v, cur;
    
    cur=t;
    while(cur!=s)
    {
    	u=par[cur];
    	cflow=min(cflow,A[u][cur]);
    	cur=u;
    	
	}
	cur=t;
    while(cur!=s)
    {
    	u=par[cur];
    	A[u][cur]-=cflow;
    	A[cur][u]+=cflow;
    	cur=u;
	}
	return cflow;

}


int main()
{
    //freopen("Flow1.txt", "r", stdin);
    int cc=1, t;
    cin >> t;
    while(t--)
    {
    	int i, u, v, cap, m, n, ans=0, src, dest;
    	mm(A,0);
    	mm(par,-1);
    	ffr(i,0,MX) G[i].clear();
        cin >> n >> src >> dest >> m;
        ffr(i,0,m)
        {
            cin >> u >> v >> cap;
            G[u].pb(v);
            //G[v].pb(u);
            //if(cap>0)
            Or[u][v]=cap;
			A[u][v]=cap;

        }
        //cin >> src >> dest;
        pf("%d\n", Or[0][1]);
        while(pathexists(src,dest)) ans+=sendflow(src, dest);
        pf("Case %d: %d\n", cc++, ans);
        
        /*bool viss[n];
    	mm(viss,0);
    	dfs(src, viss);

    	for (int i = 0; i < n; i++)
      		for (int j = 0; j < n; j++)
         		if (viss[i] && !viss[j] && A[i][j]>0)
              cout << i << " - " << j << endl;*/
        ffr(i,0,n)
        {
        	int szz=G[i].size(), j;
        	ffr(j,0,szz)
        	{
        		v=G[i][j];
        		if(Or[i][v]-A[i][v]>0)
        		pf("O-A:: From %d to %d: %d\n", i, v, Or[i][v]-A[i][v]);
        		//pf("O:: %d to %d: %d\n",i, G[i][j], Or[i][j]);
        		//pf("A:: %d to %d: %d\n",i, G[i][j], A[i][j]);
        	}
        }
        
    }
    return 0;
}

/*
3

6 0 5 8
0 1 11
0 2 12
2 1 1
1 3 12
2 4 11
4 5 4
4 3 7
3 5 19

6 0 5 9
0 1 16
0 2 13
2 1 4
1 3 12
4 3 7
3 2 9
3 5 20
4 5 4
2 4 14

4 0 3 5
0 1 1000
0 2 1000
1 3 1000
2 3 1000
1 2 1
*/
