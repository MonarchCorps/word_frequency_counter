//
// Created by David Okocha on 05/05/2026.
//

#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long ht_hash(const HashTable* ht, const char* word)
{
    unsigned long hash = 5381;
    int c;

    // change how the compiler interprets each byte when you do math on it
    const unsigned char* str = (unsigned char*)word;

    // *str = get the current character from the string
    // *str++ = move the pointer to the next character (after reading)
    // c = store the character in c
    // while loop checks to see if condition is not zero

    while ((c = *str++))
    {
        // hash * 33 + c — bit shift for speed
        hash = ((hash << 5) + hash) + c;
    }

    return hash % ht->size;
}

HashTable* ht_create(const int size)
{
    HashTable* ht = malloc(sizeof(HashTable));

    if (ht == NULL)
    {
        perror("Out of memory");
        return NULL;
    }

    ht->count = 0;
    ht->size = size;
    ht->buckets = calloc(size, sizeof(Entry*));

    if (ht->buckets == NULL)
    {
        perror("Out of memory");
        free(ht);
        return NULL;
    }

    return ht;
}

void ht_put(HashTable* ht, char* word)
{
    const unsigned long index = ht_hash(ht, word);

    Entry* current = ht->buckets[index];

    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->value++;
            return;
        }

        current = current->next;
    }

    Entry* newEntry = malloc(sizeof(Entry));

    if (newEntry == NULL)
    {
        perror("Out of memory");
        return;
    }

    newEntry->word = strdup(word);

    if (newEntry->word == NULL)
    {
        free(newEntry);
        return;
    }
    newEntry->value = 1;
    newEntry->next = ht->buckets[index];
    ht->buckets[index] = newEntry;
    ht->count++;
}

void ht_free(HashTable* ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        Entry* current = ht->buckets[i];

        while (current != NULL)
        {
            Entry* next = current->next;
            free(current->word);
            free(current);
            current = next;
        }
    }

    free(ht->buckets);
    free(ht);
}

Entry** ht_flat(const HashTable* ht)
{
    int flatCount = 0;
    Entry** flat = malloc(ht->count * sizeof(Entry*));

    if (flat == NULL)
    {
        printf("Out of memory");
        return NULL;
    }

    for (int i = 0; i < ht->size; i++)
    {
        Entry* current = ht->buckets[i];

        while (current != NULL)
        {
            flat[flatCount++] = current;
            current = current->next;
        }
    }

    return flat;
}

int ht_compare(const void* a, const void* b)
{
    // qsort passes pointer to each element — elements are Entry* so we get Entry**
    const Entry* nodeA = *(Entry**)a;
    const Entry* nodeB = *(Entry**)b;

    // descending — higher count first
    return nodeB->value - nodeA->value;
}
