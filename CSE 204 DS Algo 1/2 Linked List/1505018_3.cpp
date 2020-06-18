#include<stdio.h>
#include<stdlib.h>


#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999

struct listNode
{
    int item;
    struct listNode * next;
    struct listNode * prev;
};

struct listNode * list;
struct listNode * tail;

void initializeList()
{
    list = 0;  
    tail = 0;
}

int insertFirst(int item) 
{
	struct listNode * newNode ;
	newNode = (struct listNode*) malloc (sizeof(struct listNode)) ;
	newNode->item = item ;

	if(list==0) 
    {
        newNode->next = 0;
        newNode->prev = 0;
        list = newNode;
        tail = newNode;
    }
    else
    {
        newNode->next = list;
        list->prev = newNode;
        newNode->prev = 0;
        list = newNode;
    }
	return SUCCESS_VALUE ;
}


struct listNode * searchItem(int item)
{
	struct listNode * temp ;
	temp = list ; 
	while (temp != 0)
	{
		if (temp->item == item) return temp ;
		temp = temp->next ;
	}
	return 0 ; 
}



void printListForward()
{
    struct listNode * temp;
    temp = list;
    while(temp!=0)
    {
        printf("%d->", temp->item);
        temp = temp->next;
    }
    printf("\n");
}

//Task 7
void printListBackward()
{
	
    struct listNode * cur;
    cur = tail;
    while(cur!=0)
    {
        printf("%d->", cur->item);
        cur = cur->prev;
    }
    printf("\n");
}

//Task 8
int deleteAfter(int item)
{
    struct listNode * cur, * del;
	cur = searchItem(item);
	if(cur==0 || cur==tail ) return NULL_VALUE; //not found or found in the last element
	else
	{
		del = cur->next;
		cur->next = del->next;
		if(del==tail) tail=cur;
		else del->next->prev=cur;
		free(del);
	}
	return SUCCESS_VALUE;
	
}

//Task 9
int deleteLast()
{
	struct listNode *del;
	del=tail;
	if(del==list)  //only element or no element
	{
		if(del!=0) free(del); //only element
		list=0; tail=0;
		
		return SUCCESS_VALUE;
	}
	tail=tail->prev;
	tail->next=0;
	int k=del->item;
	free(del);

    return k;
}

int main()
{
    initializeList();
    while(1)
    {
        printf("1. Insert new item.\n2. Delete Last.\n3. Search item.\n");
        printf("4. Print forward.\n5. Print backward.\n6. exit.\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);
            insertFirst(item);
        }
        else if(ch==2)
        {
            int item = deleteLast();
            if(item!=NULL_VALUE) printf("Deleted: %d\n", item);
        }
        else if(ch==3)
        {
            int item;
            scanf("%d", &item);
            struct listNode * res = searchItem(item);
            if(res!=0) printf("Found.\n");
            else printf("Not found.\n");
        }
        else if(ch==4)
        {
            printListForward();
        }
        else if(ch==5)
        {
            printListBackward();
        }
        else if(ch==6)
        {
            break;
        }
    }

}
