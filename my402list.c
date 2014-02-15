#include "cs402.h"
#include "my402list.h"
#include <stdio.h>
#include <stdlib.h>

/*Internal Function*/
My402ListElem* createNode()
{ 	
	/*Free of this mem is responsibility of caller*/
  My402ListElem* newNode = (My402ListElem*) malloc(sizeof(My402ListElem));
	/*error handle*/
	if(newNode == NULL){
		printf("MALLOC FAIL");
		return NULL;
	}
	else 
		return newNode;
}

/**
*returns: pointer to last node or NULL if list is empty
*/
My402ListElem * My402ListLast(My402List *anchorElem)
{
	int i = anchorElem->num_members;
	
	if(i>0)
	{
		My402ListElem* prevNode = (anchorElem->anchor).prev;
		return prevNode;/*now pointing to the last node*/
	}
	/*else if(i==0)
	{
		return &(anchorElem->anchor);/ *If List is empty, return anchor* /
	}*/
	else
		return NULL;
	
}
/**
*returns pointer to the first node or NULL if list is empty
*/
My402ListElem * My402ListFirst(My402List *anchorElem)
{
	int i = anchorElem->num_members;
	if(i>0)
	{
		My402ListElem* curr = &(anchorElem->anchor);
		curr = curr->next;/*now pointing to the 1st node*/
		return curr;
	}
	/*else if(i==0)
	{
		return &(anchorElem->anchor);/ *If List is empty, return anchor* /
	}*/
	else
		return NULL;
	
}

int My402ListInit(My402List* anchorElem)
{
	if(anchorElem == NULL)
   	{
		return FALSE;
	}
	anchorElem->num_members = 0;
	(anchorElem->anchor).next = &(anchorElem->anchor);
	/*List points to itself at init*/
	(anchorElem->anchor).prev = &(anchorElem->anchor);
	(anchorElem->anchor).obj = NULL;
	return TRUE;
}

int My402ListLength(My402List* anchorElem)
{
 return anchorElem->num_members;

}

int My402ListEmpty(My402List * anchorElem)
{
 	if(anchorElem->num_members == 0)
	{
		return TRUE;
	}
	else
		return FALSE; 
}


int My402ListAppend(My402List * anchorElem, void* objData)
{
	My402ListElem* lastPtr = My402ListLast(anchorElem);
	My402ListElem* newNode = NULL;
 	if((newNode=createNode())==NULL)
	{
		printf("Malloc failed in Append()\n");
		return FALSE;  	
   	} 
	newNode->obj = objData;
	newNode->next = &(anchorElem->anchor);
	(anchorElem->anchor).prev = newNode;
	/*if lastPtr is NULL, then it is the 1st node in the list*/
	if(lastPtr == NULL)
	{
		newNode->prev = &(anchorElem->anchor);
		(anchorElem->anchor).next = newNode; 
	}
	else
	{
		lastPtr->next = newNode;
		newNode->prev = lastPtr;
	}
	anchorElem->num_members+=1;
	return TRUE;	

}

int My402ListPrepend(My402List * anchorElem, void* objData)
{
	My402ListElem* firstPtr = My402ListFirst(anchorElem);
	My402ListElem* newNode = NULL;
 	if((newNode = createNode())==NULL)
	{
		return FALSE;  	
   } 
	newNode->obj = objData;
	(anchorElem->anchor).next = newNode;
	newNode->prev = &(anchorElem->anchor);
	/*if firstPtr is NULL, then adding 1st element to the list*/
	if(firstPtr == NULL)
	{
		newNode->next = &(anchorElem->anchor); 
		(anchorElem->anchor).prev = newNode;
	}
	else
	{
		newNode->next = firstPtr;
		firstPtr->prev = newNode;
	}
	anchorElem->num_members++;	
	return TRUE;	
}

void My402ListUnlink(My402List* anchorElem,My402ListElem* node)
{
	if(node == NULL || anchorElem == NULL)
	{
		return;
	}
	My402ListElem* nxtNode = node->next	;
	My402ListElem* prevNode = node->prev;

	/*Not verifying if node is in the list or not*/
	free(node);
	nxtNode->prev = prevNode;
	prevNode->next = nxtNode;
	anchorElem->num_members--;
	return;
}

void My402ListUnlinkAll(My402List *anchorElem)
{
	if(anchorElem == NULL)
	{
		return;
	}
	My402ListElem* elem = My402ListFirst(anchorElem);
	if(elem == NULL)
	{
		return;
	}
	for(; elem != NULL; elem = My402ListFirst(anchorElem))
	{
			My402ListUnlink(anchorElem, elem);
	}
}

My402ListElem *My402ListNext(My402List* anchorElem, My402ListElem* curr)
{
	if(curr->next == &(anchorElem->anchor))
	{
		return NULL;	
	}
	else
		return curr->next;

}

My402ListElem *My402ListPrev(My402List* anchorElem, My402ListElem* curr)
{
	if(curr->prev == &(anchorElem->anchor))
	{
		return NULL;	
	}
	else
		return curr->prev;
}

My402ListElem *My402ListFind(My402List* anchorElem, void* objData)
{
	int i = anchorElem->num_members;
	My402ListElem* curr = My402ListFirst(anchorElem);

	while(i>0)
	{
		if(curr->obj == objData)
		{
			return curr;
		}
		else
		{
			curr = curr->next;
			i--;
		}
	}
	return NULL;
}

int My402ListInsertAfter(My402List* anchorList, void* objData, My402ListElem* curr)
{
	My402ListElem* newNode = NULL;
	if((newNode = createNode()) == NULL)
	{
		printf("Malloc failed in insertAfter\n");
		return FALSE;
	}
	if(curr == NULL)
	{
		return My402ListAppend(anchorList, objData);
	}
	else
	{
		newNode->prev = curr;
		newNode->next = curr->next;
		curr->next->prev = newNode;
		curr->next = newNode;
		newNode->obj = objData;
		anchorList->num_members++;
	}
	return TRUE;
}

int My402ListInsertBefore(My402List* anchorList, void* objData, My402ListElem* curr)
{
	My402ListElem* newNode = NULL;
	if((newNode = createNode()) == NULL)
	{
		printf("Malloc failed in insertBefore\n");
		return FALSE;
	}
	if(curr == NULL)
	{
		return My402ListPrepend(anchorList, objData);
	}
	else
	{
		newNode->next = curr;
		newNode->prev = curr->prev;
		curr->prev->next = newNode;
		curr->prev = newNode;
      newNode->obj = objData;
		anchorList->num_members++;
	}
	return TRUE;
}






