#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "myassert.h"
#include "list.h"

List_t* list_ctor()
{
    List_t* list = malloc(sizeof(*list));
    list->size = 0;
    list->first = NULL;
    list->last = NULL;

    return list;
}

int list_dtor(List_t* This)
{
    assert(This);

    ListElem_t* element = This->first;
    while (element != NULL)
    {
        ListElem_t* new_element = element->next;
        listelem_dtor(element);
        element = new_element;
    }

    This->size = -1;
    This->first = NULL;
    This->last = NULL;

    return 0;
}

int list_ok(List_t* This)
{
    if (!This)
        return 0;
    if (This->size < 0)
        return 0;
    if ((!This->first || !This->last) && (This->size > 0))
        return 0;
    int count = 0;
    ListElem_t* current_elem = This->first;
    while ((current_elem != This->last) && (current_elem != NULL) && (count < This->size))
    {
        if (!listelem_ok(current_elem))
            return 0;
        current_elem = current_elem->next;
        ++count;
    }
    if ((current_elem != NULL) && !listelem_ok(current_elem))
        return 0;
    return 1;
}

int list_dump(List_t* This, const char* name)
{
    assert(This);
    printf("%s = List_t(%s)\n"
           "{\n"
           "    size = %d\n"
           "    first = %p\n"
           "    last = %p\n"
           "}\n",
           name, list_ok(This) ? "ok" : "NOT OK!!!", This->size, This->first, This->last);

    return 0;
}

int list_insert_after(List_t* This, ListElem_t* elem, ListElem_t* after)
{
    ASSERT_OK(list, This);
    ASSERT_OK(listelem, elem);

    if (!after && This->size > 0)
        return 1;

    if (after)
        listelem_addafter(elem, after);
    else
    {
        elem->parent_list = This;
        elem->prev = NULL;
        elem->next = NULL;
    }

    if (after == This->last)
        This->last = elem;
    if (!This->first)
        This->first = elem;
    ++(This->size);

    return 0;
}

int list_insert_before(List_t* This, ListElem_t* elem, ListElem_t* before)
{
    ASSERT_OK(list, This);
    ASSERT_OK(listelem, elem);

    if (!before && This->size > 0)
        return 1;

    if (before)
        listelem_addbefore(elem, before);
    else
    {
        elem->parent_list = This;
        elem->prev = NULL;
        elem->next = NULL;
    }

    if (before == This->first)
        This->first = elem;
    if (!This->last)
        This->last = elem;
    ++(This->size);

    return 0;
}

int list_append(List_t* This, ListElem_t* elem)
{
    ASSERT_OK(list, This);
    return list_insert_after(This, elem, This->last);
}

int list_remove(List_t* This, ListElem_t* elem)
{
    ASSERT_OK(list, This);
    ASSERT_OK(listelem, elem);

    --(This->size);
    if (This->first == elem)
        This->first = elem->next;
    if (This->last == elem)
        This->last = elem->prev;
    listelem_remove(elem);

    return 0;
}

ListElem_t* list_search(List_t* This, LIST_TYPE what, int (*compare)(const LIST_TYPE first, const LIST_TYPE second))
{
    ASSERT_OK(list, This);

    ListElem_t* element = This->first;
    while (element != NULL)
    {
        if ((*compare)(what, element->content) == 1)
            return element;
        element = element->next;
    }

    return 0;
}

ListElem_t* listelem_ctor(LIST_TYPE content)
{
    ListElem_t* elem = malloc(sizeof(*elem));
    elem->content = content;
    elem->parent_list = NULL;
    elem->next = NULL;
    elem->prev = NULL;

    return elem;
}

int listelem_dtor(ListElem_t* This)
{
    assert(This);

    listelem_remove(This);
    This->next = NULL;
    This->prev = NULL;
    This->parent_list = NULL;
    free(This);

    return 0;
}

int listelem_ok(ListElem_t* This)
{
    if (!This)
        return 0;
    if (This->next)
        if (This->next->prev != This)
            return 0;
    if (This->prev)
        if (This->prev->next != This)
            return 0;
    return 1;
}

int listelem_dump(ListElem_t* This, const char* name)
{
    assert(This);

    printf("%s = ListElem_t(%s)[%p]\n"
           "{\n"
           "    content = %d\n"
           "    prev = %p\n"
           "    next = %p\n"
           "    parent_list = %p\n"
           "}\n",
           name, listelem_ok(This) ? "ok" : "NOT OK!!!", This, This->content, This->prev, This->next, This->parent_list);

    return 0;
}

int listelem_addafter(ListElem_t* This, ListElem_t* prev)
{
    ASSERT_OK(listelem, This);
    ASSERT_OK(listelem, prev);

    This->prev = prev;
    This->next = prev->next;
    This->parent_list = prev->parent_list;
    if (prev->next)
        prev->next->prev = This;
    prev->next = This;

    return 0;
}

int listelem_addbefore(ListElem_t* This, ListElem_t* next)
{
    ASSERT_OK(listelem, This);
    ASSERT_OK(listelem, next);

    This->prev = next->prev;
    This->next = next;
    This->parent_list = next->parent_list;
    if (next->prev)
        next->prev->next = This;
    next->prev = This;

    return 0;
}

int listelem_remove(ListElem_t* This)
{
    if (This->next)
        This->next->prev = This->prev;
    if (This->prev)
        This->prev->next = This->next;
    This->next = NULL;
    This->prev = NULL;
    This->parent_list = NULL;

    return 0;
}
