// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 5490;

// Hash table
node *table[N];

// Dictionary size
unsigned int dictionarySize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Check if the word format is correct
    if (word[0] == '\'' || strlen(word) > LENGTH)
    {
        return false;
    }

    // Hash the word
    int index = hash(word);

    // Search for the word in the hash table at the specified index
    node *nav = table[index];
    while (nav != NULL)
    {
        if (!strcasecmp(nav->word, word))
        {
            return true;
        }
        nav = nav->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // Sum of the ascii values
    int sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        sum += tolower(word[i]);
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Create an array to store the word
    char *temp = malloc(LENGTH * sizeof(char) + 1);
    if (temp == NULL)
    {
        printf("Could not allocate space.\n");
        fclose(file);
        return false;
    }

    // Read each characters until escape line found
    char buffer;
    int i = 0;
    while (fread(&buffer, sizeof(char), 1, file))
    {
        if (buffer != '\n' && i < LENGTH)
        {
            temp[i] = buffer;
            i++;
        }
        else
        {
            temp[i] = '\0';
            // Hash the word
            int index = hash(temp);

            // Create a new node assign the word value to the node
            node *new = malloc(sizeof(node));
            if (new == NULL)
            {
                printf("Could not allocate memory.\n");
                fclose(file);
                free(temp);
                return false;
            }
            strcpy(new->word, temp);
            new->next = table[index];
            table[index] = new;

            dictionarySize++;

            // Reinitialize the temp variable and index i
            free(temp);
            i = 0;
            // Create an array to store the word
            temp = malloc(LENGTH * sizeof(char) + 1);
            if (temp == NULL)
            {
                printf("Could not allocate space.\n");
                fclose(file);
                return false;
            }
        }
    }

    // Close the dictionary and free the memory allocated
    fclose(file);
    free(temp);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return dictionarySize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *nav = NULL;
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            nav = table[i]->next;
            free(table[i]);
            table[i] = nav;
        }
    }

    return true;
}
