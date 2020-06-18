#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

#define LIST_INIT_SIZE 2
#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999


int listMaxSize;
int * list;
int length;
int CLEARED;

int initializeList()
{
	listMaxSize = LIST_INIT_SIZE;
	list = (int*)malloc(sizeof(int)*listMaxSize) ;
	length = 0 ;
	CLEARED=0;
	if(list==NULL) return NULL_VALUE;
	return SUCCESS_VALUE;
}

//Task 1 getLength
int getLength()
{
    return length;
}

//Task 3 shrink
int shrink()
{
    if(length+length<=listMaxSize && listMaxSize>LIST_INIT_SIZE)
    {
        int *tempList;
        listMaxSize++;
        listMaxSize /=  2;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ;
        }
        free(list) ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
    }
    return SUCCESS_VALUE;
}

int searchItem(int item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

int insertItem(int newitem)
{
	int x;
	if(CLEARED && initializeList()==NULL_VALUE) return NULL_VALUE;
	if (length == listMaxSize)
	{
		int * tempList ;

		listMaxSize +=  listMaxSize ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ;
        }
        free(list) ;
        list = tempList ;
	};

	list[length++] = newitem ;

	return SUCCESS_VALUE ;
}

//Task 2 insertItemAt
int insertItemAt(int pos, int item)
{
    if(pos>=length) return NULL_VALUE;
    int x=list[pos];
    list[pos]=item;

    return insertItem(x);;
}

int deleteItemAt(int position) //version 2, do not preserve order of items
{
	if ( position >= length ) return NULL_VALUE;
	list[position] = list[--length] ;
	shrink();
	return SUCCESS_VALUE ;
}

int deleteItem(int item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return NULL_VALUE;
	deleteItemAt(position) ;
	return SUCCESS_VALUE ;
}


//Task 4 deleteLast
int deleteLast()
{
    if(length>0)
    {
        int tmp = list[--length];
        shrink();
        return tmp;
    }
    return NULL_VALUE;
}

//Task 5 clear
int clear()
{
    length=0;
    free(list);
    listMaxSize=0;
    CLEARED=1;
    return SUCCESS_VALUE;
}

//Task 6 deleteAll
int deleteAll()
{
    length=0;
    if(listMaxSize>LIST_INIT_SIZE)
	{
        listMaxSize = LIST_INIT_SIZE;
        free(list);
        list = (int*)malloc(sizeof(int)*listMaxSize) ;
        if(list==NULL) return NULL_VALUE;
    }
    return SUCCESS_VALUE;

}

void printList()
{
    for(int i=0;i<length;i++)
        printf("%d ", list[i]);
    printf(" ||| Current size: %d, current length: %d\n", listMaxSize, length);
}

int removeDuplicate(int item)
{
    int i, j;
    i=searchItem(item);
    if(i==NULL_VALUE) return i;
    for(j=i+1;j<length;j++)
    {
        if(list[j]!=item) continue;
        else
        {
            for(int k=j+1;k<length;k++)
            {
                list[k-1]=list[k];
                length--;

            }
            shrink();
            j--;
        }
    }
    return SUCCESS_VALUE;
}

int main()
{
	//freopen("Arraylist testing.txt", "r", stdin);
    initializeList();
    while(1)
    {
        //printf("\n1. Insert new item. 2. Delete item at. 3. Delete item.\n");
        //printf("4. (Add from homework). 5. Print. 6. exit.\n");

        int ch, x;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);

            x=insertItem(item);
            if(x==NULL_VALUE) printf("Could not insert %d\n", item);
        }
        else if(ch==2)
        {
            int pos;
            scanf("%d", &pos);
            deleteItemAt(pos);
        }
        else if(ch==3)
        {
            int item;
            scanf("%d", &item);

            deleteItem(item);
        }
        else if(ch==4)
        {

        	int cc;
            //printf("1. Get length 2. Insert item at 4. Delete last\n5. Clear 6. Delete all 7. Remove Duplicate\n");
            scanf("%d", &cc);
            if(cc == 1)
            {
                printf("Current Length: %d\n", getLength());
            }
            else if(cc==2)
            {
                //printf("Enter position and item : ");
                int item, pos;
                scanf("%d %d", &pos, &item);
                x=insertItemAt(pos, item);
                if(x==NULL_VALUE) printf("Could not insert\n");
            }
            else if(cc==4)
            {
                printf("Last Item is: ");
                x= deleteLast();
                if(x!=NULL_VALUE) printf("%d\n", x);
                else printf("not available\n");
            }
            else if(cc == 5)
            {
                x=clear();
                if(x==NULL_VALUE) printf("Could not clear\n");
            }
            else if(cc == 6)
            {
                x=deleteAll();
                if(x==NULL_VALUE) printf("Could not delete all\n");
            }
            else if(cc=7)
            {
                scanf("%d", &x);
                removeDuplicate(x);
            }
		}
        else if(ch==5)
        {
            printList();
        }
        else if(ch==6)
        {
            break;
        }
    }
	return 0;
}


//Task 7 postfix expression
/*
int main()
{


	initializeList();
	char s[100];
	while(scanf("%s", s)>0)
	{

		if(s[0]=='X') break;
		for(int i=0;s[i];i++)
		{
			if('0'<=s[i] && s[i]<='9') insertItem(s[i]-'0');
			else
			{
				int y=deleteLast(), x=deleteLast(), res;

				if(s[i]=='+') res=x+y;
				else if(s[i]=='-') res=x-y;
				else if(s[i]=='*') res=x*y;
				else if(s[i]=='/') res=x/y;
				insertItem(res);
                printf("%d%c%d= %d\n", x, s[i], y, res);

			}

		}
		printf("Result: %d\n", list[0]);
        clear();
	}
	return 0;
}
*/
