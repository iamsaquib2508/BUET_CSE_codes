#include<stdio.h>
#include<stdlib.h>
#define create (struct listNode*) malloc (sizeof(struct listNode)) 

#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999

struct listNode
{
    int item;
    struct listNode * next;
};

struct listNode * list;
struct listNode * tail;

void initializeList()
{
    list = 0;  
	tail = 0;
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

int deleteItem(int item)
{
	struct listNode *temp, *prev ;
	temp = list ; 
	while (temp != 0)
	{
		if (temp->item == item) break ;
		prev = temp;
		temp = temp->next ;
	}
	if (temp == 0) return NULL_VALUE ; 
	if (temp == list)
	{
		list = list->next ;
		free(temp) ;
	}
	else
	{
		prev->next = temp->next ;
		free(temp);
	}
	return SUCCESS_VALUE ;
}

void printList()
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

//Task 5a
int insertItem(int item)
{
	
	struct listNode * newnode ;
	newnode = create ;
	newnode->item = item ;
	newnode->next = list ; 
	if(list==0) tail=newnode;
	list = newnode ; 
	return SUCCESS_VALUE ;
}
//Task 5b
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
		free(del);
	}
	return SUCCESS_VALUE;
	
}

//Task 6
int insertLast(int item)
{
    struct listNode * cur, * newnode;
    newnode = create;
	newnode->next = 0;
	newnode->item = item;
	if(list==0) list=newnode; // no element in the list so far
	else tail->next=newnode;
	
	tail=newnode;
	return SUCCESS_VALUE;
	
}


int main()
{
    initializeList();
    while(1)
    {
        printf("1. Insert new item.\n2. Delete item.\n3. Search item. \n");
        printf("4. (Add from homework).\n5. Print.\n6. exit.\n");

        int ch;
        int item;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);
            insertItem(item);
        }
        else if(ch==2)
        {
            int item;
            scanf("%d", &item);
            deleteItem(item);
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
        	//ins last // del after
        	printf("1. Insert Last.\n2. Delete After\n");
        	scanf("%d", &ch);
        	if(ch==1) 
        	{
        		scanf("%d", &item);
        		insertLast(item);
			}
			else if(ch == 2)
			{
				scanf("%d", &item);
				deleteAfter(item);
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

}
