#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

typedef struct String_t
{
    char* str;
    int length;
} String_t;

String_t* String_ctor(const char* str);
int String_dtor(String_t* This);
int String_ok(String_t* This);
int String_dump(String_t* This);

#endif // STRING_H_INCLUDED
