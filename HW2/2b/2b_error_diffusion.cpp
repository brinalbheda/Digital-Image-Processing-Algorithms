// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the bridge input image is of size 600 x 400 and 
//			is in the RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

//EE 569: Homework 2 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 02/12/2019

/*
Algorithm implemented in C++:
• Read the input image “bridge.raw” whose dimensions are height_size, width_size, BytesPerPixel using fread () function
• The input image is normalized for values between 0-1
• Binarize the temporary image which has pixel values transferred from the original image
• Calculate the error diffusion matrix using Floyd-Steinberg’s (FS) matrix with and without serpentine scanning
• For raster scanning, assign a value to the output pixel by using thresholding.
  If the value of input image <= 0.5 or 127 depends on normalizing, assign a value of 255 or else assign a value of 0
• For serpentine scanning, if the row is even, check by using the modular 2 operation, assign a value to the output pixel by using thresholding. If the value of input image <= 0.5 or 127 depends on normalizing, assign a value of 255 or else assign a value of 0
• Calculate the error by subtracting new value assigned (binarized value) to the pixel after thresholding from the old value (error = output – input)
• Diffuse the error to the future neighboring pixels as shown in the matrices above
• Repeat the procedure for scanning of odd rows when modular 2 operation fails
• Repeat the procedure with Jarvis, Judice and Ninke (JJN) and Stucki matrices
• Write the computed image data array on output.raw file using the fwrite() function
*/

// To run the code, open terminal, add path and run using following commands
// g++ -o 2b 2b_error_diffusion.cpp
// ./2b bridge.raw output_2b_FS.raw 1
// ./2b bridge.raw output_2b_FS_serp.raw 1
// ./2b bridge.raw output_2b_JJN.raw 1
// ./2b bridge.raw output_2b_JJN_serp.raw 1
// ./2b bridge.raw output_2b_S.raw 1
// ./2b bridge.raw output_2b_S_serp.raw 1

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int width_size = 600;
	int height_size = 400;
	
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
	
	//golden gate bridge.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata[height_size][width_size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//Digital Halftoning

	//Problem 2b - Error Diffusion

	double error = 0;

	//defining output 3d image arrays

	// Allocate image processing data array
	unsigned char Imagedata_output_FS[height_size][width_size][BytesPerPixel];

	// Allocate image processing data array
	unsigned char Imagedata_output_FS_serp[height_size][width_size][BytesPerPixel];

	// Allocate image processing data array
	unsigned char Imagedata_output_JJN[height_size][width_size][BytesPerPixel];

	// Allocate image processing data array
	unsigned char Imagedata_output_JJN_serp[height_size][width_size][BytesPerPixel];

	// Allocate image processing data array
	unsigned char Imagedata_output_S[height_size][width_size][BytesPerPixel];

	// Allocate image processing data array
	unsigned char Imagedata_output_S_serp[height_size][width_size][BytesPerPixel];


	//defining temporary 3d image arrays for processing of matrices

	// Allocate image processing data array
	double Imagedata_temp_FS[height_size][width_size][BytesPerPixel];

	// Allocate image processing data array
	double Imagedata_temp_JJN[height_size][width_size][BytesPerPixel];

	// Allocate image processing data array
	double Imagedata_temp_S[height_size][width_size][BytesPerPixel];



	//binarize the temp image which has values transferred from original image

	for (int i=0; i<height_size; i++)
	{
		for (int j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				Imagedata_temp_FS[i][j][k] = 1 - (double)Imagedata[i][j][k] / 255;
				Imagedata_temp_JJN[i][j][k] = 1 - (double)Imagedata[i][j][k] / 255;
				Imagedata_temp_S[i][j][k] = 1 - (double)Imagedata[i][j][k] / 255;
			}
		}
	}

	//if do not normalize to 0~1, then declare variables long double as dealing with high values between 0~255




	//calculate error diffusion matrix using Floyd-Steinberg’s matrix with and without serpentine scanning
	//comment the other method and implement only one at a time
/*
	//without serpentine scanning
	for (int i=0; i<height_size; i++)
	{
		for (int j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				//normalize this temp image to 0~1
				//subtract the binarized value to estimated value to get error and output
				if (Imagedata_temp_FS[i][j][k] <= 0.5)
				{
					error = Imagedata_temp_FS[i][j][k] - 0;
					Imagedata_output_FS[i][j][k] = (unsigned char)255;
				}
				else
				{
					error = Imagedata_temp_FS[i][j][k] - 1;
					Imagedata_output_FS[i][j][k] = (unsigned char)0;
				}

				if (j+1 < width_size)
				{
					Imagedata_temp_FS[i][j+1][k] += error*7/16;
				}
				if (j > 0 && i+1 < height_size)
				{
					Imagedata_temp_FS[i+1][j-1][k] += error*3/16;
				}
				if (i+1 < height_size)
				{
					Imagedata_temp_FS[i+1][j][k] += error*5/16;
				}
				if (i+1 < height_size && j+1 < width_size)
				{
					Imagedata_temp_FS[i+1][j+1][k] += error*1/16;
				}

			}
		}
	}
*/

	//with serpentine scanning
	for (int i=0; i<height_size; i++)
	{
		if (i%2 == 0)
		{
			for (int j=0; j<width_size; j++)
			{
				for (int k=0; k<BytesPerPixel; k++)
				{
					//normalize this temp image to 0~1
					//subtract the binarized value to estimated value to get error and output
					if (Imagedata_temp_FS[i][j][k] <= 0.5)
					{
						error = Imagedata_temp_FS[i][j][k] - 0;
						Imagedata_output_FS_serp[i][j][k] = (unsigned char)255;
					}
					else
					{
						error = Imagedata_temp_FS[i][j][k] - 1;
						Imagedata_output_FS_serp[i][j][k] = (unsigned char)0;
					}

					if (j+1 < width_size)
					{
						Imagedata_temp_FS[i][j+1][k] += error*7/16;
					}
					if (j > 0 && i+1 < height_size)
					{
						Imagedata_temp_FS[i+1][j-1][k] += error*3/16;
					}
					if (i+1 < height_size)
					{
						Imagedata_temp_FS[i+1][j][k] += error*5/16;
					}
					if (i+1 < height_size && j+1 < width_size)
					{
						Imagedata_temp_FS[i+1][j+1][k] += error*1/16;
					}
					
				}
			}
		}
		if (i%2 != 0) 
		{
			for (int j=width_size - 1; j>=0; j--)
			{
				for (int k=0; k<BytesPerPixel; k++)
				{
					//normalize this temp image to 0~1
					//subtract the binarized value to estimated value to get error and output
					if (Imagedata_temp_FS[i][j][k] <= 0.5)
					{
						error = Imagedata_temp_FS[i][j][k] - 0;
						Imagedata_output_FS_serp[i][j][k] = (unsigned char)255;
					}
					else
					{
						error = Imagedata_temp_FS[i][j][k] - 1;
						Imagedata_output_FS_serp[i][j][k] = (unsigned char)0;
					}

					if (j+1 < width_size)
					{
						Imagedata_temp_FS[i][j+1][k] += error*7/16;
					}
					if (j > 0 && i+1 < height_size)
					{
						Imagedata_temp_FS[i+1][j-1][k] += error*1/16;              
					}
					if (i+1 < height_size)
					{
						Imagedata_temp_FS[i+1][j][k] += error*5/16;
					}
					if (i+1 < height_size && j+1 < width_size)
					{
						Imagedata_temp_FS[i+1][j+1][k] += error*3/16;
					}
					
				}
			}
		}
		
	}




	//calculate error diffusion matrix using Jarvis, Judice and Ninke’s matrix (JNN) with and without serpentine scanning
/*
	//without serpentine scanning
	for (int i=0; i<height_size; i++)
	{
		for (int j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				//normalize this temp image to 0~1
				//subtract the binarized value to estimated value to get error and output
				if (Imagedata_temp_JJN[i][j][k] <= 0.5)
				{
					error = Imagedata_temp_JJN[i][j][k] - 0;
					Imagedata_output_JJN[i][j][k] = (unsigned char)255;
				}
				else
				{
					error = Imagedata_temp_JJN[i][j][k] - 1;
					Imagedata_output_JJN[i][j][k] = (unsigned char)0;
				}

				if (j+1 < width_size)
				{
					Imagedata_temp_JJN[i][j+1][k] += error*7/48;
				}
				if (j+1 < width_size && i+1 < height_size)
				{
					Imagedata_temp_JJN[i+1][j+1][k] += error*5/48;
				}
				if (j+1 < width_size && i+2 < height_size)
				{
					Imagedata_temp_JJN[i+2][j+1][k] += error*3/48;
				}
				if (j+2 < width_size)
				{
					Imagedata_temp_JJN[i][j+2][k] += error*5/48;
				}
				if (j+2 < width_size && i+1 < height_size)
				{
					Imagedata_temp_JJN[i+1][j+2][k] += error*3/48;
				}
				if (j+2 < width_size && i+2 < height_size)
				{
					Imagedata_temp_JJN[i+2][j+2][k] += error*1/48;
				}
				if (i+1 < height_size)
				{
					Imagedata_temp_JJN[i+1][j][k] += error*7/48;
				}
				if (i+2 < height_size)
				{
					Imagedata_temp_JJN[i+2][j][k] += error*5/48;
				}
				if (j > 0 && i+1 < height_size)
				{
					Imagedata_temp_JJN[i+1][j-1][k] += error*5/48;
				}
				if (j > 0 && i+2 < height_size)
				{
					Imagedata_temp_JJN[i+2][j-1][k] += error*3/48;
				}
				if (j-1 > 0 && i+1 < height_size)
				{
					Imagedata_temp_JJN[i+1][j-2][k] += error*3/48;
				}
				if (j-1 > 0 && i+2 < height_size)
				{
					Imagedata_temp_JJN[i+2][j-2][k] += error*1/48;
				}

			}
		}
	}

*/
	//with serpentine scanning
	for (int i=0; i<height_size; i++)
	{
		if (i%2 == 0)
		{
			for (int j=0; j<width_size; j++)
			{
				for (int k=0; k<BytesPerPixel; k++)
				{
					//normalize this temp image to 0~1
					//subtract the binarized value to estimated value to get error and output
					if (Imagedata_temp_JJN[i][j][k] <= 0.5)
					{
						error = Imagedata_temp_JJN[i][j][k] - 0;
						Imagedata_output_JJN_serp[i][j][k] = (unsigned char)255;
					}
					else
					{
						error = Imagedata_temp_JJN[i][j][k] - 1;
						Imagedata_output_JJN_serp[i][j][k] = (unsigned char)0;
					}

					if (j+1 < width_size)
					{
						Imagedata_temp_JJN[i][j+1][k] += error*7/48;
					}
					if (j+1 < width_size && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j+1][k] += error*5/48;
					}
					if (j+1 < width_size && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j+1][k] += error*3/48;
					}
					if (j+2 < width_size)
					{
						Imagedata_temp_JJN[i][j+2][k] += error*5/48;
					}
					if (j+2 < width_size && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j+2][k] += error*3/48;
					}
					if (j+2 < width_size && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j+2][k] += error*1/48;
					}
					if (i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j][k] += error*7/48;
					}
					if (i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j][k] += error*5/48;
					}
					if (j > 0 && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j-1][k] += error*5/48;
					}
					if (j > 0 && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j-1][k] += error*3/48;
					}
					if (j-1 > 0 && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j-2][k] += error*3/48;
					}
					if (j-1 > 0 && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j-2][k] += error*1/48;
					}
						
				}
			}
		}
		if (i%2 != 0)                                                                              
		{
			for (int j=width_size - 1; j>=0; j--)
			{
				for (int k=0; k<BytesPerPixel; k++)
				{
					//normalize this temp image to 0~1
					//subtract the binarized value to estimated value to get error and output
					if (Imagedata_temp_JJN[i][j][k] <= 0.5)
					{
						error = Imagedata_temp_JJN[i][j][k] - 0;
						Imagedata_output_JJN_serp[i][j][k] = (unsigned char)255;
					}
					else
					{
						error = Imagedata_temp_JJN[i][j][k] - 1;
						Imagedata_output_JJN_serp[i][j][k] = (unsigned char)0;
					}

					if (j+1 < width_size)
					{
						Imagedata_temp_JJN[i][j+1][k] += error*7/48;
					}
					if (j+1 < width_size && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j+1][k] += error*5/48;
					}
					if (j+1 < width_size && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j+1][k] += error*3/48;
					}
					if (j+2 < width_size)
					{
						Imagedata_temp_JJN[i][j+2][k] += error*5/48;
					}
					if (j+2 < width_size && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j+2][k] += error*3/48;
					}
					if (j+2 < width_size && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j+2][k] += error*1/48;
					}
					if (i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j][k] += error*7/48;
					}
					if (i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j][k] += error*5/48;
					}
					if (j > 0 && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j-1][k] += error*5/48;
					}
					if (j > 0 && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j-1][k] += error*3/48;
					}
					if (j-1 > 0 && i+1 < height_size)
					{
						Imagedata_temp_JJN[i+1][j-2][k] += error*3/48;
					}
					if (j-1 > 0 && i+2 < height_size)
					{
						Imagedata_temp_JJN[i+2][j-2][k] += error*1/48;
					}
					
				}
			}
		}
		
	}




	//calculate error diffusion matrix using Stucki’s matrix with and without serpentine scanning
/*
	//without serpentine scanning
	for (int i=0; i<height_size; i++)
	{
		for (int j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				//normalize this temp image to 0~1
				//subtract the binarized value to estimated value to get error and output
				if (Imagedata_temp_S[i][j][k] <= 0.5)
				{
					error = Imagedata_temp_S[i][j][k] - 0;
					Imagedata_output_S[i][j][k] = (unsigned char)255;
				}
				else
				{
					error = Imagedata_temp_S[i][j][k] - 1;
					Imagedata_output_S[i][j][k] = (unsigned char)0;
				}

				if (j+1 < width_size)
				{
					Imagedata_temp_S[i][j+1][k] += error*8/42;
				}
				if (j+1 < width_size && i+1 < height_size)
				{
					Imagedata_temp_S[i+1][j+1][k] += error*4/42;
				}
				if (j+1 < width_size && i+2 < height_size)
				{
					Imagedata_temp_S[i+2][j+1][k] += error*2/42;
				}
				if (j+2 < width_size)
				{
					Imagedata_temp_S[i][j+2][k] += error*4/42;
				}
				if (j+2 < width_size && i+1 < height_size)
				{
					Imagedata_temp_S[i+1][j+2][k] += error*2/42;
				}
				if (j+2 < width_size && i+2 < height_size)
				{
					Imagedata_temp_S[i+2][j+2][k] += error*1/42;
				}
				if (i+1 < height_size)
				{
					Imagedata_temp_S[i+1][j][k] += error*8/42;
				}
				if (i+2 < height_size)
				{
					Imagedata_temp_S[i+2][j][k] += error*4/42;
				}
				if (j > 0 && i+1 < height_size)
				{
					Imagedata_temp_S[i+1][j-1][k] += error*4/42;
				}
				if (j > 0 && i+2 < height_size)
				{
					Imagedata_temp_S[i+2][j-1][k] += error*2/42;
				}
				if (j-1 > 0 && i+1 < height_size)
				{
					Imagedata_temp_S[i+1][j-2][k] += error*2/42;
				}
				if (j-1 > 0 && i+2 < height_size)
				{
					Imagedata_temp_S[i+2][j-2][k] += error*1/42;
				}

			}
		}
	}
*/

	//with serpentine scanning
	for (int i=0; i<height_size; i++)
	{
		if (i%2 == 0)
		{
			for (int j=0; j<width_size; j++)
			{
				for (int k=0; k<BytesPerPixel; k++)
				{
					//normalize this temp image to 0~1
					//subtract the binarized value to estimated value to get error and output
					if (Imagedata_temp_S[i][j][k] <= 0.5)
					{
						error = Imagedata_temp_S[i][j][k] - 0;
						Imagedata_output_S_serp[i][j][k] = (unsigned char)255;
					}
					else
					{
						error = Imagedata_temp_S[i][j][k] - 1;
						Imagedata_output_S_serp[i][j][k] = (unsigned char)0;
					}

					if (j+1 < width_size)
					{
						Imagedata_temp_S[i][j+1][k] += error*8/42;
					}
					if (j+1 < width_size && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j+1][k] += error*4/42;
					}
					if (j+1 < width_size && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j+1][k] += error*2/42;
					}
					if (j+2 < width_size)
					{
						Imagedata_temp_S[i][j+2][k] += error*4/42;
					}
					if (j+2 < width_size && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j+2][k] += error*2/42;
					}
					if (j+2 < width_size && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j+2][k] += error*1/42;
					}
					if (i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j][k] += error*8/42;
					}
					if (i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j][k] += error*4/42;
					}
					if (j > 0 && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j-1][k] += error*4/42;
					}
					if (j > 0 && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j-1][k] += error*2/42;
					}
					if (j-1 > 0 && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j-2][k] += error*2/42;
					}
					if (j-1 > 0 && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j-2][k] += error*1/42;
					}
					
				}
			}
		}
		if (i%2 != 0)                                                                                 
		{
			for (int j=width_size - 1; j>=0; j--)
			{
				for (int k=0; k<BytesPerPixel; k++)
				{
					//normalize this temp image to 0~1
					//subtract the binarized value to estimated value to get error and output
					if (Imagedata_temp_S[i][j][k] <= 0.5)
					{
						error = Imagedata_temp_S[i][j][k] - 0;
						Imagedata_output_S_serp[i][j][k] = (unsigned char)255;
					}
					else
					{
						error = Imagedata_temp_S[i][j][k] - 1;
						Imagedata_output_S_serp[i][j][k] = (unsigned char)0;
					}

					if (j+1 < width_size)
					{
						Imagedata_temp_S[i][j+1][k] += error*8/42;
					}
					if (j+1 < width_size && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j+1][k] += error*4/42;
					}
					if (j+1 < width_size && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j+1][k] += error*2/42;
					}
					if (j+2 < width_size)
					{
						Imagedata_temp_S[i][j+2][k] += error*4/42;
					}
					if (j+2 < width_size && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j+2][k] += error*2/42;
					}
					if (j+2 < width_size && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j+2][k] += error*1/42;
					}
					if (i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j][k] += error*8/42;
					}
					if (i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j][k] += error*4/42;
					}
					if (j > 0 && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j-1][k] += error*4/42;
					}
					if (j > 0 && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j-1][k] += error*2/42;
					}
					if (j-1 > 0 && i+1 < height_size)
					{
						Imagedata_temp_S[i+1][j-2][k] += error*2/42;
					}
					if (j-1 > 0 && i+2 < height_size)
					{
						Imagedata_temp_S[i+2][j-2][k] += error*1/42;
					}
					
				}
			}
		}
		
	}




	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	//fwrite(Imagedata_output_FS, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fwrite(Imagedata_output_FS_serp, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	//fwrite(Imagedata_output_JJN, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	//fwrite(Imagedata_output_JJN_serp, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	//fwrite(Imagedata_output_S, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	//fwrite(Imagedata_output_S_serp, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);	

	return 0;
}


/*
to observe output on ImageJ, 
Use: image type - 8-bit
width - 600
height - 400
*/

