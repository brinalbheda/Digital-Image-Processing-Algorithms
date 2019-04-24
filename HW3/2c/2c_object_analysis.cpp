#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int width_size = 690;
	int height_size = 500;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		//BytesPerPixel = 1; // default is grey image
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


	//Problem 2c

	int i,j;
	
	//separate red, green, blue channels/pixel values from the input color image for rice
	unsigned char r_pixel_value[height_size][width_size][1];
	unsigned char g_pixel_value[height_size][width_size][1];
	unsigned char b_pixel_value[height_size][width_size][1];

	for (i=0; i<height_size; i++)
	{
		for (j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				r_pixel_value[i][j][1] = Imagedata[i][j][0];
				g_pixel_value[i][j][1] = Imagedata[i][j][1];
				b_pixel_value[i][j][1] = Imagedata[i][j][2];
			}
		}
	}

	//covert RGB to grayscale by using Luminosity
	unsigned char Imagedata_2D_luminous[height_size][width_size][BytesPerPixel];

	for (i=0; i<height_size; i++)
	{
		for (j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				Imagedata_2D_luminous[i][j][k] = (unsigned char) (0.21 * r_pixel_value[i][j][1] + 0.72 * g_pixel_value[i][j][1] + 0.07 * b_pixel_value[i][j][1]);
			}
		}
	}


	// Constant/Fixed thresholding

	// Allocate image processing data array
	unsigned char Imagedata_fixed_thresholding[height_size][width_size][BytesPerPixel];

	for (int i=0; i<height_size; i++)
	{
		for (int j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				//compare pixel value with 127
				
                
                if (Imagedata[i][j][k] >=25 && Imagedata[i][j][k] <=60)
                {
                    Imagedata_fixed_thresholding[i][j][k] = 255;
                }
                else if (Imagedata[i][j][k] <= 127)
				{
					//if pixel value is less than 127, map it to 0
					Imagedata_fixed_thresholding[i][j][k] = 0;
				}
                else
				{
					//if pixel value is greater than 127, map it to 255
					Imagedata_fixed_thresholding[i][j][k] = 255;
				}
			}
		}
	}

 
	

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);

	return 0;

}


/*
to observe output on ImageJ, 
Use: image type - 8-bit 
width - 690
height - 500
*/
