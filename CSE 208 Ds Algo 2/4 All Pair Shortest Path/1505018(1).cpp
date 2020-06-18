#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<=b;i++)
#define pf printf
#define ll long long int
#define mm(a,b) memset(a,b,sizeof(a))
#define Vmax 1000
#define inf 1e6

using namespace std;


int Ar[Vmax][Vmax];
int d[Vmax][Vmax];
int Pre[Vmax][Vmax];
int n, e;

void floyd()
{
	int i, j, k;
	
	ffr(i,1,n)
	{
		ffr(j,1,n) 
		{
			d[i][j]=Ar[i][j];
		}
	}
	ffr(k,1,n)
	{
		ffr(i,1,n)
		{
			ffr(j,1,n)
			{
				if(d[i][k]+d[k][j]<d[i][j])
				{
					d[i][j]=d[i][k]+d[k][j];
					Pre[i][j]=Pre[k][j];
				}
			}
		}
	}
	return;
}

void print()
{
	int i, j;
	pf("Shortest Distance:\n");
	ffr(i,1,n)
	{
		ffr(j,1,n) 
		{
			if(d[i][j]==inf) pf("%5s ", "inf");
			else pf("%5d ", d[i][j]);
		}
		pf("\n");
	}
	pf("Predecessor:\n");
	ffr(i,1,n)
	{
		ffr(j,1,n) 
		{
			if(Pre[i][j]==0) pf("%5s ", "inf");
			else pf("%5d ", Pre[i][j]);
		}
		pf("\n");
	}
	
	
	return;
}

int main()
{
	int cc=1, T;
	cin >> T;
	//T=1;
	while(T--)
	{
		int i, a, b, c, j;
		ffr(i,1,Vmax)
		{
			ffr(j,1,Vmax) 
			{
				Ar[i][j]=inf;
			}
		
		}
		
		cin >> n >> e;
		ffr(i,1,e)
		{
			cin >> a >> b >> c;
			Ar[a][b]=c;
			Pre[a][b]=a;
		}
		ffr(i,1,n) Ar[i][i]=0;
		pf("Case %d:\n", cc++);
		floyd();
		print();
	}
	return 0;
}

/*
IN:

2

5 8
1 3 6
1 4 3
2 1 3
3 4 2
4 2 1
4 3 1
5 2 4
5 4 2

4 4
1 2 3
2 3 1
4 3 10
4 1 5


OUT:

Case 1:
Shortest Distance:
    0     4     4     3   inf
    3     0     7     6   inf
    6     3     0     2   inf
    4     1     1     0   inf
    6     3     3     2     0
Predecessor:
  inf     4     4     1   inf
    2   inf     4     1   inf
    2     4   inf     3   inf
    2     4     4   inf   inf
    2     4     4     5   inf
Case 2:
Shortest Distance:
    0     3     4   inf
  inf     0     1   inf
  inf   inf     0   inf
    5     8     9     0
Predecessor:
  inf     1     2     1
    2   inf     2     1
    2     4   inf     3
    4     1     2   inf

*/
