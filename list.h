
typedef struct List_t{
    void *details;
    struct List_t *next;
}List;

/*prints the whole linked list, with a virtual function that must be implemented seperetly for each object*/
void printList(List *node, void (*fptr)(void *));

/*append new item to the end of the list*/
void append(List* head_ref, void *new_data);

/*append the first item of the list*/
void pushFirst(List ** head, void *new_data);

/*pushes an item to the end of the list, if this is the first item, initialize the list to this node*/
void push(List ** head, void *new_data);

/*function that iterates threw a linked list and frees all allocated space for its components*/
void freeList(List *head,void (*funcPointer)(void *));

/* function that flushes all memory that was taken by the list pointed to by head */
/*the second parameter is the private destroy function of this specific struct*/
void Destroy_List(List **head,void (*funcPointer)(void *) );

/*counts how many nodes this list contains*/
int listLength(List *list);

/* function that searches the list for an *item, using a custom compare function that must be provided */
void* searchList(List * node, void* (*funcPointer)(void *, void*item), void* item);

/* get the n-th item in the list */
List* get(List *head_ref, int index);
