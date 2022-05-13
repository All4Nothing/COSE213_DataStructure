#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // strchr, strdup, strcmp
#include <ctype.h> // toupper

#include "adt_dlist.h"

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
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName *createName( char *str, char sex, int freq); 

/* Deletes all data in name structure and recycles memory
*/
void destroyName( void *pName);

////////////////////////////////////////////////////////////////////////////////
// prints contents of name structure
// for traverseList and traverseListR functions
void print_name(const void *dataPtr)
{
	printf( "%s\t%c\t%d\n", ((tName *)dataPtr)->name, ((tName *)dataPtr)->sex, ((tName *)dataPtr)->freq);
}

////////////////////////////////////////////////////////////////////////////////
// increases freq in name structure
// for addNode function
void increase_freq(const void *dataOutPtr, const void *dataInPtr)
{
	((tName *)dataOutPtr)->freq += ((tName *)dataInPtr)->freq;
}

////////////////////////////////////////////////////////////////////////////////
/* splits name and sex from string
	ex) "Zoe/M"
	return	1 success
			0 failure
*/
int split_name_sex(char *str, char *sex)
{
	char *p;
	
	p = strchr(str, '/');
	if (p == NULL) 
	{
		fprintf( stderr, "unexpected name/sex format\n");
		return 0;
	}
	*p++ = 0;
	*sex = *p;
	return 1;
}

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

////////////////////////////////////////////////////////////////////////////////
// compares two names in name structures
// for createList function
int cmpName( const void *pName1, const void *pName2)
{
	int ret = strcmp( ((tName *)pName1)->name, ((tName *)pName2)->name);
	if (ret == 0) return ((tName *)pName1)->sex - ((tName *)pName2)->sex;
	else return ret;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	
	char str[1024];
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
	list = createList( cmpName);
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	void *p;
	
	while(fscanf( fp, "%*d\t%s\t%c\t%d", str, &sex, &freq) != EOF)
	{
		pName = createName( str, sex, freq);
		
		ret = addNode( list, pName, increase_freq);
		
		if (ret == 2) // duplicated
		{
			destroyName( pName);
		}
	}
	
	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, P)rint, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		void *p;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list, destroyName);
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
				if (!split_name_sex(str, &sex)) break;
				
				pName = createName( str, sex, 0);

				if (searchList( list, pName, &p)) print_name( p);
				else fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a name/sex to delete: ");
				fscanf( stdin, "%s", str);
				if (!split_name_sex(str, &sex)) break;
				
				pName = createName( str, sex, 0);

				if (removeNode( list, pName, &p))
				{
					fprintf( stdout, "(%s, %c, %d) deleted\n", ((tName *)p)->name, ((tName *)p)->sex, ((tName *)p)->freq);
					destroyName( (tName *)p);
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

/* Deletes all data in name structure and recycles memory
*/
void destroyName( void *pName){
	free(((tName *)pName) -> name); //pName casting 하고 멤버에게 접근해야함

	free((tName *)pName);
}