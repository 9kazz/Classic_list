#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Logfile.h"
#include "utils.h"
#include "list.h"
#include "verify.h"

ListEl_t* get_head(List_t* list) {
    return list->fake_el->next;
}
ListEl_t* get_tail(List_t* list) {
    return list->fake_el->prev;
}
ListData_t get_data (ListEl_t* list_elem) {
    return list_elem->data;
}
ListEl_t* get_next (ListEl_t* list_elem) {
    return list_elem->next;
}
ListEl_t* get_prev (ListEl_t* list_elem) {
    return list_elem->prev;
}


List_t* List_Ctor() {

    ListEl_t* fake_el = List_Elem_Ctor(POISON);
    
    SAFE_CALLOC(list, 1, List_t)

    FAKE_EL(list) = fake_el;

    NEXT(fake_el) = fake_el;
    PREV(fake_el) = fake_el;
    
    return list;
}

ListErr_t List_Dtor(List_t* list) {
    assert(list);

    ListEl_t* list_elem      = get_tail(list);
    ListEl_t* list_elem_temp = list_elem;

    while(list_elem != FAKE_EL(list)) 
    {
        list_elem_temp = get_prev(list_elem);
        List_Elem_Dtor(list_elem);
        list_elem = list_elem_temp;
    }

    List_Elem_Dtor(list_elem);

    free(list);

    return END_WITH_SUC;
}

ListEl_t* List_Elem_Ctor(ListData_t data) {

    SAFE_CALLOC(list_elem, 1, ListEl_t)

    list_elem->data  = data;
    list_elem->prev  = NULL;
    list_elem->next  = NULL;

    return list_elem;
}

ListErr_t List_Elem_Dtor(ListEl_t* list_elem) {
    assert(list_elem);

    free(list_elem);

    return END_WITH_SUC;
}

ListEl_t* List_Insert_After(ListEl_t* list_elem, ListData_t value) {
    assert(list_elem);

    ListEl_t* new_elem = List_Elem_Ctor(value);

    PREV(new_elem) = list_elem;
    NEXT(new_elem) = get_next(list_elem);

    PREV(NEXT(list_elem)) = new_elem;
    NEXT(list_elem)       = new_elem;

    return new_elem;
}

ListEl_t* List_Insert_Before(ListEl_t* list_elem, ListData_t value) {
    assert(list_elem);

    return List_Insert_After(PREV(list_elem), value);
}

ListErr_t List_Delete(List_t* list, ListEl_t* list_elem) {
    assert(list_elem);

    if ( ! list_elem) {
        fprintf(stderr, "List_Elem_Dtor: NULL-pointer to ListEl_t\n");
        return EC_NULL_POINTER;
    }

    NEXT(PREV(list_elem)) = get_next(list_elem);
    PREV(NEXT(list_elem)) = get_prev(list_elem); 

    List_Elem_Dtor(list_elem);

    return END_WITH_SUC;
}
