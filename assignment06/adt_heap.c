#include <stdio.h>
#include <stdlib.h> // malloc, realloc

#include "adt_heap.h"

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index);


/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index);

