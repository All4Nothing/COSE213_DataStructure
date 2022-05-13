#include <stdlib.h> // malloc
#include <string.h> // strchr
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

// User structure type definition
typedef struct 
{
	char	*name;
	char	sex;
	int		freq;
} tName;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tName		*dataPtr;
	struct node	*llink; //type define이 아직 안되었기에 struct node로 사용
	struct node	*rlink;
} NODE;

typedef struct
{
	int		count;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList(void);

/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList);

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, tName *dataInPtr);

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, tName *keyPtr, tName **dataOutPtr);

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr);

/* returns number of nodes in list
*/
int countList( LIST *pList);

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList);

/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const tName *));

/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const tName *));

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr);

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu);

////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName *createName( char *str, char sex, int freq); 

/* Deletes all data in name structure and recycles memory
*/
void destroyName( tName *pNode);

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'P':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

// compares two names in name structures
// for createList function
int cmpName( const tName *pName1, const tName *pName2)
{
	int ret = strcmp( pName1->name, pName2->name);
	if (ret == 0) return pName1->sex - pName2->sex;
	else return ret;
}

// prints contents of name structure
// for traverseList and traverseListR functions
void print_name(const tName *dataPtr)
{
	printf( "%s\t%c\t%d\n", dataPtr->name, dataPtr->sex, dataPtr->freq);
}

// increases freq in name structure
// for addNode function
void increase_freq(tName *dataOutPtr, const tName *dataInPtr)
{
	dataOutPtr->freq += dataInPtr->freq;
}

// splits name and sex from string
// ex) "Zoe/M"
void split_name_sex(char *str, char *sex)
{
	char *p;
	
	p = strchr(str, '/');
	if (p == NULL) 
	{
		fprintf( stderr, "unexpected name/sex format\n");
		return;
	}
	else
	{
		*p++ = 0;
		*sex = *p;
	}
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	
	char str[1024];
	char name[100];
	char sex;
	int freq;
	
	tName *pName;
	int ret;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%*d\t%s\t%c\t%d", str, &sex, &freq) != EOF)
	{
		pName = createName( str, sex, freq);
		
		ret = addNode( list, pName);
		
		if (ret == 2) // duplicated
		{
			destroyName( pName);
		}
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		tName *p;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				traverseList( list, print_name);
				break;
			
			case BACKWARD_PRINT:
				traverseListR( list, print_name);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a name/sex to find: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				pName = createName( str, sex, 0);

				if (searchList( list, pName, &p)) print_name( p);
				else fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a name/sex to delete: ");
				fscanf( stdin, "%s", str);
				split_name_sex(str, &sex);
				
				pName = createName( str, sex, 0);

				if (removeNode( list, pName, &p))
				{
					fprintf( stdout, "(%s, %c, %d) deleted\n", p->name, p->sex, p->freq);
					destroyName( p);
				}
				else fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
		}
		
		if (action) fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}




/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList(void){
	LIST *key = (LIST *) malloc( sizeof(LIST));
	if(key == NULL){ //overflow
		return NULL;
	}
	key -> count = 0;
	key -> head = NULL; //head와 rear는 주소값이므로 주소값 = NULL;
	key -> rear = NULL;
	return key;
}

/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList){
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	
	pPre = pList -> head;
	
	while(pPre != NULL){
		pLoc = pPre -> rlink;
		destroyName(pPre -> dataPtr);
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
int addNode( LIST *pList, tName *dataInPtr){

	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	
	int search = _search( pList, &pPre, &pLoc, dataInPtr); // found: 1, notfound: 0
	
	if(search){ //found
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
// _insert로 dataInPtr -> node에 넣기
// 만든 node -> pList에 넣기
// pList null or 맨앞에 : pre == null
// 중간에
//끝에 넣는건지
//node pre와 loc 주소값 조정

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, tName *keyPtr, tName **dataOutPtr){
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	
	int search = _search( pList, &pPre, &pLoc, *dataOutPtr); // found: 1, notfound: 0
	
	if(search){
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
int searchList( LIST *pList, tName *pArgu, tName **dataOutPtr){
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
void traverseList( LIST *pList, void (*callback)(const tName *)){
	NODE *pLoc = NULL;
	
	pLoc = pList -> head;
	
	while(pLoc != NULL){
		(*callback)(pLoc -> dataPtr);
		pLoc = pLoc -> rlink;
	}
}

/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const tName *)){
	NODE *pLoc = NULL;
	
	pLoc = pList -> rear;
	
	while(pLoc != NULL){
		(*callback)(pLoc -> dataPtr);
		pLoc = pLoc -> llink;
	}
}

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr){
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
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tName **dataOutPtr){
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
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu){
	
	*pPre = NULL;
	*pLoc = pList -> head;
	
	
	while(*pLoc != NULL){
		if((cmpName((*pLoc) -> dataPtr, pArgu)) == 0){ //found
			return 1;
		}
		else if(cmpName((*pLoc) -> dataPtr, pArgu) > 0){
			return 0;			
		}
		else{
			*pPre = *pLoc;
			*pLoc = (*pLoc) -> rlink;
		}
	}
	return 0;
}
//**pPre: 포인터의 주소 저장
// pPre: 전 node, pLoc: 현 node
//함수 호출전에 이름과 성별로 pArgu 만들어서 넘기기
//pPre랑 pLoc 움직여가면서 찾아서 넘겨야 add랑 delete때 node 변겨 가능
//cmpName >0 -> 멈춤, <0 -> 계속진행

////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName *createName( char *str, char sex, int freq){
	tName *key = (tName *) malloc(sizeof(tName));
	if(key == NULL){ //overflow
		return NULL;
	}
	key -> name = (char *) malloc(256 * sizeof(char));
	
	strcpy(key -> name, str); //key.*name = name //key -> name
	key -> sex = sex; 
	key -> freq = freq;
	
	return key;
}
//*str이 아닌 str 이유

/* Deletes all data in name structure and recycles memory
*/
void destroyName( tName *pNode){
	free(pNode -> name);

	free(pNode);
}



//_search, _insert -> add, _delete -> remove
// 1. create name 
// 2. _serach, _insert, _delete