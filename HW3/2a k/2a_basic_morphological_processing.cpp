// This sample code reads in image data from a RAW image file and
// writes it into another file

// NOTE:    The code assumes that the pattern 1~4 input images are of size 375 x 375
//            and are in the RAW format. You will need to make corresponding changes to
//            accommodate images of different sizes and/or types

//EE 569: Homework 3 - By Brinal Bheda
//USC ID - 8470655102
//Email - bbheda@usc.edu
//Submission Date - 03/03/2019

/*
 Shrinking:-
 Algorithm implemented in C++:
 • Read the input image “pattern1~4.raw” whose dimensions are height_size = 375, width_size = 375, BytesPerPixel = 1
 • Convert the grayscale image into binarized image by using fixed thresholding such that
 If input pixel value <=127, set output value to 0; else 255
 • Apply the shrinking algorithm on the binarized input image
 • The 3x3 mask is converted into a 9-array elements which is converted into a string for making the calculation easy
 • Compute the Stage 1 steps:
 o Initialize the intermediate matrix with all zeros
 o Using two for loops, transverse through the whole image using the 3x3 mask
 o Find every single pixel if it is 0 or 1
 o If it is 1, calculate the bond value using the formula mentioned above and compare the 3x3 neighbor of that pixel with all the bond’s corresponding conditional masks
 o If it is 0, copy the same value and don’t change it
 o If it is a hit, put 1 in the intermediate matrix
 o If it is a miss, put 0 in the intermediate matrix
 • Compute the Stage 2 steps:
 o We have the input matrix and the intermediate matrix
 o Using two for loops, transverse through the whole image using the 3x3 mask
 o Find every single pixel if it is 0 or 1
 o If it is 1, compare the 3x3 neighbor of that pixel with all the conditional masks
 o If it is 0, copy the same value to the output and don’t change it
 o If it is a hit, copy input to output
 o If it is a miss, put 0 in the output pixel value
 • Perform stage 1 and stage 2 for many iterations until get the desired output
 • Check every element of the 3x3 neighbor with every element of the mask, such that if one element doesn’t match, break and move on to the next one
 • Iterate through all the masks similarly and if one mask completely matches, break and get the hit or miss output
 • The desired output is obtained when there is no more change to the image for another iteration and it saturates
 • Write the computed image data array on output.raw file using the fwrite() function
 
 Thinning:-
 Algorithm implemented in C++:
 • Read the input image “pattern1~4.raw” whose dimensions are height_size = 375, width_size = 375, BytesPerPixel = 1
 • Convert the grayscale image into binarized image by using fixed thresholding such that
 If input pixel value <=127, set output value to 0; else 255
 • Apply the shrinking algorithm on the binarized input image
 • The 3x3 mask is converted into a 9-array elements which is converted into a string for making the calculation easy
 • Compute the Stage 1 steps:
 o Initialize the intermediate matrix with all zeros
 o Using two for loops, transverse through the whole image using the 3x3 mask
 o Find every single pixel if it is 0 or 1
 o If it is 1, calculate the bond value using the formula mentioned above and compare the 3x3 neighbor of that pixel with all the bond’s corresponding conditional masks
 o If it is 0, copy the same value and don’t change it
 o If it is a hit, put 1 in the intermediate matrix
 o If it is a miss, put 0 in the intermediate matrix
 • Compute the Stage 2 steps:
 o We have the input matrix and the intermediate matrix
 o Using two for loops, transverse through the whole image using the 3x3 mask
 o Find every single pixel if it is 0 or 1
 o If it is 1, compare the 3x3 neighbor of that pixel with all the conditional masks
 o If it is 0, copy the same value to the output and don’t change it
 o If it is a hit, copy input to output
 o If it is a miss, put 0 in the output pixel value
 • Perform stage 1 and stage 2 for many iterations until get the desired output
 • Check every element of the 3x3 neighbor with every element of the mask, such that if one element doesn’t match, break and move on to the next one
 • Iterate through all the masks similarly and if one mask completely matches, break and get the hit or miss output
 • The desired output is obtained when there is no more change to the image for another iteration and it saturates
 • Perform bridging to join the disjoint lines in output
 • Using two for loops, transverse through the whole final iteration image using the 3x3 mask
 • Get X, X0, X1, X2, X3, X4, X5, X6, X7
 • X is the center pixel and all others are surrounding neighbor pixels
 • Calculate L1, L2, L3, L4, P1, P2, P3, P4, P5 and P6 using the Logical Operations
 • Calculate the pixel intensity value using the above variables
 • Get the final bridged image
 • Write the computed image data array on output.raw file using the fwrite() function
 
 Skeletonizing:-
 Algorithm implemented in C++:
 • Read the input image “pattern1~4.raw” whose dimensions are height_size = 375, width_size = 375, BytesPerPixel = 1
 • Convert the grayscale image into binarized image by using fixed thresholding such that
 If input pixel value <=127, set output value to 0; else 255
 • Apply the shrinking algorithm on the binarized input image
 • The 3x3 mask is converted into a 9-array elements which is converted into a string for making the calculation easy
 • Compute the Stage 1 steps:
 o Initialize the intermediate matrix with all zeros
 o Using two for loops, transverse through the whole image using the 3x3 mask
 o Find every single pixel if it is 0 or 1
 o If it is 1, calculate the bond value using the formula mentioned above and compare the 3x3 neighbor of that pixel with all the bond’s corresponding conditional masks
 o If it is 0, copy the same value and don’t change it
 o If it is a hit, put 1 in the intermediate matrix
 o If it is a miss, put 0 in the intermediate matrix
 • Compute the Stage 2 steps:
 o We have the input matrix and the intermediate matrix
 o Using two for loops, transverse through the whole image using the 3x3 mask
 o Find every single pixel if it is 0 or 1
 o If it is 1, compare the 3x3 neighbor of that pixel with all the conditional masks
 o If it is 0, copy the same value to the output and don’t change it
 o If it is a hit, copy input to output
 o If it is a miss, put 0 in the output pixel value
 • Perform stage 1 and stage 2 for many iterations until get the desired output
 • Check every element of the 3x3 neighbor with every element of the mask, such that if one element doesn’t match, break and move on to the next one
 • Iterate through all the masks similarly and if one mask completely matches, break and get the hit or miss output
 • The desired output is obtained when there is no more change to the image for another iteration and it saturates
 • Perform bridging to join the disjoint lines in output
 • Using two for loops, transverse through the whole final iteration image using the 3x3 mask
 • Get X, X0, X1, X2, X3, X4, X5, X6, X7
 • X is the center pixel and all others are surrounding neighbor pixels
 • Calculate L1, L2, L3, L4, P1, P2, P3, P4, P5 and P6 using the Logical Operations
 • Calculate the pixel intensity value using the above variables
 • Get the final bridged image
 • Write the computed image data array on output.raw file using the fwrite() function
 */

// To run the code, open terminal, add path and run using following commands
// g++ -o 2a 2a_basic_morphological_processing.cpp
// ./2a pattern1.raw output_2a_1s.raw 1
// ./2a pattern1.raw output_2a_1t.raw 1
// ./2a pattern1.raw output_2a_1k.raw 1
// ./2a pattern2.raw output_2a_2s.raw 1
// ./2a pattern2.raw output_2a_2t.raw 1
// ./2a pattern2.raw output_2a_2k.raw 1
// ./2a pattern3.raw output_2a_3s.raw 1
// ./2a pattern3.raw output_2a_3t.raw 1
// ./2a pattern3.raw output_2a_3k.raw 1
// ./2a pattern4.raw output_2a_4s.raw 1
// ./2a pattern4.raw output_2a_4t.raw 1
// ./2a pattern4.raw output_2a_4k.raw 1

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<string.h>

using namespace std;

void get2Dchannel(unsigned char* Imagedata,int imageHeight, int imageWidth,unsigned char**rpixel2D)
{
    int rowIndex,colIndex,traversalIndex=0;
    for (rowIndex = 0; rowIndex < imageHeight; rowIndex++)
    {
        for(colIndex = 0;colIndex < imageWidth; colIndex++)
        {
            rpixel2D[rowIndex][colIndex] = Imagedata[traversalIndex];
            traversalIndex++;
        }
    }
}

void getnormalized(unsigned char** rPixel2D,float **rPixel2Dfloat, int imageHeight, int imageWidth)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(rPixel2D[rowIndex][colIndex]<127)
            {
                rPixel2Dfloat[rowIndex][colIndex]=0;
            }
            else
            {
                rPixel2Dfloat[rowIndex][colIndex]=1;
            }
        }
    }
}

void getoriginalrangevalue(float** rPixelfloat,unsigned char **rPixel2Dnew, int imageHeight, int imageWidth)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rPixel2Dnew[rowIndex][colIndex]=rPixelfloat[rowIndex][colIndex]*255;
        }
    }
}

void getinvertedimage(float** rPixel2Dfloat, int imageHeight, int imageWidth,float** rpixelinvertedfloat)
{
    
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(rPixel2Dfloat[rowIndex][colIndex]==0)
            {
                rpixelinvertedfloat[rowIndex][colIndex]=0;
            }
            else
            {
                rpixelinvertedfloat[rowIndex][colIndex]=1;
            }
        }
    }
}

void combinechannel(unsigned char** rPixel2Dnew,unsigned char *combinedArray,int imageHeight, int imageWidth)
{
    int rowIndex,colIndex,traversalIndex=0;
    for (rowIndex = 0;rowIndex<imageHeight; rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth; colIndex++)
        {
            combinedArray[traversalIndex] = rPixel2Dnew[rowIndex][colIndex];
            traversalIndex++;
        }
    }
}

void getskeletonizing(float** rPixelfloat, int imageHeight, int imageWidth,float** M, float** routputArray)
{
    int iterationIndex,rowIndex,colIndex,p,q,traversalIndex=0,temp;
    int maskarray[9];
    //Defining the unconditional masks in the form of strings for ease of comparison later
    string bond4[8]={"010011000", "010110000","000110010", "000011010","001011001","111010000","100110100","000010111"};
    string
    bond6[8]={"111011000","011011001","111110000","110110100","100110110","000110111","000011111","001011011"};
    string bond7[4]={"111011001","111110100","100110111","001011111"};
    string bond8[4]={"000111111","011011011", "111111000", "110110110"};
    string bond9[8]={"111011011","011011111","111111100","111111001","111110110","110110111","100111111","001111111"};
    string bond10[4]={"111011111","111111101","111110111","101111111"};
    string bond11[4]={"111111011","111111110","110111111","011111111"};
    
    int bond=0;
    string str;
    //------Step 1----- detecting boundary to be peeled off in step 2
    for(iterationIndex=0;iterationIndex<125;iterationIndex++)
    {
        for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if (rPixelfloat[rowIndex][colIndex]==0)  //To check in center element is 0
                {
                    M[rowIndex][colIndex]=0;
                }
                else   //to start processing if the center element is 1
                {
                    for(int x=0;x<9;x++)
                    {
                        maskarray[x]=0;
                    }
                    traversalIndex=0;
                    str="\0";
                    for(p=rowIndex-1;p<=rowIndex+1;p++)
                    {
                        for(q=colIndex-1;q<=colIndex+1;q++)
                        {
                            int row=p;
                            int col=q;
                            if(row<0)
                            {
                                row=0;
                            }
                            if(row>imageHeight-1)
                            {
                                row=imageHeight-1;
                            }
                            if(col<0)
                            {
                                col=0;
                            }
                            if(col>imageWidth-1)
                            {
                                col=imageWidth-1;
                            }
                            maskarray[traversalIndex]=rPixelfloat[row][col]; //Reading contents of input image in array of 9 elements
                            traversalIndex++;
                        }
                    }
                    //Converting 9 array elements obtained from input image into a string
                    for(int i=0;i<9;i++)
                    {
                        str=str+to_string(maskarray[i]);
                    }
                    //Calculating the bond of the array -- Diagonal elements are given weight=1 and off diagonal elements are given weight=2
                    bond = 2 * (maskarray[1] + maskarray[3] + maskarray[5] + maskarray[7]) + 1 * (maskarray[0] + maskarray[2] + maskarray[6] + maskarray[8]);
                    traversalIndex=0;
                    temp=0;
                    //Check for the bond value for the mask obtianed from input image and compare with the unconditional masks
                    if(bond==4)
                    {
                        for(int i=0;i<8;i++)
                        {
                            if(str==bond4[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==6)
                    {
                        for(int i=0;i<8;i++)
                        {
                            if(str==bond6[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==7)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond7[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==8)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond8[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==9)
                    {
                        for(int i=0;i<8;i++)
                        {
                            if(str==bond9[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==10)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond10[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==11)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond11[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if(temp==1)
                    {
                        M[rowIndex][colIndex]=1;
                    }
                    else
                    {
                        M[rowIndex][colIndex]=0;
                    }
                }
            }
        }
        //------Step 2---- Peeling of the boundary generated in Step 1
        for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if (M[rowIndex][colIndex]==0)
                {
                    routputArray[rowIndex][colIndex]=rPixelfloat[rowIndex][colIndex];  //If M array generated in Step 1 has value 0, copy input pixel to output pixel
                }
                else
                {
                    for(int x=0;x<9;x++)
                    {
                        maskarray[x]=0;
                    }
                    traversalIndex=0;
                    for(p=rowIndex-1;p<=rowIndex+1;p++)
                    {
                        for(q=colIndex-1;q<=colIndex+1;q++)
                        {
                            int row=p;
                            int col=q;
                            if(row<0)
                            {
                                row=0;
                            }
                            if(row>imageHeight-1)
                            {
                                row=imageHeight-1;
                            }
                            if(col<0)
                            {
                                col=0;
                            }
                            if(col>imageWidth-1)
                            {
                                col=imageWidth-1;
                            }
                            maskarray[traversalIndex]=M[row][col];
                            traversalIndex++;
                        }
                    }
                    //Comparing the 9 element array genarated to conditional masks
                    temp=0;
                    //Spur
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==1 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==1 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //Single
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //L Corner
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //Corner Cluster
                    if(maskarray[0]==1 && maskarray[1]==1 &&  maskarray[3]==1 && maskarray[4]==1)
                    {
                        temp++;
                    }
                    if(maskarray[4]==1 && maskarray[5]==1 &&  maskarray[7]==1 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    //Tee Branch
                    if(maskarray[1]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==1)
                    {
                        temp++;
                    }
                    if(maskarray[1]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    if(maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==1 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    if(maskarray[1]==1 && maskarray[4]==1 && maskarray[5]==1 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    //Vee Branch
                    if(maskarray[0]==1 && (maskarray[6] || maskarray[7] || maskarray[8]==1) && maskarray[2]==1 && maskarray[4]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && (maskarray[2] || maskarray[5] || maskarray[8]==1) && maskarray[4]==1 && maskarray[6]==1)
                    {
                        temp++;
                    }
                    if((maskarray[0] || maskarray[1] || maskarray[2]==1) && maskarray[4]==1 && maskarray[6]==1 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if((maskarray[0] || maskarray[3] || maskarray[6]==1) && maskarray[2]==1 && maskarray[4]==1 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    //Diagonal Branch
                    if(maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==1 && maskarray[7]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[7]==0 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if(maskarray[1]==0 && maskarray[2]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //If no match is found with any conditional mask, output=0 else output=input
                    if(temp==0)
                    {
                        routputArray[rowIndex][colIndex]=0;
                    }
                    else
                    {
                        routputArray[rowIndex][colIndex]=rPixelfloat[rowIndex][colIndex];
                    }
                    rPixelfloat[rowIndex][colIndex]=routputArray[rowIndex][colIndex];
                }
            }
        }
    }
}

void getbridging(float** rpixel2D, int imageHeight,int imageWidth,float** routputarray)
{
    int maskarray[9];;
    int rowIndex,colIndex;
    for(rowIndex=1;rowIndex<imageHeight-1;rowIndex++)
    {
        for(colIndex=1;colIndex<imageWidth-1;colIndex++)
        {
            maskarray[0]=rpixel2D[rowIndex][colIndex+1];
            maskarray[1]=rpixel2D[rowIndex-1][colIndex+1];
            maskarray[2]=rpixel2D[rowIndex-1][colIndex];
            maskarray[3]=rpixel2D[rowIndex-1][colIndex-1];
            maskarray[4]=rpixel2D[rowIndex][colIndex-1];
            maskarray[5]=rpixel2D[rowIndex+1][colIndex-1];
            maskarray[6]=rpixel2D[rowIndex+1][colIndex];
            maskarray[7]=rpixel2D[rowIndex+1][colIndex+1];
            maskarray[8]=rpixel2D[rowIndex][colIndex];//X
            int L1 = (!(maskarray[8]) && !(maskarray[0]) && maskarray[1] && !(maskarray[2]) && maskarray[3] && !(maskarray[4]) && !(maskarray[5]) && !(maskarray[6]) && !(maskarray[7]));
            int L2 = (!(maskarray[8]) && !(maskarray[0]) && !(maskarray[1]) && !(maskarray[2]) && maskarray[3] && !(maskarray[4]) && maskarray[5] && !(maskarray[6]) && !(maskarray[7]));
            int L3 = (!(maskarray[8]) && !(maskarray[0]) && !(maskarray[1]) && !(maskarray[2]) && !(maskarray[3]) && !(maskarray[4]) && maskarray[5] && !(maskarray[6]) && maskarray[7]);
            int L4 = (!(maskarray[8]) && !(maskarray[0]) && maskarray[1] && !(maskarray[2]) && !(maskarray[3]) && !(maskarray[4]) && !(maskarray[5]) && !(maskarray[6]) && maskarray[7]);
            int Pq=(L1 || L2 || L3 || L4);
            int P1=(!(maskarray[2]) && !(maskarray[6]) && (maskarray[3] || maskarray[4] || maskarray[5]) && (maskarray[0] || maskarray[1] || maskarray[7]) && !(Pq));
            int P2=(!(maskarray[0]) && !(maskarray[4]) && (maskarray[1] || maskarray[2] || maskarray[3]) && (maskarray[5] || maskarray[6] || maskarray[7]) && !(Pq));
            int P3=(!(maskarray[0]) && !(maskarray[6]) && maskarray[7] && (maskarray[2] || maskarray[3] || maskarray[4]));
            int P4=(!(maskarray[0]) && !(maskarray[2]) && maskarray[1] && (maskarray[4] || maskarray[5] || maskarray[6]));
            int P5=(!(maskarray[2]) && !(maskarray[4]) && maskarray[3] && (maskarray[0] || maskarray[6] || maskarray[7]));
            int P6=(!(maskarray[4]) && !(maskarray[6]) && maskarray[5] && (maskarray[0] || maskarray[1] || maskarray[2]));
            routputarray[rowIndex][colIndex]=(maskarray[8] || (P1 || P2 || P3 || P4 || P5 || P6));
        }
    }
}

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=375;
    int imageWidth=375;
    
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
        }
    }
    
    // Allocate image data array
    unsigned char Imagedata[imageHeight*imageWidth*bytesPerPixel];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, temp to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    unsigned char combinedArray[imageHeight*imageWidth*bytesPerPixel];
    
    unsigned char** rpixel2Ddummy=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D=rpixel2Ddummy;
    
    unsigned char** rpixel2Ddummy45=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy45[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy45[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2Dskeletonized=rpixel2Ddummy45;
    
    float** rpixel2Ddummy10=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy10[i]=new float[imageWidth];
        memset(rpixel2Ddummy10[i],0,imageWidth*sizeof(float));
    }
    float** routputArray=rpixel2Ddummy10;
    
    float** rpixel2Ddummy4=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy4[i]=new float[imageWidth];
        memset(rpixel2Ddummy4[i],0,imageWidth*sizeof(float));
    }
    float** rpixelfloat=rpixel2Ddummy4;
    
    float** rpixel2Ddummy13=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy13[i]=new float[imageWidth];
        memset(rpixel2Ddummy13[i],0,imageWidth*sizeof(float));
    }
    float** rpixelinvertedfloat=rpixel2Ddummy13;
    
    float** rpixel2Ddummy15=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy15[i]=new float[imageWidth];
        memset(rpixel2Ddummy15[i],0,imageWidth*sizeof(float));
    }
    float** rpixelinvertedfloatfinal=rpixel2Ddummy15;
    
    float** rpixel2Ddummy9=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy9[i]=new float[imageWidth];
        memset(rpixel2Ddummy9[i],0,imageWidth*sizeof(float));
    }
    float** M=rpixel2Ddummy9;
    
    float** rpixel2Ddummy1533=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy1533[i]=new float[imageWidth];
        memset(rpixel2Ddummy1533[i],0,imageWidth*sizeof(float));
    }
    float** rbridge=rpixel2Ddummy1533;
    
    //Converting the 1D input image into R channel(the input image is a gray image)
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D);
    
    //Normalizing the values of pixels from 0 to 1
    getnormalized(rpixel2D,rpixelfloat,imageHeight,imageWidth);
    
    //Inverting the original image to get white foreground and black background
    getinvertedimage(rpixelfloat,imageHeight,imageWidth,rpixelinvertedfloat);
    
    //Performing skeletonizing algorithm
    getskeletonizing(rpixelinvertedfloat,imageHeight,imageWidth, M, routputArray);
    
    //Applying the bridging operation to eliminate gaps in the image
    getbridging(routputArray,imageHeight,imageWidth,rbridge);
    
    //Re inverting the image
    getinvertedimage(rbridge,imageHeight,imageWidth,rpixelinvertedfloatfinal);
    
    //Converting the values back to 0 to 255 range
    getoriginalrangevalue(rpixelinvertedfloatfinal,rpixel2Dskeletonized,imageHeight,imageWidth);
    
    //Combining the R channel to a single image
    combinechannel(rpixel2Dskeletonized,combinedArray,imageHeight,imageWidth);
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}



//Morphological Processing

/*
 1. Neighborhood definition - 4 connected and 8 connected
 2. Pixel bound - is used to simplify the implementation of 3 morphological process
 B = 2 * no. of 4 connected neighbors + 1 * no. of 8 connected neighbors
 3. Logic operations
 a. NOT  b. Union (A or B)  c. Intersection (A and B)  d. D = 0 or 1 (don't care)
 
 Shrinking : Erase the black pixels(object) such that
 -an object without holes inside erodes to a single pixel at or near it's center of mass
 -an object with holes erodes to a connected ring lying midway between each hole and its nearest outer boundary
 2 stages:
 input to loop up table (conditional) to midway value to loop up table (unconditional) to output, using 2 3x3 masks
 
 compare the 3x3 matrix of 8 connected neighbors with the 3x3 mask in the pattern table for each S, T and K and if it hits 1 then M for that center interest pixel is 1 else M = 0
 compare the 3x3 matrix with the unconditional masks of the table for each S and T and table is different for K, if hit then P = 1 else P = 0
 The above was one iteration and hence get G for that pixel location using formula
 
 for implementation, trasfer the table matrix into a long array (1x9), also from the image mask change into (1x9) vector to do compression
 hardcode all the masks in table
 padding doing is optional as the background is 0 ie black
 preprocessing to the input image - make 0 as background as black and 255 as object as white
 
 Q) When to stop the loop?
 once there are no more changes in the output image anymore
 create variable called flag
 flag = true
 while (flag){
 S/T/K;
 if (image changed from last time)
 flag = true;
 else
 flag = false;
 }
 
 Thinning is similar to Shrinking.
 
 Skeletonization - repeat steps above
 extra step is bridging to avoid breaking of connectivity, to connect the two parts again
 Here, all "X"s refer to pixels after done with sufficient iterations of skeletonization
 
 We do one layer padding on the input image with the zero padding (background padding)
 
 */


//Problem 2a - Basic Morphological Process Implementation


/*
 to observe output on ImageJ,
 Use: image type - 8-bit
 width - 375
 height - 375
 */

