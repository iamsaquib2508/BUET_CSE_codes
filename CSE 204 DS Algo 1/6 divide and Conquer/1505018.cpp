#include<bits/stdc++.h>
#define ffr(i,a,b) for(i=a;i<b;i++)
#define pf printf
#define sf(a) scanf("%d", &a)
#define mini(L,R) (L<R)?L:R

using namespace std;

class point
{
public:
    double x, y;
    point()
    {

    }
    point(double xx, double yy)
    {
        x=xx, y=yy;
    }
    double dist(point p)
    {
        return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
    }
    void show()
    {
        pf("(%.2lf,%.2lf)\n", x, y);
    }
    void setter(double xx, double yy)
    {
        x=xx, y=yy;
    }
};

class ppair
{
    point a, b;
public:
    ppair(){
    }
    void setter(point aa, point bb)
    {
        a=aa, b=bb;
    }
    void show()
    {
        a.show();
        b.show();
    }
};

double slicefind(point slice[], int n, double d, ppair &find)
{
    double ans=d;
    int i, j;
    ffr(i,0,n)
    {
        for(j=1;j<16 && i+j<n; j++)
        {
            double temp=slice[i].dist(slice[i+j]);
            if(temp<ans)
            {
                ans=temp;
                find.setter(slice[i], slice[i+j]);
            }

        }
    }
    return ans;
}


double closest(point arrx[], point arry[], int n, ppair &find)
{
    //pf("In closest: N=%d\n", n);
    int i, j;
    if(n<4)
    {
        double ans=arrx[0].dist(arrx[1]);
        find.setter(arrx[0], arrx[1]);
        ffr(i,0,n)
        {
            ffr(j,i+1,n)
            {
                double tm=arrx[i].dist(arrx[j]);
                if(tm<ans)
                {
                    ans=tm;
                    find.setter(arrx[i], arrx[j]);
                }
            }
        }
        //pf("In closest: N=%d ans=%lf\n", n, ans);
        //find.show();
        return ans;
    }

    int mid=n/2;
    ppair LL, RR;
    point Ly[n], Ry[n];
    int r=0, l=0;
    ffr(i,0,n)
    {
        if(arry[i].x<arrx[mid].x) Ly[l++]=arry[i];
        else if(arry[i].x>arrx[mid].x) Ry[r++]=arry[i];
        else
        {
            if(arry[i].y<arrx[mid].y) Ly[l++]=arry[i];
            else if(arry[i].y>arrx[mid].y) Ry[r++]=arry[i];
        }
    }
    double L=closest(arrx, Ly, mid, LL);
    double R=closest(arrx+mid, Ry, n-mid, RR);
    double d;
    if(L<R)
    {
        d=L; find=LL;
    }
    else
    {
        d=R; find=RR;
    }

    point slice[n];
    i=j=0;
    ffr(i,0,n)
    {
        if(abs(arry[i].x-arrx[mid].x)<d)
        {
            slice[j++]=arry[i];
        }
    }
    return slicefind(slice, j, d, find);

}

bool cmp(point a, point b)
{
    if(a.x<b.x) return 1;
    if(a.x>b.x) return 0;
    if(a.y<b.y) return 1;
    else return 0;
}

bool cmpy(point a, point b)
{
    return a.y<b.y;
}

double closestwr(point arrx[], int n, ppair &ans)
{
    point *arry;
    arry=(point*) malloc(n*sizeof(point));
    int i;
    ffr(i,0,n) arry[i]=arrx[i];
    sort(arrx,arrx+n,cmp);
    sort(arry,arry+n,cmpy);

    return closest(arrx, arry, n, ans);
}

int main()
{
    int n, i;
    double a, b;
	while(1)
	{
		pf("Enter number of points (-1 to exit): ");
    	sf(n);
    	if(n==-1) break;
    	if(n<2) 
    	{
    		pf("Please Enter atleast 2 points\n");
    		continue;
    	}
    	pf("Enter %d points\n", n);
    	point arr[n];
    	ffr(i,0,n)
    	{
        	scanf("%lf %lf", &a, &b);
        	arr[i].setter(a,b);
    	}
    	ppair ans;
    	pf("Closest pair of points are %lf units far. Points are:\n", closestwr(arr, n, ans));
    	ans.show();
	}
    return 0;
}

/*
5
-7 -2
-4 1
0 0
1 1
4 5

12
0 0
1 5
2 1
3 4
4 1
5 2
5 1
5 3
5 0
7 1
8 0
8 3
*/
