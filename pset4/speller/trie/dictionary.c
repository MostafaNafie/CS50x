// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// a recursive function to calculate the number of used children of all nodes of the trie (not part of the problem set)
unsigned int recChildren(node *cursor)
{
    unsigned int children = 0;

    for (int i = 0; i < N; i++)
    {
        // recursive case
        if (cursor->children[i] != NULL)
        {
            children++;
            children += recChildren(cursor->children[i]);
        }
    }

    return children;  
}

// a recursive function to calculate the number of all children of all nodes of the trie (not part of the problem set)
unsigned int recAllChildren(node *cursor)
{
    unsigned int children = 0;

    for (int i = 0; i < N; i++)
    {
        children++;
        //DEBUG printf("children no %i\n" ,children);

        // recursive case
        if (cursor->children[i] != NULL)
        {  
            children += recAllChildren(cursor->children[i]);
        }
    }

    return children;  
}

// a recursive function to calculate the number of all nodes of the trie (not part of the problem set)
unsigned int recNodes(node *cursor)
{
    unsigned int nodes = 1;

    for (int i = 0; i < N; i++)
    {
        // DEBUG printf("node no %i\n" ,nodes);

        // recursive case
        if (cursor->children[i] != NULL)
        {  
            nodes += recNodes(cursor->children[i]);
        }
    }

    return nodes;  
}

// a recursive function to calculate the number of words in the trie
unsigned int recSize(node *cursor)
{
    unsigned int size = 0;

    if (cursor->is_word == true)
        {
            size++;
        }

    for (int i = 0; i < N; i++)
    {
        // recursive case
        if (cursor->children[i] != NULL)
        {
            size += recSize(cursor->children[i]);
        }
    }

    return size;  
}

// a recursive function to free the nodes of the trie
void recFree(node *cursor)
{
    

    for (int i = 0; i < N; i++)
    {
        //DEBUG printf("node no %i\n" ,nodes);

        // recursive case
        if (cursor->children[i] != NULL)
        {  
            recFree(cursor->children[i]);
        }
    }

    free(cursor);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
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

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // create a cursor and initialize it to the root
        node *cursor = root;

        // iterate over each letter word
        for (int i = 0, j = strlen(word); i < j; i++)
        {
            char letter = word[i];
            // get the index of the letter from 0 to 25 if alpha and 26 if apostrophe
            int index = (isalpha(letter)) ? letter - 'a' : 26;
            
            // check if a node for this letter already exists
            if (cursor->children[index] == NULL)
            {
                // create a new node and allocate memory for it
                node *newNode = malloc(sizeof(node));
                if (newNode == NULL)
                {
                    unload();
                    return false;
                }
                // initialize the new node
                newNode->is_word = false;
                for (int m = 0; m < N; m++)
                {
                    newNode->children[m] = NULL;
                }
                // insert that node in its place
                cursor->children[index] = newNode;
            }

            // move the cursor to the node
            cursor = cursor->children[index];

            // check if this is the last letter in the word
            if ( i == j - 1)
            {
                cursor->is_word = true;
            }
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

    // get the number of words by passing the pointer of the root to a recursive function
    unsigned int size = recSize(root);

    return size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // create a cursor and initialize it to the root
        node *cursor = root;

        // iterate over each letter word
        for (int i = 0, j = strlen(word); i < j; i++)
        {
            char letter = word[i];
            // get the index of the letter from 0 to 25 if alpha and 26 if apostrophe
            int index = (isalpha(letter)) ? tolower(letter) - 'a' : 26;
            
            // check if a node for this letter already exists
            if (cursor->children[index] == NULL)
            {
                return false;
            }

            // move the cursor to the node
            cursor = cursor->children[index];

            // check if this is the last letter in the word
            if ( i == j - 1)
            {
                return cursor->is_word;
            }
        }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // create a cursor and initialize it to the root
    // node *cursor = root;

    // get number of all nodes in the trie
    // unsigned int nodes = recNodes(root);
    // printf("number of all nodes including root = %i\n", nodes);

    // get number of all children of all nodes in the trie (not requird in the problem set)
    // unsigned int children = recAllChildren(root);
    // printf("number of all children (also equals: number of nodes * number of children in each node) = %i\n", children);

    // get number of used children of nodes in the trie (not requird in the problem set)
    // unsigned int usedChildren = recChildren(root);
    // printf("number of used children (also equals: number of nodes - 1) = %i\n", usedChildren);


    // call a recursive function to free the nodes
    recFree(root);
    // DEBUG printf("Done Freeing\n");

    return true;
}
