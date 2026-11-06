#include <stdlib.h>
#include "lisp.h"
#include "list.h"

List* alloc_list()
{
    List* list = (List*)malloc(sizeof(List));
    list->length = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

Node* list_nth_node(List* lst, int index)
{
    Node* current = lst->first;
    for (int i = 0; i < index; i++)
        current = current->next;
    return current;
}

void list_node_free(Node* node)
{
    free(node->value);                                  // If value points to data, it isn't freed.
    free(node);
}

// Remove one element from a list. Not used (atm)
void list_remove(List* lst, int index) 
{
    Node* current = lst->first;
    Node* last = NULL;  // shut up compiler warning
    int i;
    if (index == 0)
        lst->first = current->next;
    else {
        for (i = 0; i < index; i++) {
            last = current;
            current = current->next;
        }
        last->next = current->next;
    }
    if (i == lst->length - 1)
        lst->last = last;
    list_node_free(current);
    lst->length--;
}

// Free a list and its nodes. 
// - Only frees the spine of the list.
// - Data pointed to by Values are not freed.
void list_delete(List* list)
{
    Node* current = list->first;
    for (int i = 0; i < list->length; i++) {
        Node* next = current->next;
        list_node_free(current);  
        current = next;
    }
    free(list);
}


List* list_copy_omit(List* lst, int index)
{
    List* new = alloc_list();
    Node* current = lst->first;
    for (int i = 0; i < lst->length; i++) {
        if (i != index)
            list_append(new, current->value);
        current = current->next;
    }
    return new;
}

List* list_copy(List* source)           // Only copies spine of list - does not deep copy.
{
    List* new = alloc_list();
    Node* current = source->first;
    for (int i = 0; i < source->length; i++) {
        list_append(new, current->value);
        current = current->next;
    }
    return new;
}

// Copy items from src to dest, skipping duplicates. Only used for environments.
void list_copy_new(List* dest, List* source)
{
    Node* current = source->first;
    for (int i = 0; i < source->length; i++) {
        if (list_find(dest, current->value) == -1)
            list_append(dest, current->value);
        current = current->next;
    }
}

void list_append(List* list, Value* new_item)
{
    Node* new = (Node*)malloc(sizeof(Node));
    new->value = (Value*)malloc(sizeof(Value));
    new->value->type = new_item->type;
    new->value->data = new_item->data;
    new->next = NULL;
    if (list->length == 0)
        list->first = new;
    else
        list->last->next = new;
    list->last = new;
    list->length++;
}

int list_find(List* list, Value* item)
{
    if (list->length == 0) return -1;
    Node* current = list->first;
    for (int i = 0; i < list->length; i++) {
        if (compare_values(current->value, item)) return i;
        current = current->next;
    }
    return -1;
}

List list_pop(List* list)
{
    List new_head = {
        list->length - 1,
        list->first->next,
        list->last
    };
    return new_head;
}
