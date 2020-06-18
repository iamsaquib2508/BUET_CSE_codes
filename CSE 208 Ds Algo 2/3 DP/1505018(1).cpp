#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define MAXX 10000


using namespace std;

int n,x;

int pathdir[MAXX][30];


double P[MAXX];
int cost[MAXX];


double probcnt(double r,int m)
{
    return 1 - pow((1-r),m);
}


void ff(int sz)
{
		int i, used=0;
        ffr(i,0,sz) 
        {
        	pf("%d ", pathdir[used][i]);
        	used+=(pathdir[used][i]*cost[i]);
		}
            
        pf("\n");
}

double A[MAXX][30];

double dp(int used,int stage)
{
    
    if(used>x) return 0;
    if(stage==n) return 1;
    
    if(A[used][stage]!=-1) return A[used][stage];
    double cur=0, cur2;
    
    for(int i=1;i*cost[stage]+used<=x; i++)
    {
        cur2 = dp(used+i*cost[stage],stage+1)*probcnt(P[stage],i);
        
		if(cur<cur2)
        {
            cur = cur2;
            pathdir[used][stage]=i;
        }
    }
   
    A[used][stage] = cur;
    
    return A[used][stage];

}
int main()
{
    int cc=1, tt;
    //freopen("take.txt", "r", stdin);
    cin >> tt;
    while(tt--)
    {
		scanf("%d %d",&x,&n); 
    	int i, j;
    	ffr(i,0,MAXX) 
        	ffr(j,0,30) A[i][j] = -1;
		ffr(i,0,n) cin >> P[i]; 
		ffr(i,0,n) cin >> cost[i];
        
    	pf("\nCase %d: %lf\n", cc++, dp(0,0));
    	ff(n);
    
	}
    
	return 0; 
}

/*

3

1000 3
.2 .8 .95
200 100 500

800 3
0.7 0.8 0.9
100 100 200

800 3
0.7 0.8 0.9
200 100 100


*/
