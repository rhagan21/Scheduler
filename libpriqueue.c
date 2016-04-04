/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
    q -> head = NULL;
    q -> size = 0;
    q -> compare = comparer;
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
	priqueue_node* nodeToAdd = malloc(sizeof(priqueue_node));
        nodeToAdd -> nodeInfo = ptr;
        nodeToAdd -> next = NULL;
        q -> size ++;
        //add to empty queue
        if (q -> size == 1) {
            q -> head = nodeToAdd;
            return 0;
        }
        else {
            //compare the info to add to the info at the head to see which one is smaller.
            //if info added is smaller or equal to, add to head of queue
            int compareResult;
            compareResult = q -> compare(ptr, q -> head -> nodeInfo);
            if (compareResult <= 0) {
                nodeToAdd -> next = q -> head;
                q -> head = nodeToAdd;
                return 0;
            }
            //if info added is greater than the head node
            else {
                priqueue_node* temp = q -> head;
                int count = 1;
                while (temp -> next != NULL) {
                    printf("here\n");
                    compareResult = q -> compare(ptr, temp -> next -> nodeInfo);
                    printf("%i\n", compareResult);
                    if (compareResult <= 0) {
                        nodeToAdd -> next = temp -> next;
                        temp -> next = nodeToAdd;
                        return count;
                    }
                    temp = temp -> next;
                    count ++;
                }
                temp -> next = nodeToAdd;
                return count;
            }
        }
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
    if(q -> head != NULL) {
        return q -> head;
    }
    else {
	return NULL;
    }
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
    if (q -> head == NULL) {
	return NULL;
    }
    else {
        priqueue_node* temp = q -> head;
        q -> head = q -> head -> next;
        void* infoToReturn = temp -> nodeInfo;
        free(temp);
        q -> size --;
        return infoToReturn;
    }
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
    if (q -> size < index) {
	return NULL;
    }
    else {
        priqueue_node* temp = q -> head;
        int count = 0;
        while (count != index) {
            temp = temp -> next;
            count ++;
        }
        void* infoToReturn = temp -> nodeInfo;
        return infoToReturn;
    }
}


/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
    if (q -> size == 0) {
	return 0;
    }
    else {
        priqueue_node* temp = q -> head;
        void* nodeToDelete;
        int count = 0;
        int numRemoved = 0;
        while (temp != NULL) {
            if (temp -> nodeInfo == ptr) {
                temp = temp -> next;
                nodeToDelete = priqueue_remove_at(q, count);
                count --;
                numRemoved ++;
            }
            else {
                temp = temp -> next;
            }
            count ++;
        }
        return numRemoved;
    }
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
    if (q -> size < index) {
	return NULL;
    }
    else if (index == 0) {
        priqueue_node* temp = q -> head;
        q -> head = temp -> next;
        void* infoToReturn = temp -> nodeInfo;
        free(temp);
        q -> size --;
        return infoToReturn;
    }
    else {
        priqueue_node* temp = q -> head;
        priqueue_node* prev = NULL;
        int count = 0;
        while (count != index) {
            prev = temp;
            temp = temp -> next;
        }
        prev = temp -> next;
        void* infoToReturn = temp -> nodeInfo;
        free(temp);
        q -> size --;
        return infoToReturn;
    }
}


/**
  Returns the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
	return q -> size;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
    while (q -> size > 0) {
        priqueue_node* temp = q -> head;
        q -> head = temp -> next;
        free(temp);
        q -> size --;
    }
}
