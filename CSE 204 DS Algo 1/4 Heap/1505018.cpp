#include<stdio.h>


#define MAX_HEAP_SIZE 100000

#define MAXREAL 999999.0
#define pf printf

class HeapItem
{
public:
	int data; 
	float key; 
};


class MinHeap
{
public:
	HeapItem * A; //stores heap items, e.g., nodes
	int heapLength;
	int * map;

	MinHeap() //constructor
	{
		A = new HeapItem[MAX_HEAP_SIZE];
		map = new int[MAX_HEAP_SIZE];
		
		heapLength=0;
		map[1]=0;
		map[0]=0;
	}

	~MinHeap() //destructor
	{
		if(map) delete [] map;
		if(A) delete [] A;
		map = 0; //set to NULL after deletion
		A = 0; //set to NULL after deletion
	}

	void initialize(int v[], int n)
	{
		heapLength = n;
		for(int i=0; i<n; i++) 
		{
			A[i+1].data = v[i];
			A[i+1].key = MAXREAL;
			map[v[i]] = i+1; 
		}
	}

    void insertItem(int data, float key)
    {
        //Write your codes here
        if(heapLength+1==MAX_HEAP_SIZE) throw 1;
        if(map[data])
        printf("Map of %d: %d\n", data, map[data]);
        if(map[data]!=0) throw 2;
        heapLength++;
        HeapItem item;
        item.data = data; item.key=key;
        A[heapLength]=item;
        map[data]=heapLength;
        buHeapify(heapLength);
        
    }

	HeapItem removeMin() 
	{
	    //write your codes here
	    if(heapLength<1) throw 1;
	    HeapItem ret = A[1];
	    map[A[1].data]=0;
	    A[1]=A[heapLength--];
	    
	    heapify(1);
	    return ret;

	}


	void updateKey(int data, float key)
	{
		
		int pos=map[data];
		//printf("%d\n", pos);
		if(pos==0) throw 1;
		if(A[pos].key>key)
		{
			A[pos].data=data;
			A[pos].key=key;
			buHeapify(pos);
		}
		else if(A[pos].key<key)
		{
			A[pos].data=data;
			A[pos].key=key;
			heapify(pos);
		}

	}


	float getKey(int data)
	{
		int i = map[data];
		return A[i].key;
	}

	void heapify(int i)
	{
		int l,r,smallest;
		while(1)
		{
			l=i+i;    
			r=l+1;    
			smallest=i;

			if(l>heapLength && r>heapLength)
				break; 
			else if(r>heapLength)
				smallest = l;
			else if(l>heapLength)
				smallest = r;
			else if( A[l].key < A[r].key )
				smallest = l;
			else
				smallest = r;

			if(A[i].key <= A[smallest].key)
				break;	
            else
			{
				HeapItem t;
				t=A[i];
				A[i]=A[smallest];
				map[A[i].data]=i; //map[A[smallest].data]=i; 
				A[smallest]=t;
				map[A[smallest].data]=smallest; //map[A[i].data]=smallest;
				i=smallest;
			}

		}
	}

	void buHeapify(int i)
	{
		
		while(1)
		{
		
			if(i==1 || A[i/2].key <=A[i].key) break; 
			HeapItem t;
			t=A[i];
			A[i]=A[i/2];
			map[A[i].data]=i;
			A[i/2]=t;
			map[A[i/2].data]=i/2;
			i/=2;
		}
		
	}

    void printHeap()
    {
        printf("Heap length: %d\n", heapLength);
        int lat=2;
        for(int i=1;i<=heapLength;i++)
        {
            printf("(%d,%.3f) ", A[i].data, A[i].key);
            if(i+1==lat) 
            {
            	printf("\n");
            	lat+=lat;
			}
            
        }
        printf("\n\n");
    }

	bool Empty()
	{
		if(heapLength==0) return true;
		else return false;
	}
};


int main()
{
	
	
    int ch;
    int data;
    float key;
    MinHeap heap;
    //bool exit = false;
    while(1)
    {
        //printf("1. Insert 2. RemoveMin 3.Update 4. Print 5. Exit.\n");
        scanf("%d",&ch);
        
        if(ch==1)
        {
		
            scanf("%d %f",&data,&key);
            try
            {
            	heap.insertItem(data, key);
            	heap.printHeap();
			}
			catch(int i)
			{
				if(i==1)
				printf("Heap Max Size Reached\n");
				if(i==2)
				printf("Data Already Present\n");
			}
            
        
        }
        
        if(ch==2)
        {
		
            try
            {
            	HeapItem item;
            	item = heap.removeMin();
            	printf("Removed: (%d,%.2f)\n", item.data, item.key);
            	heap.printHeap();
			}
			catch(int i)
			{
				if(i==1)
				printf("Heap Underflow\n");
			}
            
            
        }
        if(ch==3)
        {
            scanf("%d %f",&data,&key);
            try
            {
            	heap.updateKey(data,key);
            	heap.printHeap();
			}
			catch(int i)
			{
				if(i==1) printf("Data Not Found\n");
			}
            
        }
        if(ch==4) heap.printHeap();
        if(ch==5) break;
        	
        
    }
    return 0;
}
