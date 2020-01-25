/*****************************************************************************
* File:      LinkedList.c                                                    *
* Author:    Sean Ashton * Student ID: 14866636                              *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Implement a linked list for general use                         *
* Reference: None.                                                           *
* Comments:  This linked list implementation is used to form a queue of      *
*             commands to draw                                               *
* Requires:  Nothing. List dependencies here                                 *
* Created:   04/10/2018 * Last Modified: 08/10/2018                          *
*****************************************************************************/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H 1
#define FALSE 0
#define TRUE !FALSE

/* Purpose: struct representing a node in a linked list */
typedef struct ListNode {
    struct ListNode* next;
    void* data;
} ListNode;

/* Purpose: struct representing a Linked List */
typedef struct LinkedList {
struct ListNode* head;
struct ListNode* tail;
int numNodes;
} LinkedList;

/* Self explanatory, returns error code */
int insertFirst( LinkedList*, void* );
int insertLast( LinkedList*, void* );
int removeFirst( LinkedList* );
#endif
