#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "string_utils.h"

int main()
{
    FILE* pF = fopen("word.txt", "r");
    if (pF == NULL)
    {
        perror("Couldn't open file");
        return 1;
    }

    char buffer[1024];

    const int size = 10007;
    HashTable* ht = ht_create(size);

    if (ht == NULL)
    {
        perror("Out of memory");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        char* token = strtok(buffer, " ,.!?;:\"'()-\n\t\r/\\@#$%^&*+=[]{}|<>~`");

        while (token != NULL)
        {
            to_lowercase(token);
            ht_put(ht, token);
            token = strtok(NULL, " ,.!?;:\"'()-\n\t\r/\\@#$%^&*+=[]{}|<>~`");
        }
    }

    Entry** flat = ht_flat(ht);

    qsort(flat, ht->count, sizeof(Entry*), ht_compare);

    const int top = ht->count < 20 ? ht->count : 20;
    printf("%-20s %s\n", "WORD", "COUNT");
    printf("%-20s %s\n", "----", "-----");
    for (int i = 0; i < top; i++)
    {
        printf("%-20s %d\n", flat[i]->word, flat[i]->value);
    }
    free(flat);

    fclose(pF);
    ht_free(ht);
    return 0;
}
