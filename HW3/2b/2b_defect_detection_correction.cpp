// This sample code reads in image data from a RAW image file and
// writes it into another file

// NOTE:    The code assumes that the pattern 1~4 input images are of size 375 x 375
//            and are in the RAW format. You will need to make corresponding changes to
//            accommodate images of different sizes and/or types

//EE 569: Homework 3 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 03/03/2019

// To run the code, open terminal, add path and run using following commands
// g++ -o 2b 2b_defect_detection_correction.cpp
// ./2b deer.raw output_2b.raw 1

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <list>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int width_size = 550;
	int height_size = 691;
	
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
	
	//deer.raw grayscale input image
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

	//Morphological Processing

	unsigned char Imagedata_output[height_size][width_size][BytesPerPixel];

	/*
	think in a reverse way of 2a, connected edges, how many number of objects, number of connected backgrounds due to connectivity - 4 connected not 8
	Use connected components wiki (edge connected) - try something different
	*/

	//Problem 2b - Defect Detection and Correction 

	int count = 0;

    //detect and locate the black dot defect pixel locations
	for (int i=0; i<height_size; i++)
	{
		for (int j=0; j<width_size; j++)
		{
			if (Imagedata[i][j][1] == 0 && Imagedata[i-1][j-1][1] == 255 && Imagedata[i-1][j][1] == 255 && Imagedata[i-1][j+1][1] == 255 && Imagedata[i][j-1][1] == 255 && Imagedata[i][j+1][1] == 255 && Imagedata[i+1][j-1][1] == 255 && Imagedata[i+1][j][1] == 255 && Imagedata[i+1][j+1][1] == 255)
			{
				cout << "Black dot location rows: " << i << endl;
				cout << "Black dot location columns: " << j << endl;
                count++;
				// filling the holes (black dots) into white 
                Imagedata[i][j][1] = 255;
			}
            
		}
	}
    
    cout << "Number of black dots or defects in the deer body: " << count << endl;

	
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


