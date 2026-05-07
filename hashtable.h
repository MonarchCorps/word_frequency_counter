//
// Created by David Okocha on 05/05/2026.
//

#ifndef WORD_FREQUENCY_COUNTER_HASH_TABLE_H
#define WORD_FREQUENCY_COUNTER_HASH_TABLE_H

typedef struct Entry
{
    char* word;
    int value;
    struct Entry* next;
} Entry;

typedef struct
{
    int count; // how many key-value pairs are stored. Goes up on put, down on delete.
    int size; // how many buckets exist. Set once at creation, never changes.
    Entry** buckets;
} HashTable;

HashTable* ht_create(int size);
void ht_put(HashTable* ht, char* word);
unsigned long ht_hash(const HashTable* ht, const char* word);
void ht_free(HashTable* ht);
Entry** ht_flat(const HashTable* ht);
int ht_compare(const void* a, const void* b);

#endif //WORD_FREQUENCY_COUNTER_HASH_TABLE_H
