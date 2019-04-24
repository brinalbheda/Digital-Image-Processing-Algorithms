// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the cat input image is of size 390 x 300 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

//EE 569: Homework 1 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 01/22/2019

/*
Algorithm implemented in C++:
• Read cat.raw image which is a gray scale image using fread() function
• Define 3 color channels: channel 0→Red, channel 1→Green, channel 2→Blue
• Add two boundary lines to the input image by boundary extension using mirror reflecting on all sides of the image border
• Apply demosaicing by using the 5x5 convolution filter for the bilinear interpolation
• Adding the remaining two missing green/red/blue channel values to the R/G/B pixel location by using the formula mentioned above by the combination of 8 different filters
• Using two sets of functions for green pixel location for odd and even values
• While applying MHC, since the correction terms use plus and minus coefficients, it will make some values go above 255 or below 0, assign 255 to values above 255 and 0 to values below 0
• Crop the boundary extended image to the original image size after the computation is done
• Write the computed image data array on output.raw file using the fwrite() function
*/

// To run the code, open terminal, add path and run using following commands
// g++ -o 1b 1b_mhc_demosaicing.cpp
// ./1b cat.raw output_1b.raw 3

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 3;
	int width_size = 390;
	int height_size = 300;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			//Size = atoi(argv[4]);
		}
	}
	
	// Allocate image data array of input image
	unsigned char Imagedata[height_size][width_size][1];     //BytesPerPixel = 1

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height_size*width_size*1, file);     //BytesPerPixel = 1 -> gray image
	fclose(file);
 

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	//Problem 1b

	int i,j,k;             //i->columns, j->rows, k->BytesPerPixel
	//color channel for each pixel in the array is: R,G,B,R,G,B,R,...
	int R_channel = 0;       //red channel
	int G_channel = 1;       //green channel
	int B_channel = 2;       //blue channel
	int x,y;
	double z;                //z->temp value

	//boundary extension by 2 pixels on all sides

	//add 1 boundary line on all sides of the input image
	int extend_boundary = 1;

	unsigned char Image_with_boundary_extension0[height_size+2*extend_boundary][width_size+2*extend_boundary][BytesPerPixel];

	//boundary extension by using mirror-reflecting to deal with boundary pixels

	//getting the input image at the centre point on boundary extended image
	for (int i = 0; i < height_size; i++)
    {
        for (int j = 0; j < width_size; j++)
        {
            for (int k = 0; k < BytesPerPixel; k++)
			{
				Image_with_boundary_extension0[i+extend_boundary][j+extend_boundary][k] = Imagedata[i][j][0];
			}
		}
	}

	//adding the top boundary
	for (int j = extend_boundary; j < width_size+2*extend_boundary; j++)
    {
      
		for (int k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension0[0][j][k] = Image_with_boundary_extension0[extend_boundary+1][j][k];  
		}
		
	}

	//adding the left boundary

	for (int i = 0; i < height_size+2*extend_boundary; i++)
    {
		for (int k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension0[i][0][k] = Image_with_boundary_extension0[i][extend_boundary+1][k];
		}
	}

	//adding the bottom boundary

	for (int j = extend_boundary; j < width_size+2*extend_boundary; j++)
    {
		for (int k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension0[2*extend_boundary+height_size-1][j][k] = Image_with_boundary_extension0[height_size-1][j][k];
		}
	}

	//adding the right boundary

	for (int i = 0; i < height_size+2*extend_boundary; i++)
    {
            for (int k = 0; k < BytesPerPixel; k++)
			{
                Image_with_boundary_extension0[i][2*extend_boundary+width_size-1][k] = Image_with_boundary_extension0[i][width_size-1][k];
			}
	}

	//adding the boundary extension by one pixel on the height and width size
	height_size += 2 * extend_boundary;
	width_size += 2 * extend_boundary;


	//add 1 more boundary line on all sides of the input image
	int extend_boundary1 = 1;

	unsigned char Image_with_boundary_extension[height_size+2*extend_boundary1][width_size+2*extend_boundary1][BytesPerPixel];

	//boundary extension by using mirror-reflecting to deal with boundary pixels

	//getting the input image at the centre point on boundary extended image
	for (int i = 0; i < height_size; i++)
    {
        for (int j = 0; j < width_size; j++)
        {
            for (int k = 0; k < BytesPerPixel; k++)
			{
				Image_with_boundary_extension[i+extend_boundary1][j+extend_boundary1][k] = Image_with_boundary_extension0[i][j][0];
			}
		}
	}

	//adding the top boundary
	for (int j = extend_boundary1; j < width_size+2*extend_boundary1; j++)
    {
      
		for (int k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension[0][j][k] = Image_with_boundary_extension[extend_boundary1+1][j][k];  
		}
		
	}

	//adding the left boundary

	for (int i = 0; i < height_size+2*extend_boundary1; i++)
    {
		for (int k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension[i][0][k] = Image_with_boundary_extension[i][extend_boundary1+1][k];
		}
	}

	//adding the bottom boundary

	for (int j = extend_boundary1; j < width_size+2*extend_boundary1; j++)
    {
		for (int k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension[2*extend_boundary1+height_size-1][j][k] = Image_with_boundary_extension[height_size-1][j][k];
		}
	}

	//adding the right boundary

	for (int i = 0; i < height_size+2*extend_boundary1; i++)
    {
            for (int k = 0; k < BytesPerPixel; k++)
			{
                Image_with_boundary_extension[i][2*extend_boundary1+width_size-1][k] = Image_with_boundary_extension[i][width_size-1][k];
			}
	}

	//adding the boundary extension by one pixel on the height and width size
	height_size += 2 * extend_boundary1;
	width_size += 2 * extend_boundary1;



	//MHC Improved Linear Demosaicing

	//if using convolution - implement using 5x5 filter

	//Adding missing blue and green values at Red color pixel
	for (int i = 2; i < height_size-2; i+=2)
	{
		for (int j = 3; j < width_size-2; j+=2)
		{
			x = i;
			y = j;

			//adding blue
			z = (0.125 * (2.0 * Image_with_boundary_extension[x - 1][y - 1][B_channel] + 2.0 * Image_with_boundary_extension[x + 1][y - 1][B_channel] + 2.0 * Image_with_boundary_extension[x - 1][y + 1][B_channel] + 2.0 * Image_with_boundary_extension[x + 1][y + 1][B_channel] + 6.0 * Image_with_boundary_extension[x][y][R_channel] - 1.5 * Image_with_boundary_extension[x - 2][y][R_channel] - 1.5 * Image_with_boundary_extension[x + 2][y][R_channel] - 1.5 * Image_with_boundary_extension[x][y - 2][R_channel] - 1.5 * Image_with_boundary_extension[x][y + 2][R_channel]));
			if(z > 255)
			{
				z = 255;
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][B_channel] = (unsigned char)z;

			//adding green
			z = (0.125 * (2.0 * Image_with_boundary_extension[x - 1][y][G_channel] + 2.0 * Image_with_boundary_extension[x + 1][y][G_channel] + 2.0 * Image_with_boundary_extension[x][y + 1][G_channel] + 2.0 * Image_with_boundary_extension[x][y - 1][G_channel] + 4 * Image_with_boundary_extension[x][y][R_channel] - Image_with_boundary_extension[x - 2][y][R_channel] - Image_with_boundary_extension[x + 2][y][R_channel] - Image_with_boundary_extension[x][y - 2][R_channel] - Image_with_boundary_extension[x][y + 2][R_channel]));
			if(z > 255)
			{
				z = 255;
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][G_channel] = (unsigned char)z;

		}
	}

	//Adding missing red and blue values at Green color pixel
	for (int i = 3; i < height_size-2; i+=2)
	{
		for (int j = 3; j < width_size-2; j+=2)
		{
			//for odd rows:
			x = i;
			y = j;

			//adding red
			z = (0.125 * (4.0 * Image_with_boundary_extension[x - 1][y][R_channel] + 4.0 * Image_with_boundary_extension[x + 1][y][R_channel] + 5.0 * Image_with_boundary_extension[x][y][G_channel] - Image_with_boundary_extension[x + 2][y][G_channel] - Image_with_boundary_extension[x - 2][y][G_channel] + 0.5 * Image_with_boundary_extension[x][y + 2][G_channel] + 0.5 * Image_with_boundary_extension[x][y - 2][G_channel] - Image_with_boundary_extension[x - 1][y - 1][G_channel] - Image_with_boundary_extension[x + 1][y - 1][G_channel] - Image_with_boundary_extension[x - 1][y + 1][G_channel] - Image_with_boundary_extension[x + 1][y + 1][G_channel]));
			if(z > 255)
			{
				z = 255;	
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][R_channel] = (unsigned char)z;

			//adding blue
			z = (0.125 * (4.0 * Image_with_boundary_extension[x][y - 1][B_channel] + 4.0 * Image_with_boundary_extension[x][y + 1][B_channel] + 5.0 * Image_with_boundary_extension[x][y][G_channel] + 0.5 * Image_with_boundary_extension[x + 2][y][G_channel] + 0.5 * Image_with_boundary_extension[x - 2][y][G_channel] - Image_with_boundary_extension[x][y + 2][G_channel] - Image_with_boundary_extension[x][y - 2][G_channel] - Image_with_boundary_extension[x - 1][y - 1][G_channel] - Image_with_boundary_extension[x + 1][y - 1][G_channel] - Image_with_boundary_extension[x - 1][y + 1][G_channel] - Image_with_boundary_extension[x + 1][y + 1][G_channel]));
			if(z > 255)
			{
				z = 255;
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][B_channel] = (unsigned char)z;

		}
	}

	//Adding missing red and blue values at Green color pixel
	for (int i = 2; i < height_size-2; i+=2)
	{
		for (int j = 2; j < width_size-2; j+=2)
		{
			//for even rows:
			x = i;
			y = j;

			//adding red
			z = (0.125 * (4.0 * Image_with_boundary_extension[x][y + 1][R_channel] + 4.0 * Image_with_boundary_extension[x][y - 1][R_channel] + 5.0 * Image_with_boundary_extension[x][y][G_channel] + 0.5 * Image_with_boundary_extension[x + 2][y][G_channel] + 0.5 * Image_with_boundary_extension[x - 2][y][G_channel] - Image_with_boundary_extension[x][y + 2][G_channel] - Image_with_boundary_extension[x][y - 2][G_channel] - Image_with_boundary_extension[x - 1][y - 1][G_channel] - Image_with_boundary_extension[x + 1][y - 1][G_channel] - Image_with_boundary_extension[x - 1][y + 1][G_channel] - Image_with_boundary_extension[x + 1][y + 1][G_channel]));
			if(z > 255)
			{
				z = 255;
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][R_channel] = (unsigned char)z;

			//adding blue
			z = (0.125 * (4.0 * Image_with_boundary_extension[x - 1][y][B_channel] + 4.0 * Image_with_boundary_extension[x + 1][y][B_channel] + 5.0 * Image_with_boundary_extension[x][y][G_channel] - Image_with_boundary_extension[x + 2][y][G_channel] - Image_with_boundary_extension[x - 2][y][G_channel] + 0.5 * Image_with_boundary_extension[x][y + 2][G_channel] + 0.5 * Image_with_boundary_extension[x][y - 2][G_channel] - Image_with_boundary_extension[x - 1][y - 1][G_channel] - Image_with_boundary_extension[x + 1][y - 1][G_channel] - Image_with_boundary_extension[x - 1][y + 1][G_channel] - Image_with_boundary_extension[x + 1][y + 1][G_channel]));
			if(z > 255)
			{
				z = 255;
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][B_channel] = (unsigned char)z;

		}
	}

	//Adding missing red and green values at Blue color pixel
	for (int i = 3; i < height_size-2; i+=2)
	{
		for (int j = 2; j < width_size-2; j+=2)
		{
			x = i;
			y = j;

			//adding red
			z = (0.125 * (2.0 * Image_with_boundary_extension[x - 1][y - 1][R_channel] + 2.0 * Image_with_boundary_extension[x + 1][y - 1][R_channel] + 2.0 * Image_with_boundary_extension[x - 1][y + 1][R_channel] + 2.0 * Image_with_boundary_extension[x + 1][y + 1][R_channel] + 6.0 * Image_with_boundary_extension[x][y][B_channel] - 1.5 * Image_with_boundary_extension[x - 2][y][B_channel] - 1.5 * Image_with_boundary_extension[x + 2][y][B_channel] - 1.5 * Image_with_boundary_extension[x][y - 2][B_channel] - 1.5 * Image_with_boundary_extension[x][y + 2][B_channel]));
			if(z > 255)
			{
				z = 255;
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][R_channel] = (unsigned char)z;

			//adding green
			z = (0.125 * (2.0 * Image_with_boundary_extension[x - 1][y][G_channel] + 2.0 * Image_with_boundary_extension[x + 1][y][G_channel] + 2.0 * Image_with_boundary_extension[x][y + 1][G_channel] + 2.0 * Image_with_boundary_extension[x][y - 1][G_channel] + 4.0 * Image_with_boundary_extension[x][y][B_channel] - Image_with_boundary_extension[x - 2][y][B_channel] - Image_with_boundary_extension[x + 2][y][B_channel] - Image_with_boundary_extension[x][y - 2][B_channel] - Image_with_boundary_extension[x][y + 2][B_channel]));
			if(z > 255)
			{
				z = 255;
			}
			else if(z < 0)
			{
				z = 0;
			}
			Image_with_boundary_extension[x][y][G_channel] = (unsigned char)z;

		}
	}


	unsigned char Image_with_RGB_colors[height_size][width_size][BytesPerPixel];

	for (int i = 0; i < height_size; i++)
    {
        for (int j = 0; j < width_size; j++)
        {
            for (int k = 0; k < BytesPerPixel; k++)
			{
				Image_with_RGB_colors[i][j][k] = Image_with_boundary_extension[i][j][k];
			}
		}
	}

	//crop image to original size	

	height_size -= 2 * extend_boundary;
	width_size -= 2 * extend_boundary;

	height_size -= 2 * extend_boundary1;
	width_size -= 2 * extend_boundary1;

	unsigned char Imagedata_output[height_size][width_size][BytesPerPixel];

	for (int i = 0; i < height_size; i++)
    {
        for (int j = 0; j < width_size; j++)
        {
            for (int k = 0; k < BytesPerPixel; k++)
			{
                Imagedata_output[i][j][k] = Image_with_RGB_colors[i+extend_boundary+extend_boundary1][j+extend_boundary+extend_boundary1][k];
			}
        }
    }


	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_output, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


/*
to observe output on ImageJ, 
Use: image type - 24-bit RGB
width - 390
height - 300
*/

