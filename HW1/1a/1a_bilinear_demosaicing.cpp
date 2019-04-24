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
• Define 3 color channels: channel 0→Red, channel 1→Green, channel 2→ Blue
• Add one boundary line to the input image by boundary extension using mirror reflecting on all sides of the image border
• Apply demosaicing by using the 3x3 convolution filter for the bilinear interpolation
• Adding the remaining two missing green/red/blue channel values to the R/G/B pixel location by using the formula mentioned above
• Using two sets of functions for green pixel location for odd and even values 
• Crop the boundary extended image to the original image size after the computation is done
• Write the computed image data array on output.raw file using the fwrite() function
*/

// To run the code, open terminal, add path and run using following commands
// g++ -o 1a 1a_bilinear_demosaicing.cpp
// ./1a cat.raw output_1a.raw 3

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
	
	//Problem 1a

	int i,j,k;             //i->columns, j->rows, k->BytesPerPixel
	//color channel for each pixel in the array is: R,G,B,R,G,B,R,...
	int R_channel = 0;       //red channel
	int G_channel = 1;       //green channel
	int B_channel = 2;       //blue channel
	int x,y;

	//add 1 boundary line on all sides of the input image
	int extend_boundary = 1;

	//Allocate image data array of extended boundary image
	unsigned char Image_with_boundary_extension[height_size+2*extend_boundary][width_size+2*extend_boundary][BytesPerPixel];

	/*
	cout << "BytePerpixel:" << BytesPerPixel << endl;
	cout << "Height: " << height_size << endl;
	cout << "Width: " << width_size << endl;
	cout << "-------------------------" << endl;
	*/

	//boundary extension by using mirror-reflecting to deal with boundary pixel
	
	//getting the input image at the centre point on boundary extended image
	for (i = 0; i < height_size; i++)
    {
        for (j = 0; j < width_size; j++)
        {
            for (k = 0; k < BytesPerPixel; k++)
			{
				Image_with_boundary_extension[i+extend_boundary][j+extend_boundary][k] = Imagedata[i][j][0];
			}
		}
	}

	//adding the top boundary
	for (j = extend_boundary; j < width_size+2*extend_boundary; j++)
    {
      
		for (k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension[0][j][k] = Image_with_boundary_extension[extend_boundary+1][j][k];  
		}
		
	}

	//adding the left boundary

	for (i = 0; i < height_size+2*extend_boundary; i++)
    {
		for (k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension[i][0][k] = Image_with_boundary_extension[i][extend_boundary+1][k];
		}
	}

	//adding the bottom boundary

	for (j = extend_boundary; j < width_size+2*extend_boundary; j++)
    {
		for (k = 0; k < BytesPerPixel; k++)
		{
			Image_with_boundary_extension[2*extend_boundary+height_size-1][j][k] = Image_with_boundary_extension[height_size-1][j][k];
		}
	}

	//adding the right boundary

	for (i = 0; i < height_size+2*extend_boundary; i++)
    {
            for (k = 0; k < BytesPerPixel; k++)
			{
                Image_with_boundary_extension[i][2*extend_boundary+width_size-1][k] = Image_with_boundary_extension[i][width_size-1][k];
			}
	}

	//adding the boundary extension by one pixel on the height and width size
	height_size += 2 * extend_boundary;
	width_size += 2 * extend_boundary;

	//demosaicing

	//for bilinear interpolation implementation if convolution - using 3x3 filters so enlarge image by one pixel on each side

	//Adding missing blue and green values at Red color pixel
	for (i = 1; i < height_size-extend_boundary; i+=2)
	{
		for (j = 2; j < width_size-extend_boundary; j+=2)
		{
			x = i;
			y = j;

			//adding blue
			Image_with_boundary_extension[x][y][B_channel] = (unsigned char)(0.25*(double)(Image_with_boundary_extension[x-1][y-1][B_channel]+Image_with_boundary_extension[x-1][y+1][B_channel]+Image_with_boundary_extension[x+1][y-1][B_channel]+Image_with_boundary_extension[x+1][y+1][B_channel]));
			
			//adding green
			Image_with_boundary_extension[x][y][G_channel] = (unsigned char)(0.25*(double)(Image_with_boundary_extension[x][y-1][G_channel]+Image_with_boundary_extension[x-1][y][G_channel]+Image_with_boundary_extension[x][y+1][G_channel]+Image_with_boundary_extension[x+1][y][G_channel]));
		}
	}

	//Adding missing red and blue values at Green color pixel
	for (i = 1; i < height_size-extend_boundary; i+=2)
	{
		for (j = 1; j < width_size-extend_boundary; j+=2)
		{
			//for odd rows:
			x = i;
			y = j;

			//adding red
			Image_with_boundary_extension[x][y][R_channel] = (unsigned char)(0.5*(double)(Image_with_boundary_extension[x][y-1][R_channel]+Image_with_boundary_extension[x][y+1][R_channel]));
			
			//adding blue
			Image_with_boundary_extension[x][y][B_channel] = (unsigned char)(0.5*(double)(Image_with_boundary_extension[x-1][y][B_channel]+Image_with_boundary_extension[x+1][y][B_channel]));


	
		}
	}

	//Adding missing red and blue values at Green color pixel
	for (i = 2; i < height_size-extend_boundary; i+=2)
	{
		for (j = 2; j < width_size-extend_boundary; j+=2)
		{
			//for even rows:
			x = i;
			y = j;

			//adding red
			Image_with_boundary_extension[x][y][R_channel] = (unsigned char)(0.5*(double)(Image_with_boundary_extension[x-1][y][R_channel]+Image_with_boundary_extension[x+1][y][R_channel]));
			
			//adding blue
			Image_with_boundary_extension[x][y][B_channel] = (unsigned char)(0.5*(double)(Image_with_boundary_extension[x][y-1][B_channel]+Image_with_boundary_extension[x][y+1][B_channel]));

		}
	}

	//Adding missing red and green values at Blue color pixel
	for (i = 2; i < height_size-extend_boundary; i+=2)
	{
		for (j = 1; j < width_size-extend_boundary; j+=2)
		{
			x = i;
			y = j;

			//adding red
			Image_with_boundary_extension[x][y][R_channel] = (unsigned char)(0.25*(double)(Image_with_boundary_extension[x-1][y-1][R_channel]+Image_with_boundary_extension[x-1][y+1][R_channel]+Image_with_boundary_extension[x+1][y-1][R_channel]+Image_with_boundary_extension[x+1][y+1][R_channel]));
			
			//adding green
			Image_with_boundary_extension[x][y][G_channel] = (unsigned char)(0.25*(double)(Image_with_boundary_extension[x][y-1][G_channel]+Image_with_boundary_extension[x-1][y][G_channel]+Image_with_boundary_extension[x][y+1][G_channel]+Image_with_boundary_extension[x+1][y][G_channel]));
		}
	}
	
	/*
	if (!(file=fopen("Image_with_Boundary.raw","wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Image_with_boundary_extension, sizeof(unsigned char), (height_size+2*extend_boundary)*(width_size+2*extend_boundary)*BytesPerPixel, file);
	fclose(file);
	*/

	unsigned char Image_with_RGB_colors[height_size][width_size][BytesPerPixel];

	for (i = 0; i < height_size; i++)
    {
        for (j = 0; j < width_size; j++)
        {
            for (k = 0; k < BytesPerPixel; k++)
			{
				Image_with_RGB_colors[i][j][k] = Image_with_boundary_extension[i][j][k];
			}
		}
	}

	/*
	if (!(file=fopen("Image_with_RGB.raw","wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Image_with_RGB_colors, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);
	*/

	//crop image to original size

	height_size -= 2 * extend_boundary;
	width_size -= 2 * extend_boundary;

	unsigned char Imagedata_output[height_size][width_size][BytesPerPixel];

	for (i = 0; i < height_size; i++)
    {
        for (j = 0; j < width_size; j++)
        {
            for (k = 0; k < BytesPerPixel; k++)
			{
                Imagedata_output[i][j][k] = Image_with_RGB_colors[i+extend_boundary][j+extend_boundary][k];
			}
        }
    }


	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	/*
	cout << "BytePerpixel:" << BytesPerPixel << endl;
	cout << "Height: " << height_size << endl;
	cout << "Width: " << width_size << endl;
	*/
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

