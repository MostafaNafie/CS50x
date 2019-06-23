// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // create a new node and allocate memory to it
        node *newNode = malloc(sizeof(node));
        if (file == NULL)
        {
            unload();
            return false;
        }
        // initialize the pointer to next to avoid memory errors
        newNode->next = NULL;
        // populate that node with the word
        strcpy(newNode->word, word);
        // determine the bucket to put the node into
        int bucket = hash(word);
        // inset the node into the bucket
        if (hashtable[bucket] != NULL)
        {
            newNode->next = hashtable[bucket];
        }
        hashtable[bucket] = newNode;
        // DEBUG printf("%s %s %i\n", word, newNode->word, bucket);
    }
        
    // DEBUG node *testNode = hashtable[2]->next;
    // DEBUG printf("%s %s\n", hashtable[2]->word, testNode->word);

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int size = 0;
    // loop through buckets from 0 to 25
    for (int i = 0; i < N; i++)
    {
        // create a cursor to traverse the linked list
        node *cursor = hashtable[i];
        // loop through the nodes in the bucket
        while (cursor != NULL)
        {
            size++;
            cursor = cursor->next;
        }
    }
    // DEBUG printf("%i\n", size);
    return size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // find which bucket the word would be in
    int bucket = hash(word);
    // search that bucket for the word
    // create a cursor to traverse the linked list
    node *cursor = hashtable[bucket];
    // loop through the nodes in the bucket
    while (cursor != NULL)
    {
        // return true if the word is in the bucket
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // loop through buckets from 0 to 25
    for (int i = 0; i < N; i++)
    {
        // create a cursor to traverse the linked list
        node *cursor = hashtable[i];
        // loop through the nodes in the bucket
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}