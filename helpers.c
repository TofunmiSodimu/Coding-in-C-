#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Assign variables
    int blue;
    int red;
    int green;

    // Iterate through all pixels in image
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            // Get pixel values
            blue = image[x][y].rgbtBlue;
            red = image[x][y].rgbtRed;
            green = image[x][y].rgbtGreen;

            // Calculate average of pixel values and assign it to each pixel
            image[x][y].rgbtBlue = round((blue + red + green) / 3.0);
            image[x][y].rgbtRed = round((blue + red + green) / 3.0);
            image[x][y].rgbtGreen = round((blue + red + green) / 3.0);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Assign variables
    RGBTRIPLE placeholder[height][width];

    // Iterate through all pixels in image
    for (int x = 0; x < height; x++)
    {
        // Store pixels in a placeholder
        for (int y = 0; y < width; y++)
        {
            placeholder[x][y].rgbtBlue = image[x][y].rgbtBlue;
            placeholder[x][y].rgbtRed = image[x][y].rgbtRed;
            placeholder[x][y].rgbtGreen = image[x][y].rgbtGreen;
        }

        // Move pixels from left to right of image
        int z = 0;
        for (int a = (width - 1); a >= 0; a--)
        {
            image[x][z].rgbtBlue = placeholder[x][a].rgbtBlue;
            image[x][z].rgbtRed = placeholder[x][a].rgbtRed;
            image[x][z].rgbtGreen = placeholder[x][a].rgbtGreen;
            z++;
        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Assign variables
    int Blue;
    int Red;
    int Green;
    int sumBlue = 0;
    int sumRed = 0;
    int sumGreen = 0;
    int count1;
    int count2;
    int z;
    int a;
    double number;
    RGBTRIPLE placeholder[height][width];

    // Iterate through all pixels in image
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            // Assign variables
            sumBlue = 0;
            sumRed = 0;
            sumGreen = 0;
            number = 0;
            count1 = 0;
            count2 = 0;
            z = y - 1;
            a = x - 1;

            // Choose pixel values from three columns
            while (count2 < 3)
            {
                if (((a > 0) || (a == 0)) && (a < height))
                {
                    // Choose pixel values from three rows
                    while (count1 < 3)
                    {
                        if (((z > 0) || (z == 0)) && (z < width))
                        {
                            // Add up values of pixels surrounding and including current pixel
                            Blue = image[a][z].rgbtBlue;
                            Red = image[a][z].rgbtRed;
                            Green = image[a][z].rgbtGreen;
                            sumBlue += Blue;
                            sumRed += Red;
                            sumGreen += Green;
                            number++;
                        }
                        z++;
                        count1++;
                    }
                }
                count1 = 0;
                a++;
                count2++;
                z = y - 1;
            }

            // Calculate average of pixels surrounding and including current pixel and store in array
            placeholder[x][y].rgbtBlue = round(sumBlue / number);
            placeholder[x][y].rgbtRed = round(sumRed / number);
            placeholder[x][y].rgbtGreen = round(sumGreen / number);
        }
    }

    // Assign new values from placeholder array to each pixel
    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l].rgbtBlue = placeholder[k][l].rgbtBlue;
            image[k][l].rgbtRed = placeholder[k][l].rgbtRed;
            image[k][l].rgbtGreen = placeholder[k][l].rgbtGreen;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Assign variables
    int Blue;
    int Red;
    int Green;
    int xsumBlue = 0;
    int xsumRed = 0;
    int xsumGreen = 0;
    int ysumBlue = 0;
    int ysumRed = 0;
    int ysumGreen = 0;
    int count1;
    int count2;
    int z;
    int a;
    double blueEdge;
    double redEdge;
    double greenEdge;
    RGBTRIPLE placeholder2[height][width];
    RGBTRIPLE box[3][3];
    int xfilter[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int yfilter[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Iterate through all pixels in image
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            xsumBlue = 0;
            xsumRed = 0;
            xsumGreen = 0;
            ysumBlue = 0;
            ysumRed = 0;
            ysumGreen = 0;
            count1 = 0;
            count2 = 0;
            z = y - 1;
            a = x - 1;

            // Choose pixels from three colums
            while (count2 < 3)
            {
                if (((a > 0) || (a == 0)) && (a < height))
                {
                    // Choose pixels from three rows
                    while (count1 < 3)
                    {
                        // Store values of pixels around current pixel, including current pixel, in an array called box
                        if (((z > 0) || (z == 0)) && (z < width))
                        {
                            box[count2][count1].rgbtBlue = image[a][z].rgbtBlue;
                            box[count2][count1].rgbtRed = image[a][z].rgbtRed;
                            box[count2][count1].rgbtGreen = image[a][z].rgbtGreen;
                        }

                        // If surrounding pixel does not exist, assign zero to the new pixel value
                        else
                        {
                            box[count2][count1].rgbtBlue = 0x0;
                            box[count2][count1].rgbtRed = 0x0;
                            box[count2][count1].rgbtGreen = 0x0;
                        }
                        z++;
                        count1++;
                    }
                }

                // If surrounding pixel does not exist, assign zero to the new pixel value
                else
                {
                    box[count2][0].rgbtBlue = 0x0;
                    box[count2][0].rgbtRed = 0x0;
                    box[count2][0].rgbtGreen = 0x0;
                    box[count2][1].rgbtBlue = 0x0;
                    box[count2][1].rgbtRed = 0x0;
                    box[count2][1].rgbtGreen = 0x0;
                    box[count2][2].rgbtBlue = 0x0;
                    box[count2][2].rgbtRed = 0x0;
                    box[count2][2].rgbtGreen = 0x0;
                }
                count1 = 0;
                a++;
                count2++;
                z = y - 1;
            }

            // Multiply values in box array by the x and y Sobel filter and sum up the values
            for (int d = 0; d < 3; d++)
            {
                for (int e = 0; e < 3; e++)
                {
                    xsumBlue += (box[d][e].rgbtBlue * xfilter[d][e]);
                    xsumRed += (box[d][e].rgbtRed * xfilter[d][e]);
                    xsumGreen += (box[d][e].rgbtGreen * xfilter[d][e]);
                    ysumBlue += (box[d][e].rgbtBlue * yfilter[d][e]);
                    ysumRed += (box[d][e].rgbtRed * yfilter[d][e]);
                    ysumGreen += (box[d][e].rgbtGreen * yfilter[d][e]);
                }
            }

            // Find the square root of the summed up squared x and y sums
            blueEdge = round(sqrt((xsumBlue * xsumBlue) + (ysumBlue * ysumBlue)));
            redEdge = round(sqrt((xsumRed * xsumRed) + (ysumRed * ysumRed)));
            greenEdge = round(sqrt((xsumGreen * xsumGreen) + (ysumGreen * ysumGreen)));

            // Store calculated values in placeholder array if less than 255, else store 255
            if (blueEdge < 255)
            {
                placeholder2[x][y].rgbtBlue = blueEdge;
            }
            else
            {
                placeholder2[x][y].rgbtBlue = 255;
            }

            if (greenEdge < 255)
            {
                placeholder2[x][y].rgbtGreen = greenEdge;
            }
            else
            {
                placeholder2[x][y].rgbtGreen = 255;
            }

            if (redEdge < 255)
            {
                placeholder2[x][y].rgbtRed = redEdge;
            }
            else
            {
                placeholder2[x][y].rgbtRed = 255;
            }
            xsumBlue = 0;
            xsumRed = 0;
            xsumGreen = 0;
            ysumBlue = 0;
            ysumRed = 0;
            ysumGreen = 0;
        }

        // Clear box array
        for (int g = 0; g < 3; g++)
        {
            for (int h = 0; h < 3; h++)
            {
                box[g][h].rgbtBlue = 0x0;
                box[g][h].rgbtRed = 0x0;
                box[g][h].rgbtGreen = 0x0;
            }
        }
    }

    // Assign values in placeholder array as new pixel values in image
    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            image[k][l].rgbtBlue = placeholder2[k][l].rgbtBlue;
            image[k][l].rgbtRed = placeholder2[k][l].rgbtRed;
            image[k][l].rgbtGreen = placeholder2[k][l].rgbtGreen;

        }
    }
    return;
}
