
package pkg1505018;



import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.Scanner;


class worker implements Runnable
{
    Thread t;
    WorkItem item;
    Queue st;
    int rem_works;
    int tn; 
    matrix res;
    
    worker(Queue q, matrix n, int rem, int tt)
    {
        t=new Thread(this, " ");
        st=q;
        res=n;
        rem_works=rem;
        tn=tt;
        t.start();
    }
    @Override
    public void run() 
    {
        
        while(rem_works>0) 
        {
            item=st.top();
            int len=item.a.length, sum=0, i;
            for(i=0;i<len;i++)
            {
                sum+= (item.a[i]*item.b[i]);
            }
            
            res.mat[item.i][item.j]=sum;
            rem_works--;
        }
    }
}

class matrix
{
    Scanner scan=new Scanner(System.in);
    int row, col;
    int mat[][];
    matrix(int a, int b)
    {
        row=a; col=b;
        mat=new int[row][col];
    }
    void take()
    {
        int i, j;
        for(i=0;i<row;i++)
        {
            for(j=0;j<col;j++)
            {
                mat[i][j]=scan.nextInt();
            }
        }
    }
    void show()
    {
        int i, j;
        for(i=0;i<row;i++)
        {
            for(j=0;j<col;j++)
            {
                System.out.print(mat[i][j]+ " ");
            }
            System.out.println("");
        }
    }
    matrix mul(matrix b)
    {
        matrix res= new matrix(row, b.col);
        int i, j, sum, k;
        
        for(i=0;i<row;i++)
        {
            for(j=0;j<b.col;j++)
            {
                sum=0;
                for(k=0;k<col;k++)
                {
                    sum+=(mat[i][k]*b.mat[k][j]);
                }
                res.mat[i][j]=sum;
            }
        }
        return res;
    }
}

class WorkItem
{
    int i, j;
    int a[], b[];
    WorkItem(int ii, int jj, int aa[], int bb[])
    {
        i=ii; j=jj;
        a=new int[aa.length];
        b=new int[bb.length];
        System.arraycopy(aa, 0, a, 0, aa.length);
        System.arraycopy(bb, 0, b, 0, bb.length);
        
    }
    WorkItem(WorkItem k)
    {
        i=k.i; j=k.j;
        a=new int[k.a.length];
        b=new int[k.b.length];
        System.arraycopy(k.a, 0, a, 0, k.a.length);
        System.arraycopy(k.b, 0, b, 0, k.b.length);
    }
}

class Queue
{
    WorkItem a[];
    int max_lim;
    int start, end;
    Queue(int i)
    {
        a= new WorkItem[i];
        max_lim=i;
        start=end=0;
    }
    synchronized void push(WorkItem k)
    {
        if(start==max_lim)
        {
            System.out.println("Queue Limit Exceeded");
            return;
        }
        //System.out.println("putting element "+ start);
        a[start]=new WorkItem(k);
        start++;
        notifyAll();
    }
    synchronized WorkItem top()
    {
        while(end==start)
        {
            try
            {
                //System.out.println("waiting in top");
                wait();
            }catch(InterruptedException x)
            {
                System.out.println("Synchronization Error");
            }
        }
        //System.out.println("taking element "+ end);
        WorkItem k=new WorkItem(a[end]);
        end++;
        notifyAll();
        return k;
    }
}

public class Main 
{
    public static void main(String[] args) 
    {
        
        
        int m1, n1, i, j, n_workers, m2, n2;
        Scanner scan=new Scanner(System.in);
        WorkItem ww;
        n_workers=scan.nextInt();

        //A take
        //System.out.println("Put First Matrix");
        m1=scan.nextInt(); n1=scan.nextInt();
        matrix A=new matrix(m1,n1);
        A.take();

        //B take
        //System.out.println("Put Second Matrix");
        m2=scan.nextInt(); n2=scan.nextInt();
        matrix B=new matrix(m2,n2);
        B.take();
        //B= B.transpose();
        
        int total_items=A.row*B.col;
        Queue q=new Queue(total_items);
        matrix result=new matrix(A.row, B.col);
        worker W[]=new worker[n_workers];
        int per_worker=total_items/n_workers;
        //initializing workers, the queue is still empty
        for(i=0;i<n_workers;i++)
        {
            W[i]= new worker(q, result, per_worker, i);
        }
        //pushing workitems in queue
       
        for(i=0;i<A.row;i++)
        {
            for(j=0;j<B.col;j++)
            {
                int a[]=new int[A.col];
                System.arraycopy(A.mat[i], 0, a, 0, A.col);
                int b[]=new int[A.col];
                for(int k=0;k<A.col;k++)
                {
                    b[k]=B.mat[k][j];
                }
                ww=new WorkItem(i,j,a,b);
                q.push(ww);
            }
        }
        //workers' array construct
        for(i=0;i<n_workers;i++)
        {
            try
            {
                W[i].t.join();
            }catch(InterruptedException x)
            {
                System.out.println("Join Error");
            }
        }
        System.out.println("Resultant Matrix Using Multiple Threads");
        result.show();
        //for single thread checking
        /*
        System.out.println("Resultant Matrix Using Single Thread");
        result=A.mul(B);
        result.show();
        */
        
    }
}

/*in:
4
7 5
41	67	34	0	69
24	78	58	62	64
5	45	81	27	61
91	95	42	27	36
91	4	2	53	92
82	21	16	18	95
47	26	71	38	69
5 4
12	67	99	35
94	3	11	22
33	73	64	41
11	53	68	47
44	62	57	37


out:
10948	9708	10905	6856
13032	13330	14810	10216
9944	11596	11487	8012
13289	13111	16630	9598
6165	14768	18029	9250
7864	13569	16012	8349
8805	14702	16000	9467	
*/
/*
10 
10 5 
1 7 4 9 3 
8 7 4 0 2 
7 4 8 3 1 
9 8 7 8 5 
9 3 8 4 2 
9 8 4 1 9 
6 4 8 1 6 
8 3 1 9 4 
6 5 7 6 2 
8 4 7 1 3 
5 4 
9 8 1 1 
0 4 7 3 
7 6 2 2 
4 6 9 3 
1 1 7 4 
*/