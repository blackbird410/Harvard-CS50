#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // Ask the user the message
    string message = get_string("Message: ");

    //Convert the string into 8-bit binary numbers
    char bins[1000] = "";
    char n;
    char bits[8];
    char binaries[1000];
    int k, counter = 0;

    // For every character
    for (int i = 0; i < strlen(message); i++)
    {
        // Take its ascii value and save it in n
        n = message[i];
        k = 0;
        while (n > 0)
        {
            bits[k] = (n % 2) + '0';
            n /= 2;
            k++;
        }

        while (k < 8)
        {
            bits[k] = '0';
            k++;
        }

        for (int j = 7; j >= 0; j--)
        {
            // Reverse adding of the current bits
            binaries[counter] = bits[j];
            counter++;
        }
    }
    binaries[counter] = '\0';
    strcpy(bins, binaries);

    //Print the encoded message using the given print_bulb function
    char currentBit;
    for (int i = 0; i < strlen(bins); i++)
    {
        currentBit = bins[i];
        if (currentBit == 48)
        {
            print_bulb(0);
        }
        else
        {
            print_bulb(1);
        }

        if (i >= 7 && (i + 1) % 8 == 0)
        {
            printf("\n");
        }
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}

