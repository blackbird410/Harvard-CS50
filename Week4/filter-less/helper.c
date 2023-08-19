#include "helpers.h"
#include <math.h>


int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }

    return b;
}

int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }

    return b;
}

typedef struct
{
    DWORD  rgbtBlue;
    DWORD  rgbtGreen;
    DWORD  rgbtRed;
} __attribute__((__packed__))
RGBSUM;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE avg;
    // Parse the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average RGB value for each pixels
            avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // Change all RGB values to the average value
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE sepiaRed, sepiaGreen, sepiaBlue, originalRed, originalGreen, originalBlue;

    // Parse the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute the new RGB values
            sepiaRed = min(round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue), 255);
            sepiaGreen = min(round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue), 255);
            sepiaBlue = min(round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue), 255);

            // Check if each values is greater than 0
            if (sepiaRed < 0 || sepiaGreen < 0 || sepiaBlue < 0)
            {
                sepiaRed = max(sepiaRed, 0);
                sepiaGreen = max(sepiaGreen, 0);
                sepiaBlue = max(sepiaBlue, 0);
            }

            // Change all RGB values to the new computed value
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    int k;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Taking the index to swap from the right
            k = width - j - 1;
            if (j != k)
            {
                // Assigning the pixel from the left to a temp variable
                temp = image[i][j];

                // Swapping the pixels
                image[i][j] = image[i][k];
                image[i][k] = temp;
            }
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBSUM sum;
    RGBTRIPLE copy[height][width];
    int h, v;
    float counter;

    // Copy the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Parse the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum.rgbtBlue = 0;
            sum.rgbtGreen = 0;
            sum.rgbtRed = 0;
            counter = 0;
            v = i - 1;

            if (v < 0)
            {
                v = 0;
            }

            // For each pixels add the neighboring pixels values to the sum
            while (v < (i + 2) && v < height)
            {
                h = j - 1;
                if (h < 0)
                {
                    h = 0;
                }
                while (h < (j + 2) && h < width)
                {
                    sum.rgbtBlue += copy[v][h].rgbtBlue;
                    sum.rgbtGreen += copy[v][h].rgbtGreen;
                    sum.rgbtRed += copy[v][h].rgbtRed;

                    h++;
                    counter++;
                }

                v++;
            }

            // Do the average and assign it to the current pixel
            image[i][j].rgbtBlue = round(sum.rgbtBlue / counter);
            image[i][j].rgbtGreen = round(sum.rgbtGreen / counter);
            image[i][j].rgbtRed = round(sum.rgbtRed / counter);
        }
    }
}

