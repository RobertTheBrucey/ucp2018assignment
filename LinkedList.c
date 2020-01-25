/*****************************************************************************
* File:      LinkedList.c                                                    *
* Author:    Sean Ashton * Student ID: 14866636                              *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Implement a linked list for general use                         *
* Reference: A modified version of this code has been submitted for Lab 7    *
*               structs, though it was written here first and then adapted   *
*               for the lab completion.                                      *
* Comments:  Double ended, single linked list. Implements inserting at       *
*             beginning or end, removal from the beginning.                  *
* Requires:  stdlib.h, ctype.h                                               *
* Created:   04/10/2018 * Last Modified: 13/10/2018                          *
*****************************************************************************/
#include "LinkedList.h"
#include <stdlib.h>
#include <ctype.h>

/*****************************************************************************
* Name: insertFirst
* Purpose: insert a new ListNode at the beginning of the Linked List
* Imports:  LinkedList* list. the list to add the element to.
*           void* inData, a pointer to the data for the new node to point to
* Exports: returnCode, 0 for success, 1 for invalid list
* Assertions:   PRE: list is a pointer to a linked list.
*               POST: a node will be added to the beginning of the list
*****************************************************************************/
int insertFirst( LinkedList *list, void *inData )
{
    ListNode *newNd;
    /* 0: success, 1: invalid list */
    int returnCode = 0;
    if ( list == NULL )
    {
        returnCode = 1;
    }
    else
    {
    newNd = (ListNode*)malloc( sizeof( ListNode ) );
    newNd->data = inData;
    newNd->next = NULL;
        if ( list->head == NULL )/*Insert first element*/
        {
            list->head = newNd;
            list->tail = newNd;
        }
        else /*Insert Subsequent elements*/
        {
            newNd->next = list->head;
            list->head = newNd;
        }
        list->numNodes++;
    }
    return returnCode;
}

/*****************************************************************************
* Name: insertLast
* Purpose: insert a new ListNode at the end of the Linked List
* Imports:  LinkedList* list. the list to add the element to.
*           void* inData, a pointer to the data for the new node to point to
* Exports: returnCode, 0 for success, 1 for invalid list
* Assertions:   PRE: list is a pointer to a linked list.
*               POST: a node will be added to the end ofi the list
*****************************************************************************/
int insertLast( LinkedList *list, void *inData )
{
    ListNode *newNd;
    /* 0: success, 1: invalid list */
    int returnCode = 0;
    if ( list == NULL )
    {
        returnCode = 1;
    }
    else
    {
    newNd = (ListNode*)malloc( sizeof( ListNode ) );
    newNd->data = inData;
    newNd->next = NULL;
        if ( list->head == NULL )/*Insert first element*/
        {
            list->head = newNd;
            list->tail = newNd;
        }
        else /*Insert Subsequent elements*/
        {
            list->tail->next = newNd;
            list->tail = newNd;
        }
        list->numNodes++;
    }
    return returnCode;
}


/*****************************************************************************
* Name: removeFirst
* Purpose: remove the first Node of the Linked List
* Imports:  LinkedList* list. the list to add the element to.
* Exports: returnCode, 0 for success, 1 for invalid list, 2 for empty list
* Assertions:   PRE: list is a pointer to a linked list.
*               POST: First node will be deleted
*****************************************************************************/
int removeFirst( LinkedList *list )
{
    ListNode *tempNode;
    /* 0: success, 1: invalid list, 2: empty list */
    int returnCode = 0;
    if ( list == NULL ) /* Check if list exists */
    {
        returnCode = 1;
    }
    else
    {
        /* Check if list is empty or not*/
        if ( list->head == NULL )
        {
            returnCode = 2;
        }
        /* Remove last node. */
        else if ( list->head->next != NULL )
        {
            tempNode = list->head;
            list->head = tempNode->next;
            /* Free the memory in use by the Node's data */
            if ( tempNode->data != NULL )
            {
                free( tempNode->data );
            }
            free( tempNode );
            list->numNodes--;
        }
        else /* Remove not last node */
        {
            free( list->head->data );
            free( list->head );
            list->head = NULL;
            list->numNodes = 0;
        }
    }
    return returnCode;
}

/* removeLast is omitted as it is complex and inefficient without
    doubly linked lists and is not required for this project */
