#include <stdlib.h> // malloc

#include "adt_dlist.h"

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, void *dataInPtr){
	NODE *new = (NODE *)malloc( sizeof(NODE));
	new -> dataPtr = dataInPtr;
	
	if(new == NULL){ //overflow
		return 0; 
	}
	
	if(pPre == NULL){ //insert null list, first node
		new -> rlink = pList -> head;
		new -> llink = NULL;
		pList -> head = new;
	} 
	else{
		new -> rlink = pPre -> rlink; //new NODE
		new -> llink = pPre; 
		
	}	
	
	if(new -> rlink == NULL){
		pList -> rear = new;
	} 
	else{
		new -> rlink -> llink = new;
	}
	
	if(pPre != NULL){
		pPre -> rlink = new;
	}

	pList -> count ++;
	
	return 1;
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr){
	*dataOutPtr = pLoc -> dataPtr;
	
	if(pPre == NULL){
		pList -> head = pLoc -> rlink;
	}
	else
	{
		pPre -> rlink = pLoc -> rlink;
	}
	
	if(pLoc -> rlink == NULL){
		pList -> rear = pPre;
	}
	else
	{
		pPre -> rlink = pLoc -> rlink;
		pLoc -> rlink -> llink = pPre;
	}
	
	free(pLoc);
	
	pList -> count --;
}

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu){
	
	*pPre = NULL;
	*pLoc = pList -> head;
	
	
	while(*pLoc != NULL){
		if((pList->compare((*pLoc) -> dataPtr, pArgu)) == 0){ //found
			return 1;
		}
		else if(pList->compare((*pLoc) -> dataPtr, pArgu) > 0){
			return 0;			
		}
		else{
			*pPre = *pLoc;
			*pLoc = (*pLoc) -> rlink;
		}
	}
	return 0;
}

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( int (*compare)(const void *, const void *)){ 
	LIST *key = (LIST *) malloc( sizeof(LIST));
	if(key == NULL){ //overflow
		return NULL;
	}
	key -> count = 0;
	key -> head = NULL; //head와 rear는 주소값이므로 주소값 = NULL;
	key -> rear = NULL;
	key -> compare = compare;
	return key;
}

/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList, void (*callback)(void *)){
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	
	pPre = pList -> head;
	
	while(pPre != NULL){
		pLoc = pPre -> rlink;
		(*callback)(pPre -> dataPtr);
		free(pPre);
		pPre = pLoc;
	}
	
	free(pList);
}

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *, const void *)){

	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	
	int search = _search( pList, &pPre, &pLoc, dataInPtr); // found: 1, notfound: 0
	
	if(search){ //found
		(*callback)(pLoc->dataPtr, dataInPtr);
		return 2;
	}
	else{ //not found
		if(_insert( pList, pPre, dataInPtr)){
			return 1; //insert
		} 
		else{
			return 0; //overflow
		}
	}
}
/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr){
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	
	int search = _search( pList, &pPre, &pLoc, keyPtr); // found: 1, notfound: 0
	
	if(search){
		*dataOutPtr = pLoc -> dataPtr;
		_delete(pList, pPre, pLoc, dataOutPtr);
		return 1;
	}
	return 0;
}

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, void *pArgu, void **dataOutPtr){
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	
	int search = _search( pList, &pPre, &pLoc, pArgu);
	
	if(search){
		*dataOutPtr = pLoc -> dataPtr;
	}
	return search;
}

/* returns number of nodes in list
*/
int countList( LIST *pList){
	return pList -> count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList);

/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const void *)){
	NODE *pLoc = NULL;
	
	pLoc = pList -> head;
	
	while(pLoc != NULL){
		(*callback)(pLoc -> dataPtr);
		pLoc = pLoc -> rlink;
	}
}
/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const void *)){
	NODE *pLoc = NULL;
	
	pLoc = pList -> rear;
	
	while(pLoc != NULL){
		(*callback)(pLoc -> dataPtr);
		pLoc = pLoc -> llink;
	}
}