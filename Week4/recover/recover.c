#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check the command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    // Check if the forensic image can be opened
    if (file == NULL)
    {
        printf("File could not be opened.\n");
        return 1;
    }

    int i = 0, j = 0, sig_counter = 0;
    int file_count = 0;
    FILE *output;

    // Read the file as blocks of 512 bytes
    uint8_t buffer[512];
    while (fread(buffer, 1, 512, file))
    {
        i = 0;
        // When a signature is found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            // Close the previous file
            if (file_count > 0)
            {
                fclose(output);
            }
            // Create the new output file and start writing in it
            char filename[8];
            sprintf(filename, "%03i.jpg", file_count);
            output = fopen(filename, "w");
            if (output ==  NULL)
            {
                fclose(file);
                printf("Could not create the file %s.\n", filename);
                return 1;
            }
            fwrite(buffer, sizeof(uint8_t), sizeof(buffer), output);
            file_count++;
        }
        else
        {
            // If a file has already been created, write the data until new jpeg file is found
            if (file_count > 0)
            {
                fwrite(buffer, sizeof(uint8_t), sizeof(buffer), output);
            }
        }
        i++;

    }

    // Close the opened files and free the memories allocated
    fclose(file);
    fclose(output);

    return 0;
}

