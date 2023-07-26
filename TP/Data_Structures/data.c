#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the linked list structure
typedef struct node
{
    char name[20];
    struct node *next;
} node;


// Define a function to see the content of the list
void vizualize(node *list);

// Define a function to free the linked list
void unload(node *list);

int main(int argc, char *argv[])
{
    // Create a datalist that takes n number of names with linked list
    
    // Check the terminal argument
    if (argc != 2)
    {
        printf("Wrong usage: ./data 4\n");
        return 1;
    }

    int i = 0;
    while(i < strlen(argv[1]))
    {
        if (argv[1][i] < 48 || argv[1][i] > 57)
        {
            printf("Wrong usage: ./data 4\nYou must give a number parameter.\n");
            return 1;
        }
        i++;
    }

    int number = atoi(argv[1]);

    // Initialize a list
    node *list = NULL;

    for (i = 0; i < number; i++)
    {
        // Create a new node and allocate some memory to it
        node *n = malloc(sizeof(node));
        // Check if memory was allocated
        if (n == NULL)
        {
            printf("Program failed to allocate memory.\nExiting...\n");
            return 1;
        }

        // Add the value to the node
        printf("Enter a name: ");
        gets(n->name);
        
        // Add the node to the beginning of the list
        n->next = list;

        // Update the list head
        list = n;
    } 

    // Vizualize the list
    vizualize(list);

    // Free the memory allocated
    unload(list);
    return 0;

}

void vizualize(node *list)
{
    node *ptr = list;
    printf("\n+--- LIST VIZUALIZER ---+\n");
    while (ptr != NULL)
    {
        printf("\t%s\n", ptr->name);
        ptr = ptr->next;
    }
    printf("+--- LIST END ---+\n\n");   
}

void unload(node *list)
{
    // This function takes a linked list as a parameter and free the memory allocated
    while(list != NULL)
    {
        node *ptr = list->next;
        free(list);
        list = ptr;
    }
}