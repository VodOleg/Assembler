#include <stdio.h>
#include "utils.h"
#include "list.h"
#include "DataStructures.h"
#include <stdlib.h>
#include <string.h>

/* Function to print nodes in a given linked list. funcPointer is used
   to access the function to be used for printing current node data.*/
void printList(List *node, void (*funcPointer)(void *))
{
    while (node != NULL)
    {
        if( node->details != NULL){
            (*funcPointer)(node->details);
            node = node->next;
        } else {
            printf("Error occured while trying to print details that are pointing to NULL\n");
            return;
        }
    }
}
/*the first node of the list must be pushed with this function*/
void pushFirst(List ** head, void *new_data){
    List *new_list= (List*)malloc(sizeof(List));
     
        new_list->next = NULL;
        new_list->details = new_data;
        (*head) = new_list;
        return;
    
}


/*push the struct object to a new node at the end of the list*/
void append(List* head_ref, void *new_data)
{
    List *current = head_ref;
    while(current->next != NULL){
        current = current->next;
    }
    

    current->next = (List*)malloc(sizeof(List));
 
    current->next->details = new_data;
    current->next->next = NULL;
 
}

/*pushes an item to the end of the list, if this is the first item, initialize the list to this node*/
void push(List ** head, void *new_data){
    if (new_data==NULL){
        printf("Runtime error, while trying to append NULL into node details\n");
        return;
    }
    if(*head==NULL) {
        pushFirst(head,new_data);
        return;
    }
    append(*head,new_data);
}

/*function that iterates threw a linked list and frees all allocated space for its components*/
void freeList(List *head,void (*funcPointer)(void *)){
    
    while(head->next !=NULL){
        (*funcPointer)(head->details);
        head=head->next;
    }
    (*funcPointer)(head->details);
    
}
/* function that cleans every node in linked list */ 
void Destroy_List(List **head,void (*funcPointer)(void *) ){
    List* next = NULL;

    freeList(*head,(*funcPointer));

    while((*head)!=NULL){
        next = (*head)->next;
        free((*head)->details);  
        free(*head);
        *head=next;
    }
}


/*counts how many nodes this list contains*/
int listLength(List *list){
    int counter=0;
    List * current = list;
    while(current!=NULL){
        current=current->next;
        counter++;
    }
    return counter;
}


/* function that searches the list for an *item, using a custom compare function that must be provided */
void* searchList(List * node, void* (*funcPointer)(void *, void*item), void* item){
    
    while (node != NULL)
    {
        if( node->details != NULL){
           if( (*funcPointer)(node->details, item)==NULL){
                node = node->next;
           }else { return node; }
        } else {
            printf("Error occured while trying to print details that are pointing to NULL\n");
            return NULL;
        }
    }
    return NULL;
}

/* get the n-th item in the list */
List* get(List *head_ref, int index){
        List *current = head_ref;
        int i = 0;
    while(current != NULL && i<index ){
        current = current->next;
        i++;
    }
    
    return current;
    
}