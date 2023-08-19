#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool test_arg(int argc, string argv[]);
string rotate(string plaintext, int key);

int main(int argc, string argv[])
{
    // Check if the key is correct
    if (!test_arg(argc, argv))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Save the key
    int key = 0;

    string str = argv[1];

    // converting string to number
    for (int i = 0; str[i] != '\0'; i++)
    {
        key = key * 10 + (str[i] - 48);
    }

    // Ask the user for the plaintext
    string plaintext = get_string("Plaintext:  ");

    // Encrypt the message
    string ciphertext = rotate(plaintext, key);

    // Return the encrypted message
    printf("ciphertext: %s", ciphertext);
    printf("\n");

    return 0;
}

bool test_arg(int argc, string argv[])
{
    if (argc != 2)
    {
        return false;
    }

    string key = argv[1];
    // If any character of the key is not a number : only_digits
    for (int i = 0; i < strlen(key); i++)
    {
        if ((char) key[i] < 48 || (char) key[i] > 57)
        {
            return false;
        }
    }

    return true;
}

string rotate(string plaintext, int k)
{
    char currentChar;
    char c;
    int uOl;

    for (int i = 0; i < strlen(plaintext); i++)
    {
        currentChar = plaintext[i];
        if ((currentChar > 64 && currentChar < 91) || (currentChar > 96 && currentChar < 123))
        {
            uOl = 64;
            if (currentChar > 96 && currentChar < 123)
            {
                uOl = 96;
            }
            currentChar -= uOl;
            c = ((int) currentChar + k) % 26;
            plaintext[i] = (char) c + uOl;
        }
    }
    return plaintext;
}

