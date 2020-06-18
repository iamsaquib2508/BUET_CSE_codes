#include<stdio.h>
#include<stdlib.h>

#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999
#define create (struct listNode*) malloc (sizeof(struct listNode)) 

struct listNode
{
    int item;
    struct listNode * next;
};

struct listNode * list;

void initializeList()
{
    list = 0;  
}

int insertItem(int item) 
{
	struct listNode * newNode ;
	newNode = (struct listNode*) malloc (sizeof(struct listNode)) ;
	newNode->item = item ;
	newNode->next = list ; 
	list = newNode ; 
	return SUCCESS_VALUE ;
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

//Task 1
int insertLast(int item)
{
	struct listNode * cur, * newnode;
	newnode = create;
	newnode->next = 0;
	newnode->item = item;
	if(list==0) //no element in the list
	{
		list=newnode;
		return SUCCESS_VALUE;
	}
	cur=list;
	while(cur->next!=0)
	{
		cur=cur->next;
	}
	//end is reached
	cur->next=newnode;
	
	return SUCCESS_VALUE;
	
}

//Task 2
int insertBefore(int olditem, int newitem)
{
	struct listNode * cur, * newnode, *prev;
	if(list==0 || list->item==olditem) //no element in the list or 1st element is the desired one
	{
		return insertItem(newitem);
	}
	
	cur = list;
	
	while(cur!=0) //looking for the element maintaining its previous
	{
		if(cur->item==olditem) break;
		prev = cur;
		cur = cur->next;
	}
	if(cur==0) return insertItem(newitem); //not found
	else 
	{
		newnode = create;
		newnode->item = newitem;
		newnode->next = cur;
		prev->next = newnode;
		return SUCCESS_VALUE;
	}
	
}

//Task 3
int deleteAfter(int item)
{
	struct listNode * cur, * del;
	cur = searchItem(item);
	if(cur==0 || cur->next==0 ) return NULL_VALUE; //not found or found in the last element
	else
	{
		del = cur->next;
		cur->next = del->next;
		free(del);
	}
	
	return SUCCESS_VALUE;
}

//Task 4
int deleteLast()
{
	struct listNode * cur, *prev;
	if(list==0 )  //no element in the list
	{
		return NULL_VALUE;
	}
	
	cur = list;
	prev=0;
	while(cur->next!=0)
	{
	    
		prev = cur;
		cur = cur->next;
	}
	
	if(prev==0)  list=0; //only element in the list
	else prev->next=0;
	free(cur);
	
	return SUCCESS_VALUE;
}

int main()
{
	//freopen("LLtesting.txt", "r",  stdin);
    initializeList();
    while(1)
    {
        printf("1. Insert new item.\n2. Delete item.\n3. Search item. \n");
        printf("4. (Add from homework).\n5. Print.\n6. exit.\n");

        int ch;
        int item, newitem;
        scanf("%d",&ch);
        if(ch==1)
        {
            scanf("%d", &item);
            insertItem(item);
        }
        else if(ch==2)
        {
            
            scanf("%d", &item);
            deleteItem(item);
        }
        else if(ch==3)
        {
            
            scanf("%d", &item);
            struct listNode * res = searchItem(item);
            if(res!=0) printf("Found.\n");
            else printf("Not found.\n");
        }
        else if(ch==4)
        {
        	//insert last  //insert before  //delete after  //delete last
        	printf("1. Insert Last.\n2. Insert Before.\n3. Delete After.\n4. Delete Last\n");
        	scanf("%d", &ch);
        	if(ch==1) 
        	{
        		scanf("%d", &item);
        		insertLast(item);
			}
			else if(ch==2)
			{
				scanf("%d %d", &item, &newitem);
				insertBefore(item, newitem);
			}
			else if(ch == 3)
			{
				scanf("%d", &item);
				deleteAfter(item);
			}
			else if(ch==4)
			{
				deleteLast();
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
