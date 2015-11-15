#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"

String_t* String_ctor(const char* str)
{
    assert(str);

    String_t* string = malloc(sizeof(*string));
    string->str = strdup(str);
    if (!string->str)
        return 0;
    string->length = strlen(str);

    return string;
}

int String_dtor(String_t* This)
{
    assert(This);

    free(This->str);
    This->str = 0;
    This->length = -1;
    free(This);

    return 0;
}

int String_ok(String_t* This)
{
    if (!This)
        return 0;
    if (!This->str)
        return 0;
    if (This->length < 0)
        return 0;
    if (strlen(This->str) != This->length)
        return 0;
    return 1;
}

int String_dump(String_t* This)
{
    assert(This);

    return 0;
}
