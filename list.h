#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "string.h"

#define LIST_TYPE String_t*

typedef struct ListElem_t
{
    LIST_TYPE content;
    struct List_t* parent_list;
    struct ListElem_t* prev;
    struct ListElem_t* next;
} ListElem_t;

typedef struct List_t
{
    int size;
    struct ListElem_t* first;
    struct ListElem_t* last;
} List_t;

List_t* list_ctor();
int list_dtor(List_t* This);
int list_ok(List_t* This);
int list_dump(List_t* This, const char* name);
int list_insert_elem_after(List_t* This, ListElem_t* elem, ListElem_t* after);
int list_insert_after(List_t* This, LIST_TYPE value, ListElem_t* after);
int list_insert_elem_before(List_t* This, ListElem_t* elem, ListElem_t* before);
int list_insert_before(List_t* This, LIST_TYPE value, ListElem_t* before);
int list_append_elem(List_t* This, ListElem_t* elem);
int list_append(List_t* This, LIST_TYPE value);
int list_remove_elem(List_t* This, ListElem_t* elem);
ListElem_t* list_search(List_t* This, LIST_TYPE what, int (*compare)(const LIST_TYPE first, const LIST_TYPE second));

ListElem_t* listelem_ctor(LIST_TYPE content);
int listelem_dtor(ListElem_t* This);
int listelem_ok(ListElem_t* This);
int listelem_dump(ListElem_t* This, const char* name);
int listelem_addafter(ListElem_t* This, ListElem_t* prev);
int listelem_addbefore(ListElem_t* This, ListElem_t* next);
int listelem_remove(ListElem_t* This);

#endif // LIST_H_INCLUDED
