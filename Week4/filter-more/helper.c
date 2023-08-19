#include "helpers.h"
#include <math.h>
#include <stdio.h>

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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBSUM sum, sumX, sumY;
    RGBTRIPLE copy[height][width];
    int h, v, wh, wv;
    int gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    int gy[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
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
            sumX.rgbtBlue = 0;
            sumX.rgbtGreen = 0;
            sumX.rgbtRed = 0;
            sumY.rgbtBlue = 0;
            sumY.rgbtGreen = 0;
            sumY.rgbtRed = 0;
            counter = 0;
            v = i - 1;
            wv = 0;
            if (v < 0)
            {
                v = 0;
                wv = 1;
            }

            // For each pixels in the neighboring pixels compute the weighted sum
            while (v < (i + 2) && v < height)
            {
                h = j - 1;
                wh = 0;
                if (h < 0)
                {
                    h = 0;
                    wh = 1;
                }

                while (h < (j + 2) && h < width)
                {

                    // Calculate the weighted sum for each color value
                    sumX.rgbtBlue += copy[v][h].rgbtBlue * gx[wv][wh];
                    sumX.rgbtGreen += copy[v][h].rgbtGreen * gx[wv][wh];
                    sumX.rgbtRed += copy[v][h].rgbtRed * gx[wv][wh];

                    sumY.rgbtBlue += copy[v][h].rgbtBlue * gy[wv][wh];
                    sumY.rgbtGreen += copy[v][h].rgbtGreen * gy[wv][wh];
                    sumY.rgbtRed += copy[v][h].rgbtRed * gy[wv][wh];

                    // printf("GX: %i\nGy: %i\nSxBlue: %i\n SyBlue: %i\n", gx[wv][wh], gy[wv][wh], sumX.rgbtBlue, sumY.rgbtBlue);

                    h++;
                    counter++;
                    wh++;
                }

                v++;
                wv++;
            }

            // Combine the two weighted sum
            image[i][j].rgbtBlue = min(round(sqrt(sumX.rgbtBlue * sumX.rgbtBlue + sumY.rgbtBlue * sumY.rgbtBlue)), 255);
            image[i][j].rgbtGreen = min(round(sqrt(sumX.rgbtGreen * sumX.rgbtGreen + sumY.rgbtGreen * sumY.rgbtGreen)), 255);
            image[i][j].rgbtRed = min(round(sqrt(sumX.rgbtRed * sumX.rgbtRed + sumY.rgbtRed * sumY.rgbtRed)), 255);

        }
    }
}

