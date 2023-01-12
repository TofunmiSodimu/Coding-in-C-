// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Declare head of linked list for each cell in hashtable
node *head[N];

// Counter for words in dictionary
long word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Declare variables
    unsigned int key = 0;
    int count = 0;

    // Get key for word
    key = hash(word);

    node *traversal2 = malloc(sizeof(node));
    traversal2->word[0] = 'a';
    traversal2->next = NULL;
    traversal2 = table[key];
    node *placeholder2 = table[key];

    // Check through words in letter section of dictionary
    while (traversal2 != NULL)
    {
        count = 0;

        // Check if word is same as word in traversal node
        for (unsigned long i = 0; i < strlen(word); i++)
        {
            if (toupper(traversal2->word[i]) == toupper(word[i]))
            {
                count++;
            }
            else
            {
                continue;
            }
        }
        if ((count == strlen(word)) && (traversal2->word[count] == '\0'))
        {
            return true;
        }

        // Move to next node
        else
        {
            placeholder2 = traversal2->next;
            traversal2 = placeholder2;
        }
    }
    free(traversal2);
    free(placeholder2);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Declare variables
    char d;
    unsigned int key;
    int myindex = 0;
    bool node_full[N];

    // Fill node_full array with false
    for (int a = 0; a < N; a++)
    {
        node_full[a] = false;
    }

    // Open dictionary
    FILE *file2 = fopen(dictionary, "r");

    // If unable to open dictionary
    if (file2 == NULL)
    {
        return false;
    }

    // Create new cell/node
    node *cell = malloc(sizeof(node));
    if (cell == NULL)
    {
        return 1;
    }
    cell->next = NULL;
    myindex = 0;

    // Iterate through words in dictionary
    while (fread(&d, sizeof(char), 1, file2) == 1)
    {
        // Add character to new word
        if (isalpha(d) || (d == '\''))
        {
            cell->word[myindex] = d;
            myindex++;
        }
        else if (d == '\n')
        {
            // End word
            cell->word[myindex] = '\0';

            // Get key for word from hash function
            key = hash(cell->word);
            word_count++;

            // If table cell with corresponding key has a NULL pointer
            if (node_full[key] == false)
            {
                //Link word to hashtable
                table[key] = cell;

                // Note head of linked list
                head[key] = cell;

                // Update node_full array
                node_full[key] = true;
            }

            // If cell in table is full
            else
            {
                // Link word to other word in node
                cell->next = table[key];

                // Link cell in hashtable to new word
                table[key] = cell;

                // Note head of linked list
                head[key] = cell;
            }
            // Create new cell/node
            cell = malloc(sizeof(node));
            if (cell == NULL)
            {
                return 1;
            }
            cell->next = NULL;
            myindex = 0;
        }
    }
    free(cell);

    // Check whether there was an error while reading dictionary
    if (ferror(file2))
    {
        fclose(file2);
        return false;
    }

    // Close dictionary
    fclose(file2);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through each cell in hashtable and clear associated linked lists
    for (int i = 0; i < N; i++)
    {
        // Do this till the table cell does not point to any node
        while (table[i] != NULL)
        {
            node *placeholder = table[i]->next;
            free(table[i]);
            table[i] = placeholder;
        }
    }
    return true;
}
