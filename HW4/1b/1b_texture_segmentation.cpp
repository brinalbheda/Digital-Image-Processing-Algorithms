// 1. Laws feature extraction: Apply all 25 Laws filters to the input image and get 25 gray–scale images.
//2. Energy feature computation: Use a window approach to computer the energy measure for each pixel based on the results from Step 1. Try a couple of different window sizes. After this step, we obtain 25-D energy feature vecto𝑇r for each pixel.
//3. Energy feature normalization: ed by the filter 𝐿5 𝐿5 is not a useful feature for texture classification and segmentation. Use its energy to normal all other features at each pixel.
//4. Segmentation: Use the k–means algorithm to perform segmentation on the composite texture images based on the 25-D energy feature vectors.


/*
• Read the input image “comb.raw” whose dimensions are height_size = 510, width_size = 510, BytesPerPixel = 1 using fread() function
 • The window size variable is defined and the output is observed for different window sizes say 7, 15, 31
 • Subtract the mean of all pixel values in the window or average of the pixel values in the window in the input image from the original input image center pixel
 • Perform the boundary extension by pixel replication
 • Define the 5 1D masks for 5x5 Laws filter – L5, E5, S5, W5, R5
 • Calculate the 5x5 masks of Laws filter by finding tensor products of the above
 5 masks in different combinations
 • Apply the 25 Laws filters on the input images
 • Calculate the average energy features of the filtered image for the 25 filters
 and do feature normalization
 • The input image has 25 feature energy
  • Compute the localized energy for each pixel of the Laws filter output images Localize the energy of each pixel, this is done by dividing each pixel value by L5TL5 pixel energy value in the 25 energy arrays
  • Initialize using the kmeans++ algorithm
  • Use k means clustering algorithm mentioned above for k = 7 on the feature vectors
  • Calculate the Euclidean distance of each pixel with 7 centroids and label that pixel
  • Update the new centroid values by averaging the feature values in each cluster and iteratively repeat the steps until the centroid converges
  • Assign the 7 labels these 7 gray levels (0, 42, 84, 126, 168, 210, 255) mentioned above, to each cluster after k means algorithm execution
  • Display the output image for different window sizes

 */


//g++ -o 1b 1b_texture_segmentation.cpp
//./1b comb.raw output_1b_N20.raw 1 510 510

#include <stdio.h>
#include <iostream>
#include <cmath>

using namespace std;

void get2Dchannel(double* Imagedata,int imageHeight, int imageWidth,double **rpixel2D, int iterationIndex)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rpixel2D[rowIndex][colIndex]=Imagedata[iterationIndex];
            iterationIndex++;
        }
    }
}

void gettensorproduct(double* mask1,double* mask2,double** output)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            output[i][j]=mask1[i]*mask2[j];
        }
    }
}

void getdouble(unsigned char* Imagedata,double* Imagedatanew,int imageDimension)
{
    for(int i=0;i<imageDimension;i++)
    {
        Imagedatanew[i]=(double)(Imagedata[i]);
    }
}

void getLawsfilteredimage(double** rpixel2D,double** filter,int imageHeight,int imageWidth,double** filteredoutputArray)
{
    double** rpixel2Ddummy41=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy41[i]=new double[imageWidth];
        memset(rpixel2Ddummy41[i],0,imageWidth*sizeof(double));
    }
    double** rpixelDCremoved=rpixel2Ddummy41;
    
    for(int i=0;i<imageHeight;i++)
    {
        for(int j=0;j<imageWidth;j++)
        {
            rpixelDCremoved[i][j]=rpixel2D[i][j];
        }
    }
    int i,j;
    int n,x,p,q;
    int Nmean=20;
    double sorted[Nmean*Nmean];
    //Finding the mean for all pixels in the array using a window. Keep updating the mean in the mean array
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<Nmean*Nmean;x++)
            {
                sorted[x]=0;
            }
            int k=0;
            for(p=i-(Nmean-1)/2;p<=i+(Nmean-1)/2;p++)
            {
                for(q=j-(Nmean-1)/2;q<=j+(Nmean-1)/2;q++)
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
                    sorted[k]=rpixelDCremoved[rowIndex][colIndex];
                    k++;
                }
            }
            double meanVal=0;
            for(n=0;n<Nmean*Nmean;n++)
            {
                meanVal=meanVal+sorted[n];
            }
            meanVal=meanVal/(20.00*20.00);
            rpixelDCremoved[i][j]=rpixelDCremoved[i][j]-meanVal;
        }
    }
    
    int m=0;
    int N=3;
    double filter1D[N*N];
    
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            filter1D[m]=filter[i][j];
            m++;
        }
    }
    double energyArr[N*N];
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<N*N;x++)
            {
                sorted[x]=0;
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
                    sorted[k]=rpixelDCremoved[rowIndex][colIndex];
                    k++;
                }
            }
            // Finding the energy for each image with respect to all the laws filters
            double energyVal=0;
            for(n=0;n<N*N;n++)
            {
                energyArr[n]=filter1D[n]*sorted[n];
                energyVal=energyVal+energyArr[n];
            }
            filteredoutputArray[i][j]=energyVal;
        }
    }
}

void getEnergy(double** filteredoutput,int imageHeight,int imageWidth,double** energyArray)
{
    int i,j;
    int n,x,p,q;
    int N=20;
    double sorted[N*N];
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<N*N;x++)
            {
                sorted[x]=0;
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
                    sorted[k]=filteredoutput[rowIndex][colIndex];
                    k++;
                }
            }
            double energyVal=0;
            for(n=0;n<N*N;n++)
            {
                energyVal=energyVal+pow(sorted[n],2);
            }
            energyVal=energyVal/(double)(N*N);
            energyArray[i][j]=energyVal;
        }
    }
}

void getKmeansclustering(double** energyArray1,double** energyArray2,double** energyArray3,double** energyArray4,double** energyArray5,double** energyArray6,double** energyArray7,double** energyArray8,double** energyArray9,double** energyArray10,double** energyArray11,double** energyArray12,double** energyArray13,double** energyArray14,double** energyArray15,double** energyArray16,double** energyArray17,double** energyArray18,double** energyArray19,double** energyArray20,double** energyArray21,double** energyArray22,double** energyArray23,double** energyArray24,double** energyArray25,int* label,int imageHeight,int imageWidth,unsigned char* outputArray)
{
    double** rpixel2Ddummy392=new double* [imageHeight*imageWidth];
    for(int i=0;i<imageHeight*imageWidth;i++)
    {
        rpixel2Ddummy392[i]=new double[9];
        memset(rpixel2Ddummy392[i],0,9*sizeof(double));
    }
    double** featurevector=rpixel2Ddummy392;
    
    double* energyArray1final=new double [imageHeight*imageWidth];
    double* energyArray2final=new double [imageHeight*imageWidth];
    double* energyArray3final=new double [imageHeight*imageWidth];
    double* energyArray4final=new double [imageHeight*imageWidth];
    double* energyArray5final=new double [imageHeight*imageWidth];
    double* energyArray6final=new double [imageHeight*imageWidth];
    double* energyArray7final=new double [imageHeight*imageWidth];
    double* energyArray8final=new double [imageHeight*imageWidth];
    double* energyArray9final=new double [imageHeight*imageWidth];
    double* energyArray10final=new double [imageHeight*imageWidth];
    double* energyArray11final=new double [imageHeight*imageWidth];
    double* energyArray12final=new double [imageHeight*imageWidth];
    double* energyArray13final=new double [imageHeight*imageWidth];
    double* energyArray14final=new double [imageHeight*imageWidth];
    double* energyArray15final=new double [imageHeight*imageWidth];
    double* energyArray16final=new double [imageHeight*imageWidth];
    double* energyArray17final=new double [imageHeight*imageWidth];
    double* energyArray18final=new double [imageHeight*imageWidth];
    double* energyArray19final=new double [imageHeight*imageWidth];
    double* energyArray20final=new double [imageHeight*imageWidth];
    double* energyArray21final=new double [imageHeight*imageWidth];
    double* energyArray22final=new double [imageHeight*imageWidth];
    double* energyArray23final=new double [imageHeight*imageWidth];
    double* energyArray24final=new double [imageHeight*imageWidth];
    double* energyArray25final=new double [imageHeight*imageWidth];
    
    
    for(int i=0;i<imageHeight;i++)
    {
        for(int j=0;j<imageWidth;j++)
        {
            energyArray1[i][j]=energyArray1[i][j]/energyArray9[i][j];
            energyArray2[i][j]=energyArray2[i][j]/energyArray9[i][j];
            energyArray3[i][j]=energyArray3[i][j]/energyArray9[i][j];
            energyArray4[i][j]=energyArray4[i][j]/energyArray9[i][j];
            energyArray5[i][j]=energyArray5[i][j]/energyArray9[i][j];
            energyArray6[i][j]=energyArray6[i][j]/energyArray9[i][j];
            energyArray7[i][j]=energyArray7[i][j]/energyArray9[i][j];
            energyArray8[i][j]=energyArray8[i][j]/energyArray9[i][j];
            energyArray9[i][j]=energyArray9[i][j]/energyArray9[i][j];
            energyArray10[i][j]=energyArray10[i][j]/energyArray9[i][j];
            energyArray11[i][j]=energyArray11[i][j]/energyArray9[i][j];
            energyArray12[i][j]=energyArray12[i][j]/energyArray9[i][j];
            energyArray13[i][j]=energyArray13[i][j]/energyArray9[i][j];
            energyArray14[i][j]=energyArray14[i][j]/energyArray9[i][j];
            energyArray15[i][j]=energyArray15[i][j]/energyArray9[i][j];
            energyArray16[i][j]=energyArray16[i][j]/energyArray9[i][j];
            energyArray17[i][j]=energyArray17[i][j]/energyArray9[i][j];
            energyArray18[i][j]=energyArray18[i][j]/energyArray9[i][j];
            energyArray19[i][j]=energyArray19[i][j]/energyArray9[i][j];
            energyArray20[i][j]=energyArray20[i][j]/energyArray9[i][j];
            energyArray21[i][j]=energyArray21[i][j]/energyArray9[i][j];
            energyArray22[i][j]=energyArray22[i][j]/energyArray9[i][j];
            energyArray23[i][j]=energyArray23[i][j]/energyArray9[i][j];
            energyArray24[i][j]=energyArray24[i][j]/energyArray9[i][j];
            energyArray25[i][j]=energyArray25[i][j]/energyArray9[i][j];
        }
    }
    int b=0;
    for(int i=0;i<imageHeight;i++)
    {
        for(int j=0;j<imageWidth;j++)
        {
            energyArray1final[b]=energyArray1[i][j];
            energyArray2final[b]=energyArray2[i][j];
            energyArray3final[b]=energyArray3[i][j];
            energyArray4final[b]=energyArray4[i][j];
            energyArray5final[b]=energyArray5[i][j];
            energyArray6final[b]=energyArray6[i][j];
            energyArray7final[b]=energyArray7[i][j];
            energyArray8final[b]=energyArray8[i][j];
            energyArray9final[b]=energyArray9[i][j];
            energyArray10final[b]=energyArray10[i][j];
            energyArray11final[b]=energyArray11[i][j];
            energyArray12final[b]=energyArray12[i][j];
            energyArray13final[b]=energyArray13[i][j];
            energyArray14final[b]=energyArray14[i][j];
            energyArray15final[b]=energyArray15[i][j];
            energyArray16final[b]=energyArray16[i][j];
            energyArray17final[b]=energyArray17[i][j];
            energyArray18final[b]=energyArray18[i][j];
            energyArray19final[b]=energyArray19[i][j];
            energyArray20final[b]=energyArray20[i][j];
            energyArray21final[b]=energyArray21[i][j];
            energyArray22final[b]=energyArray22[i][j];
            energyArray23final[b]=energyArray23[i][j];
            energyArray24final[b]=energyArray24[i][j];
            energyArray25final[b]=energyArray25[i][j];
            b++;
        }
    }
    for(int i=0;i<imageHeight*imageWidth;i++)
    {
        featurevector[i][0]=energyArray1final[i];
        featurevector[i][1]=energyArray2final[i];
        featurevector[i][2]=energyArray3final[i];
        featurevector[i][3]=energyArray4final[i];
        featurevector[i][4]=energyArray5final[i];
        featurevector[i][5]=energyArray6final[i];
        featurevector[i][6]=energyArray7final[i];
        featurevector[i][7]=energyArray8final[i];
        featurevector[i][8]=energyArray9final[i];
    }
    //Initializing centroids
    double meanArray[7][9];
    for(int j=0;j<9;j++)
    {
        meanArray[0][j]=featurevector[21095][j];
        meanArray[1][j]=featurevector[24895][j];
        meanArray[2][j]=featurevector[55093][j];
        meanArray[3][j]=featurevector[132891][j];
        meanArray[4][j]=featurevector[222680][j];
        meanArray[5][j]=featurevector[218883][j];
        meanArray[6][j]=featurevector[178439][j];
    }
    for(int c=0;c<1;c++)
    {
        memset(label,0,imageHeight*imageWidth*sizeof(int));
        double dist[7];
        double r=0,s=0,t=0,u=0,v=0,w=0,q=0;
        for(int i=0;i<imageHeight*imageWidth;i++)
        {
            memset(dist,0,7*sizeof(double));
            //Finding distance of data point from each centroid
            dist[0]=sqrt(pow((featurevector[i][0]-meanArray[0][0]),2)+pow((featurevector[i][1]-meanArray[0][1]),2)+pow((featurevector[i][2]-meanArray[0][2]),2)+pow((featurevector[i][3]-meanArray[0][3]),2)+pow((featurevector[i][4]-meanArray[0][4]),2)+pow((featurevector[i][5]-meanArray[0][5]),2)+pow((featurevector[i][6]-meanArray[0][6]),2)+pow((featurevector[i][7]-meanArray[0][7]),2)+pow((featurevector[i][8]-meanArray[0][8]),2));
            
            dist[1]=sqrt(pow((featurevector[i][0]-meanArray[1][0]),2)+pow((featurevector[i][1]-meanArray[1][1]),2)+pow((featurevector[i][2]-meanArray[1][2]),2)+pow((featurevector[i][3]-meanArray[1][3]),2)+pow((featurevector[i][4]-meanArray[1][4]),2)+pow((featurevector[i][5]-meanArray[1][5]),2)+pow((featurevector[i][6]-meanArray[1][6]),2)+pow((featurevector[i][7]-meanArray[1][7]),2)+pow((featurevector[i][8]-meanArray[1][8]),2));
            
            dist[2]=sqrt(pow((featurevector[i][0]-meanArray[2][0]),2)+pow((featurevector[i][1]-meanArray[2][1]),2)+pow((featurevector[i][2]-meanArray[2][2]),2)+pow((featurevector[i][3]-meanArray[2][3]),2)+pow((featurevector[i][4]-meanArray[2][4]),2)+pow((featurevector[i][5]-meanArray[2][5]),2)+pow((featurevector[i][6]-meanArray[2][6]),2)+pow((featurevector[i][7]-meanArray[2][7]),2)+pow((featurevector[i][8]-meanArray[2][8]),2));
            
            dist[3]=sqrt(pow((featurevector[i][0]-meanArray[3][0]),2)+pow((featurevector[i][1]-meanArray[3][1]),2)+pow((featurevector[i][2]-meanArray[3][2]),2)+pow((featurevector[i][3]-meanArray[3][3]),2)+pow((featurevector[i][4]-meanArray[3][4]),2)+pow((featurevector[i][5]-meanArray[3][5]),2)+pow((featurevector[i][6]-meanArray[3][6]),2)+pow((featurevector[i][7]-meanArray[3][7]),2)+pow((featurevector[i][8]-meanArray[3][8]),2));
            
            dist[4]=sqrt(pow((featurevector[i][0]-meanArray[4][0]),2)+pow((featurevector[i][1]-meanArray[4][1]),2)+pow((featurevector[i][2]-meanArray[4][2]),2)+pow((featurevector[i][3]-meanArray[4][3]),2)+pow((featurevector[i][4]-meanArray[4][4]),2)+pow((featurevector[i][5]-meanArray[4][5]),2)+pow((featurevector[i][6]-meanArray[4][6]),2)+pow((featurevector[i][7]-meanArray[4][7]),2)+pow((featurevector[i][8]-meanArray[4][8]),2));
            
            dist[5]=sqrt(pow((featurevector[i][0]-meanArray[5][0]),2)+pow((featurevector[i][1]-meanArray[5][1]),2)+pow((featurevector[i][2]-meanArray[5][2]),2)+pow((featurevector[i][3]-meanArray[5][3]),2)+pow((featurevector[i][4]-meanArray[5][4]),2)+pow((featurevector[i][5]-meanArray[5][5]),2)+pow((featurevector[i][6]-meanArray[5][6]),2)+pow((featurevector[i][7]-meanArray[5][7]),2)+pow((featurevector[i][8]-meanArray[5][8]),2));
            
            dist[6]=sqrt(pow((featurevector[i][0]-meanArray[6][0]),2)+pow((featurevector[i][1]-meanArray[6][1]),2)+pow((featurevector[i][2]-meanArray[6][2]),2)+pow((featurevector[i][3]-meanArray[6][3]),2)+pow((featurevector[i][4]-meanArray[6][4]),2)+pow((featurevector[i][5]-meanArray[6][5]),2)+pow((featurevector[i][6]-meanArray[6][6]),2)+pow((featurevector[i][7]-meanArray[6][7]),2)+pow((featurevector[i][8]-meanArray[6][8]),2));
            
            double min=dist[0];
            for(int v=0;v<7;v++)
            {
                if (dist[v]<min)
                    min=dist[v];
            }
            //Determind the minimum distance and assign the data point to that cluster
            if(min==dist[0])
            {
                outputArray[i]=(unsigned char)0;
                label[i]=1;
                r++;
            }
            if(min==dist[1])
            {
                outputArray[i]=(unsigned char)42;
                label[i]=2;
                s++;
            }
            if(min==dist[2])
            {
                outputArray[i]=(unsigned char)84;
                label[i]=3;
                t++;
            }
            if(min==dist[3])
            {
                outputArray[i]=(unsigned char)126;
                label[i]=4;
                u++;
            }
            if(min==dist[4])
            {
                outputArray[i]=(unsigned char)168;
                label[i]=5;
                v++;
            }
            if(min==dist[5])
            {
                outputArray[i]=(unsigned char)210;
                label[i]=6;
                w++;
            }
            if(min==dist[6])
            {
                outputArray[i]=(unsigned char)255;
                label[i]=7;
                q++;
            }
        }
        //Updating centroids in each iteration
        double sum1[9]={0},sum2[9]={0},sum3[9]={0},sum4[9]={0},sum5[9]={0},sum6[9]={0},sum7[9]={0};
        for(int i=0;i<imageHeight*imageWidth;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(label[i]==1)
                {
                    sum1[j]=sum1[j]+featurevector[i][j];
                }
                else if(label[i]==2)
                {
                    sum2[j]=sum2[j]+featurevector[i][j];
                }
                else if(label[i]==3)
                {
                    sum3[j]=sum3[j]+featurevector[i][j];
                }
                else if(label[i]==4)
                {
                    sum4[j]=sum4[j]+featurevector[i][j];
                }
                else if(label[i]==5)
                {
                    sum5[j]=sum5[j]+featurevector[i][j];
                }
                else if(label[i]==6)
                {
                    sum6[j]=sum6[j]+featurevector[i][j];
                }
                else if(label[i]==7)
                {
                    sum7[j]=sum7[j]+featurevector[i][j];
                }
            }
        }
        for(int j=0;j<9;j++)
        {
            meanArray[0][j]=(sum1[j]/(double)r);
            meanArray[1][j]=(sum2[j]/(double)s);
            meanArray[2][j]=(sum3[j]/(double)t);
            meanArray[3][j]=(sum4[j]/(double)u);
            meanArray[4][j]=(sum5[j]/(double)v);
            meanArray[5][j]=(sum6[j]/(double)w);
            meanArray[6][j]=(sum7[j]/(double)q);
        }
    }
}

void getcombinechannels(double** thresholdedimage,unsigned char* combinedArray,int imageHeight,int imageWidth)
{
    int rowIndex,colIndex,count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            combinedArray[count]=(unsigned char)thresholdedimage[rowIndex][colIndex];
            count++;
        }
    }
}


int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=510;
    int imageWidth=510;
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerPixel = 1] [Height=255] [Width=255]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 3){
        bytesPerPixel = 1; // default is grey image
    }
    else {
        bytesPerPixel = atoi(argv[3]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 4){
            imageWidth= atoi(argv[4]);
            imageHeight= atoi(argv[5]);
        }
    }
    
    // Allocate image data array
    int imageDimension=imageHeight*imageWidth*bytesPerPixel;
    unsigned char Imagedata[imageDimension];
    double  Imagedatanew[imageDimension];
    unsigned char  combinedArray[imageDimension];
    
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
    //25D
    double L5[5]={1.0/16.0,4.0/16.0,6.0/16.0,4.0/16.0,1.0/16.0};
    double E5[5]={-1.0/6.0,-2.0/6.0,0,2.0/6.0,1.0/6.0};
    double S5[5]={-1.0/4.0,0,2.0/4.0,0,-1.0/4.0};
    double W5[5]={-1.0/6.0,2.0/6.0,0,-2.0/6.0,1.0/6.0};
    double R5[5]={1.0/16.0,-4.0/16.0,6.0/16.0,-4.0/16.0,1.0/16.0};
    //9D
    double E3[3]={-1.0/2.0,0,1.0/2.0};
    double S3[3]={-1.0/2.0,2.0/2.0,-1.0/2.0};
    double L3[3]={1.0/6.0,2.0/6.0,1.0/6.0};
    
    double** rpixel2Ddummy1=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy1[i]=new double[3];
        memset(rpixel2Ddummy1[i],0,3*sizeof(double));
    }
    double** E3E3=rpixel2Ddummy1;
    
    double** rpixel2Ddummy11=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy11[i]=new double[3];
        memset(rpixel2Ddummy11[i],0,3*sizeof(double));
    }
    double** E3S3=rpixel2Ddummy11;
    
    double** rpixel2Ddummy12=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy12[i]=new double[3];
        memset(rpixel2Ddummy12[i],0,3*sizeof(double));
    }
    double** E3L3=rpixel2Ddummy12;
    
    double** rpixel2Ddummy13=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy13[i]=new double[3];
        memset(rpixel2Ddummy13[i],0,3*sizeof(double));
    }
    double** S3E3=rpixel2Ddummy13;
    
    double** rpixel2Ddummy14=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy14[i]=new double[3];
        memset(rpixel2Ddummy14[i],0,3*sizeof(double));
    }
    double** S3S3=rpixel2Ddummy14;
    
    double** rpixel2Ddummy15=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy15[i]=new double[3];
        memset(rpixel2Ddummy15[i],0,3*sizeof(double));
    }
    double** S3L3=rpixel2Ddummy15;
    
    double** rpixel2Ddummy16=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy16[i]=new double[3];
        memset(rpixel2Ddummy16[i],0,3*sizeof(double));
    }
    double** L3E3=rpixel2Ddummy16;
    
    double** rpixel2Ddummy17=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy17[i]=new double[3];
        memset(rpixel2Ddummy17[i],0,3*sizeof(double));
    }
    double** L3S3=rpixel2Ddummy17;
    
    double** rpixel2Ddummy18=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy18[i]=new double[3];
        memset(rpixel2Ddummy18[i],0,3*sizeof(double));
    }
    double** L3L3=rpixel2Ddummy18;
    //
    double** rpixel2Ddummy21=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy21[i]=new double[imageWidth];
        memset(rpixel2Ddummy21[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D=rpixel2Ddummy21;
    
    double** rpixel2Ddummy22=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy22[i]=new double[imageWidth];
        memset(rpixel2Ddummy22[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput1=rpixel2Ddummy22;

    double** rpixel2Ddummy23=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy23[i]=new double[imageWidth];
        memset(rpixel2Ddummy23[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput2=rpixel2Ddummy23;

    double** rpixel2Ddummy24=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy24[i]=new double[imageWidth];
        memset(rpixel2Ddummy24[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput3=rpixel2Ddummy24;

    double** rpixel2Ddummy25=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy25[i]=new double[imageWidth];
        memset(rpixel2Ddummy25[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput4=rpixel2Ddummy25;

    double** rpixel2Ddummy26=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy26[i]=new double[imageWidth];
        memset(rpixel2Ddummy26[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput5=rpixel2Ddummy26;

    double** rpixel2Ddummy27=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy27[i]=new double[imageWidth];
        memset(rpixel2Ddummy27[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput6=rpixel2Ddummy27;

    double** rpixel2Ddummy28=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy28[i]=new double[imageWidth];
        memset(rpixel2Ddummy28[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput7=rpixel2Ddummy28;

    double** rpixel2Ddummy29=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy29[i]=new double[imageWidth];
        memset(rpixel2Ddummy29[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput8=rpixel2Ddummy29;

    double** rpixel2Ddummy30=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy30[i]=new double[imageWidth];
        memset(rpixel2Ddummy30[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput9=rpixel2Ddummy30;
    
    double** rpixel2Ddummy56=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy56[i]=new double[imageWidth];
        memset(rpixel2Ddummy56[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput10=rpixel2Ddummy56;
    
    double** rpixel2Ddummy57=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy57[i]=new double[imageWidth];
        memset(rpixel2Ddummy57[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput11=rpixel2Ddummy57;
    
    double** rpixel2Ddummy58=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy58[i]=new double[imageWidth];
        memset(rpixel2Ddummy58[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput12=rpixel2Ddummy58;
    
    double** rpixel2Ddummy59=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy59[i]=new double[imageWidth];
        memset(rpixel2Ddummy59[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput13=rpixel2Ddummy59;
    
    double** rpixel2Ddummy60=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy60[i]=new double[imageWidth];
        memset(rpixel2Ddummy60[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput14=rpixel2Ddummy60;
    
    double** rpixel2Ddummy61=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy61[i]=new double[imageWidth];
        memset(rpixel2Ddummy61[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput15=rpixel2Ddummy61;
    
    double** rpixel2Ddummy62=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy62[i]=new double[imageWidth];
        memset(rpixel2Ddummy62[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput16=rpixel2Ddummy62;
    
    double** rpixel2Ddummy63=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy63[i]=new double[imageWidth];
        memset(rpixel2Ddummy63[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput17=rpixel2Ddummy63;
    
    double** rpixel2Ddummy64=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy64[i]=new double[imageWidth];
        memset(rpixel2Ddummy64[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput18=rpixel2Ddummy64;
    
    double** rpixel2Ddummy65=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy65[i]=new double[imageWidth];
        memset(rpixel2Ddummy65[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput19=rpixel2Ddummy65;
    
    double** rpixel2Ddummy66=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy66[i]=new double[imageWidth];
        memset(rpixel2Ddummy66[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput20=rpixel2Ddummy66;
    
    double** rpixel2Ddummy67=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy67[i]=new double[imageWidth];
        memset(rpixel2Ddummy67[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput21=rpixel2Ddummy67;
    
    double** rpixel2Ddummy68=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy68[i]=new double[imageWidth];
        memset(rpixel2Ddummy68[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput22=rpixel2Ddummy68;
    
    double** rpixel2Ddummy69=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy69[i]=new double[imageWidth];
        memset(rpixel2Ddummy69[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput23=rpixel2Ddummy69;
    
    double** rpixel2Ddummy70=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy70[i]=new double[imageWidth];
        memset(rpixel2Ddummy70[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput24=rpixel2Ddummy70;
    
    double** rpixel2Ddummy71=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy71[i]=new double[imageWidth];
        memset(rpixel2Ddummy71[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput25=rpixel2Ddummy71;

    double** rpixel2Ddummy31=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy31[i]=new double[imageWidth];
        memset(rpixel2Ddummy31[i],0,imageWidth*sizeof(double));
    }
    double** energyArray1=rpixel2Ddummy31;

    double** rpixel2Ddummy32=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy32[i]=new double[imageWidth];
        memset(rpixel2Ddummy32[i],0,imageWidth*sizeof(double));
    }
    double** energyArray2=rpixel2Ddummy32;

    double** rpixel2Ddummy33=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy33[i]=new double[imageWidth];
        memset(rpixel2Ddummy33[i],0,imageWidth*sizeof(double));
    }
    double** energyArray3=rpixel2Ddummy33;
    
    double** rpixel2Ddummy34=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy34[i]=new double[imageWidth];
        memset(rpixel2Ddummy34[i],0,imageWidth*sizeof(double));
    }
    double** energyArray4=rpixel2Ddummy34;
    
    double** rpixel2Ddummy35=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy35[i]=new double[imageWidth];
        memset(rpixel2Ddummy35[i],0,imageWidth*sizeof(double));
    }
    double** energyArray5=rpixel2Ddummy35;
    
    double** rpixel2Ddummy36=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy36[i]=new double[imageWidth];
        memset(rpixel2Ddummy36[i],0,imageWidth*sizeof(double));
    }
    double** energyArray6=rpixel2Ddummy36;
    
    double** rpixel2Ddummy37=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy37[i]=new double[imageWidth];
        memset(rpixel2Ddummy37[i],0,imageWidth*sizeof(double));
    }
    double** energyArray7=rpixel2Ddummy37;
    
    double** rpixel2Ddummy38=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy38[i]=new double[imageWidth];
        memset(rpixel2Ddummy38[i],0,imageWidth*sizeof(double));
    }
    double** energyArray8=rpixel2Ddummy38;
    
    double** rpixel2Ddummy39=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy39[i]=new double[imageWidth];
        memset(rpixel2Ddummy39[i],0,imageWidth*sizeof(double));
    }
    double** energyArray9=rpixel2Ddummy39;
    
    double** rpixel2Ddummy40=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy40[i]=new double[imageWidth];
        memset(rpixel2Ddummy40[i],0,imageWidth*sizeof(double));
    }
    double** energyArray10=rpixel2Ddummy40;
    
    double** rpixel2Ddummy41=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy41[i]=new double[imageWidth];
        memset(rpixel2Ddummy41[i],0,imageWidth*sizeof(double));
    }
    double** energyArray11=rpixel2Ddummy41;
    
    double** rpixel2Ddummy42=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy42[i]=new double[imageWidth];
        memset(rpixel2Ddummy42[i],0,imageWidth*sizeof(double));
    }
    double** energyArray12=rpixel2Ddummy42;
    
    double** rpixel2Ddummy43=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy43[i]=new double[imageWidth];
        memset(rpixel2Ddummy43[i],0,imageWidth*sizeof(double));
    }
    double** energyArray13=rpixel2Ddummy43;
    
    double** rpixel2Ddummy44=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy44[i]=new double[imageWidth];
        memset(rpixel2Ddummy44[i],0,imageWidth*sizeof(double));
    }
    double** energyArray14=rpixel2Ddummy44;
    
    double** rpixel2Ddummy45=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy45[i]=new double[imageWidth];
        memset(rpixel2Ddummy45[i],0,imageWidth*sizeof(double));
    }
    double** energyArray15=rpixel2Ddummy45;
    
    double** rpixel2Ddummy46=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy46[i]=new double[imageWidth];
        memset(rpixel2Ddummy46[i],0,imageWidth*sizeof(double));
    }
    double** energyArray16=rpixel2Ddummy46;
    
    double** rpixel2Ddummy47=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy47[i]=new double[imageWidth];
        memset(rpixel2Ddummy47[i],0,imageWidth*sizeof(double));
    }
    double** energyArray17=rpixel2Ddummy47;
    
    double** rpixel2Ddummy48=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy48[i]=new double[imageWidth];
        memset(rpixel2Ddummy48[i],0,imageWidth*sizeof(double));
    }
    double** energyArray18=rpixel2Ddummy48;
    
    double** rpixel2Ddummy49=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy49[i]=new double[imageWidth];
        memset(rpixel2Ddummy49[i],0,imageWidth*sizeof(double));
    }
    double** energyArray19=rpixel2Ddummy49;
    
    double** rpixel2Ddummy50=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy50[i]=new double[imageWidth];
        memset(rpixel2Ddummy50[i],0,imageWidth*sizeof(double));
    }
    double** energyArray20=rpixel2Ddummy50;
    
    double** rpixel2Ddummy51=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy51[i]=new double[imageWidth];
        memset(rpixel2Ddummy51[i],0,imageWidth*sizeof(double));
    }
    double** energyArray21=rpixel2Ddummy51;
    
    double** rpixel2Ddummy52=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy52[i]=new double[imageWidth];
        memset(rpixel2Ddummy52[i],0,imageWidth*sizeof(double));
    }
    double** energyArray22=rpixel2Ddummy52;
    
    double** rpixel2Ddummy53=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy53[i]=new double[imageWidth];
        memset(rpixel2Ddummy53[i],0,imageWidth*sizeof(double));
    }
    double** energyArray23=rpixel2Ddummy53;
    
    double** rpixel2Ddummy54=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy54[i]=new double[imageWidth];
        memset(rpixel2Ddummy54[i],0,imageWidth*sizeof(double));
    }
    double** energyArray24=rpixel2Ddummy54;
    
    double** rpixel2Ddummy55=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy55[i]=new double[imageWidth];
        memset(rpixel2Ddummy55[i],0,imageWidth*sizeof(double));
    }
    double** energyArray25=rpixel2Ddummy55;
    
    
    double** rpixel2Ddummy72=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy72[i]=new double[5];
        memset(rpixel2Ddummy72[i],0,5*sizeof(double));
    }
    double** L5L5=rpixel2Ddummy72;
    
    double** rpixel2Ddummy73=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy73[i]=new double[5];
        memset(rpixel2Ddummy73[i],0,5*sizeof(double));
    }
    double** L5E5=rpixel2Ddummy73;
    
    double** rpixel2Ddummy74=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy74[i]=new double[5];
        memset(rpixel2Ddummy74[i],0,5*sizeof(double));
    }
    double** L5S5=rpixel2Ddummy74;
    
    double** rpixel2Ddummy75=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy75[i]=new double[5];
        memset(rpixel2Ddummy75[i],0,5*sizeof(double));
    }
    double** L5W5=rpixel2Ddummy75;
    
    double** rpixel2Ddummy76=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy76[i]=new double[5];
        memset(rpixel2Ddummy76[i],0,5*sizeof(double));
    }
    double** L5R5=rpixel2Ddummy76;
    
    double** rpixel2Ddummy77=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy77[i]=new double[5];
        memset(rpixel2Ddummy77[i],0,5*sizeof(double));
    }
    double** E5L5=rpixel2Ddummy77;
    
    double** rpixel2Ddummy78=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy78[i]=new double[5];
        memset(rpixel2Ddummy78[i],0,5*sizeof(double));
    }
    double** E5E5=rpixel2Ddummy78;
    
    double** rpixel2Ddummy79=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy79[i]=new double[5];
        memset(rpixel2Ddummy79[i],0,5*sizeof(double));
    }
    double** E5S5=rpixel2Ddummy79;
    
    double** rpixel2Ddummy80=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy80[i]=new double[5];
        memset(rpixel2Ddummy80[i],0,5*sizeof(double));
    }
    double** E5W5=rpixel2Ddummy80;
    
    double** rpixel2Ddummy81=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy81[i]=new double[5];
        memset(rpixel2Ddummy81[i],0,5*sizeof(double));
    }
    double** E5R5=rpixel2Ddummy81;
    
    double** rpixel2Ddummy82=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy82[i]=new double[5];
        memset(rpixel2Ddummy82[i],0,5*sizeof(double));
    }
    double** S5L5=rpixel2Ddummy82;
    
    double** rpixel2Ddummy83=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy83[i]=new double[5];
        memset(rpixel2Ddummy83[i],0,5*sizeof(double));
    }
    double** S5E5=rpixel2Ddummy83;
    
    double** rpixel2Ddummy84=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy84[i]=new double[5];
        memset(rpixel2Ddummy84[i],0,5*sizeof(double));
    }
    double** S5S5=rpixel2Ddummy84;
    
    double** rpixel2Ddummy85=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy85[i]=new double[5];
        memset(rpixel2Ddummy85[i],0,5*sizeof(double));
    }
    double** S5W5=rpixel2Ddummy85;
    
    double** rpixel2Ddummy86=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy86[i]=new double[5];
        memset(rpixel2Ddummy86[i],0,5*sizeof(double));
    }
    double** S5R5=rpixel2Ddummy86;
    
    double** rpixel2Ddummy87=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy87[i]=new double[5];
        memset(rpixel2Ddummy87[i],0,5*sizeof(double));
    }
    double** W5L5=rpixel2Ddummy87;
    
    double** rpixel2Ddummy88=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy88[i]=new double[5];
        memset(rpixel2Ddummy88[i],0,5*sizeof(double));
    }
    double** W5E5=rpixel2Ddummy88;
    
    double** rpixel2Ddummy89=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy89[i]=new double[5];
        memset(rpixel2Ddummy89[i],0,5*sizeof(double));
    }
    double** W5S5=rpixel2Ddummy89;
    
    double** rpixel2Ddummy90=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy90[i]=new double[5];
        memset(rpixel2Ddummy90[i],0,5*sizeof(double));
    }
    double** W5W5=rpixel2Ddummy90;
    
    double** rpixel2Ddummy91=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy91[i]=new double[5];
        memset(rpixel2Ddummy91[i],0,5*sizeof(double));
    }
    double** W5R5=rpixel2Ddummy91;
    
    double** rpixel2Ddummy92=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy92[i]=new double[5];
        memset(rpixel2Ddummy92[i],0,5*sizeof(double));
    }
    double** R5L5=rpixel2Ddummy92;
    
    double** rpixel2Ddummy93=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy93[i]=new double[5];
        memset(rpixel2Ddummy93[i],0,5*sizeof(double));
    }
    double** R5E5=rpixel2Ddummy93;
    
    double** rpixel2Ddummy94=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy94[i]=new double[5];
        memset(rpixel2Ddummy94[i],0,5*sizeof(double));
    }
    double** R5S5=rpixel2Ddummy94;
    
    double** rpixel2Ddummy95=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy95[i]=new double[5];
        memset(rpixel2Ddummy95[i],0,5*sizeof(double));
    }
    double** R5W5=rpixel2Ddummy95;
    
    double** rpixel2Ddummy96=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy96[i]=new double[5];
        memset(rpixel2Ddummy96[i],0,5*sizeof(double));
    }
    double** R5R5=rpixel2Ddummy96;
    
    
    //Get 9 filters using tensor products
    gettensorproduct(E3,E3,E3E3);   //E3E3 Laws filter
    gettensorproduct(E3,S3,E3S3);   //E3S3 Laws filter
    gettensorproduct(E3,L3,E3L3);   //E3W3 Laws filter
    gettensorproduct(S3,E3,S3E3);   //S3E3 Laws filter
    gettensorproduct(S3,S3,S3S3);   //S3S3 Laws filter
    gettensorproduct(S3,L3,S3L3);   //S3W3 Laws filter
    gettensorproduct(L3,E3,L3E3);   //W3E3 Laws filter
    gettensorproduct(L3,S3,L3S3);   //W3S3 Laws filter
    gettensorproduct(L3,L3,L3L3);   //W3W3 Laws filter
  /*
    //Get 25 filters using tensor products
    gettensorproduct(L5,L5,L5L5);   //L5L5 Laws filter
    gettensorproduct(L5,E5,L5E5);   //L5E5 Laws filter
    gettensorproduct(L5,S5,L5S5);   //L5S5 Laws filter
    gettensorproduct(L5,W5,L5W5);   //L5W5 Laws filter
    gettensorproduct(L5,R5,L5R5);   //L5R5 Laws filter
    gettensorproduct(E5,L5,E5L5);   //E5L5 Laws filter
    gettensorproduct(E5,E5,E5E5);   //E5E5 Laws filter
    gettensorproduct(E5,S5,E5S5);   //E5S5 Laws filter
    gettensorproduct(E5,W5,E5W5);   //E5W5 Laws filter
    gettensorproduct(E5,R5,E5R5);   //E5R5 Laws filter
    gettensorproduct(S5,L5,S5L5);   //S5L5 Laws filter
    gettensorproduct(S5,E5,S5E5);   //S5E5 Laws filter
    gettensorproduct(S5,S5,S5S5);   //S5S5 Laws filter
    gettensorproduct(S5,W5,S5W5);   //S5W5 Laws filter
    gettensorproduct(S5,R5,S5R5);   //S5R5 Laws filter
    gettensorproduct(W5,L5,W5L5);   //W5L5 Laws filter
    gettensorproduct(W5,E5,W5E5);   //W5E5 Laws filter
    gettensorproduct(W5,S5,W5S5);   //W5S5 Laws filter
    gettensorproduct(W5,W5,W5W5);   //W5W5 Laws filter
    gettensorproduct(W5,R5,W5R5);   //W5R5 Laws filter
    gettensorproduct(R5,L5,R5L5);   //R5L5 Laws filter
    gettensorproduct(R5,E5,R5E5);   //R5E5 Laws filter
    gettensorproduct(R5,S5,R5S5);   //R5S5 Laws filter
    gettensorproduct(R5,W5,R5W5);   //R5W5 Laws filter
    gettensorproduct(R5,R5,R5R5);   //R5R5 Laws filter
*/
    //Converting image in unsigned char to double format for ease of solving
    getdouble(Imagedata,Imagedatanew,imageDimension);
    
    //Convert 1D input image to 2D R channel( the input image is gray scale a image)
    get2Dchannel(Imagedatanew,imageHeight,imageWidth,rpixel2D,0);

    //Get the Laws filtered input images
    getLawsfilteredimage(rpixel2D,E3E3,imageHeight,imageWidth,filteredoutput1);
    getLawsfilteredimage(rpixel2D,E3S3,imageHeight,imageWidth,filteredoutput2);
    getLawsfilteredimage(rpixel2D,E3L3,imageHeight,imageWidth,filteredoutput3);
    getLawsfilteredimage(rpixel2D,S3E3,imageHeight,imageWidth,filteredoutput4);
    getLawsfilteredimage(rpixel2D,S3S3,imageHeight,imageWidth,filteredoutput5);
    getLawsfilteredimage(rpixel2D,S3L3,imageHeight,imageWidth,filteredoutput6);
    getLawsfilteredimage(rpixel2D,L3E3,imageHeight,imageWidth,filteredoutput7);
    getLawsfilteredimage(rpixel2D,L3S3,imageHeight,imageWidth,filteredoutput8);
    getLawsfilteredimage(rpixel2D,L3L3,imageHeight,imageWidth,filteredoutput9);
 /*
    //Get the Laws filtered input images - 25D
    getLawsfilteredimage(rpixel2D,L5L5,imageHeight,imageWidth,filteredoutput1);
    getLawsfilteredimage(rpixel2D,L5E5,imageHeight,imageWidth,filteredoutput2);
    getLawsfilteredimage(rpixel2D,L5S5,imageHeight,imageWidth,filteredoutput3);
    getLawsfilteredimage(rpixel2D,L5W5,imageHeight,imageWidth,filteredoutput4);
    getLawsfilteredimage(rpixel2D,L5R5,imageHeight,imageWidth,filteredoutput5);
    getLawsfilteredimage(rpixel2D,E5L5,imageHeight,imageWidth,filteredoutput6);
    getLawsfilteredimage(rpixel2D,E5E5,imageHeight,imageWidth,filteredoutput7);
    getLawsfilteredimage(rpixel2D,E5S5,imageHeight,imageWidth,filteredoutput8);
    getLawsfilteredimage(rpixel2D,E5W5,imageHeight,imageWidth,filteredoutput9);
    getLawsfilteredimage(rpixel2D,E5R5,imageHeight,imageWidth,filteredoutput10);
    getLawsfilteredimage(rpixel2D,S5L5,imageHeight,imageWidth,filteredoutput11);
    getLawsfilteredimage(rpixel2D,S5E5,imageHeight,imageWidth,filteredoutput12);
    getLawsfilteredimage(rpixel2D,S5S5,imageHeight,imageWidth,filteredoutput13);
    getLawsfilteredimage(rpixel2D,S5W5,imageHeight,imageWidth,filteredoutput14);
    getLawsfilteredimage(rpixel2D,S5R5,imageHeight,imageWidth,filteredoutput15);
    getLawsfilteredimage(rpixel2D,W5L5,imageHeight,imageWidth,filteredoutput16);
    getLawsfilteredimage(rpixel2D,W5E5,imageHeight,imageWidth,filteredoutput17);
    getLawsfilteredimage(rpixel2D,W5S5,imageHeight,imageWidth,filteredoutput18);
    getLawsfilteredimage(rpixel2D,W5W5,imageHeight,imageWidth,filteredoutput19);
    getLawsfilteredimage(rpixel2D,W5R5,imageHeight,imageWidth,filteredoutput20);
    getLawsfilteredimage(rpixel2D,R5L5,imageHeight,imageWidth,filteredoutput21);
    getLawsfilteredimage(rpixel2D,R5E5,imageHeight,imageWidth,filteredoutput22);
    getLawsfilteredimage(rpixel2D,R5S5,imageHeight,imageWidth,filteredoutput23);
    getLawsfilteredimage(rpixel2D,R5W5,imageHeight,imageWidth,filteredoutput24);
    getLawsfilteredimage(rpixel2D,R5R5,imageHeight,imageWidth,filteredoutput25);
   */

    //Find the energy for all Laws filtered images
    getEnergy(filteredoutput1,imageHeight,imageWidth,energyArray1);
    getEnergy(filteredoutput2,imageHeight,imageWidth,energyArray2);
    getEnergy(filteredoutput3,imageHeight,imageWidth,energyArray3);
    getEnergy(filteredoutput4,imageHeight,imageWidth,energyArray4);
    getEnergy(filteredoutput5,imageHeight,imageWidth,energyArray5);
    getEnergy(filteredoutput6,imageHeight,imageWidth,energyArray6);
    getEnergy(filteredoutput7,imageHeight,imageWidth,energyArray7);
    getEnergy(filteredoutput8,imageHeight,imageWidth,energyArray8);
    getEnergy(filteredoutput9,imageHeight,imageWidth,energyArray9);
 /*
    //Find the energy for all Laws filtered images - 25D
    getEnergy(filteredoutput1,imageHeight,imageWidth,energyArray1);
    getEnergy(filteredoutput2,imageHeight,imageWidth,energyArray2);
    getEnergy(filteredoutput3,imageHeight,imageWidth,energyArray3);
    getEnergy(filteredoutput4,imageHeight,imageWidth,energyArray4);
    getEnergy(filteredoutput5,imageHeight,imageWidth,energyArray5);
    getEnergy(filteredoutput6,imageHeight,imageWidth,energyArray6);
    getEnergy(filteredoutput7,imageHeight,imageWidth,energyArray7);
    getEnergy(filteredoutput8,imageHeight,imageWidth,energyArray8);
    getEnergy(filteredoutput9,imageHeight,imageWidth,energyArray9);
    getEnergy(filteredoutput10,imageHeight,imageWidth,energyArray10);
    getEnergy(filteredoutput11,imageHeight,imageWidth,energyArray11);
    getEnergy(filteredoutput12,imageHeight,imageWidth,energyArray12);
    getEnergy(filteredoutput13,imageHeight,imageWidth,energyArray13);
    getEnergy(filteredoutput14,imageHeight,imageWidth,energyArray14);
    getEnergy(filteredoutput15,imageHeight,imageWidth,energyArray15);
    getEnergy(filteredoutput16,imageHeight,imageWidth,energyArray16);
    getEnergy(filteredoutput17,imageHeight,imageWidth,energyArray17);
    getEnergy(filteredoutput18,imageHeight,imageWidth,energyArray18);
    getEnergy(filteredoutput19,imageHeight,imageWidth,energyArray19);
    getEnergy(filteredoutput20,imageHeight,imageWidth,energyArray20);
    getEnergy(filteredoutput21,imageHeight,imageWidth,energyArray21);
    getEnergy(filteredoutput22,imageHeight,imageWidth,energyArray22);
    getEnergy(filteredoutput23,imageHeight,imageWidth,energyArray23);
    getEnergy(filteredoutput24,imageHeight,imageWidth,energyArray24);
    getEnergy(filteredoutput25,imageHeight,imageWidth,energyArray25);
*/
    int *label=new int [imageHeight*imageWidth];
    unsigned char* outputArray=new unsigned char[imageHeight*imageWidth];

    // Apply the K-means clustering algorithm to segment the image into 6 categories
getKmeansclustering(energyArray1,energyArray2,energyArray3,energyArray4,energyArray5,energyArray6,energyArray7,energyArray8,energyArray9,energyArray10,energyArray11,energyArray12,energyArray13,energyArray14,energyArray15,energyArray16,energyArray17,energyArray18,energyArray19,energyArray20,energyArray21,energyArray22,energyArray23,energyArray24,energyArray25,label,imageHeight,imageWidth,outputArray);

    for(int i=0;i<imageHeight*imageWidth*bytesPerPixel;i++)
    {
        combinedArray[i]=outputArray[i];
    }
    
    
    //Combining output array into 1 array
    //getcombinechannels(filteredoutput6,combinedArray,imageHeight,imageWidth);
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    //fwrite(outputArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(luminosityArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}





