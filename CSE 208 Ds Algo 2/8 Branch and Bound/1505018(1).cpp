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


vector<int> vec[100];

bool cmp(vector<int> a, vector<int> b)
{
    
    return a.size() > b.size();
}

class Node
{
    public:
    int level, got, bound;
    string model;
    string modelfin;
    
    bool operator<(const Node b) const
    {
    	bound<b.bound;
	}
};

#define INFT 1e7

int bound(Node u, int no_of_subsets, int universal)
{
    universal++;
    int cc=0, cur, i;
    ffr(i,1,universal)
    {
    	if(u.model[i]=='0') cc++;
	}
    int got_bound = u.got;
 	int ret;
    if(u.level==no_of_subsets-1)
    {
    	if(cc==0) ret= got_bound;
    	else ret= INFT;
	}
 	else
 	ret= got_bound+ceil(cc*1.0/vec[u.level+1].size());
 	
 	
 	return ret;
}


Node setcover( int no_of_subsets, int universal)
{
    
    priority_queue<Node> Q;
    Node u, v;
 
    int i, sz;
    u.level = -1;
    u.got = 0;
	
	u.model='-';
    ffr(i,0,universal) u.model+='0';
    u.modelfin='-';
    ffr(i,0,no_of_subsets) u.modelfin+='0';
    Q.push(u);
	
	
    int mingot = no_of_subsets;
    Node ans;
    while (!Q.empty())
    {
        u = Q.top();
        Q.pop();
 
        
        if (u.level == no_of_subsets-1)
        {
        	if(mingot>u.got) 
        	{
        		mingot=u.got;
        		ans=u;
			}
        	continue;
		}

        v.level = u.level + 1;
        v.got = u.got + 1;
		v.model=u.model;
		v.modelfin=u.modelfin;
		v.modelfin[v.level]='1';
        sz=vec[v.level].size();
        ffr(i,0,sz)
        {
        	v.model[vec[v.level][i]]='1';
		}
 
        
        v.bound = bound(v, no_of_subsets, universal);
        
        if (v.bound < mingot) 
            Q.push(v);
 
        
        v.model=u.model;
        v.modelfin=u.modelfin;
        v.got = u.got;
        v.bound = bound(v, no_of_subsets, universal);
        
        if (v.bound < mingot)
            Q.push(v);
    }
 
    return ans;
}


int main()
{
	int N, k, i, j, A, a;
	//freopen("ININ.txt", "r", stdin);
	cin >> N;
	cin >> k;
	ffr(i,0,k)
	{
		cin >> A;
		ffr(j,0,A)
		{
			cin >> a;
			vec[i].pb(a);
		}
	}
	sort(vec,vec+k,cmp);
	
	Node ans=setcover(k, N);
	cout << endl << ans.got << endl;
	ffr(i,0,k)
	{
		if(ans.modelfin[i]=='1')
		{
			cout << endl;
			A=vec[i].size();
			ffr(j,0,A)
			{
				cout << vec[i][j] << " ";
			}
		}
	}
	return 0;
	
	
}

/*
6 4
3 1 2 3
2 2 4
2 3 4
3 4 5 6
*/
