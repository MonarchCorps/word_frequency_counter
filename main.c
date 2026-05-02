#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WordNode
{
    char word[100];
    int count;
    struct WordNode* next;
} WordNode;

WordNode* map[10007];

// Returns a 64-bit unsigned hash value for a given string — large range prevents
// overflow during multiplication, unsigned ensures safe modulo indexing into map
unsigned long djb2_hash(unsigned char* str);
void to_lowercase(char* str);
void insert(WordNode* map[], const char* word);
int compare(const void* a, const void* b);

int main(void)
{
    FILE* pF = fopen("word.txt", "r");
    char buffer[1024] = {0};

    if (pF == NULL)
    {
        printf("Couldn't open the file\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), pF) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        char* token = strtok(buffer, " ,.!?;:\"'()-\n\t\r/\\@#$%^&*+=[]{}|<>~`");

        while (token != NULL)
        {
            to_lowercase(token);
            insert(map, token);
            token = strtok(NULL, " ,.!?;:\"'()-\n\t\r/\\@#$%^&*+=[]{}|<>~`");
        }
    }

    fclose(pF);

    // stage 5 — collect all nodes into flat array
    int capacity = 100;
    int count = 0;
    WordNode** flat = malloc(capacity * sizeof(WordNode*));
    if (flat == NULL)
    {
        printf("Out of memory\n");
        return 1;
    }

    for (int i = 0; i < 10007; i++)
    {
        WordNode* node = map[i];
        while (node != NULL)
        {
            // grow array if full
            if (count == capacity)
            {
                capacity *= 2;
                WordNode** temp = realloc(flat, capacity * sizeof(WordNode*));
                if (temp == NULL)
                {
                    printf("Out of memory\n");
                    free(flat);
                    return 1;
                }
                flat = temp;
            }
            flat[count++] = node;
            node = node->next;
        }
    }

    // stage 6 — sort by frequency descending, print top 20
    qsort(flat, count, sizeof(WordNode*), compare);

    int top = count < 20 ? count : 20;
    printf("%-20s %s\n", "WORD", "COUNT");
    printf("%-20s %s\n", "----", "-----");
    for (int i = 0; i < top; i++)
    {
        printf("%-20s %d\n", flat[i]->word, flat[i]->count);
    }

    // stage 7 — free all nodes, then free flat array
    // free nodes first — flat array holds the only pointers to them
    for (int i = 0; i < 10007; i++)
    {
        WordNode* node = map[i];
        while (node != NULL)
        {
            WordNode* next = node->next; // save next before freeing current
            free(node);
            node = next;
        }
    }

    free(flat);

    return 0;
}

int compare(const void* a, const void* b)
{
    // qsort passes pointer to each element — elements are WordNode* so we get WordNode**
    WordNode* nodeA = *(WordNode**)a;
    WordNode* nodeB = *(WordNode**)b;

    // descending — higher count first
    return nodeB->count - nodeA->count;
}

void to_lowercase(char* str)
{
    for (int i = 0; str[i]; i++)
    {
        // (unsigned char) cast reinterprets bit pattern as unsigned first
        // so a char that was -28 becomes 228 — valid input to tolower
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

unsigned long djb2_hash(unsigned char* str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        // hash * 33 + c — bit shift for speed
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
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
        printf("Out of memory\n");
        return;
    }

    strcpy(newNode->word, word);
    newNode->count = 1;
    newNode->next = map[index]; // attach to front of chain
    map[index] = newNode; // update map to point to new node
}
