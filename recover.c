#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Number of bytes in a block
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open memory card
    FILE *raw_file = fopen(argv[1], "r");

    // Print error message if unable to open memory card
    if (raw_file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Declare variables
    uint8_t buffer[BLOCK_SIZE];
    string name1 = "000000000";
    string name = malloc(strlen(name1));
    int number = 0;
    FILE *outputfile;

    // Do the following until end of file is reached
    while (fread(buffer, BLOCK_SIZE, 1, raw_file) == 1)
    {
        // If beginning of jpeg is found, ope a new file
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (number == 0)
            {
                sprintf(name, "%03i.jpg", number);
                outputfile = fopen(name, "w");
                fwrite(buffer, BLOCK_SIZE, 1, outputfile);
                number++;
            }

            else if (number > 0)
            {
                // Close current jpeg before attempting to open another one
                fclose(outputfile);
                sprintf(name, "%03i.jpg", number);
                outputfile = fopen(name, "w");
                fwrite(buffer, BLOCK_SIZE, 1, outputfile);
                number++;
            }
        }

        // Else, keep writing into previous jpeg
        else
        {
            if (number > 0)
            {
                fwrite(buffer, BLOCK_SIZE, 1, outputfile);
            }
        }
    }
    // Close memory card
    fclose(raw_file);
}