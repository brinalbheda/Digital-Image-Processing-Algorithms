// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the input images rose_dark, rose_bright, rose_mix are of size 400 x 400 
//			and are in the RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

//EE 569: Homework 1 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 01/22/2019

/*
Method A:-
 
 First step is to obtain the histogram - to count the total number of pixels associated with each pixel intensity (0-255). Second step is to calculate the normalized probability histogram - to calculate probability of each pixel intensity in the image matrix. Probability is the number of pixels divided by the total number of pixels. Third step is to calculate cumulative probability. Now we round up the decimal values obtained to the lower integer values (floor rounding). Forth step is create mapping table ie x to CDF(x)*255. Hence, the original image has been transformed to the equalized image with different intensity on each pixel.
 
Algorithm implemented in C++:
• Read rose_dark.raw and rose_bright.raw input images using fread() function and get height, width and bytesperpixel values
• Run 3 nested for loops for height (400), width (400) and bytesperpixel (=1) to access each pixel in the original image
• Define the histograms of original image, equalized image and transfer function as unsigned char
• Find the count of 0 to 255 pixels in the image
• Normalize this count to get probability density function (pdf) of pixel values
• Now add the pdf cumulatively as shown in the table above to get cdf of pixel values
• Multiply the cdf by 255
• Get the floor rounded values for every 0 to 255 pixels
• Replace the original image pixel values with the new corresponding rounded pixel values which are calculated
• Write the computed contrast enhanced image on output.raw file using the fwrite() function
• Write the histograms to text file and plot using the excel

Method B:-
 
 First, we need to decide how many pixels go in each bucket. Second step is to assign pixels in the original image to corresponding bucket.
 The figure above shows that we have to arrange the pixels from 0 to 255 and then assign equal number of pixels to each bucket. The bucket size is defined as the total number of pixels in the image divided by 256 (say, image is of 400x400 size, divide by 256 = 625). After equal number of pixels are assigned in a bucket, the original image is replaced with pixels that are assigned for each bucket. Thus, this method ensures that equal number of pixels are present in every 0 to 255 buckets.
 
Algorithm implemented in C++:
• Read rose_dark.raw and rose_bright.raw input images using fread() function and get height, width and bytesperpixel values
• Run 3 nested for loops for height (400), width (400) and bytesperpixel (=1) to access each pixel in the original image
• Define the histograms of original image, equalized image and transfer function as unsigned char
• Initialize 3 different 1D arrays to store values for the row index, column index and pixel value
• Sort these pixels in the order of 0 to 255
• Store the corresponding row index and column index for each pixel in the 1D arrays defined above
• Change the corresponding pixel values at the given locations according to the bucket size such that each bucket will contain equal number of pixels 
• Replace the pixel values in the input image to new pixel values calculated by the bucket filling method and use the row and column index tracked in 1D arrays to put the new values of pixels
• Write the CDF based histogram equalized contrast enhanced image on output.raw file using the fwrite() function
• Write the histograms to text file and plot using the excel
*/

// To run the code, open terminal, add path and run using following commands
// g++ -o 1c 1c_histogram_equalization.cpp
// ./1c rose_dark.raw output_1c_rose_dark_method_A.raw 1
// ./1c rose_bright.raw output_1c_rose_bright_method_A.raw 1
// ./1c rose_dark.raw output_1c_rose_dark_method_B.raw 1
// ./1c rose_bright.raw output_1c_rose_bright_method_B.raw 1
// ./1c rose_mix.raw output_1c_rose_mix_method_A.raw 1
// ./1c rose_mix.raw output_1c_rose_mix_method_B.raw 1

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <cmath>
#include <algorithm> 

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel = 1;
	int Size = 400;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 400]" << endl;
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
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//Problem 1c
	
	//histogram manipulation

	//Method A : the transfer-function-based histogram equalization method

	/* 1. Obtain the histogram ie count the frequency of pixels of each grayscale value (0~255)
	2. Calculate the normalized probabilty histogram ie divide the histogram by total number of pixels
	3. Calculate the CDF.
	4. Create the mapping table ie mapping rule : x to CDF(x)*255
	*/

	// Allocate image output data array for the bucket filling method
	unsigned char Imagedata_transfer[Size][Size][BytesPerPixel];
	
	int i,j,k;

	int p_value[255] = {0};
	for (i=0; i<=255; i++)
	{
		p_value[i] = i;

		//cout << p_value[i] << endl;
	}

	int pixel_frequency[256] = {0};

	//Run 3 nested for loops for height (400), width (400) and bytesperpixel (=1) to access each pixel in the original image

	//Find the count of 0 to 255 pixels in the image

	for (i=0; i<Size; i++)
	{
		for (j=0; j<Size; j++)
		{
			for (k=0; k<BytesPerPixel; k++)
			{
				//cout << "[Imagedata[i][j][k]" << (int)Imagedata[i][j][k] << endl;

				pixel_frequency[(int)Imagedata[i][j][k]] += 1;
				
			}
		}
	}

	for (i=0; i<=255; i++)
	{
		//plot histogram of rose of input
		//cout << pixel_frequency[i] << endl;
	}

	double normalized_probability[256] = {0};

	//Normalize this count to get probability density function (pdf) of pixel values

	for (i=0; i<=255; i++)
	{
		//cout << "pixel_frequency[i]" << pixel_frequency[i] << endl;
		normalized_probability[i] += ((double)pixel_frequency[i])/(Size*Size);
		//cout << "normalized_probability[i]" << normalized_probability[i] << endl;

	}

	double CDF[256] = {0};
	CDF[0] = normalized_probability[0];

	//add the pdf cumulatively to get cdf of pixel values

	for (i=1; i<=255; i++)
	{
		CDF[i] = CDF[i-1] + normalized_probability[i];			
	}

	//cout << "CDF: " << CDF[255] << endl;

	double mul_cdf[256];
	double rounded_values[256];

	//Multiply the cdf by 255
	//Get the floor rounded values for every 0 to 255 pixels

	for (i=0; i<=255; i++)
	{
		//cout << "CDF: " << CDF[i] << endl;
		mul_cdf[i] = CDF[i]*255;
		rounded_values[i] = floor(mul_cdf[i]);

		//for transfer function of rose
		//cout << rounded_values[i] << endl;
	}

	//Replace the original image pixel values with the new corresponding rounded pixel values which are calculated

	for (i=0; i<Size; i++)
	{
		for (j=0; j<Size; j++)
		{
			for (k=0; k<BytesPerPixel; k++)
			{
				//cout << "[Imagedata[i][j][k]" << (int)Imagedata[i][j][k] << endl;

				Imagedata_transfer[i][j][k] = rounded_values[(int)Imagedata[i][j][k]];
				//Imagedata[i][j][k] = ((unsigned char)(rounded_values[i]));

				//cout << "[Imagedata[i][j][k]" << (int)Imagedata[i][j][k] << endl;
			}
		}
	}

	//Find the count of 0 to 255 pixels in the enhanced output image to plot histogram

	int pixel_frequency2[256] = {0};

	for (i=0; i<Size; i++)
	{
		for (j=0; j<Size; j++)
		{
			for (k=0; k<BytesPerPixel; k++)
			{
				//cout << "[Imagedata[i][j][k]" << (int)Imagedata[i][j][k] << endl;
				
				pixel_frequency2[(int)Imagedata_transfer[i][j][k]] += 1;
				
			}
		}
	}

	for (i=0; i<=255; i++)
	{
		//cout << i << endl;
		//plot histogram of rose for A
		//cout << pixel_frequency2[i] << endl;
	}

	double normalized_probability2[256] = {0};

	//Normalize this count to get probability density function (pdf) of pixel values

	for (i=0; i<=255; i++)
	{
		//cout << "pixel_frequency2[i]" << pixel_frequency2[i] << endl;
		normalized_probability2[i] += ((double)pixel_frequency2[i])/(Size*Size);
		//cout << "normalized_probability2[i]" << normalized_probability2[i] << endl;

	}

	double CDF2[256] = {0};
	CDF2[0] = normalized_probability2[0];

	//add the pdf cumulatively to get cdf of pixel values

	for (i=1; i<=255; i++)
	{
		CDF2[i] = CDF2[i-1] + normalized_probability2[i];			
	}

	//cout << "CDF: " << CDF2[255] << endl;

	double mul_cdf2[256];
	double rounded_values2[256];

	//Multiply the cdf by 255
	//Get the floor rounded values for every 0 to 255 pixels

	for (i=0; i<=255; i++)
	{
		//cout << "CDF: " << CDF2[i] << endl;
		mul_cdf2[i] = CDF2[i]*255;
		rounded_values2[i] = floor(mul_cdf2[i]);

		//for transfer function of rose for A
		//cout << rounded_values2[i] << endl;
	}


	/*
	//Write the histograms to text file
	double Pixelfrequency[255];
	ofstream hist;
	//hist.open("/Users⁩/brinalbheda⁩/Desktop⁩/DIP HW1⁩/1c⁩");
	hist << "Pixel count, Pixel values" << endl;

	for (i=0; i<=255; i++)
	{
    	//cout << "Pixelfrequency" << pixel_frequency[i] << endl;
		//cout << i << endl;
	}

	hist.close();
	*/





	//Method B : the cumulative-probablity-based histogram equalization method (cdf/bucket-filling method)

	// Allocate image output data array for the bucket filling method
	unsigned char Imagedata_bucket[Size][Size][BytesPerPixel];

	//Initialize 3 different 1D arrays to store values for the row index, column index and pixel value
	unsigned int height_i[Size*Size];
	unsigned int width_i[Size*Size];
	unsigned char pixel[Size*Size];

	unsigned int temp_count = 0;

	//Sort these pixels in the order of 0 to 255, since first for loop starts with pixel values
	//Store the corresponding row index and column index for each pixel in the 1D arrays defined above

	for(int p = 0; p < 256; p++) 
	{
        for(int i = 0; i < Size ; i++) 
		{
            for(int j=0; j < Size; j++) 
			{
				for (int k=0; k<BytesPerPixel; k++)
				{
                	if(((int)Imagedata[i][j][k]) == p) 
					{
                    	pixel[temp_count] = p;
						height_i[temp_count] = i;
                    	width_i[temp_count] = j;
                    	temp_count = temp_count + 1;
                	}
				}
            }
        }
    }
    
	unsigned int bucket_size = ceil(((float)(Size*Size) / 256.0));        //bucket size is (400*400/256)=625
    
	//Change the corresponding pixel values at the given locations according to the bucket size such that each bucket will contain equal number of pixels ie 225

    unsigned int tc = 0;
    unsigned char tp = 0;
    unsigned int ta = 0;
    for(int i = 0; i < (Size*Size) ; i++) 
	{
        if(tc != bucket_size) 
		{
            pixel[ta] = tp;
            ta = ta + 1;
            tc = tc + 1;
        }
        else
		{
            tp = tp + 1;
            tc = 0;
        }
    }

    //Replace the pixel values in the input image to new pixel values calculated by the bucket filling method and use the row and column index tracked in 1D arrays to put the new values of pixels

    unsigned int th;
    unsigned int tw;
    for(int i=0; i < (Size*Size); i++) {
        th = height_i[i];
        tw =  width_i[i];

        Imagedata_bucket[th][tw][0] = pixel[i];
    }

	
	//Find the count of 0 to 255 pixels in the enhanced output image to plot histogram

	int pixel_frequency1[256] = {0};

	for (i=0; i<Size; i++)
	{
		for (j=0; j<Size; j++)
		{
			for (k=0; k<BytesPerPixel; k++)
			{
				//cout << "[Imagedata[i][j][k]" << (int)Imagedata[i][j][k] << endl;

				pixel_frequency1[(int)Imagedata_bucket[i][j][k]] += 1;
				
			}
		}
	}

	for (i=0; i<=255; i++)
	{
		//plot histogram of rose for B
		//cout << pixel_frequency1[i] << endl;
	}

	double normalized_probability1[256] = {0};

	//Normalize this count to get probability density function (pdf) of pixel values

	for (i=0; i<=255; i++)
	{
		//cout << "pixel_frequency1[i]" << pixel_frequency1[i] << endl;
		normalized_probability1[i] += ((double)pixel_frequency1[i])/(Size*Size);
		//cout << "normalized_probability1[i]" << normalized_probability1[i] << endl;

	}

	double CDF1[256] = {0};
	CDF1[0] = normalized_probability1[0];

	//add the pdf cumulatively to get cdf of pixel values

	for (i=1; i<=255; i++)
	{
		CDF1[i] = CDF1[i-1] + normalized_probability1[i];			
	}

	//cout << "CDF: " << CDF1[255] << endl;

	double mul_cdf1[256];
	double rounded_values1[256];

	//Multiply the cdf by 255
	//Get the floor rounded values for every 0 to 255 pixels

	for (i=0; i<=255; i++)
	{
		//cout << "CDF: " << CDF1[i] << endl;
		mul_cdf1[i] = CDF1[i]*255;
		rounded_values1[i] = floor(mul_cdf1[i]);

		//for transfer function of rose for B
		//cout << rounded_values1[i] << endl;
	}

	/*
	//Write the histograms to text file
	double Pixelfrequency1[255];
	ofstream hist1;
	hist1.open("/Users⁩/brinalbheda⁩/Desktop⁩/DIP HW1⁩/1c⁩");
	hist1 << "Pixel count, Pixel values" << endl;
	
	for (i=0; i<=255; i++)
	{
    	//cout << "Pixelfrequency1" << pixel_frequency1[i] << endl;
	}

	hist1.close();

	*/


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	//fwrite(Imagedata_transfer, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fwrite(Imagedata_bucket, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


/*
to observe output on ImageJ, 
Use: image type - 8-bit
width - 400
height - 400
*/

