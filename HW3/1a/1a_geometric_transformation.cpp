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
 1) Find the coordinates of corners in each sub-image denoted by Fi [x,y], i=1,2,3. 2) Design a generic geometric transform (qx, qy) = i (px, py) which maps point (px, py) to point (qx, qy) in the i-th sub-image. Here, you need to combine one or more translation, rotation and scaling operations together. After the generic geometric transformation, the transformed ith sub-image should be a square image with its sides aligned with the horizontal and vertical axes.
 3) For scaling-up, implement the interpolation function Θ(·) such that Θ(Fi[i (x, y)]) is of size 160 x 160. Drop redundant pixels when scaling-down.
 4) Find the coordinates of the holes (specifically the top-left corners).
 5) Fill {Θ(Fi[i (x, y)]), i = 1,2,3} into the holes to get the final image. We assume “lighthouse1.raw” goes to the left hole, “lighthouse2.raw” goes to top, and “lighthouse3.raw” goes to bottom-right.
 Bi-linear interpolation may be needed to generate the pixel value at fractional positions.
 */

// To run the code, open terminal, add path and run using following commands
// g++ -o 1a 1a_geometric_transformation.cpp

// ./1a lighthouse.raw lighthouse1.raw lighthouse2.raw lighthouse3.raw output_1a.raw 

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	FILE* file1 = NULL;
	int BytesPerPixel = 1;
	int width_size = 512;
	int height_size = 512;

	int width_size1 = 256;
	int height_size1 = 256;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	
	//lighthouse.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata[height_size][width_size][BytesPerPixel];

	//lighthouse1.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata1[height_size1][width_size1][BytesPerPixel];

	//lighthouse2.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata2[height_size1][width_size1][BytesPerPixel];

	//lighthouse3.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata3[height_size1][width_size1][BytesPerPixel];

	//lighthouse.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedata_output[height_size][width_size][BytesPerPixel];

	//lighthouse.raw grayscale input image
	// Allocate image data array
	unsigned char Imagedatao[height_size][width_size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file);
	fclose(file);

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Imagedata1, sizeof(unsigned char), height_size1*width_size1*BytesPerPixel, file);
	fclose(file);

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[3],"rb"))) {
		cout << "Cannot open file: " << argv[3] <<endl;
		exit(1);
	}
	fread(Imagedata2, sizeof(unsigned char), height_size1*width_size1*BytesPerPixel, file);
	fclose(file);

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[4],"rb"))) {
		cout << "Cannot open file: " << argv[3] <<endl;
		exit(1);
	}
	fread(Imagedata3, sizeof(unsigned char), height_size1*width_size1*BytesPerPixel, file);
	fclose(file);

	//char o[] = "/Users/brinalbheda/Documents/geo.raw";
	//file1 = fopen(o,"rb");

	// if (!(file1=fopen(argv[4],"rb"))) {
	// 	cout << "Cannot open file: " << argv[4] <<endl;
	// 	exit(1);
	// }
	fread(Imagedatao, sizeof(unsigned char), height_size*width_size*BytesPerPixel, file1);
	fclose(file1);

	if (!(file=fopen(argv[5],"wb"))) {
		cout << "Cannot open file: " << argv[5] << endl;
		exit(1);
	}
	fwrite(Imagedatao, sizeof(unsigned char), 512*512*1, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//Geometric Modification 

	/*

	Geometric Modification - manipulation of pixel data
	
	How to do forward and reverse mapping to get the geometric transformation of input image

	Linear transformation ie linear mapping between the input image and desired output image
	1. Scaling - linear multiplication using horizontal and vertical components
	2. Rotation - using sin and cos, calculating angle
	3. Translation - tx and ty

	Order of these operations doesn't matter as it is linear, can change order of implementation of all three

	a. Use reverse address mapping because if use forward, ie multiply with input vector we do not get whole image
		say in case of scaling up, there are holes in output image
		hence, do reverse order - use a blank image as output image where fit in all the details pixel by pixel

	b. bilinear interpolation - fit in all the details pixel by pixel


	//Problem 1a - Geometric Transformation

	3 images for 3 holes to fill - realign them and rotate them to the correct position and translate them from one position to place in original image
	a. scan the input image line by line to find the location of holes 
	b. find 4 co-ordinates of pixel locations for the holes - find the corners, rectangular shape --> calculate width, height and angle of the image and find the central point
	c. apple reverse mapping trick - multiply all the 3 matrices of processes 
		from output pixel value location go to corresponding input pixel location and carry the value and transfer it to output image, however
		sometimes the values same around it, so take averaging at the output pixel location 

	patch image is given with tilted angles and rectangular shape given - so first locate 4 corners, scan it from left to right and define some patterns to locate the corners
	top and bottom corners have different patterns as compared to right and left corners
	
	make use of geometric property of images
	for matrix multiplication use tools like python numpy package or MATLAB
	matrix multiplication is used a lot in deep learning - TensorFlow, Keras, PyTorch

	*/

	int i,j,k;

	//finding the coordinates of corners in each sub-image lighthouse1~3.raw

	unsigned char Imagedata_temp[height_size1][width_size1][BytesPerPixel];

	//perform reverse address mapping converting (x,y) to (u,v)

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
	//house
	//row scanning
	int top1x;
	int top1y;
	for(i = 0; i< height_size1; i++)
	{
		for(j = 0; j<width_size1;j++)
		{
			if(Imagedata1[i][j][k]!=255)
			{
				top1x = i;
				top1y = j;

			}
		}
	}

	int bottom1x;
	int bottom1y;
	for(i = height_size1; i>=0; i--)
	{
		for(j = 0; j<width_size1;j++)
		{
			if(Imagedata1[i][j][k]!=255)
			{
				bottom1x = i;
				bottom1y = j;

			}
		}
	}

	//column scanning
	int left1x;
	int left1y;
	for(j = 0;j<width_size1;j++)
	{
		for(i = 0; i<height_size1; i++)
		{
			if(Imagedata1[i][j][k]!=255)
			{
				left1x = i;
				left1y = j;
			}
		}
	}

	int right1x;
	int right1y;
	for(j = width_size1;j>0;j--)
	{
		for(i = 0; i<height_size1; i++)
		{
			if(Imagedata1[i][j][k]!=255)
			{
				right1x = i;
				right1y = j;
			}
		}
	}




	// lighthouse2

	//row scanning
	int top2x;
	int top2y;
	for(i = 0; i< height_size1; i++)
	{
		for(j = 0; j<width_size1;j++)
		{
			if(Imagedata2[i][j][k]!=255)
			{
				top2x = i;
				top2y = j;

			}
		}
	}

	int bottom2x;
	int bottom2y;
	for(i = height_size1; i>=0; i--)
	{
		for(j = 0; j<width_size1;j++)
		{
			if(Imagedata2[i][j][k]!=255)
			{
				bottom2x = i;
				bottom2y = j;

			}
		}
	}

	//column scanning
	int left2x;
	int left2y;
	for(j = 0;j<width_size1;j++)
	{
		for(i = 0; i<height_size1; i++)
		{
			if(Imagedata2[i][j][k]!=255)
			{
				left2x = i;
				left2y = j;
			}
		}
	}

	int right2x;
	int right2y;
	for(j = width_size1;j>0;j--)
	{
		for(i = 0; i<height_size1; i++)
		{
			if(Imagedata2[i][j][k]!=255)
			{
				right2x = i;
				right2y = j;
			}
		}
	}

	//rocks
	//row scanning
	int top3x;
	int top3y;
	for(i = 0; i< height_size1; i++)
	{
		for(j = 0; j<width_size1;j++)
		{
			if(Imagedata3[i][j][k]!=255)
			{
				top3x = i;
				top3y = j;

			}
		}
	}

	int bottom3x;
	int bottom3y;
	for(i = height_size1; i>=0; i--)
	{
		for(j = 0; j<width_size1;j++)
		{
			if(Imagedata3[i][j][k]!=255)
			{
				bottom3x = i;
				bottom3y = j;

			}
		}
	}

	//column scanning
	int left3x;
	int left3y;
	for(j = 0;j<width_size1;j++)
	{
		for(i = 0; i<height_size1; i++)
		{
			if(Imagedata3[i][j][k]!=255)
			{
				left3x = i;
				left3y = j;
			}
		}
	}

	int right3x;
	int right3y;
	for(j = width_size1;j>0;j--)
	{
		for(i = 0; i<height_size1; i++)
		{
			if(Imagedata3[i][j][k]!=255)
			{
				right3x = i;
				right3y = j;
			}
		}
	}




	//we are not doing any geometric modification on the pixel value itself, we are calculating the pixel position values
	//using inverse matrix calculation

	float ratioXa = 0.687;
	float ratioYa = 0.687;

	unsigned char Imagedata_la[height_size1][width_size1][BytesPerPixel];

	//scaling for lighthouse1
	for(i=0; i<height_size; i++)
    {
        for(j=0; j<width_size; j++)
        {
			for (k = 0; k < BytesPerPixel; k++)
			{
				int u1;
				int v1;
				u1 = (int)i*ratioXa;
				v1 = (int)j*ratioYa;
				Imagedata_la[u1][v1][k] = Imagedata1[i][j][k];	
			}
		}
	}

	//For scaling-up, implement the bilinear interpolation function, f(x,y) is of size 160 x 160, drop redundant pixels when scaling-down.

	int roundedMappedPixelHeight,roundedMappedPixelWidth;
	unsigned char Pixela[height_size1][width_size1];
	unsigned char Imagedata_resizeda[height_size1][width_size1];

	for(i=0;i<height_size;i++)
    {
        float mappedPixelHeight = (110.0/160.0) * i;  // resizing ratio - scaling
        roundedMappedPixelHeight= (int)(mappedPixelHeight);   //to floor the float value to the nearest smaller integer value
        
        if (roundedMappedPixelHeight>=height_size1-1)  // to check if the image does not exceed the size 512
        {
            roundedMappedPixelHeight=roundedMappedPixelHeight-1;
        }
        
        //Find the difference between the float value and floored integer value
        float delx=mappedPixelHeight-roundedMappedPixelHeight;
        
        for(j=0;j<width_size;j++)
        {
            float mappedPixelWidth = (110.0/160.0) * j;
            roundedMappedPixelWidth=(int)(mappedPixelWidth);
            
            if (roundedMappedPixelHeight>=width_size1-1)
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

	float ratioXb = 0.906;
	float ratioYb = 0.906;

	unsigned char Imagedata_lb[height_size1][width_size1][BytesPerPixel];

	//scaling for lighthouse2
	for(i=0; i<height_size; i++)
    {
        for(j=0; j<width_size; j++)
        {
			for (k = 0; k < BytesPerPixel; k++)
			{
				int u1;
				int v1;
				u1 = (int)i*ratioXb;
				v1 = (int)j*ratioYb;
				Imagedata_lb[u1][v1][k] = Imagedata2[i][j][k];	
			}
		}
	}

	//For scaling-up, implement the bilinear interpolation function, f(x,y) is of size 160 x 160, drop redundant pixels when scaling-down.

	unsigned char Pixelb[height_size1][width_size1];
	unsigned char Imagedata_resizedb[height_size1][width_size1];

	for(i=0;i<height_size;i++)
    {
        float mappedPixelHeight = (145.0/160.0) * i;  // resizing ratio - scaling
        roundedMappedPixelHeight= (int)(mappedPixelHeight);   //to floor the float value to the nearest smaller integer value
        
        if (roundedMappedPixelHeight>=height_size1-1)  // to check if the image does not exceed the size 512
        {
            roundedMappedPixelHeight=roundedMappedPixelHeight-1;
        }
        
        //Find the difference between the float value and floored integer value
        float delx=mappedPixelHeight-roundedMappedPixelHeight;
        
        for(j=0;j<width_size;j++)
        {
            float mappedPixelWidth = (145.0/160.0) * j;
            roundedMappedPixelWidth=(int)(mappedPixelWidth);
            
            if (roundedMappedPixelHeight>=width_size1-1)
            {
                roundedMappedPixelHeight=roundedMappedPixelHeight-1;
            }
            
            float dely=mappedPixelWidth-roundedMappedPixelWidth;
            
            //Calculating resized image intensity values using bilinear interpolation
            Imagedata_resizedb[i][j]= (unsigned char)((1-delx)*(1-dely)*Pixelb[roundedMappedPixelHeight][roundedMappedPixelWidth]
				+(delx)*(1-dely)*Pixelb[roundedMappedPixelHeight+1][roundedMappedPixelWidth]+
				(1-delx)*(dely)*Pixelb[roundedMappedPixelHeight][roundedMappedPixelWidth+1]+
            	(delx)*(dely)*Pixelb[roundedMappedPixelHeight+1][roundedMappedPixelWidth+1]);
		}

	}

	//to make the rocks image smaller
	float ratioXc = 1.325;
	float ratioYc = 1.325;

	unsigned char Imagedata_lc[height_size1][width_size1][BytesPerPixel];

	//scaling for lighthouse3
	for(i=0; i<height_size; i++)
    {
        for(j=0; j<width_size; j++)
        {
			for (k = 0; k < BytesPerPixel; k++)
			{
				int u1;
				int v1;
				u1 = (int)i*ratioXc;
				v1 = (int)j*ratioYc;
				Imagedata_lc[u1][v1][k] = Imagedata3[i][j][k];	
			}
		}
	}

	//For scaling-up, implement the bilinear interpolation function, f(x,y) is of size 160 x 160, drop redundant pixels when scaling-down.

	unsigned char Pixelc[height_size1][width_size1];
	unsigned char Imagedata_resizedc[height_size1][width_size1];

	for(i=0;i<height_size;i++)
    {
        float mappedPixelHeight = (212.0/160.0) * i;  // resizing ratio - scaling
        roundedMappedPixelHeight= (int)(mappedPixelHeight);   //to floor the float value to the nearest smaller integer value
        
        if (roundedMappedPixelHeight>=height_size1-1)  // to check if the image does not exceed the size 512
        {
            roundedMappedPixelHeight=roundedMappedPixelHeight-1;
        }
        
        //Find the difference between the float value and floored integer value
        float delx=mappedPixelHeight-roundedMappedPixelHeight;
        
        for(j=0;j<width_size;j++)
        {
            float mappedPixelWidth = (212.0/160.0) * j;
            roundedMappedPixelWidth=(int)(mappedPixelWidth);
            
            if (roundedMappedPixelHeight>=width_size1-1)
            {
                roundedMappedPixelHeight=roundedMappedPixelHeight-1;
            }
            
            float dely=mappedPixelWidth-roundedMappedPixelWidth;
            
            //Calculating resized image intensity values using bilinear interpolation
            Imagedata_resizedc[i][j]= (unsigned char)((1-delx)*(1-dely)*Pixelc[roundedMappedPixelHeight][roundedMappedPixelWidth]
				+(delx)*(1-dely)*Pixelc[roundedMappedPixelHeight+1][roundedMappedPixelWidth]+
				(1-delx)*(dely)*Pixelc[roundedMappedPixelHeight][roundedMappedPixelWidth+1]+
            	(delx)*(dely)*Pixelc[roundedMappedPixelHeight+1][roundedMappedPixelWidth+1]);
		}

	}

	//u and v values may not be integer values and hence we use bilinear interpolation


	//rotation for lighthouse1

	float theta_a = 70.854;
	unsigned char Imagedata_rotateda[height_size1][width_size1][BytesPerPixel];

	for(int i = 0; i<height_size1; i++)
	{
		for(int j = 0; j<width_size1; j++)
		{
			int u1;
			int v1;
			u1 = i*cos(theta_a) + j*sin(theta_a);
			v1 = j*cos(theta_a) - i*sin(theta_a);
			Imagedata_rotateda[u1][v1][k] = Imagedata1[i][j][k];
			
		}
	}

	//rotation for lighthouse2

	float theta_b = 130.423;
	unsigned char Imagedata_rotatedb[height_size1][width_size1][BytesPerPixel];

	for(int i = 0; i<height_size1; i++)
	{
		for(int j = 0; j<width_size1; j++)
		{
			int u1;
			int v1;
			u1 = i*cos(theta_b) + j*sin(theta_b);
			v1 = j*cos(theta_b) - i*sin(theta_b);
			Imagedata_rotatedb[u1][v1][k] = Imagedata2[i][j][k];
			
		}
	}

	//rotation for lighthouse3

	float theta_c = 350.054;
	unsigned char Imagedata_rotatedc[height_size1][width_size1][BytesPerPixel];

	for(int i = 0; i<height_size1; i++)
	{
		for(int j = 0; j<width_size1; j++)
		{
			int u1;
			int v1;
			u1 = i*cos(theta_c) + j*sin(theta_c);
			v1 = j*cos(theta_c) - i*sin(theta_c);
			Imagedata_rotatedc[u1][v1][k] = Imagedata3[i][j][k];
			
		}
	}

/*
	//finding the coordinates of the holes (specifically the top-left corners) of lighthouse.raw input image
	//Harris corner detection

	// Initialize all local variables
    int imageWidth = width_size;
    int imageHeight = height_size;
    map<int,vector<int> > corners;
    matrix<int,double> sobelOperator_x(3,3,1);
    sobelOperator_x.setMatrixByValues(9,-1.0,0.0,1.0,-2.0,0.0,2.0,-1.0,0.0,1.0);
    matrix<int,double> sobelOperator_y(3,3,1);
    sobelOperator_y = sobelOperator_x.transpose();
    int extendBy = 1;
    int windowSize = 3;
    int i = 0;
    map<int,vector<double> > pointsOfInterest;

    // Prepare image
    imageData extendedImage = imageObject->extendImage(extendBy);


    // Apply sobel operator
    matrix<int,double> imageGradient_x = filterApply(extendedImage,sobelOperator_x,"convolutionWithAbsolute");
    matrix<int,double> imageGradient_y = filterApply(extendedImage,sobelOperator_y,"convolutionWithAbsolute");

    // Run harris corner detection algorithm
    matrix<int,double> RMatrix(imageHeight-1,imageWidth-1,1);
    matrix<int,double> MMatrix(2,2,1);
    double RValue = 0.0;
    double value_xx = 0, value_yy = 0,value_xy = 0;

    for(int rowIndex = 1;rowIndex < imageHeight-1; rowIndex++) {
        for (int columnIndex = 1; columnIndex < imageWidth-1; columnIndex++) {

            value_xx = 0.0;
            value_xy = 0.0;
            value_yy = 0.0;

            for (int windowRow = -1; windowRow < 2; windowRow++) {
                for (int windowColumn = -1; windowColumn < 2; windowColumn++) {

                    value_xx += imageGradient_x.getMatrixValues(rowIndex+windowRow,columnIndex+windowColumn,0) * imageGradient_x.getMatrixValues(rowIndex+windowRow,columnIndex+windowColumn,0);
                    value_xy += imageGradient_y.getMatrixValues(rowIndex+windowRow,columnIndex+windowColumn,0) * imageGradient_x.getMatrixValues(rowIndex+windowRow,columnIndex+windowColumn,0);
                    value_yy += imageGradient_y.getMatrixValues(rowIndex+windowRow,columnIndex+windowColumn,0) * imageGradient_y.getMatrixValues(rowIndex+windowRow,columnIndex+windowColumn,0);

                }
            }

            // Setting up MMatrix and calculating R value
            MMatrix.setMatrixByValues(4,value_xx,value_xy,value_xy,value_yy);
            RValue = MMatrix.determinant(MMatrix) - 0.04*pow(MMatrix.trace(MMatrix),2.0);

            // Thresholding R value
            if(RValue > RThreshold){

				//cout << rowIndex <<" " << columnIndex<< " " << RValue <<endl;
                pointsOfInterest[i].push_back(rowIndex);
                pointsOfInterest[i].push_back(columnIndex);
                pointsOfInterest[i].push_back(RValue);
                pointsOfInterest[i].push_back(0.0);
                i++;
            }

        }
    }
*/


	//filling into the holes to get the final image, assuming the lighthouse1 goes to left hole, lighthouse2 goes up and lighthouse3 goes to bottom right

	//translation for lighthouse1

	float transXa = 157;               //left top corner points of the white hole
	float transYa = 62;
	unsigned char Imagedata_translatea[height_size1][width_size1][BytesPerPixel];

	for(int i = 0; i<height_size1; i++)
	{
		for(int j = 0; j<width_size1; j++)
		{
			int u1;
			int v1;
			u1 = i + transXa;
			v1 = j + transYa;
			Imagedata_translatea[u1][v1][k] = Imagedata1[i][j][k];
			
		}
	}

	//translation for lighthouse2

	float transXb = 31;
	float transYb = 278;
	unsigned char Imagedata_translateb[height_size1][width_size1][BytesPerPixel];

	for(int i = 0; i<height_size1; i++)
	{
		for(int j = 0; j<width_size1; j++)
		{
			int u1;
			int v1;
			u1 = i + transXb;
			v1 = j + transYb;
			Imagedata_translateb[u1][v1][k] = Imagedata2[i][j][k];
			
		}
	}

	//translation for lighthouse3

	float transXc = 328;
	float transYc = 326;
	unsigned char Imagedata_translatec[height_size1][width_size1][BytesPerPixel];

	for(int i = 0; i<height_size1; i++)
	{
		for(int j = 0; j<width_size1; j++)
		{
			int u1;
			int v1;
			u1 = i + transXc;
			v1 = j + transYc;
			Imagedata_translatec[u1][v1][k] = Imagedata3[i][j][k];
			
		}
	}

	//med filter

	unsigned char Imagedata_denoised_median[height_size1][width_size1][BytesPerPixel];

	int window_size = 3;
	//for the mask
    signed int initial_N = - floor(window_size/2.0);
	signed int end_N = floor(window_size/2.0);
	signed int m,l;

	signed int height_i;    //row index
	signed int width_i;     //column index
	unsigned char Image_temp1;
    int middle_value = ceil((window_size*window_size)/2);
    
    for (i = 0; i < height_size; i ++) 
	{
        for (j = 0; j < width_size; j++) 
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
                    if(height_i > height_size-1) 
					{
                        height_i = height_size-1;
                    }
                    if(width_i > width_size-1) 
					{
                        width_i = width_size-1;
                    }

                    Image_temp1 = Imagedata[height_i][width_i][BytesPerPixel];
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




	// Write image data (filename specified by second argument) from image data matrix

	// if (!(file=fopen(argv[5],"wb"))) {
	// 	cout << "Cannot open file: " << argv[5] << endl;
	// 	exit(1);
	// }
	fwrite(Imagedata_output, sizeof(unsigned char), 512*512*1, file);
	fclose(file);

	return 0;
}


/*
to observe output on ImageJ, 
Use: image type - 8-bit 
width - 512
height - 512
*/

