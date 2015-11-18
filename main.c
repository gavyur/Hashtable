#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"
#include "list.h"

#define INPUT_FILENAME "onegin.txt"
#define OUTPUT_FILENAME "hashtable.txt"
#define HASHTABLE_SIZE 65

#define MY_NAME "GavYur"
#define GREET(program, version) printf("#--- " program " v" version " (%s %s) by " MY_NAME "\n\n", __DATE__, __TIME__)


int read_file(const char* filename, char** buf_addr, int* len);
int make_hashtable(char* buffer);
unsigned int compute_hash(const char* str);
int string_compare(const String_t* str1, const String_t* str2);

int main()
{
    GREET("Hashtable", "0.1");

    char* buffer = 0;
    int buffer_len = 0;
    if (read_file(INPUT_FILENAME, &buffer, &buffer_len))
    {
        printf("Error opening file ");
        perror(INPUT_FILENAME);
        return 1;
    }

    make_hashtable(buffer);

    free(buffer);

    return 0;
}

int read_file(const char* filename, char** buf_addr, int* len)
{
    assert(filename);
    assert(buf_addr);
    assert(len);

    FILE* stream = fopen(filename, "rb");
    if (!stream)
        return 1;
    if (fseek(stream, 0, SEEK_END))
        return 2;

    *len = ftell(stream);
    rewind(stream);
    char* buffer = (char*) calloc(*len, sizeof(*buffer));
    fread(buffer, *len, sizeof(*buffer), stream);
    fclose(stream);
    *buf_addr = buffer;
    return 0;
}

int make_hashtable(char* buffer)
{
    assert(buffer);

    List_t* hashtable[HASHTABLE_SIZE] = {};

    for (int i = 0; i < HASHTABLE_SIZE; ++i)
        hashtable[i] = list_ctor();

    char* pch = strtok(buffer, " !\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~\n");
    while (pch != NULL)
    {
        String_t* str = String_ctor(pch);
        unsigned int hash = compute_hash(pch);
        if (list_search(hashtable[hash % HASHTABLE_SIZE], str, string_compare))
        {
            String_dtor(str);
        } else
            list_append(hashtable[hash % HASHTABLE_SIZE], str);
        pch = strtok(NULL, "  !\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~\n");
    }
    FILE* stream = fopen(OUTPUT_FILENAME, "wb");
    if (!stream)
        return 1;
    write_hashtable(hashtable, stream);
    printf("Hashtable was written to %s!\n", OUTPUT_FILENAME);

    fclose(stream);
    for (int i = 0; i < HASHTABLE_SIZE; ++i)
    {
        ListElem_t* element = hashtable[i]->first;
        while (element != NULL)
        {
            String_dtor(element->content);
            element = element->next;
        }
        list_dtor(hashtable[i]);
    }

    return 0;
}

int write_hashtable(List_t* hashtable[], FILE* stream)
{
    fprintf(stream, "Hashtable:\n"
                    "[hash] (items count): items\n\n");
    for (int i = 0; i < HASHTABLE_SIZE; ++i)
    {
        fprintf(stream, "[%d] (%d)", i, hashtable[i]->size);
        if (hashtable[i]->size)
            fprintf(stream, ": ");
        ListElem_t* element = hashtable[i]->first;
        while (element != NULL)
        {
            if (element != hashtable[i]->first)
                fprintf(stream, ", ");
            fprintf(stream, "%s", element->content->str);
            element = element->next;
        }
        fprintf(stream, "\n");
    }

    return 0;
}

unsigned int compute_hash(const char* str)
{
    unsigned int hash = 0;

    while (*str)
    {
        hash ^= (unsigned char)(*str);
        hash += (hash >> 1) | (hash << 31);
        ++str;
    }

    return hash;
}

int string_compare(const String_t* str1, const String_t* str2)
{
    if (!strcmp(str1->str, str2->str))
        return 1;
    return 0;
}
