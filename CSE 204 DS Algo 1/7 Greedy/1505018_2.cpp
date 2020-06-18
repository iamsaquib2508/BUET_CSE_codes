#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define mini(a,b) a<b? a:b

using namespace std;

class obj
{
public:	
	double w, val, rate;
	int ori;
	obj(){
	}
	
	void sett(double ww, double vv, int orii)
	{
		w=ww, val=vv, rate=val/w, ori=orii;
	}
	void print()
	{
		pf("%.4lf %.4lf\n", w, val);
	}
	bool operator<(obj b)
	{
		return rate>b.rate;
	}
};

obj* sortt(obj* a, int n)
{
	if(n==1) return a;
	int mid=n/2;
	obj *L, *R;
	L=sortt(a,mid);
	R=sortt(a+mid,n-mid);
	
	obj *aa=new obj[n];
	int i, li, ri;
	li=ri=0;
	ffr(i,0,n)
	{
		if(li==mid) aa[i]=R[ri++];
		else if(ri==n-mid) aa[i]=L[li++];
		else if(L[li]<R[ri]) aa[i]=L[li++];
		else aa[i]=R[ri++];
	}
	return aa;
	
}


int main()
{
	int i, n;
	obj *a, *aa;
	cin >> n;
	//a = new obj[n];
	aa= new obj[n];
	
	double w, v;
	ffr(i,0,n)
	{
		cin >> w >> v;
		aa[i].sett(w,v,i);
	}
	a=sortt(aa,n);
	//sort(a,a+n);
	//ffr(i,0,n) a[i].print();
	pf("\n");
	cin >> w;
	double s=0;
	i=0;
	while(w>0)
	{
		if(i==n) break;
		obj b=a[i];
		double curw=b.w, cost;
		curw=mini(w,curw);
		w-=curw;
		
		cost=curw*b.rate;
		s+=cost;
		i++;
	}
	pf("\n%.4lf\n", s);
	int j=0;
	double ss=0;
	ffr(j,0,i)
	{
		pf("%d ", a[j].ori+1);
		ss+=a[j].val;
		if(j+1==i && i!=n)
		{
			pf("%.4lf %.4lf\n", (ss-s)/a[j].rate, (ss-s));
		}
		else a[j].print();
	}
	return 0;
}

/*
5
10 50
4 60
20 40
3 21
8 48
20

out:
154.0000
2 4.0000 60.0000
4 3.0000 21.0000
5 8.0000 48.0000
1 5.0000 25.0000
*/

