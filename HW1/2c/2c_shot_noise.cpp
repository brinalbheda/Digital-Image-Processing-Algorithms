// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the pepper_dark_noise input image is of size 256 x 256 
//			and is in the RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

//EE 569: Homework 1 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 01/22/2019

// To run the code, open terminal, add path and run using following commands
// g++ -o 2c 2c_shot_noise.cpp
// for getting input to BM3D
// ./2c pepper_dark_noise.raw pepper_dark_noise_gaussian.raw 1
// run BM3D.m code on matlab
// using the output from BM3D
// ./2c pepper_dark_noise_gaussian.raw output_2c_biased.raw 1
// ./2c pepper_dark_noise_gaussian.raw output_2c_unbiased.raw 1

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int Size = 256;
	
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
			Size = atoi(argv[4]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata_input[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_input, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	for (i=0; i < Size; i++)
	{
		for (j=0; j < Size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				Imagedata[i][j][k] = Imagedata_input[i][j][k];
			}
		}
	}


	//Apply the Anscombe root transformation on each pixel

	int i,j,k;

	for (i=0; i < Size; i++)
	{
		for (j=0; j < Size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				Imagedata[i][j][k] = (2*(pow((Imagedata[i][j][k]+3/8),0.5)));
			}
		}
	}


	//Apply BM3D or Gaussian low pass filter after the above part and get the image
	
	//Apply the inverse Anscombe root transformation on each pixel on the output image by BM3D or Gaussian low pass filter
	//Also comment the above part


	//biased
	for (i=0; i < Size; i++)
	{
		for (j=0; j < Size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				Imagedata[i][j][k] = (pow(((int)(Imagedata[i][j][k]))/2),2) - (3/8);
			}
		}
	}


	//unbiased
	for (i=0; i < Size; i++)
	{
		for (j=0; j < Size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				Imagedata[i][j][k] = (pow(((int)(Imagedata[i][j][k]))/2),2) - (1/8);
			}
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


/*
to observe output on ImageJ, 
Use: image type - 8-bit
width - 256
height - 256
*/

