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
    int imageHeight=691;
    int imageWidth=550;
    
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





