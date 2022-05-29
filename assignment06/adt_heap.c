#include <stdio.h>
#include <stdlib.h> // malloc, realloc

#include "adt_heap.h"

/*
typedef struct
{
    void **heapArr;
    int	last;
    int	capacity;
    int (*compare) (void *arg1, void *arg2);
} HEAP;
*/

/* Reestablishes heap by moving data in child up to correct location heap array
 */
// index = heap->last
static void _reheapUp(HEAP *heap, int index)
{
    while (index != 0 && heap->compare(heap->heapArr[index], heap->heapArr[(index - 1) / 2]))
    {
        void *temp;
        temp = heap->heapArr[index];
        heap->heapArr[index] = heap->heapArr[(index - 1) / 2];
        heap->heapArr[(index - 1) / 2] = temp;
        index = (index - 1) / 2;
    }
}

/* Reestablishes heap by moving data in root down to its correct location in the heap
 */
// 삭제는 root(index=0)에서만 일어남
// index = 0;
// left = 2i + 1, right = 2i + 2
static void _reheapDown(HEAP *heap, int index)
{
    void *left_subtree = NULL;
    void *right_subtree = NULL;
    int largest;

    if (index * 2 + 1 <= heap->last)
    {
        left_subtree = heap->heapArr[index * 2 + 1];

        if (index * 2 + 2 <= heap->last)
        {
            right_subtree = heap->heapArr[index * 2 + 2];
        }

        // set largest index
        if (heap->compare(left_subtree, right_subtree) > 0)
        {
            largest = index * 2 + 1;
        }
        else
        {
            largest = index * 2 + 2;
        }

        if (heap->compare(heap->heapArr[index], heap->heapArr[largest]) < 0)
        {
            void *temp;
            temp = heap->heapArr[index];
            heap->heapArr[index] = heap->heapArr[largest];
            heap->heapArr[largest] = temp;

            _reheapDown(heap, largest);
        }
    }
}

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create(int capacity, int (*compare)(void *arg1, void *arg2))
{
    HEAP *newHeap = (HEAP *)malloc(sizeof(HEAP));

    if (newHeap == NULL)
    {
        return NULL;
    }

    newHeap->heapArr = (void **)malloc(sizeof(void *) * capacity);
    newHeap->last = -1;
    newHeap->capacity = capacity;
    newHeap->compare = compare;

    return newHeap;
}

/* Free memory for heap
 */
void heap_Destroy(HEAP *heap)
{
    void *dataOutPtr;

    while (!heap_Empty(heap))
    {
        dataOutPtr = heap->heapArr[heap->last];
        heap->last--;
        free(dataOutPtr);
    }

    free(heap->heapArr);
    free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
// TODO
// FIXME
int heap_Insert(HEAP *heap, void *dataPtr);

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
// TODO
// FIXME
int heap_Delete(HEAP *heap, void **dataOutPtr);

/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(HEAP *heap)
{
    if (heap->last == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Print heap array */
void heap_Print(HEAP *heap, void (*print_func)(void *data))
{
    for (int i = 0; i < heap->last; i++)
    {
        print_func(heap->heapArr[i]);
    }
    printf("\n");
}
