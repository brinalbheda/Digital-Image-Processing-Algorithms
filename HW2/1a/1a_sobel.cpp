// This sample code reads in image data from a RAW image file and
// writes it into another file

// NOTE:    The code assumes that the tiger and pig input images are of size 481 x 321
//            and are in the RAW format. You will need to make corresponding changes to
//            accommodate images of different sizes and/or types

//EE 569: Homework 2 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 02/12/2019

/*
 Algorithm implemented in C++:
 • Read the input image “tiger.raw” and “pig.raw” whose dimensions are height_size, width_size, BytesPerPixel
 • Convert the RGB color image into a gray scale image by using the luminosity formula mentioned above
 • Apply the Gx gradient and Gy gradient masks on each pixel value and find the magnitude of the image by using the magnitude formula shown above using Gx and Gy
 • Since the values obtained for the magnitude, Gx gradient and Gy gradient are very high, we normalize the value in the range of 0 to 255. Thus, the minimum and maximum values for the gradient are calculated
 • Normalization is done using the formula:
 Normalized image = (input image – minimum value) / (maximum value – minimum value)
 • Find the Cumulative Distribution function (cdf) of the magnitude with increasing order of intensities
 • Take the threshold as 90% of the edge value and 10% of the pixels in the cdf plot.
 • The intensity value corresponding to the pixel threshold is calculated:
 Output image = 0 if input pixel value > threshold
 = 255 if input pixel value < threshold
 • Combine the thresholded output image
 • Write the computed image data array on output.raw file using the fwrite() function
 */

// To run the code, open terminal, add path and run using following commands
// g++ -o 1a 1a_sobel.cpp

// ./1a tiger.raw output_1a_tiger_gray.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_gx.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_gy.raw 3 481 321
// ./1a pig.raw output_1a_pig_gray.raw 3 481 321
// ./1a pig.raw output_1a_pig_gx.raw 3 481 321

// ./1a tiger.raw output_1a_tiger_threshold_60.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_threshold_70.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_threshold_80.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_threshold_85.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_threshold_90.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_threshold_95.raw 3 481 321
// ./1a tiger.raw output_1a_tiger_threshold_98.raw 3 481 321
// ./1a pig.raw output_1a_pig_threshold_60.raw 3 481 321
// ./1a pig.raw output_1a_pig_threshold_70.raw 3 481 321
// ./1a pig.raw output_1a_pig_threshold_80.raw 3 481 321
// ./1a pig.raw output_1a_pig_threshold_85.raw 3 481 321
// ./1a pig.raw output_1a_pig_threshold_90.raw 3 481 321
// ./1a pig.raw output_1a_pig_threshold_95.raw 3 481 321
// ./1a pig.raw output_1a_pig_threshold_98.raw 3 481 321


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;


void get2Dchannel(unsigned char* Imagedata,int imageHeight, int imageWidth,unsigned char **rpixel2D, int iterationIndex)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rpixel2D[rowIndex][colIndex]=Imagedata[iterationIndex];
            iterationIndex=iterationIndex+3;
        }
    }
}

void getRGBtoGrayscale(unsigned char** rpixel2D,unsigned char** gpixel2D,unsigned char** bpixel2D,unsigned char** luminosityArray,int imageHeight,int imageWidth)
{
    int rowIndex=0,colIndex=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            luminosityArray[rowIndex][colIndex]= (unsigned char) (0.21*rpixel2D[rowIndex][colIndex]+0.72*gpixel2D[rowIndex][colIndex]+0.07*bpixel2D[rowIndex][colIndex]);
        }
    }
}

void getGradient(unsigned char** luminosityArray,int imageHeight,int imageWidth,int** magnitude,int** gxArray,int** gyArray)
{
    int i,j;
    int n,x,p,q,gx=0,gy=0;
    int N=3;
    int sorted[N*N],gxValue[N*N],gyValue[N*N];
    int maskArraygx[9]={-1,0,1,-2,0,2,-1,0,1};  // Vertical Edge Detector mask
    int maskArraygy[9]={1,2,1,0,0,0,-1,-2,-1};  // Horizontal Edge Detector mask
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<N*N;x++)
            {
                sorted[x]=0;
                gxValue[x]=0;
                gyValue[x]=0;
            }
            int k=0;
            for(p=i-(N-1)/2;p<=i+(N-1)/2;p++)
            {
                for(q=j-(N-1)/2;q<=j+(N-1)/2;q++)
                {
                    int rowIndex=p;
                    int colIndex=q;
                    if(rowIndex<0)
                    {
                        rowIndex=0;
                    }
                    if(rowIndex>imageHeight-1)
                    {
                        rowIndex=imageHeight-1;
                    }
                    if(colIndex<0)
                    {
                        colIndex=0;
                    }
                    if(colIndex>imageWidth-1)
                    {
                        colIndex=imageWidth-1;
                    }
                    sorted[k]=(int)luminosityArray[rowIndex][colIndex];
                    k++;
                }
            }
            gx=0;gy=0;
            for(n=0;n<N*N;n++)
            {
                gxValue[n]=maskArraygx[n]*sorted[n];
                gyValue[n]=maskArraygy[n]*sorted[n];
                gx=gx+gxValue[n];
                gy=gy+gyValue[n];
            }
            magnitude[i][j]=sqrt(gx*gx+gy*gy);
            gxArray[i][j]=gx;
            gyArray[i][j]=gy;
        }
    }
}

void getnormalization(int** magnitude, int imageHeight,int imageWidth,int** normalizedArray)
{
    int rowIndex,colIndex;
    int max=magnitude[0][0];
    int min=magnitude[0][0];
   
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(max<magnitude[rowIndex][colIndex])
            {
                max=magnitude[rowIndex][colIndex];
            }
            if(min>magnitude[rowIndex][colIndex])
            {
                min=magnitude[rowIndex][colIndex];
            }
        }
    }
    //Normalizing the values between 0 and 255
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            normalizedArray[rowIndex][colIndex]=(int)(255*((double)(magnitude[rowIndex][colIndex]-min)/(double)(max-min)));
        }
    }
}

void getcdf(int** normalizedArray,int imageHeight,int imageWidth,int* normalizedArrayHeight,int* normalizedArrayWidth,int* intensity)
{
    int k,m=0,j=0,rowIndex,colIndex;
    for(k=0;k<=255;k++)
    {
        for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if (normalizedArray[rowIndex][colIndex]==k)
                {
                    normalizedArrayHeight[m]=rowIndex;
                    normalizedArrayWidth[j]=colIndex;
                    intensity[k]=intensity[k]+1;
                    j++;
                    m++;
                }
            }
        }
    }
}

void getthreshold(int** normalizedArray,int* normalizedArrayHeight,int* normalizedArrayWidth,int imageHeight,int imageWidth,int threshold,unsigned char** thresholdedimage)
{
    int rowIndex,colIndex;
    int thresholdImageValue=0;
    int l=0;
    //Checking for intensity threshold value corresponding to 90% number of pixels
    for(int i=0;i<imageHeight;i++)
    {
        for(int j=0;j<imageWidth;j++)
        {
            if(l==threshold)
            {
                thresholdImageValue=normalizedArray[normalizedArrayHeight[l]][normalizedArrayWidth[l]];
            }
            l++;
        }
    }
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(normalizedArray[rowIndex][colIndex]<thresholdImageValue)
                thresholdedimage[rowIndex][colIndex]=(unsigned char)255;
            else if(normalizedArray[rowIndex][colIndex]>thresholdImageValue)
                thresholdedimage[rowIndex][colIndex]=(unsigned char)0;
        }
    }
}

void getcombinechannels(unsigned char** thresholdedimage,unsigned char* combinedArray,int imageHeight,int imageWidth)
{
    int rowIndex,colIndex,count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            combinedArray[count]=thresholdedimage[rowIndex][colIndex];
            count++;
        }
    }
}

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=321;
    int imageWidth=481;
    string fileName;
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerPixel = 1] [Height=255] [Width=255]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 4){
        bytesPerPixel = 1; // default is grey image
    }
    else {
        bytesPerPixel = atoi(argv[3]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 5){
            imageWidth= atoi(argv[4]);
            imageHeight= atoi(argv[5]);
            //fileName=argv[6];
        }
    }
    
    // Allocate image data array
    int imageDimension=imageHeight*imageWidth*bytesPerPixel;
    unsigned char Imagedata[imageDimension];
    unsigned char combinedArray[imageDimension];
    unsigned char combinedArrayGX[imageDimension];
    unsigned char combinedArrayGY[imageDimension];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    
    //Declarations for all the arrays
    unsigned char** rpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D=rpixel2Ddummy1;
    
    unsigned char** gpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        gpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(gpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** gpixel2D=gpixel2Ddummy1;
    
    unsigned char** bpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** bpixel2D=bpixel2Ddummy1;
    
    unsigned char** bpixel2Ddummy12=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy12[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy12[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** luminosityArray=bpixel2Ddummy12;
    
    int** bpixel2Ddummy121=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy121[i]=new int[imageWidth];
        memset(bpixel2Ddummy121[i],0,imageWidth*sizeof(int));
    }
    int** magnitude=bpixel2Ddummy121;
    
    int** bpixel2Ddummy11=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy11[i]=new int[imageWidth];
        memset(bpixel2Ddummy11[i],0,imageWidth*sizeof(int));
    }
    int** normalizedArray=bpixel2Ddummy11;
    
    int** bpixel2Ddummy112=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy112[i]=new int[imageWidth];
        memset(bpixel2Ddummy112[i],0,imageWidth*sizeof(int));
    }
    int** gxArray=bpixel2Ddummy112;
    
    int** bpixel2Ddummy115=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy115[i]=new int[imageWidth];
        memset(bpixel2Ddummy115[i],0,imageWidth*sizeof(int));
    }
    int** gyArray=bpixel2Ddummy115;
    
    int** bpixel2Ddummy116=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy116[i]=new int[imageWidth];
        memset(bpixel2Ddummy116[i],0,imageWidth*sizeof(int));
    }
    int** gxArrayNormalized=bpixel2Ddummy116;
    
    int** bpixel2Ddummy117=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy117[i]=new int[imageWidth];
        memset(bpixel2Ddummy117[i],0,imageWidth*sizeof(int));
    }
    int** gyArrayNormalized=bpixel2Ddummy117;
    
    int* normalizedArrayHeight=new int[imageHeight*imageWidth];
    memset(normalizedArrayHeight,0,imageHeight*imageWidth*sizeof(int));
    
    int* normalizedArrayWidth=new int[imageWidth*imageHeight];
    memset(normalizedArrayWidth,0,imageWidth*imageHeight*sizeof(int));
    
    unsigned char** bpixel2Ddummy119=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy119[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy119[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** thresholdedImage=bpixel2Ddummy119;
    
    //Seperating R,G,B channels from the ipout image
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D,0);
    get2Dchannel(Imagedata,imageHeight,imageWidth,gpixel2D,1);
    get2Dchannel(Imagedata,imageHeight,imageWidth,bpixel2D,2);
    
    //Convert R,G,B image to gray scale image using luminosity method-- Gray value= 0.21R + 0.72G + 0.07B
    getRGBtoGrayscale(rpixel2D,gpixel2D,bpixel2D,luminosityArray,imageHeight,imageWidth);
    
    //Use the Sobel Filter
    getGradient(luminosityArray,imageHeight,imageWidth,magnitude,gxArray,gyArray);
    
    //Normalize the values in the range 0 to 255
    getnormalization(magnitude,imageHeight,imageWidth,normalizedArray);
    getnormalization(gxArray,imageHeight,imageWidth,gxArrayNormalized);
    getnormalization(gyArray,imageHeight,imageWidth,gyArrayNormalized);
    
    int rowIndex,colIndex,count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            combinedArrayGX[count]=gxArrayNormalized[rowIndex][colIndex];
            count++;
        }
    }
    count=0;;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            combinedArrayGY[count]=gyArrayNormalized[rowIndex][colIndex];
            count++;
        }
    }
    
    int intensity[256]={0};
    //Get the cdf of the normalized intensity values
    getcdf(normalizedArray,imageHeight,imageWidth,normalizedArrayHeight,normalizedArrayWidth,intensity);
    
    int intensityfinal[256]={0};
    intensityfinal[0]=intensity[0];
    
    for(int i=0;i<256;i++)
    {
        if(i>0)
        {
            intensityfinal[i]=intensityfinal[i-1]+intensity[i];
        }
    }
    
    int threshold=(0.98)*(imageHeight*imageWidth);  //set the threshold so that top 10% pixels are considered as an edge
    
    //threshold the sobel filtered image to get a binary image indicating presence or absence of edge
    getthreshold(normalizedArray,normalizedArrayHeight,normalizedArrayWidth,imageHeight,imageWidth,threshold,thresholdedImage);
    
    //Combining the R,G,B channels of output square and disc images into a single image
    getcombinechannels(thresholdedImage,combinedArray,imageHeight,imageWidth);
  /*
    //To write contents into a file used to obtain histogram
    FILE *histFile;
    histFile=fopen(fileName.c_str(),"w");
    
    if(histFile!=nullptr)
    {
        for(int i=0;i<=255;i++)
        {
            fprintf(histFile,"%d\t%d\n",i,intensityfinal[i]);
        }
    }
    fclose(histFile);
 */
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(combinedArrayGX, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(combinedArrayGY, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(luminosityArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}


/*
 to observe output on ImageJ,
 Use: image type - 8-bit
 width - 481
 height - 321
 */

