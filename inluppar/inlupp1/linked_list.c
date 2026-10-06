#include "linked_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

/// @brief  Represents a single link in the liked list
typedef struct link link_t;

/// @brief structure of the linked list nodes (links) holding an integer value
struct link {
    int value; // the integer value stored in the link
    link_t *next; // a pointer to the next link in the list or NULL if end of list
};

/// @brief A loinked list with one sentinel node and a tail pointer
struct list {
    link_t *first; //pointer to sentinel node
    link_t *last; // pointer to last link in list, or sentinel is list is empty
    int size; // number of element in list, excluding sentinel node, gives size in O(1)
};

/// @brief Allocated and initializes a new link 
/// @param value the integer stored in the link
/// @param next pointer to the next link in the list or NULL if end of list
/// @return pointer to the link we just created or NULL if memory allocation fails
static link_t *link_create(int value, link_t *next) {
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}

/// @brief Finds the link immediately preceding the given index in the list, O(n)
/// @param list The linked list to iterate through.
/// @param index The target index.
/// @return A pointer to the link before the link at index, or NULL if list is empty.
static link_t *find_previous_link(ioopm_list_t *list, int index) {
    link_t *previous = list->first; 

    for (int i = 0; i < index; ++i) {
        previous = previous->next;
    }
    return previous;
}

/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_list_create(void) {
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    link_t *sentinel = calloc(1, sizeof(link_t));

    list->first = sentinel;
    list->last = sentinel;
    return list;
}

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_list_destroy(ioopm_list_t *list) {
    link_t *current = list->first;
    //free all links first
    while (current != NULL) {
        link_t *next = current->next; // spara next innan vi frigör current
        free(current);
        current = next;
    }
    //when all links have been freed, free list. 
    free(list);
}

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_list_append(ioopm_list_t *list, int value) {

    link_t *new_link = link_create(value, NULL);

    list->last->next = new_link;
    list->last = list->last->next;
    list->size += 1;
}

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void ioopm_list_prepend(ioopm_list_t *list, int value) {
    //ioopm_list_insert is O(n) because of the for loop in find_previous_link, however since loop is only run one time
    //when index = 0 ioopm_list_insert(list, 0, value) is O(1), therefore ioopm_list_prepend is also O(1) even though it is using ioopm_list_insert. 
    ioopm_list_insert(list, 0, value);

    ////old code before insert was implemented:

        // //create new link containing value and the first real node after the sentinel
        // link_t *new_link = link_create(value, list->first->next);

        // //make sentinel node point to our new link
        // list->first->next = new_link;

        // //check if sentinel was the only node
        // //if it was list.last was also pointing at sentinel and must now be updated
        // if (list->last == list->first) {
        //     list->last = list->first->next;
        // }

        // list->size += 1;
    
}

/// @brief Return the first element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the head of
int ioopm_list_head(ioopm_list_t *list) {
    assert(!ioopm_list_is_empty(list));
    //since list.first is the sentinel node
    return list->first->next->value;
}

/// @brief Return the last element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the last element of
int ioopm_list_last(ioopm_list_t *list) {
    assert(!ioopm_list_is_empty(list));
    return list->last->value;
}

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @pre 0 <= index <= length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
void ioopm_list_insert(ioopm_list_t *list, int index, int value) {
    assert(index >= 0 && index <= ioopm_list_size(list));
    
    link_t *previous = find_previous_link(list, index);
    previous->next = link_create(value, previous->next);
    
    if (previous == list->last) {
        list->last = previous->next;
    }
    list->size += 1;
}

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
int ioopm_list_remove(ioopm_list_t *list, int index) {
    assert(index >= 0 && index < list->size);

    link_t *previous = find_previous_link(list, index);
    link_t *to_remove = previous->next;
    int value_removed = to_remove->value;

    if (to_remove == list->last) {
        list->last = previous;
    }

    previous->next = to_remove->next;
    free(to_remove);
    list->size = list->size -1;
    return value_removed;
}

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
int ioopm_list_get(ioopm_list_t *list, int index) {
    //assert to crash the program if precondition is not followed
    assert(index >= 0 && index < list->size) ;
    link_t *previous = find_previous_link(list, index);
    return previous->next->value;
}

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
int ioopm_list_size(ioopm_list_t *list) {
    return list->size;
}

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_list_is_empty(ioopm_list_t *list) {
    if (list->size == 0) {
        return true;
    }
    else {
        return false;
    }
}