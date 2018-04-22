/*
 * File:    dll.c
 * Author:  Brett Heithold
 * Description: This is the implementation file for the DLL (doubly-linked list)
 * class.
 */


#include "dll.h"
#include <stdlib.h>
#include <assert.h>


/*
 *  Type: DLLNODE
 *  Description: This is the DLLNODE struct used to construct the doubly-linked
 *  list data structure.
 */
typedef struct dllnode {
    void *value;
    struct dllnode *next;
    struct dllnode *prev;
} DLLNODE;

/*
 *  Constructor: newDLLNODE
 *  Usage:  DLLNODE *n = newDLLNODE(value, n, p);
 *  Description: This constructor instantiates a new DLLNODE object.
 */
DLLNODE *newDLLNODE(void *value, DLLNODE *next, DLLNODE *prev) {
    DLLNODE *n = malloc(sizeof(DLLNODE));
    assert(n != 0);
    n->value = value;
    n->next = next;
    n->prev = prev;
    return n;
}

/*
 * Method:  getDLLNODEvalue
 * Usage:   void *value = getDLLNODEvalue(n);
 * Description: This method returns the generic value stored in a DLLNODE struct.
 */
void *getDLLNODEvalue(DLLNODE *n) {
    return n->value;
}

/*
 *  Method: setDLLNODEvalue
 *  Usage:  void *value = setDLLNODEvalue(n, newValue);
 *  Description: This method sets the value of a DLLNODE. The method returns the
 *  old value.
 */
void *setDLLNODEvalue(DLLNODE *n, void *value) {
    void *oldValue = getDLLNODEvalue(n);
    n->value = value;
    return oldValue;
}

/*
 *  Method: getDLLNODEnext
 *  Usage:  DLLNODE *next = getDLLNODEnext(n);
 *  Description: This method returns the next pointer of a DLLNODE struct.
 */
DLLNODE *getDLLNODEnext(DLLNODE *n) {
    return n->next;
}

/*
 *  Method: setDLLNODEnext
 *  Usage:  setDLLNODEnext(n, newNext);
 *  Description: This method sets the next pointer of a DLLNODE struct with a new
 *  DLLNODE struct.
 */
void setDLLNODEnext(DLLNODE *n, DLLNODE *newNext) {
    assert(n != 0);
    n->next = newNext;
}

/*
 *  Method: getDLLNODEprev
 *  Usage:  DLLNODE *prev = getDLLNODEprev(n);
 *  Description: This method returns the prev pointer of a DLLNODE struct.
 */
DLLNODE *getDLLNODEprev(DLLNODE *n) {
    return n->prev;
}

/*
 *  Method: setDLLNODEprev
 *  Usage:  setDLLNODEprev(n, newPrev);
 *  Description: This method sets the prev pointer of a DLLNODE struct with a new
 *  DLLNODE struct.
 */
void setDLLNODEprev(DLLNODE *n, DLLNODE *newPrev) {
    assert(n != 0);
    n->prev = newPrev;
}


// Private DLL method prototypes
static DLLNODE *addToFront(DLL *items, void *value);
static DLLNODE *addToBack(DLL *items, void *value);
static DLLNODE *insertAtIndex(DLL *items,int, void *value);
static void *removeFromFront(DLL *items);
static void *removeFromBack(DLL *items);
static void *removeFromIndex(DLL *items, int index);
static DLLNODE *getNodeAtIndex(DLL *items, int index);


/*
 *  Type:   DLL
 *  Description: This DLL struct is the basis of the DLL class.
 *  This doubly-linked list is a linear doubly-linked list with both head and
 *  tail pointers.
 */
struct DLL {
    DLLNODE *head;
    DLLNODE *tail;
    DLLNODE *iterator;
    int size;

    // Public methods
    void (*display)(void *, FILE *);
    void (*free)(void *);

    // Private methods
    DLLNODE *(*addToFront)(DLL *, void *);
    DLLNODE *(*addToBack)(DLL *, void *);
    DLLNODE *(*insertAtIndex)(DLL *,int, void *);
    void *(*removeFromFront)(DLL *);
    void *(*removeFromBack)(DLL *);
    void *(*removeFromIndex)(DLL *, int);
    DLLNODE *(*getNodeAtIndex)(DLL *, int);
};

/*
 *  Constructor:    newDLL
 *  Usage:  DLL *items = newDLL(displayINTEGER, freeINTEGER);
 *  Description: This is the constructor for the DLL class. This constructor
 *  instantiates a new DLL object.
 */
DLL *newDLL(void (*d)(void *, FILE *), void (*f)(void *)) {
    DLL *items = malloc(sizeof(DLL));
    assert(items != 0);
    items->head = NULL;
    items->tail = NULL;
    items->iterator = NULL;
    items->size = 0;
    items->display = d;
    items->free = f;
    items->addToFront = addToFront;
    items->addToBack = addToBack;
    items->insertAtIndex = insertAtIndex;
    items->removeFromFront = removeFromFront;
    items->removeFromBack = removeFromBack;
    items->removeFromIndex = removeFromIndex;
    items->getNodeAtIndex = getNodeAtIndex;
    return items;
}

/*
 *  Method: insertDLL
 *  Usage:  insertDLL(items, index, value);
 *  Description: This method inserts a generic value into a DLL object. This
 *  method can calls private helper methods to insert the value at the head,
 *  tail, or at a given index of a DLL object. It runs in constant time for
 *  insertions at a constant distance from the front and from the back.
 *  The doubly-linked list uses zero-based indexing.
 */
void *insertDLL(DLL *items, int index, void *value) {
    assert(items != 0);
    assert(index >= 0 && index <= items->size);
    DLLNODE *n;
    if (index == 0) {
        // Value is to be added at the front of the list
        n = items->addToFront(items, value);
    }
    else if (index == items->size) {
        // Value is to be added at the back of the list
        n = items->addToBack(items, value);
    }
    else {
        // Value is to be inserted at an index between 1 and items->size - 1
        n = items->insertAtIndex(items, index, value);
    }
    return n;
}

/*
 *  Method: removeDLL
 *  Usage:  void *value = removeDLL(items, index);
 *  Description: This method removes values at a given index. It runs in
 *  constant time for removals at a constant distance from the front
 *  and from the back.
 */
void *removeDLL(DLL *items, int index) {
    assert(items != 0);
    assert(items->size > 0);
    assert(index >= 0 && index < items->size);
    void *oldValue;
    if (index == 0) {
        // Remove from front
        oldValue = items->removeFromFront(items);
    }
    else if (index == items->size - 1) {
        // Remove from back
        oldValue = items->removeFromBack(items);
    }
    else {
        // Remove from index
        oldValue = items->removeFromIndex(items, index);
    }
    return oldValue;
}

/*
 * Method:
 * Usage:
 * Description:
 */
void removeDLLall(DLL *items) {
    assert(items != 0);
    DLLNODE *curr = items->head;
    DLLNODE *tmp;
    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        free((DLLNODE *)tmp);
    }
    items->head = NULL;
    items->tail = NULL;
    items->size = 0;
}

/*
 * Method:
 * Usage:
 * Description:
 */
void *removeDLLnode(DLL *items, void *v) {
    assert(items != 0);
    assert(v != 0);
    DLLNODE *node = v;
    void *rv = getDLLNODEvalue(node);
    if (items->size == 1) {
        // removing only value from list
        items->head = NULL;
        items->tail = NULL;
    }
    else if (node == items->head) {
        // removal at head of list
        setDLLNODEprev(node->next, NULL);
        items->head = node->next;
    }
    else if (node == items->tail) {
        // removal at tail of list
        setDLLNODEnext(node->prev, NULL);
        items->tail = node->prev;
    }
    else {
        // removal between head and tail
        setDLLNODEnext(node->prev, node->next);
        setDLLNODEprev(node->next, node->prev);
    }
    items->size--;
    free((DLLNODE *) node);
    return rv;
}

/*
 * Method:
 * Usage:
 * Description:
 */
void firstDLL(DLL *items) {
    assert(items != 0);
    items->iterator = items->head;
}

/*
 * Method:
 * Usage:
 * Description:
 */
void lastDLL(DLL *items) {
    assert(items != 0);
    items->iterator = items->tail;
}

/*
 * Method:
 * Usage:
 * Description:
 */
int moreDLL(DLL *items) {
    assert(items != 0);
    return items->iterator == NULL ? 0 : 1;
}

/*
 * Method:
 * Usage:
 * Description:
 */
void nextDLL(DLL *items) {
    assert(items != 0);
    if (items->iterator == items->tail) {
        items->iterator = NULL;
    }
    else {
        items->iterator = items->iterator->next;
    }
}

/*
 * Method:
 * Usage:
 * Description:
 */
void prevDLL(DLL *items) {
    assert(items != 0);
    if (items->iterator == items->head) {
        items->iterator = NULL;
    }
    else {
        items->iterator = items->iterator->prev;
    }
}

/*
 * Method:
 * Usage:
 * Description:
 */
void *currentDLL(DLL *items) {
    assert(items != 0);
    return getDLLNODEvalue(items->iterator);
}

/*
 *  Method: unionDLL
 *  Usage:  unionDLL(recipient, donor);
 *  Description: The union method takes two lists and moves all the items
 *  in the donor list to the recipient list. If the recipient list has the
 *  items {{3,4,5}} and the donor list has the items {{1,2}}, then, after the
 *  union, the donor list will be empty and recipient list will have the
 *  items {{3,4,5,1,2}}. The union method runs in constant time.
 */
void unionDLL(DLL *recipient, DLL *donor) {
    assert(recipient != 0 && donor != 0);
    if (recipient->size == 0 && donor->size == 0) {
        return;
    }
    else if (recipient->size > 0 && donor->size == 0) {
        return;
    }
    else if (recipient->size == 0 && donor->size > 0) {
        recipient->head = donor->head;
        recipient->tail = donor->tail;
        recipient->size = donor->size;
    }
    else {
        setDLLNODEnext(recipient->tail, donor->head);
        setDLLNODEprev(donor->head, recipient->tail);
        recipient->tail = donor->tail;
        recipient->size += donor->size;
    }
        donor->head = NULL;
        donor->tail = NULL;
        donor->size = 0;
}


/* Method:  getDLL
 * Usage:   void *value = getDLL(items, index);
 * Description: The method returns the value at the given index.
 * It runs in constant time for retrievals at a constant distance from the
 * front and from the back.
 */
void *getDLL(DLL *items, int index) {
    assert(items != 0);
    assert(index >= 0 && index < items->size);
    if (index == 0) {
        return getDLLNODEvalue(items->head);
    }
    else if (index == items->size - 1) {
        return getDLLNODEvalue(items->tail);
    }
    else {
        DLLNODE *curr = items->getNodeAtIndex(items, index);
        return getDLLNODEvalue(curr);
    }
}

/*
 *  Method: setDLL
 *  Usage:  void *value = setDLL(items, index, value);
 *  Description: The method updates the value at the given index. If the given
 *  index is a valid index for the list, the replaced value is returned.
 *  If the given index is equal to the size, the value is appended to the list
 *  and a null pointer is returned. It runs in constant time for updates at a
 *  constant distance from the front and from the back.
 */
void *setDLL(DLL *items, int index, void *value) {
    // TODO: Can I do better?
    assert(items != 0);
    assert(index >= 0 && index <= items->size);
    void *oldValue = NULL;
    if (index == 0) {
        // set value of head
        oldValue = setDLLNODEvalue(items->head, value);
    }
    else if (index == items->size - 1) {
        // set value of tail
        oldValue = setDLLNODEvalue(items->tail, value);
    }
    else if (index == items->size) {
        // Add new DLLNODE to end of list
        items->addToBack(items, value);
    }
    else {
        DLLNODE *n = items->getNodeAtIndex(items, index);
        oldValue = setDLLNODEvalue(n, value);
    }
    return oldValue;
}

/*
 *  Method: sizeDLL
 *  Usage:  int size = sizeDLL(items);
 *  Description: This method returns the number of values stored in
 *  a DLL object. This method runs in constant time.
 */
int sizeDLL(DLL *items) {
    assert(items != 0);
    return items->size;
}

/*
 *  Method: displayDLL
 *  Usage:  displayDLL(items, stdout);
 *  Example Output: {{5,6,2,9,1}}
 */
void displayDLL(DLL *items, FILE *fp) {
    assert(items != 0);
    fprintf(fp, "{{");
    DLLNODE *curr = items->head;
    while (curr != NULL) {
        items->display(getDLLNODEvalue(curr), fp);
        curr = getDLLNODEnext(curr);
        if (curr != NULL) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "}}");
}

/*
 *  Method: displayDLLdebug
 *  Usage:  displayDLLdebug(items, stdout);
 *  Example Output: head->{{5,6,2,9,1}},tail->{{1}}
 */
void displayDLLdebug(DLL *items, FILE *fp) {
    assert(items != 0);
    fprintf(fp, "head->{{");
    DLLNODE *curr = items->head;
    while (curr != NULL) {
        items->display(getDLLNODEvalue(curr), fp);
        if (getDLLNODEnext(curr) != NULL) {
            fprintf(fp, ",");
        }
        curr = getDLLNODEnext(curr);
    }
    fprintf(fp, "}},tail->{{");
    if (items->size > 0) {
        items->display(getDLLNODEvalue(items->tail), fp);
    }
    fprintf(fp, "}}");
}

/*
 *  Method: freeDLL
 *  Usage:  freeDLL(items);
 *  Description: This method walks through the list, freeing the generic
 *  values (using the passed-in freeing function) and the nodes that hold them.
 *  If the freeing function is null, the generic value is not freed.
 */
void freeDLL(DLL *items) {
    assert(items != 0);
    DLLNODE *curr = items->head;
    DLLNODE *tmp;
    while (curr != NULL) {
        if (items->free != NULL) {
            items->free(getDLLNODEvalue(curr));
        }
        tmp = curr;
        curr = getDLLNODEnext(curr);
        free(tmp);
    }
    free(items);
}


/************************* Private Methods **************************/

/*
 *  Method (private):   addToFront
 *  Usage:  addToFront(items, value);
 *  Description: This method adds a value to the head of a DLL object.
 */
DLLNODE *addToFront(DLL *items, void *value) {
    assert(items != 0);
    DLLNODE *n = newDLLNODE(value, items->head, NULL);
    if (items->size == 0) {
        // List is empty, set head and tail to same node
        items->head = n;
        items->tail = n;
    }
    else {
        // List is non-empty
        setDLLNODEprev(items->head, n);
        items->head = n;
    }
    items->size++;
    return n;
}

/*
 *  Method (private):   addToBack
 *  Usage:  addToBack(items, value);
 *  Description: This method adds a value to the tail of a DLL object.
 */
DLLNODE *addToBack(DLL *items, void *value) {
    assert(items != 0);
    DLLNODE *n;
    if (items->size == 0) {
        // List is empty
        items->addToFront(items, value);
    }
    else {
        n = newDLLNODE(value, NULL, items->tail);
        setDLLNODEnext(items->tail, n);
        items->tail = n;
    }
    items->size++;
    return n;
}

/*
 *  Method (private):   insertAtIndex
 *  Usage:  insertAtIndex(items, index, value);
 *  Description: This method inserts a value at a given index of a DLL object.
 */
DLLNODE *insertAtIndex(DLL *items, int index, void *value) {
    assert(items != 0);
    assert(index >= 0 && index <= items->size);
    DLLNODE *n;
    if (index == 0) {
        n = items->addToFront(items, value);
    }
    else if (index == items->size) {
        n = items->addToBack(items, value);
    }
    else {
        // get node prev to node at index
        DLLNODE *curr = items->getNodeAtIndex(items, index - 1);
        n = newDLLNODE(value, curr->next, curr);
        setDLLNODEprev(curr->next, n);
        setDLLNODEnext(curr, n);
        items->size++;
    }
    return n;
}

/*
 *  Method (private):   removeFromFront
 *  Usage:  void *value = removeFromFront(items);
 *  Description: This method removes and returns the generic value stored
 *  at the head of a DLL object.
 */
void *removeFromFront(DLL *items) {
    assert(items != 0);
    DLLNODE *oldHead = items->head;
    void *oldValue = getDLLNODEvalue(oldHead);
    items->head = getDLLNODEnext(oldHead);
    items->size--;
    if (items->size == 0) {
        // List is going empty
        items->tail = NULL;
    }
    free(oldHead);
    return oldValue;
}

/*
 *  Method (private):   removeFromBack
 *  Usage:  void *value = removeFromBack(items);
 *  Description: This method removes and returns the generic value stored
 *  at the tail of a DLL object.
 */
void *removeFromBack(DLL *items) {
    assert(items != 0);
    void *oldValue;
    if (items->size == 1) {
        // List is going empty
        oldValue = items->removeFromFront(items);
    }
    else {
        DLLNODE *curr = getDLLNODEprev(items->tail); // get next to last node
        oldValue = getDLLNODEvalue(items->tail);
        free(items->tail);
        items->tail = curr;
        setDLLNODEnext(curr, NULL);
        items->size--;
    }
    return oldValue;
}

/*
 *  Method (private):   removeFromIndex
 *  Usage:  void *value = removeFromIndex(items, index);
 *  Description: This is a helper method for the removeDLL method that returns
 *  the value at the given index and removes the DLLNODE from that index.
 */
void *removeFromIndex(DLL *items, int index) {
    assert(items != 0);
    void *oldValue;
    if (index == 0) {
        oldValue = items->removeFromFront(items);
    }
    else if (index == items->size - 1) {
        oldValue = items->removeFromBack(items);
    }
    else {
        // get node prev to index
        DLLNODE *curr = items->getNodeAtIndex(items, index - 1);
        DLLNODE *oldNode = getDLLNODEnext(curr);
        oldValue = getDLLNODEvalue(oldNode);
        setDLLNODEnext(curr, getDLLNODEnext(oldNode));
        setDLLNODEprev(getDLLNODEnext(oldNode), curr);
        items->size--;
        if (items->size == 0) {
            // List went empty
            items->head = NULL;
            items->tail = NULL;
        }
        free(oldNode);
    }
    return oldValue;
}

/*
 *  Method (private): getNodeAtIndex
 *  Usage   DLLNODE *n = getNodeAtIndex(items, index);
 *  Description: This method returns the DLLNODE at the given index.
 */
DLLNODE *getNodeAtIndex(DLL *items, int index) {
    DLLNODE *curr;
    if (items->size - index <= index) {
        // traverse from the tail of the list
        curr = items->tail;
        int newIndex = items->size - index - 1;
        while (newIndex > 0) {
            curr = getDLLNODEprev(curr);
            newIndex--;
        }
    }
    else {
        // traverse from the head of the list
        curr = items->head;
        while (index > 0) {
            curr = getDLLNODEnext(curr);
            index--;
        }
    }
    return curr;
}
