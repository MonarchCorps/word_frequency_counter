#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct WordNode
{
    char word[100];
    int count;
    struct WordNode* next;
} WordNode;

unsigned long djb2_hash(unsigned char* str);
void to_lowercase(char* str);
void insert(WordNode* map[], const char* word);

WordNode* map[10007];

int main(void)
{
    FILE* pF = fopen("word.txt", "r");
    char buffer[1024];

    if (pF == NULL)
    {
        printf("Couldn't open file\n");
        return 1;
    }
    else
    {
        while (fgets(buffer, sizeof(buffer), pF) != NULL)
        {
            char* token = strtok(buffer, " ,.!?;:\"'()-\n\t\r/\\@#$%^&*+=[]{}|<>~`");

            while (token != NULL)
            {
                to_lowercase(token);
                unsigned long index = djb2_hash((unsigned char*)token) % 10007;
                insert(map, token); // store it, don't just print

                token = strtok(NULL, " ,.!?;:\"'()-\n\t\r/\\@#$%^&*+=[]{}|<>~`");
            }
        }

        fclose(pF);

        // temporary — print first 10 non-empty slots
        int printed = 0;
        for (int i = 0; i < 10007 && printed < 10; i++)
        {
            if (map[i] != NULL)
            {
                WordNode* node = map[i];
                while (node != NULL)
                {
                    printf("%s: %d\n", node->word, node->count);
                    node = node->next;
                    printed++;
                }
            }
        }
    }

    return 0;
}

unsigned long djb2_hash(unsigned char* str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void to_lowercase(char* str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

void insert(WordNode* map[], const char* word)
{
    unsigned long index = djb2_hash((unsigned char*)word) % 10007;

    WordNode* current = map[index];

    // walk the chain — check if word already exists
    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->count++; // found — increment and done
            return;
        }
        current = current->next;
    }

    // word not found — create new node
    WordNode* newNode = malloc(sizeof(WordNode));
    if (newNode == NULL)
    {
        printf("Error: out of memory\n");
        return;
    }

    strcpy(newNode->word, word);
    newNode->count = 1;
    newNode->next = map[index]; // attach to front of chain
    map[index] = newNode; // update map to point to new node
}
