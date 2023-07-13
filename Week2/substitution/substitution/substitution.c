#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int UPPERCASE_L_BOUND = 64;
const int UPPERCASE_U_BOUND = 91;
const int LOWERCASE_L_BOUND = 96;
const int LOWERCASE_U_BOUND = 123;

char *substitute(char *plaintext, char *key);

int main(int argc, string argv[])
{
    // Checks if one argument has been provided
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Checks if the provided key is correct
    int l = strlen(argv[1]);
    if (l != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    char *key = argv[1];
    // Checks for invalid characters or duplicate characters
    for (int i = 0; i < l; i++)
    {
        if (toupper(key[i]) < UPPERCASE_L_BOUND || toupper(key[i]) > UPPERCASE_U_BOUND)
        {
            printf("Key must contain letters.\n");
            return 1;
        }

        for (int j = 0; j < l; j++)
        {
            if (key[i] == key[j] && i != j)
            {
                printf("Key must not contain duplicate letters.\n");
                return 1;
            }
        }
    }

    // Ask the user for the plaintext
    char *plaintext = get_string("plaintext: ");

    // Encrypt and return the encrypted text to the output
    printf("ciphertext: %s\n", substitute(plaintext, key));
}

char *substitute(char *plaintext, char *key)
{
    int length = strlen(plaintext);
    int char_index = 0;

    // Parse the plaintext
    for (int i = 0; i < length; i++)
    {
        // Checks if the current character is a letter
        if (toupper(plaintext[i]) > UPPERCASE_L_BOUND && toupper(plaintext[i]) < UPPERCASE_U_BOUND)
        {
            // Checks if it is uppercase
            if (plaintext[i] > UPPERCASE_L_BOUND && (int) plaintext[i] < UPPERCASE_U_BOUND)
            {
                // Substitute the characters based on the key array
                char_index = plaintext[i] - UPPERCASE_L_BOUND - 1;
                plaintext[i] = toupper(key[char_index]);
            }
            else
            {
                // Substitute the characters based on the key array
                char_index = plaintext[i] - LOWERCASE_L_BOUND - 1;
                plaintext[i] = tolower(key[char_index]);
            }
        }
    }

    return plaintext;
}