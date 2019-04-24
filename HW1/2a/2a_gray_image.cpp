// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the pepper_noise/pepper_uni input image is of size 256 x 256 
//			and is in the RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

//EE 569: Homework 1 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 01/22/2019

/*
Algorithm implemented in C++:
• Read rose_color_noise.raw input images using fread() function and get height, width and bytesperpixel values
• Run 3 nested for loops for height (256), width (256) and bytesperpixel (=3) to access each pixel in the original image
• Provide the required NxN window size
• Apply the Mean/Median/Gaussian/NLM/Bilateral filter using the algorithm mentioned below
• Create a 3D array to store the denoised output
• Calculate the PSNR to compare the performance
• Write the computed image output data array on denoised_rose_color.raw file using the fwrite() function

Mean (Low Pass Filter) algorithm in C++:
• For the index of rows and columns of filter, if index < 0 then assign 0
• If row index greater than row, assign row value and if column index greater than column, assign column value
• This is done for boundary pixels to avoid boundary effect as there is no extension in boundary
• Now store NxN neighborhood pixels in 1D array
• Find the average of the array
• The denoised image pixel value is calculated using this average value

Median algorithm in C++:
• For the index of rows and columns of filter, if index < 0 then assign 0
• If row index greater than row, assign row value and if column index greater than column, assign column value
• This is done for boundary pixels to avoid boundary effect as there is no extension in boundary
• Now store NxN neighborhood pixels in 1D array
• Sort the array using the bubble sort algorithm
• Find the middle value of this sorted array
• The denoised image pixel value is calculated using this middle value

Gaussian algorithm in C++:
• Calculate the Gaussian NxN window using formula and normalize it
• For the index of rows and columns of filter window, if index < 0 then assign 0
• If row index greater than row, assign row value and if column index greater than column, assign column value
• This is done for boundary pixels to avoid boundary effect as there is no extension in boundary
• Now store NxN neighborhood pixels in 1D array
• Multiply the corresponding pixel values with Gaussian window
• The denoised image pixel value is calculated using this value

Bilateral algorithm in C++:
 ''One more denoising method is using Bilateral filters. The above filters may result in blur and hence to retain the sharp edges, this filter is used. Weights depend not only on the Euclidean distance of pixels but also on the difference on the pixel values.''
 
• For the index of rows and columns of filter, if index < 0 then assign 0
• If row index greater than row, assign row value and if column index greater than column, assign column value
• This is done for boundary pixels to avoid boundary effect as there is no extension in boundary
• Calculate weight by denoting the sigma parameters - sigma c is higher and sigma s is smaller
• Apply the equation and find Y
• The denoised image pixel value is calculated using this value
*/


//observe output for 5 different filters - mean, median, gaussian, bilateral, nlm and for each filter, observe output for different window sizes - 3, 5, 7, 9

// To run the code, open terminal, add path and run using following commands
// g++ -o 2a 2a_gray_image.cpp

// ./2a pepper_uni.raw output_2a_mean_n3.raw 1
// ./2a pepper_uni.raw output_2a_mean_n5.raw 1
// ./2a pepper_uni.raw output_2a_mean_n7.raw 1
// ./2a pepper_uni.raw output_2a_mean_n9.raw 1

// ./2a pepper_uni.raw output_2a_median_n3.raw 1
// ./2a pepper_uni.raw output_2a_median_n5.raw 1
// ./2a pepper_uni.raw output_2a_median_n7.raw 1
// ./2a pepper_uni.raw output_2a_median_n9.raw 1

// ./2a pepper_uni.raw output_2a_gaussian_n3.raw 1
// ./2a pepper_uni.raw output_2a_gaussian_n5.raw 1
// ./2a pepper_uni.raw output_2a_gaussian_n7.raw 1
// ./2a pepper_uni.raw output_2a_gaussian_n9.raw 1

// ./2a pepper_uni.raw output_2a_bilateral_n3.raw 1
// ./2a pepper_uni.raw output_2a_bilateral_n5.raw 1
// ./2a pepper_uni.raw output_2a_bilateral_n7.raw 1
// ./2a pepper_uni.raw output_2a_bilateral_n9.raw 1

// ./2a pepper_uni.raw output_2a_nlm.raw 1


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <string>
#include <algorithm>

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
	
	// Allocate input image data array
	unsigned char Imagedata_input[Size][Size][BytesPerPixel];

	// Read image input (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_input, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//Problem 2a
	
	int i,j,k;

	//Apply a filter of size NxN to the noisy image

	//mask size/window size defines the NxN filter size, eg. 3x3 filter
	//mask_size/window_size -> N can take odd values like 3,5,7,9..
	int window_size = 3;       

	// Allocate temp denoised image data array
	unsigned char Imagedata_denoised_mean[Size][Size][BytesPerPixel];

	// Allocate temp denoised image data array
	unsigned char Imagedata_denoised_median[Size][Size][BytesPerPixel];

	// Allocate temp denoised image data array
	unsigned char Imagedata_denoised_gaussian[Size][Size][BytesPerPixel];

	// Allocate temp denoised image data array
	unsigned char Imagedata_denoised_bilateral[Size][Size][BytesPerPixel];

	// Allocate output denoised image data array
	unsigned char Imagedata_output[Size][Size][BytesPerPixel];

	

/*

	//Mean filter (low pass filter)
	
	//for the mask
	signed int initial_N = - floor(window_size/2.0);
	signed int end_N = floor(window_size/2.0);
	signed int m,l;

	signed int height_i;    //row index
	signed int width_i;     //column index
	unsigned char Image_temp;

	for (i = 0; i < Size; i ++) 
	{
        for (j = 0; j < Size; j++)
		{
            unsigned int sum = 0;
            float average = 0.0;
			
            for(m = initial_N ; m <= end_N ; m ++) 
			{
                for(l = initial_N ; l <= end_N ; l ++) 
				{
                    height_i = i + m;
                    width_i = j + l;

                    if(height_i < 0) 
					{
                        height_i = 0;
                    }
                    if(width_i < 0) 
					{
                        width_i = 0;
                    }
                    if(height_i > Size-1) 
					{
                        height_i = Size-1;
                    }
                    if(width_i > Size-1) 
					{
                        width_i = Size-1;
                    }
                    
					Image_temp = Imagedata_input[height_i][width_i][BytesPerPixel];
                    sum = sum + Image_temp;
                }
            }

            average = ((float)sum) / ((float)(window_size*window_size));
            Imagedata_denoised_mean[i][j][BytesPerPixel] = floor(average);
        }
    }




	//Median filter

	//for the mask
    signed int initial_N = - floor(window_size/2.0);
	signed int end_N = floor(window_size/2.0);
	signed int m,l;

	signed int height_i;    //row index
	signed int width_i;     //column index
	unsigned char Image_temp1;
    int middle_value = ceil((window_size*window_size)/2);
    
    for (i = 0; i < Size; i ++) 
	{
        for (j = 0; j < Size; j++) 
		{
            unsigned char Image_temp2[window_size * window_size];
            int t = 0;

            for(m = initial_N ; m <= end_N ; m ++)
			{
                for(l = initial_N ; l <= end_N ; l ++) 
				{  
                    height_i = i + m;
                    width_i = j + l;

                    if(height_i < 0) 
					{
                        height_i = 0;
                    }
                    if(width_i < 0) 
					{
                        width_i = 0;
                    }
                    if(height_i > Size-1) 
					{
                        height_i = Size-1;
                    }
                    if(width_i > Size-1) 
					{
                        width_i = Size-1;
                    }

                    Image_temp1 = Imagedata_input[height_i][width_i][BytesPerPixel];
                    Image_temp2[t] = Image_temp1;
                    t = t + 1;
                }
            }

            int sorting_value;
            for(int r = 0; r < window_size * window_size ; r++)
			{
                for(int s = 0; s < (window_size * window_size) - 1; s++) 
				{
                    if (Image_temp2[s] > Image_temp2[s+1]) 
					{
                        sorting_value = Image_temp2[s];
                        Image_temp2[s] = Image_temp2[s+1];
                        Image_temp2[s+1] = sorting_value;
                    }
                }
            }

            Imagedata_denoised_median[i][j][BytesPerPixel] = Image_temp2[middle_value];
        }
    }




	//Gaussian filter 

    signed int initial_N = - floor(window_size/2.0);
	signed int end_N = floor(window_size/2.0);
	int p,q;
	signed int x,y;
	signed int height_i;    //row index
	signed int width_i;     //column index

	float s = 1.0;         //sigma
    int c = 0;
	
    double ws = 0;         //window sum
	int m,l;

	unsigned char Image_temp4;

	//gaussian window
	double window_g[window_size*window_size];

    for(m=0; m < window_size ; m++)
	{
        for(l = 0; l < window_size; l++) 
		{
            int p = m - ((window_size-1) / 2);
            int q = l - ((window_size-1) / 2);
            window_g[c] = ((1.0) /((2*3.14*pow(s,2)))) * exp( - ((pow(l,2) + pow(m,2)) / (2* pow(s,2))));
            ws = ws + window_g[c];
            c++ ;
        }
    }
    
    for(i= 0; i < window_size * window_size ; i++) 
	{
        window_g[i] = ((float)window_g[i]) / ((float)ws);
    }
    
    for (int i = 0; i < Size; i ++) 
	{
        for (int j = 0; j < Size; j++) 
		{
            unsigned char Image_temp3[window_size * window_size];
            int t = 0;

            for(x = initial_N ; x <= end_N ; x ++) 
			{
                for(y = initial_N ; y <= end_N ; y ++) 
				{
                    height_i = i + x;
                    width_i = j + y;

                    if(height_i < 0) 
					{
                        height_i = 0;
                    }
                    if(width_i < 0) 
					{
                        width_i = 0;
                    }
                    if(height_i > Size-1) 
					{
                        height_i = Size-1;
                    }
                    if(width_i > Size-1) 
					{
                        width_i = Size-1;
                    }

                    Image_temp4 = Imagedata_input[height_i][width_i][BytesPerPixel];
                    Image_temp3[t] = Image_temp4;
                    t = t + 1;
                }
            }

            double wp = 0.0;

            for(int i = 0; i < (window_size * window_size); i++)
			{
                wp = wp + (Image_temp3[i] * window_g[i]);
            }

            Imagedata_denoised_gaussian[i][j][BytesPerPixel] = (unsigned char) wp;
        }
    }




	//Bilateral filter

	int N=3;	//N*N neighborhood   --> window size
    int l,h;
    int m,n,x;
    int t;
    double sigma1=3,sigma2=100,sum_val,sum_wgt;

	//matrix extension
    unsigned char tmp1[Size][Size+N-1][BytesPerPixel];
    unsigned char tmp2[Size+N-1][Size+N-1][BytesPerPixel];

    for (l=0;l<3;l++)
    {
        for(i=0;i<=Size-1;i++)
        {
            for(j=(N-1)/2;j<=Size-1+(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata_input[i][j-(N-1)/2][l];
            }
        }
        for(i=0;i<=Size-1;i++)
        {
            for(j=0;j<(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata_input[i][(N-1)/2-j][l];
                tmp1[i][Size+(N-1)/2+j][l] = Imagedata_input[i][Size-((N-1)/2+j)][l];
            }
        }
        for(i=(N-1)/2;i<=Size-1+(N-1)/2;i++)
        {
            for(j=0;j<=Size+(N-1)-1;j++)
            {
                tmp2[i][j][l] = tmp1[i-(N-1)/2][j][l];
            }

        }
        for(j=0;j<=Size+(N-1)-1;j++)
        {
            for(i=0;i<(N-1)/2;i++)
            {
                tmp2[i][j][l] = tmp1[(N-1)/2-i][j][l];
                tmp2[Size+(N-1)/2+i][j][l] = tmp1[Size-((N-1)/2+i)][j][l];
            }
        }

    }

    //impulse noise filter using cross shape
    int a[2*N-1];
    int tmp_val;

    for(l=0;l<3;l++)
    {
        for(i=(N-1)/2;i<=Size-1+(N-1)/2;i++)
        {
            for(j=(N-1)/2;j<=Size-1+(N-1)/2;j++)
            {
                x=0;
                for(m=0;m<=(N-1)/2;	m++)
                {
                    for(n=0;n<=(N-1)/2;n++)
                    {
                        if(m==0&&n==0)
                        {
                            a[x]=(int)tmp2[i][j][l];
                            x++;
                        }
                        if(m!=0&&n==0)
                        {
                            a[x]=(int)tmp2[i+m][j][l];
                            x++;
                            a[x]=(int)tmp2[i-m][j][l];
                            x++;
                        }
                        if(m==0&&n!=0)
                        {
                            a[x]=(int)tmp2[i][j+n][l];
                            x++;
                            a[x]=(int)tmp2[i][j-n][l];
                            x++;
                        }
                    }
                }

                t=2*N-1;
                for(k=0;k<2*N-1;k++)
                {
                    for(h=0;h<=t-2;h++)
                    {
                        if(a[h]>a[h+1])
                        {
                            tmp_val=a[h];
                            a[h]=a[h+1];
                            a[h+1]=tmp_val;
                        }
                    }
                    t--;
                }

                Imagedata_input[i-(N-1)/2][j-(N-1)/2][l]=(unsigned char)a[N];
            }
        }
    }

    //matrix extension after eliminating impulse noise
    for (l=0;l<3;l++)
    {
        for(i=0;i<=Size-1;i++)
        {
            for(j=(N-1)/2;j<=Size-1+(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata_input[i][j-(N-1)/2][l];
            }
        }
        for(i=0;i<=Size-1;i++)
        {
            for(j=0;j<(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata_input[i][(N-1)/2-j][l];
                tmp1[i][Size+(N-1)/2+j][l] = Imagedata_input[i][Size-((N-1)/2+j)][l];
            }
        }
        for(i=(N-1)/2;i<=Size-1+(N-1)/2;i++)
        {
            for(j=0;j<=Size+(N-1)-1;j++)
            {
                tmp2[i][j][l] = tmp1[i-(N-1)/2][j][l];
            }

        }
        for(j=0;j<=Size+(N-1)-1;j++)
        {
            for(i=0;i<(N-1)/2;i++)
            {
                tmp2[i][j][l] = tmp1[(N-1)/2-i][j][l];
                tmp2[Size+(N-1)/2+i][j][l] = tmp1[Size-((N-1)/2+i)][j][l];
            }
        }

    }

    // bilateral filter
    for(l=0;l<3;l++)
    {
        for(i=(N-1)/2;i<=Size-1+(N-1)/2;i++)
        {
            for(j=(N-1)/2;j<=Size-1+(N-1)/2;j++)
            {
                sum_val=0;
                sum_wgt=0;

                for(m=0;m<=(N-1)/2;	m++)
                {
                    for(n=0;n<=(N-1)/2;n++)
                    {
                        if(m==0&&n==0)
                        {

                            sum_val=sum_val+exp(-1*0*0/(2*sigma2*sigma2))*exp(-1*0*0/(2*sigma1*sigma1))*(int)tmp2[i+m][j][l];
                            sum_wgt=sum_wgt+exp(-1*0*0/(2*sigma2*sigma2))*exp(-1*0*0/(2*sigma1*sigma1));;
                        }
                        if(m!=0&&n==0)
                        {
                            x=(int)tmp2[i+m][j][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*m*m/(2*sigma1*sigma1))*(int)tmp2[i+m][j][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*m*m/(2*sigma1*sigma1));
                            x=(int)tmp2[i-m][j][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*m*m/(2*sigma1*sigma1))*(int)tmp2[i-m][j][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*m*m/(2*sigma1*sigma1));
                        }
                        if(m==0&&n!=0)
                        {
                            x=(int)tmp2[i][j+n][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*n*n/(2*sigma1*sigma1))*(int)tmp2[i][j+n][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*n*n/(2*sigma1*sigma1));
                            x=(int)tmp2[i][j+n][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*n*n/(2*sigma1*sigma1))*(int)tmp2[i][j-n][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*n*n/(2*sigma1*sigma1));
                        }
                        if(m!=0&&n!=0)
                        {
                            x=(int)tmp2[i+m][j+n][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1))*(int)tmp2[i+m][j+n][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1));
                            x=(int)tmp2[i+m][j-n][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1))*(int)tmp2[i+m][j-n][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1));
                            x=(int)tmp2[i-m][j+n][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1))*(int)tmp2[i-m][j+n][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1));
                            x=(int)tmp2[i-m][j-n][l]-(int)tmp2[i][j][l];
                            sum_val=sum_val+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1))*(int)tmp2[i-m][j-n][l];
                            sum_wgt=sum_wgt+exp(-1*x*x/(2*sigma2*sigma2))*exp(-1*(m*m+n*n)/(2*sigma1*sigma1));
                        }
                    }
                }

                Imagedata_denoised_bilateral[i-(N-1)/2][j-(N-1)/2][l]=(unsigned char)(sum_val/sum_wgt);
            }
        }
    }
    

*/


	//Assigning the denoised image (for mean/median/gaussian) to the output image
	for (i=0; i < Size; i++)
	{
		for (j=0; j < Size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				//Imagedata_output[i][j][k] = Imagedata_denoised_mean[i][j][k];
				//Imagedata_output[i][j][k] = Imagedata_denoised_median[i][j][k];
				//Imagedata_output[i][j][k] = Imagedata_denoised_gaussian[i][j][k];
				//Imagedata_output[i][j][k] = Imagedata_denoised_bilateral[i][j][k];
			}
		}
	}


	//PSNR
	
	unsigned int mse = 0;           //mean square error
	float psnr;          //peak to signal noise value

	for (i = 0; i < Size; i++)
	{
		for (j = 0; j < Size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				mse = mse + (pow((Imagedata_output[i][j][k] - Imagedata_input[i][j][k]),2));
			}
		}
	}

	//cout << "mse: " << mse << endl;
	
	//normalize mse
	//calculate psnr in db
	psnr = 10 * (log10((255*255)/(mse / ((float)(Size * Size)))));

	cout << "PSNR: " << psnr << endl;


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_output, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


/*
to observe output on ImageJ, 
Use: image type - 8-bit
width - 256
height - 256
*/

