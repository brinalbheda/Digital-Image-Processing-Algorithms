// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the lighthouse input images are of size 512 x 512 
//			and are in the RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

//EE 569: Homework 3 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 02/12/2019

/*
 Algorithm implemented in C++:
 • Read the input image “hat.raw” whose dimensions are height_size = 512, width_size = 512, BytesPerPixel = 1
 • Consider the 4 triangles in the square input image and calculate the three vertices and the mid-points of each side of each of the 4 triangles, thus we get 6 values corresponding to the 4 triangles each
 • Each triangle is mapped to a warped image, the curved edge is center pixel value of the side of the image subtract by 128 which forms the arc
 • Use anchor points, each point at 3 edges and each center point of 3 sides
 • Use the above 6 points to calculate the coefficients of A and B using the long
 formula shown above
 • Calculate u and v for every pixel using the forward elliptical warping formula
 • Retrace u and v pixel locations to 0 to 512
 • Ouput the Forward warping pixel
 • Using bilinear interpolation, find the pixel intensity and copy the value in
 output image.
 • Then map all these points to the warped output image
 • Repeat the procedure for the other 3 triangles for finding the coefficients of A
 and B
 • Check for the values lie inside the image and put conditions to limit the values
 in the range >0 and <512
 • Calculate xnew and ynew for every u,v using Reserve Address Mapping
 formula
 • Retrace xnew and ynew to 0 to 512
 • Output the Reverse warping pixel
 • Map all these values obtained to the warped image
 • Write the computed image data array on output.raw file using the fwrite() function
 */

// To run the code, open terminal, add path and run using following commands
// g++ -o 1b 1b_spatial_warping.cpp

// ./1b hat.raw output_1b.raw 

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

void reverseMapping(int a0, int a1, int a2, int a3, int a4, int a5, int b0, int b1, int b2, int b3, int b4, int b5 )
{
	int width_size = 512;
	int BytesPerPixel = 1;
	int height_size = 512;
	for(int i = 0; i<height_size; i++)
	{
		for(int j = 0; j<width_size; j++)
		{
			for (int k = 0; k < BytesPerPixel; k++)
			{
				int ur, vr;
				ur = a0 + a1*i + a2*j + a3*i*i + a4*i*j + a5*j*j;
				vr = b0 + b1*i + b2*j + b3*i*i + b4*i*j + b5*j*j;
			}
		}
	}

}
/*
void findingCoefficients(int u0, int u1, int u2, int u3, int u4, int u5, int v0, int v1, int v2, int v3, int v4, int v5 )
{
	for(int i = 0; i<height_size; i++)
	{
		for(int j = 0; j<width_size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				int a0, a1, a2, a3, a4, a5;
				int b0, b1, b2, b3, b4, b5;

			
			}
		}
	}

}
*/
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	FILE* file1 = NULL;
	int BytesPerPixel = 1;
	int width_size = 512;
	int height_size = 512;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	

	//hat.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata[height_size][width_size][BytesPerPixel];

	//hat.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata_output[height_size][width_size][BytesPerPixel];

	//hat.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedatao[height_size][width_size][BytesPerPixel];

	// Allocate image data array
	unsigned char Imagedata_tempo[height_size][width_size][BytesPerPixel];

	// Allocate image data array
	unsigned char Imagedata_dummy[height_size][width_size][BytesPerPixel];

	// Allocate image data array
	unsigned char Imagedata_pixel_locations[height_size][width_size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);

	// Read image (filename specified by first argument) into image data matrix
	//char o[] = "/Users/brinalbheda/Documents/hat.raw";
	//file1 = fopen(o,"rb");

	// if (!(file1=fopen(argv[4],"rb"))) {
	// 	cout << "Cannot open file: " << argv[4] <<endl;
	// 	exit(1);
	// }
	fread(Imagedatao, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file1);
	fclose(file1);

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[5] << endl;
		exit(1);
	}
	fwrite(Imagedatao, sizeof(unsigned char), 512*512*1, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//Geometric Modification 

	//Problem 1b - Spatial Warping 

	/*

	output is non-linear operation of input
	find more number of points here as compared to 1a --> these are the anchor points which are used to find the co-efficients
	6 number of unknowns of A and B each 
	so to solve find 6 points for output to input and find all the co-efficients 
	rest operations same as 1a

	trick is we have one output image given
	divide rectangular shape into 4 triangular regions, each triangle is mapped to a warped image
	the curved edge is center pixel value of the side of the image - 128 (height by which the sides of the image is pulled to curve inside)
	this is done using anchor points - each point at 3 edges and each center point of 3 sides -> 6 points
	use these 6 points to calculate the co-efficients of A and B
	
	*/

	int i,j,k;

	unsigned char Imagedata_temp[height_size][width_size][BytesPerPixel];

	//working on temp data 

	for(i=0; i<height_size; i++)
    {
        for(j=0; j<width_size; j++)
        {
			for (k = 0; k < BytesPerPixel; k++)
			{
				Imagedata_temp[i][j][k] = Imagedata[i][j][k];
			}
		}
	}

	/*
	// Constant/Fixed thresholding

	// Allocate image processing data array
	unsigned char Imagedata_fixed_thresholding[height_size][width_size][BytesPerPixel];

	for (int i=0; i<height_size; i++)
	{
		for (int j=0; j<width_size; j++)
		{
			for (int k=0; k<BytesPerPixel; k++)
			{
				//compare pixel value with 250
				if (Imagedata[i][j][k] < 250)
				{
					//if pixel value is less than 250, map it to 0
					Imagedata_fixed_thresholding[i][j][k] = 0;
				}
				else 
				{
					//if pixel value is greater than 250, map it to 255
					Imagedata_fixed_thresholding[i][j][k] = 255;
				}
			}
		}
	}
*/

	//locating the corner points and center points (midpoints)
	int originX = 0;
	int originY = 0;
	
	int rightTopCentX = 0;
	int rightTopCentY = width_size/2;

	int centerX = height_size/2;
	int centerY = width_size/2;

	int rightTopX = 0;
	int rightTopY = width_size;

	int midTx,midTy;
	int midLx,midLy;
	int midRx,midRy;

	midTy = centerY;
	midTx = 0;

	midLx = (originX + centerX)/2;
	midLy = (originY + centerY)/2;

	midRx = (centerX + rightTopX)/2;
	midRy = (centerY+ rightTopY)/2;

	int leftBottomX = height_size;
	int leftBottomY = 0;

	int leftTopCentX = height_size/2;
	int leftTopCentY = 0;

	int leftBottomTriX = (centerX + leftBottomX)/2;
	int leftBottomTriY = (centerY + leftBottomY)/2;

	
	int rightBottomX = height_size;
	int rightBottomY = width_size;

	int rightBottomTriX = (centerX + rightBottomX)/2;
	int rightBottomTriY = (centerY + rightBottomY)/2;

	int bottomCentX = (leftBottomX + rightBottomX)/2;
	int bottomCentY = (leftBottomY + rightBottomY)/2;

	int rightCenterX = (rightTopX + rightBottomX)/2;
	int rightCenterY = (rightTopY + rightBottomY)/2;


	int roundedMappedPixelHeight,roundedMappedPixelWidth;
	unsigned char Pixela[height_size][width_size];
	unsigned char Imagedata_resizeda[height_size][width_size];

	for(i=0;i<height_size;i++)
    {
        float mappedPixelHeight = i;  // resizing ratio - scaling
        roundedMappedPixelHeight= (int)(mappedPixelHeight);   //to floor the float value to the nearest smaller integer value
        
        if (roundedMappedPixelHeight>=height_size-1)  // to check if the image does not exceed the size 512
        {
            roundedMappedPixelHeight=roundedMappedPixelHeight-1;
        }
        
        //Find the difference between the float value and floored integer value
        float delx=mappedPixelHeight-roundedMappedPixelHeight;
        
        for(j=0;j<width_size;j++)
        {
		float mappedPixelWidth = j;
		roundedMappedPixelWidth=(int)(mappedPixelWidth);
		
		if (roundedMappedPixelHeight>=width_size-1)
		{
			roundedMappedPixelHeight=roundedMappedPixelHeight-1;
		}
		
		float dely=mappedPixelWidth-roundedMappedPixelWidth;
		
		//Calculating resized image intensity values using bilinear interpolation
		Imagedata_resizeda[i][j]= (unsigned char)((1-delx)*(1-dely)*Pixela[roundedMappedPixelHeight][roundedMappedPixelWidth]
			+(delx)*(1-dely)*Pixela[roundedMappedPixelHeight+1][roundedMappedPixelWidth]+
			(1-delx)*(dely)*Pixela[roundedMappedPixelHeight][roundedMappedPixelWidth+1]+
			(delx)*(dely)*Pixela[roundedMappedPixelHeight+1][roundedMappedPixelWidth+1]);
		}
	}

	int arc_height = 128;
	float a = rightTopCentX;
	float b = rightTopCentY;

	reverseMapping(originX, centerX, rightTopX, rightTopCentX, midLx , midRx,originY, centerY, rightTopY, rightTopCentY, midLy , midRy);

/*
	int u1 = centerX;
	int v1 = centerY;

	float x0 = rightTopX;
	float y0 = rightTopY;

	float pi = 3.14;

	double a2_b2 = a*a - b*b;
    double u_x0 = u1 - x0;
    double v_y0 = v1 - y0;
    double theta = std::atan(v_y0/u_x0);

    if (u_x0 < 0.0 && v_y0 >= 0.0) 
	{
      theta = theta + pi;
    }
    if (u_x0 < 0.0 && v_y0 < 0.0) 
	{
      theta = theta - pi;
    }

    // use Newton's method to find root
    double f = ( a2_b2 * std::sin(theta) * std::cos(theta) - u_x0 * a * std::sin(theta) + v_y0 * b * std::cos(theta) );
    double df = ( a2_b2 * (std::cos(theta)*std::cos(theta) - std::sin(theta)*std::sin(theta)) - u_x0 * a * std::cos(theta) - v_y0 * b * std::sin(theta) );

    for (int i=0; i<5; i++) 
	{
      theta = theta - f/df;
      f = ( a2_b2 * std::sin(theta) * std::cos(theta) - u_x0 * a * std::sin(theta) + v_y0 * b * std::cos(theta) );
      df = ( a2_b2 * (std::cos(theta)*std::cos(theta) - std::sin(theta)*std::sin(theta)) - u_x0 * a * std::cos(theta) - v_y0 * b * std::sin(theta) );
    }

    // calculate distance, |p-r|
    double dx = u1 - a * std::cos(theta) - x0;
    double dy = v1 - b * std::sin(theta) - y0;
    double distance = std::sqrt(dx*dx + dy*dy);
*/    	

	unsigned int p,q;
    float x,y,u,v;
    int rowIndex,colIndex;
	unsigned char pix[height_size][width_size][BytesPerPixel];
	unsigned char pixel[height_size][width_size][BytesPerPixel];

    for(rowIndex=0;rowIndex<height_size;rowIndex++)
    {
        for(colIndex=0;colIndex<width_size;colIndex++)
        {
			for (k = 0; k < BytesPerPixel; k++)
			{
				x=(rowIndex-((height_size/2)-1))/(float)(((height_size/2)-1)); //Normalizing values between -1 to 1
				y=(colIndex-((width_size/2)-1))/(float)(((width_size/2)-1));
				u=x*sqrt(1-((pow(y,2))/2));
				v=y*sqrt(1-((pow(x,2))/2));
				p=(u*((height_size/2)-1))+((height_size/2)-1); //Renomrlaizing values between 0 to height_size
				q=(v*((width_size/2)-1))+((width_size/2)-1);
				pix[p][q][k]=pixel[rowIndex][colIndex][k];
			}
        }
    }

    unsigned int l,m;
    float xnew,ynew;
    for(rowIndex=0;rowIndex<height_size;rowIndex++)
    {
        for(colIndex=0;colIndex<width_size;colIndex++)
        {
			for (k = 0; k < BytesPerPixel; k++)
			{
				x=(rowIndex-((height_size/2)-1))/(float)(((height_size/2)-1)); //Normalizing values between -1 to 1
				y=(colIndex-((width_size/2)-1))/(float)(((width_size/2)-1));
				u=x*sqrt(1-((pow(y,2))/2));
				v=y*sqrt(1-((pow(x,2))/2));
				//xnew=1.00/2.00*((sqrt((2+(pow(u,2))-(pow(v,2))+(2*(sqrt(2)*u)))))-(sqrt((2+(pow(u,2))-(pow(v,2))-(2*(sqrt(2)*u))))));
				//ynew=1.00/2.00*((sqrt((2-(pow(u,2))+(pow(v,2))+(2*(sqrt(2)*v)))))-(sqrt((2-(pow(u,2))+(pow(v,2))-(2*(sqrt(2)*v))))));
				l=round(xnew*((height_size/2)-1))+((height_size/2)-1); //Renomrlaizing values between 0 to height_size
				m=round(ynew*((width_size/2)-1))+((width_size/2)-1);
				pix[l][m][k]=pixel[rowIndex][colIndex][k];
			}
        }
    }

	for(int i = 0; i<height_size; i++)
	{
		for(int j = 0; j<width_size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				Imagedata_tempo[i][j][k] = pix[i][j][k];
			}
		}
	}

	float transXa;               
	float transYa;
	unsigned char Imagedata_translatea[height_size][width_size][BytesPerPixel];

	for(int i = 0; i<height_size; i++)
	{
		for(int j = 0; j<width_size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				int u1;
				int v1;
				u1 = i + transXa;
				v1 = j + transYa;
				Imagedata_translatea[u1][v1][k] = Imagedata[i][j][k];
			}
		}
	}

	for(int i = 0; i<height_size; i++)
	{
		for(int j = 0; j<width_size; j++)
		{
			for (k = 0; k < BytesPerPixel; k++)
			{
				if (Imagedata_pixel_locations[i][j][k] < 0)
				{
					break;
				}
				else if (Imagedata_pixel_locations[i][j][k] > height_size)
				{
					break;
				}
				else
				Imagedata_output[i][j][k] = Imagedata_pixel_locations[i][j][k];
			}
		}
	}



	// Write image data (filename specified by second argument) from image data matrix

	// if (!(file=fopen(argv[2],"wb"))) {
	// 	cout << "Cannot open file: " << argv[2] << endl;
	// 	exit(1);
	// }
	fwrite(Imagedata_output, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


/*
 to observe output on ImageJ,
 Use: image type - 8-bit
 width - 512
 height - 512
 */

