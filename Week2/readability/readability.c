#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //Take the input of the user
    string text = get_string("Text: ");

    // Count the number of letters, words and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Coleman-Liau formula : index = 0.0588 * L - 0.296 * S - 15.8
    float l = ((float) letters / words) * 100;
    float s = ((float) sentences / words) * 100;
    int index = round(0.0588 * l - 0.296 * s - 15.8);

    //Return the grade level
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}

int count_letters(string text)
{
    int letters = 0;
    char currentChar;

    for (int i = 0; i < strlen(text); i++)
    {
        currentChar = toupper(text[i]);
        if (currentChar > 64 && currentChar < 91)
        {
            // Counting letters
            letters += 1;
        }
    }

    return letters;
}

int count_words(string text)
{
    int words = 1;
    char currentChar;

    for (int i = 0; i < strlen(text); i++)
    {
        currentChar = toupper(text[i]);
        if (currentChar == 32)
        {
            words += 1;
        }
    }
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    char currentChar;

    for (int i = 0; i < strlen(text); i++)
    {
        currentChar = toupper(text[i]);
        if (currentChar == 33 || currentChar == 46 || currentChar == 63)
        {
            sentences += 1;
        }
    }
    return sentences;
}

//******************THIS VERSION IS BETTER AND FASTER
//******************CONTAINS LESS LOOPS OVER THE STRING, ONE IS ENOUGH

// int checkGrade(string text);

// int main(void)
// {
//     //Take the input of the user
//     string text = get_string("Text: ");

//     //Send it to a function that will determine the grade level
//     int grade = checkGrade(text);

//     //Return the grade level
//     if (grade >= 16)
//     {
//         printf("Grade 16+\n");
//     }
//     else if (grade < 1)
//     {
//         printf("Before Grade 1\n");
//     }
//     else
//     {
//         printf("Grade %i\n", grade);
//     }

// }

// int checkGrade(string text)
// {
//     int letters = 0;
//     int words = 0;
//     int sentences = 0;
//     char currentChar;

//     // Count the number of letters, words and sentences
//     for (int i = 0; i < strlen(text); i++)
//     {
//         currentChar = toupper(text[i]);
//         if (currentChar > 64 && currentChar < 91)
//         {
//             // Counting letters
//             letters += 1;
//         }

//         if (currentChar == 32)
//         {
//             words += 1;
//         }

//         if (currentChar == 33 || currentChar == 46 || currentChar == 63)
//         {
//             sentences += 1;
//         }
//     }
//     // Adding the last word
//     words += 1;

//     // Coleman-Liau formula : index = 0.0588 * L - 0.296 * S - 15.8
//     float l = ((float) letters / words) * 100;
//     float s = ((float) sentences / words) * 100;
//     int index = round(0.0588 * l - 0.296 * s - 15.8);

//     return index;
// }
