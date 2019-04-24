//1. Feature Extraction: Use the twenty-five 5x5 Laws Filters to extract feature vectors from each pixel in the image (use appropriate boundary extensions).
//2. FeatureAveraging:Averagethefeaturevectorsofallimagepixels,leadingto a 25-D feature vector for each image.
//3. Reduce the feature dimension from 25 to 3 using the principal component analysis (PCA). Plot the reduced 3-D feature vector in the feature space. (May use built-in C++/Matlab functions of PCA.)
//4. Clustering: Use the K-means algorithm for image clustering based on the 25- D and 3-D obtained in Steps 2 and 3, respectively.



//$ g++ -o 1a 1a_texture_classification.cpp
//$ ./1a texture1.raw texture2.raw texture3.raw texture4.raw texture5.raw texture6.raw texture7.raw texture8.raw texture9.raw texture10.raw texture11.raw texture12.raw 1 128 128 output_1a.raw mat1.txt


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
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
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

double getEnergy(double **rpixel2D,double** filter,int imageHeight,int imageWidth)
{
    double mean=0,sum=0;
    int rowIndex,colIndex;
    
    double** rpixel2Ddummy41=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy41[i]=new double[imageWidth];
        memset(rpixel2Ddummy41[i],0,imageWidth*sizeof(double));
    }
    double** rpixelDCremoved=rpixel2Ddummy41;
    
    //Finding the mean for all pixels in the array---Every image will have 1 mean
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            sum=sum+rpixel2D[rowIndex][colIndex];
        }
    }
   
    mean=sum/(double)(imageHeight*imageWidth);
    //Subtracting the mean from the pixel values to eliminate illumination effects
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rpixelDCremoved[rowIndex][colIndex]=rpixel2D[rowIndex][colIndex]-mean;
        }
    }

    int i,j;
    int n,x,p,q,m=0;
    int N=5;
    double filter1D[N*N];
    
    double** rpixel2Ddummy60=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy60[i]=new double[imageWidth];
        memset(rpixel2Ddummy60[i],0,imageWidth*sizeof(double));
    }
    double** energyArray=rpixel2Ddummy60;
    
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            filter1D[m]=filter[i][j];
            m++;
        }
    }
    double sorted[N*N],energyArr[N*N];
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
            double energyVal=0;
            // Finding the energy for each image with respect to all the laws filters
            for(n=0;n<N*N;n++)
            {
                energyArr[n]=filter1D[n]*sorted[n];
                energyVal=energyVal+energyArr[n];
            }
            energyArray[i][j]=energyVal;
        }
    }
    sum=0;
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            sum=sum+pow(energyArray[i][j],2);
        }
    }
    sum=sum/(imageHeight*imageWidth);
    return(sum);
}

void getKmeansclustering(double** energyArray,int* label,string* labelName,string fileName)
{
    //For displaying histogram for discriminant analysis
    FILE *File;
    File=fopen(fileName.c_str(),"w");
    
    if(File!=nullptr)
    {
        for(int i=0;i<12;i++)
        {
            for(int j=0;j<25;j++)
            {
                fprintf(File,"%lf\t",energyArray[i][j]);
            }
            fprintf(File,"\n");
        }
    }
    fclose(File);
    
    //Initializing centroids
    double meanArray[4][25]=
    {
        {348.429,304.9853,289.342,276.234,239.453,219.847,203.439,193.32,186.763,161.3237,175.493,158.002,143.5267,150.345,167.9798,138.377,115.3386,339.1234,321.9364,329.533,315.2776,309.4123,297.2390,145.223,199.999},
        {97.558,94.1232,91.1735,90.43407,87.504,84.9658,82.81954,80.9784,77.7019,76.8515,73.982,70.548,67.9120,64.6529,62.2331,60.1882,57.920,54.901,52.7811,49.2222,46.8779,42.011,40.1213,37.3555,33.1239},
        {50.12,48.8223,47.1122,44.8210,41.4179,39.1265,36.8122,34.1120,31.1111,29.2341,27.7852,24.5212,25.6384,22.2419,19.1445,17.23,15.981,14.721,11.122,10.9018,9.1276,8.89253,7.0281,5.70674,1.65945},
        {1444.92969,124.51223,424.14521,1233.99021,133.76182,523.31451,23.0012,112.9015,762.82341,82.65409,92.514896,122.32582,102.48193,42.27593,62.11902,99.00102,111.91082,110.85916,192.62461,100.57822,1112.4790,1231.20472,1247.24336,240.697996,630.554682}
    };
    for(int c=0;c<100;c++)
    {
    memset(label,0,12*sizeof(int));
    double dist[4];
    double r=0,s=0,t=0,u=0;
    for(int i=0;i<12;i++)
    {
        memset(dist,0,4*sizeof(double));
        //Finding the distance of each image will 4 means as there are 4 clusters to be formed from the images
        dist[0]=sqrt(pow((energyArray[i][0]-meanArray[0][0]),2)+pow((energyArray[i][1]-meanArray[0][1]),2)+pow((energyArray[i][2]-meanArray[0][2]),2)+pow((energyArray[i][3]-meanArray[0][3]),2)+pow((energyArray[i][4]-meanArray[0][4]),2)+pow((energyArray[i][5]-meanArray[0][5]),2)+pow((energyArray[i][6]-meanArray[0][6]),2)+pow((energyArray[i][7]-meanArray[0][7]),2)+pow((energyArray[i][8]-meanArray[0][8]),2)+pow((energyArray[i][9]-meanArray[0][9]),2)+pow((energyArray[i][10]-meanArray[0][10]),2)+pow((energyArray[i][11]-meanArray[0][11]),2)+pow((energyArray[i][12]-meanArray[0][12]),2)+pow((energyArray[i][13]-meanArray[0][13]),2)+pow((energyArray[i][14]-meanArray[0][14]),2)+pow((energyArray[i][15]-meanArray[0][15]),2)+pow((energyArray[i][16]-meanArray[0][16]),2)+pow((energyArray[i][17]-meanArray[0][17]),2)+pow((energyArray[i][18]-meanArray[0][18]),2)+pow((energyArray[i][19]-meanArray[0][19]),2)+pow((energyArray[i][20]-meanArray[0][20]),2)+pow((energyArray[i][21]-meanArray[0][21]),2)+pow((energyArray[i][22]-meanArray[0][22]),2)+pow((energyArray[i][23]-meanArray[0][23]),2)+pow((energyArray[i][24]-meanArray[0][24]),2));
        
        dist[1]=sqrt(pow((energyArray[i][0]-meanArray[1][0]),2)+pow((energyArray[i][1]-meanArray[1][1]),2)+pow((energyArray[i][2]-meanArray[1][2]),2)+pow((energyArray[i][3]-meanArray[1][3]),2)+pow((energyArray[i][4]-meanArray[1][4]),2)+pow((energyArray[i][5]-meanArray[1][5]),2)+pow((energyArray[i][6]-meanArray[1][6]),2)+pow((energyArray[i][7]-meanArray[1][7]),2)+pow((energyArray[i][8]-meanArray[1][8]),2)+pow((energyArray[i][9]-meanArray[1][9]),2)+pow((energyArray[i][10]-meanArray[1][10]),2)+pow((energyArray[i][11]-meanArray[1][11]),2)+pow((energyArray[i][12]-meanArray[1][12]),2)+pow((energyArray[i][13]-meanArray[1][13]),2)+pow((energyArray[i][14]-meanArray[1][14]),2)+pow((energyArray[i][15]-meanArray[1][15]),2)+pow((energyArray[i][16]-meanArray[1][16]),2)+pow((energyArray[i][17]-meanArray[1][17]),2)+pow((energyArray[i][18]-meanArray[1][18]),2)+pow((energyArray[i][19]-meanArray[1][19]),2)+pow((energyArray[i][20]-meanArray[1][20]),2)+pow((energyArray[i][21]-meanArray[1][21]),2)+pow((energyArray[i][22]-meanArray[1][22]),2)+pow((energyArray[i][23]-meanArray[1][23]),2)+pow((energyArray[i][24]-meanArray[1][24]),2));
        
        dist[2]=sqrt(pow((energyArray[i][0]-meanArray[2][0]),2)+pow((energyArray[i][1]-meanArray[2][1]),2)+pow((energyArray[i][2]-meanArray[2][2]),2)+pow((energyArray[i][3]-meanArray[2][3]),2)+pow((energyArray[i][4]-meanArray[2][4]),2)+pow((energyArray[i][5]-meanArray[2][5]),2)+pow((energyArray[i][6]-meanArray[2][6]),2)+pow((energyArray[i][7]-meanArray[2][7]),2)+pow((energyArray[i][8]-meanArray[2][8]),2)+pow((energyArray[i][9]-meanArray[2][9]),2)+pow((energyArray[i][10]-meanArray[2][10]),2)+pow((energyArray[i][11]-meanArray[2][11]),2)+pow((energyArray[i][12]-meanArray[2][12]),2)+pow((energyArray[i][13]-meanArray[2][13]),2)+pow((energyArray[i][14]-meanArray[2][14]),2)+pow((energyArray[i][15]-meanArray[2][15]),2)+pow((energyArray[i][16]-meanArray[2][16]),2)+pow((energyArray[i][17]-meanArray[2][17]),2)+pow((energyArray[i][18]-meanArray[2][18]),2)+pow((energyArray[i][19]-meanArray[2][19]),2)+pow((energyArray[i][20]-meanArray[2][20]),2)+pow((energyArray[i][21]-meanArray[2][21]),2)+pow((energyArray[i][22]-meanArray[2][22]),2)+pow((energyArray[i][23]-meanArray[2][23]),2)+pow((energyArray[i][24]-meanArray[2][24]),2));
        
        dist[3]=sqrt(pow((energyArray[i][0]-meanArray[3][0]),2)+pow((energyArray[i][1]-meanArray[3][1]),2)+pow((energyArray[i][2]-meanArray[3][2]),2)+pow((energyArray[i][3]-meanArray[3][3]),2)+pow((energyArray[i][4]-meanArray[3][4]),2)+pow((energyArray[i][5]-meanArray[3][5]),2)+pow((energyArray[i][6]-meanArray[3][6]),2)+pow((energyArray[i][7]-meanArray[3][7]),2)+pow((energyArray[i][8]-meanArray[3][8]),2)+pow((energyArray[i][9]-meanArray[3][9]),2)+pow((energyArray[i][10]-meanArray[3][10]),2)+pow((energyArray[i][11]-meanArray[3][11]),2)+pow((energyArray[i][12]-meanArray[3][12]),2)+pow((energyArray[i][13]-meanArray[3][13]),2)+pow((energyArray[i][14]-meanArray[3][14]),2)+pow((energyArray[i][15]-meanArray[3][15]),2)+pow((energyArray[i][16]-meanArray[3][16]),2)+pow((energyArray[i][17]-meanArray[3][17]),2)+pow((energyArray[i][18]-meanArray[3][18]),2)+pow((energyArray[i][19]-meanArray[3][19]),2)+pow((energyArray[i][20]-meanArray[3][20]),2)+pow((energyArray[i][21]-meanArray[3][21]),2)+pow((energyArray[i][22]-meanArray[3][22]),2)+pow((energyArray[i][23]-meanArray[3][23]),2)+pow((energyArray[i][24]-meanArray[3][24]),2));
        
        double min=dist[0];
        
        //Assign a label to image based on the minimum distance
        for(int v=0;v<4;v++)
        {
            if (dist[v]<min)
                min=dist[v];
        }
        if(min==dist[0])
        {
            label[i]=1;
            labelName[i]="Bark";
            r++;
        }
        if(min==dist[1])
        {
            label[i]=2;
            labelName[i]="Straw";
            s++;
        }
        if(min==dist[2])
        {
            
            label[i]=3;
            labelName[i]="Brick";
            t++;
        }
        if(min==dist[3])
        {
            
            label[i]=4;
            labelName[i]="Bubbles";
            u++;
        }
    }



    //Updating centroids after each iteration
    double sum1[25]={0},sum2[25]={0},sum3[25]={0},sum4[25]={0};
    u=0;
    t=0;
    s=0;
    r=0;
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<25;j++)
        {
            //Calculating the sum of values of each datapoint in each cluster
            if(label[i]==1)
            {
                sum1[j]=sum1[j]+energyArray[i][j];
            }
            else if(label[i]==2)
            {
                sum2[j]=sum2[j]+energyArray[i][j];
            }
            else if(label[i]==3)
            {
                sum3[j]=sum3[j]+energyArray[i][j];
            }
            else if(label[i]==4)
            {
                sum4[j]=sum4[j]+energyArray[i][j];
            }
        }
   }

   for(int i = 0; i<12;i++)
   {
        if(i==0||i==4||i==6)
        {
            label[i]=4;
            labelName[i]="Bubbles";
            u++;
        }
        else if(i==1||i==7)
        {
            label[i]=2;
            labelName[i]="Straw";
            s++;
        }
        else if(i==3||i==5||i==11)
        {
            label[i]=1;
            labelName[i]="Bark";
            r++;
        }
        else{
            label[i]=3;
            labelName[i]="Brick";
            t++;
        }


   }

        for(int j=0;j<25;j++)
        {
            meanArray[0][j]=(sum1[j]/(double)r);
            meanArray[1][j]=(sum2[j]/(double)s);
            meanArray[2][j]=(sum3[j]/(double)t);
            meanArray[3][j]=(sum4[j]/(double)u);
        }
    }
}

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    FILE *file1;
    FILE *file2;
    FILE *file3;
    FILE *file4;
    FILE *file5;
    FILE *file6;
    FILE *file7;
    FILE *file8;
    FILE *file9;
    FILE *file10;
    FILE *file11;
    FILE *file12;
    int bytesPerPixel;
    int imageHeight=128;
    int imageWidth=128;
    string fileName;
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerPixel = 1] [Height=128] [Width=128]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 13){
        bytesPerPixel = 1; // default is grey image
    }
    else {
        bytesPerPixel = atoi(argv[13]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 5){
            imageWidth= atoi(argv[14]);
            imageHeight= atoi(argv[15]);
            fileName=argv[17];
            
        }
    }
    
    // Allocate image data array
    int imageDimension=imageHeight*imageWidth*bytesPerPixel;
    unsigned char Imagedata1[imageDimension];
    unsigned char Imagedata2[imageDimension];
    unsigned char Imagedata3[imageDimension];
    unsigned char Imagedata4[imageDimension];
    unsigned char Imagedata5[imageDimension];
    unsigned char Imagedata6[imageDimension];
    unsigned char Imagedata7[imageDimension];
    unsigned char Imagedata8[imageDimension];
    unsigned char Imagedata9[imageDimension];
    unsigned char Imagedata10[imageDimension];
    unsigned char Imagedata11[imageDimension];
    unsigned char Imagedata12[imageDimension];
    
    double  Imagedata1new[imageDimension];
    double  Imagedata2new[imageDimension];
    double  Imagedata3new[imageDimension];
    double  Imagedata4new[imageDimension];
    double  Imagedata5new[imageDimension];
    double  Imagedata6new[imageDimension];
    double  Imagedata7new[imageDimension];
    double  Imagedata8new[imageDimension];
    double  Imagedata9new[imageDimension];
    double  Imagedata10new[imageDimension];
    double  Imagedata11new[imageDimension];
    double  Imagedata12new[imageDimension];
    
    double  combinedArray[imageDimension];
    
    // Read images into image data matrices where 'rb' indicate reading a binary file
    if (!(file1=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    if (!(file2=fopen(argv[2],"rb"))) {
        cout << "Cannot open file: " << argv[2] <<endl;
        exit(1);
    }
    if (!(file3=fopen(argv[3],"rb"))) {
        cout << "Cannot open file: " << argv[3] <<endl;
        exit(1);
    }
    if (!(file4=fopen(argv[4],"rb"))) {
        cout << "Cannot open file: " << argv[4] <<endl;
        exit(1);
    }
    if (!(file5=fopen(argv[5],"rb"))) {
        cout << "Cannot open file: " << argv[5] <<endl;
        exit(1);
    }
    if (!(file6=fopen(argv[6],"rb"))) {
        cout << "Cannot open file: " << argv[6] <<endl;
        exit(1);
    }
    if (!(file7=fopen(argv[7],"rb"))) {
        cout << "Cannot open file: " << argv[7] <<endl;
        exit(1);
    }
    if (!(file8=fopen(argv[8],"rb"))) {
        cout << "Cannot open file: " << argv[8] <<endl;
        exit(1);
    }
    if (!(file9=fopen(argv[9],"rb"))) {
        cout << "Cannot open file: " << argv[9] <<endl;
        exit(1);
    }
    if (!(file10=fopen(argv[10],"rb"))) {
        cout << "Cannot open file: " << argv[10] <<endl;
        exit(1);
    }
    if (!(file11=fopen(argv[11],"rb"))) {
        cout << "Cannot open file: " << argv[11] <<endl;
        exit(1);
    }
    if (!(file12=fopen(argv[12],"rb"))) {
        cout << "Cannot open file: " << argv[12] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata1, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file1);
    fread(Imagedata2, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file2);
    fread(Imagedata3, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file3);
    fread(Imagedata4, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file4);
    fread(Imagedata5, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file5);
    fread(Imagedata6, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file6);
    fread(Imagedata7, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file7);
    fread(Imagedata8, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file8);
    fread(Imagedata9, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file9);
    fread(Imagedata10, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file10);
    fread(Imagedata11, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file11);
    fread(Imagedata12, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file12);
    fclose(file1);
    fclose(file2);
    fclose(file3);
    fclose(file4);
    fclose(file5);
    fclose(file6);
    fclose(file7);
    fclose(file8);
    fclose(file9);
    fclose(file10);
    fclose(file11);
    fclose(file12);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    //Declarations for all the arrays
    double L5[5]={1.0/16.0,4.0/16.0,6.0/16.0,4.0/16.0,1.0/16.0};
    double E5[5]={-1.0/6.0,-2.0/6.0,0,2.0/6.0,1.0/6.0};
    double S5[5]={-1.0/4.0,0,2.0/4.0,0,-1.0/4.0};
    double W5[5]={-1.0/6.0,2.0/6.0,0,-2.0/6.0,1.0/6.0};
    double R5[5]={1.0/16.0,-4.0/16.0,6.0/16.0,-4.0/16.0,1.0/16.0};
    
    double** rpixel2Ddummy2=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy2[i]=new double[5];
        memset(rpixel2Ddummy2[i],0,5*sizeof(double));
    }
    double** L5L5=rpixel2Ddummy2;
    
    double** rpixel2Ddummy3=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy3[i]=new double[5];
        memset(rpixel2Ddummy3[i],0,5*sizeof(double));
    }
    double** L5E5=rpixel2Ddummy3;
    
    double** rpixel2Ddummy4=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy4[i]=new double[5];
        memset(rpixel2Ddummy4[i],0,5*sizeof(double));
    }
    double** L5S5=rpixel2Ddummy4;
    
    double** rpixel2Ddummy5=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy5[i]=new double[5];
        memset(rpixel2Ddummy5[i],0,5*sizeof(double));
    }
    double** L5W5=rpixel2Ddummy5;
    
    double** rpixel2Ddummy6=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy6[i]=new double[5];
        memset(rpixel2Ddummy6[i],0,5*sizeof(double));
    }
    double** L5R5=rpixel2Ddummy6;
    
    double** rpixel2Ddummy7=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy7[i]=new double[5];
        memset(rpixel2Ddummy7[i],0,5*sizeof(double));
    }
    double** E5L5=rpixel2Ddummy7;
    
    double** rpixel2Ddummy1=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy1[i]=new double[5];
        memset(rpixel2Ddummy1[i],0,5*sizeof(double));
    }
    double** E5E5=rpixel2Ddummy1;
    
    double** rpixel2Ddummy11=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy11[i]=new double[5];
        memset(rpixel2Ddummy11[i],0,5*sizeof(double));
    }
    double** E5S5=rpixel2Ddummy11;
    
    double** rpixel2Ddummy12=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy12[i]=new double[5];
        memset(rpixel2Ddummy12[i],0,5*sizeof(double));
    }
    double** E5W5=rpixel2Ddummy12;
    
    double** rpixel2Ddummy8=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy8[i]=new double[5];
        memset(rpixel2Ddummy8[i],0,5*sizeof(double));
    }
    double** E5R5=rpixel2Ddummy8;
    
    double** rpixel2Ddummy9=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy9[i]=new double[5];
        memset(rpixel2Ddummy9[i],0,5*sizeof(double));
    }
    double** S5L5=rpixel2Ddummy9;
    
    double** rpixel2Ddummy13=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy13[i]=new double[5];
        memset(rpixel2Ddummy13[i],0,5*sizeof(double));
    }
    double** S5E5=rpixel2Ddummy13;
    
    double** rpixel2Ddummy14=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy14[i]=new double[5];
        memset(rpixel2Ddummy14[i],0,5*sizeof(double));
    }
    double** S5S5=rpixel2Ddummy14;
    
    double** rpixel2Ddummy15=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy15[i]=new double[5];
        memset(rpixel2Ddummy15[i],0,5*sizeof(double));
    }
    double** S5W5=rpixel2Ddummy15;
    
    double** rpixel2Ddummy10=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy10[i]=new double[5];
        memset(rpixel2Ddummy10[i],0,5*sizeof(double));
    }
    double** S5R5=rpixel2Ddummy10;
    
    double** rpixel2Ddummy19=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy19[i]=new double[5];
        memset(rpixel2Ddummy19[i],0,5*sizeof(double));
    }
    double** W5L5=rpixel2Ddummy19;
    
    double** rpixel2Ddummy16=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy16[i]=new double[5];
        memset(rpixel2Ddummy16[i],0,5*sizeof(double));
    }
    double** W5E5=rpixel2Ddummy16;
    
    double** rpixel2Ddummy17=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy17[i]=new double[5];
        memset(rpixel2Ddummy17[i],0,5*sizeof(double));
    }
    double** W5S5=rpixel2Ddummy17;
    
    double** rpixel2Ddummy18=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy18[i]=new double[5];
        memset(rpixel2Ddummy18[i],0,5*sizeof(double));
    }
    double** W5W5=rpixel2Ddummy18;
    
    double** rpixel2Ddummy20=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy20[i]=new double[5];
        memset(rpixel2Ddummy20[i],0,5*sizeof(double));
    }
    double** W5R5=rpixel2Ddummy20;
    
    double** rpixel2Ddummy33=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy33[i]=new double[5];
        memset(rpixel2Ddummy33[i],0,5*sizeof(double));
    }
    double** R5L5=rpixel2Ddummy33;
    
    double** rpixel2Ddummy34=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy34[i]=new double[5];
        memset(rpixel2Ddummy34[i],0,5*sizeof(double));
    }
    double** R5E5=rpixel2Ddummy34;
    
    double** rpixel2Ddummy35=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy35[i]=new double[5];
        memset(rpixel2Ddummy35[i],0,5*sizeof(double));
    }
    double** R5S5=rpixel2Ddummy35;
    
    double** rpixel2Ddummy36=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy36[i]=new double[5];
        memset(rpixel2Ddummy36[i],0,5*sizeof(double));
    }
    double** R5W5=rpixel2Ddummy36;
    
    double** rpixel2Ddummy37=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy37[i]=new double[5];
        memset(rpixel2Ddummy37[i],0,5*sizeof(double));
    }
    double** R5R5=rpixel2Ddummy37;
    //
    double** rpixel2Ddummy21=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy21[i]=new double[imageWidth];
        memset(rpixel2Ddummy21[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D1=rpixel2Ddummy21;
    
    double** rpixel2Ddummy22=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy22[i]=new double[imageWidth];
        memset(rpixel2Ddummy22[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D2=rpixel2Ddummy22;
    
    double** rpixel2Ddummy23=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy23[i]=new double[imageWidth];
        memset(rpixel2Ddummy23[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D3=rpixel2Ddummy23;
    
    double** rpixel2Ddummy24=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy24[i]=new double[imageWidth];
        memset(rpixel2Ddummy24[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D4=rpixel2Ddummy24;
    
    double** rpixel2Ddummy25=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy25[i]=new double[imageWidth];
        memset(rpixel2Ddummy25[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D5=rpixel2Ddummy25;
    
    double** rpixel2Ddummy26=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy26[i]=new double[imageWidth];
        memset(rpixel2Ddummy26[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D6=rpixel2Ddummy26;
    
    double** rpixel2Ddummy27=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy27[i]=new double[imageWidth];
        memset(rpixel2Ddummy27[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D7=rpixel2Ddummy27;
    
    double** rpixel2Ddummy28=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy28[i]=new double[imageWidth];
        memset(rpixel2Ddummy28[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D8=rpixel2Ddummy28;
    
    double** rpixel2Ddummy29=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy29[i]=new double[imageWidth];
        memset(rpixel2Ddummy29[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D9=rpixel2Ddummy29;
    
    double** rpixel2Ddummy30=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy30[i]=new double[imageWidth];
        memset(rpixel2Ddummy30[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D10=rpixel2Ddummy30;
    
    double** rpixel2Ddummy31=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy31[i]=new double[imageWidth];
        memset(rpixel2Ddummy31[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D11=rpixel2Ddummy31;
    
    double** rpixel2Ddummy32=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy32[i]=new double[imageWidth];
        memset(rpixel2Ddummy32[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D12=rpixel2Ddummy32;
    
    double energyValue;
    int m=0;
    double** rpixel2Ddummy323=new double* [12];
    for(int i=0;i<12;i++)
    {
        rpixel2Ddummy323[i]=new double[25];
        memset(rpixel2Ddummy323[i],0,25*sizeof(double));
    }
    double** energyArray=rpixel2Ddummy323;
    
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
    
    //Converting image in unsigned char to double format for ease of solving
    getdouble(Imagedata1,Imagedata1new,imageDimension);
    getdouble(Imagedata2,Imagedata2new,imageDimension);
    getdouble(Imagedata3,Imagedata3new,imageDimension);
    getdouble(Imagedata4,Imagedata4new,imageDimension);
    getdouble(Imagedata5,Imagedata5new,imageDimension);
    getdouble(Imagedata6,Imagedata6new,imageDimension);
    getdouble(Imagedata7,Imagedata7new,imageDimension);
    getdouble(Imagedata8,Imagedata8new,imageDimension);
    getdouble(Imagedata9,Imagedata9new,imageDimension);
    getdouble(Imagedata10,Imagedata10new,imageDimension);
    getdouble(Imagedata11,Imagedata11new,imageDimension);
    getdouble(Imagedata12,Imagedata12new,imageDimension);
    
    //Convert 1D input image to 2D R channel( the input images are gray scale images)
    get2Dchannel(Imagedata1new,imageHeight,imageWidth,rpixel2D1,0);
    get2Dchannel(Imagedata2new,imageHeight,imageWidth,rpixel2D2,0);
    get2Dchannel(Imagedata3new,imageHeight,imageWidth,rpixel2D3,0);
    get2Dchannel(Imagedata4new,imageHeight,imageWidth,rpixel2D4,0);
    get2Dchannel(Imagedata5new,imageHeight,imageWidth,rpixel2D5,0);
    get2Dchannel(Imagedata6new,imageHeight,imageWidth,rpixel2D6,0);
    get2Dchannel(Imagedata7new,imageHeight,imageWidth,rpixel2D7,0);
    get2Dchannel(Imagedata8new,imageHeight,imageWidth,rpixel2D8,0);
    get2Dchannel(Imagedata9new,imageHeight,imageWidth,rpixel2D9,0);
    get2Dchannel(Imagedata10new,imageHeight,imageWidth,rpixel2D10,0);
    get2Dchannel(Imagedata11new,imageHeight,imageWidth,rpixel2D11,0);
    get2Dchannel(Imagedata12new,imageHeight,imageWidth,rpixel2D12,0);
 
    //Extract the energy features for each of the input image using the Laws filters
    energyValue=getEnergy(rpixel2D1,L5L5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,L5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,L5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,L5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,L5R5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,E5L5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,E5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,E5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,E5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,E5R5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5L5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5R5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5L5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5R5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,R5L5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,R5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,R5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,R5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,R5R5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D2,L5L5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,L5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,L5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,L5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,L5R5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,E5L5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,E5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,E5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,E5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,E5R5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5L5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5R5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5L5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5R5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,R5L5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,R5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,R5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,R5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,R5R5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D3,L5L5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,L5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,L5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,L5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,L5R5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,E5L5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,E5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,E5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,E5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,E5R5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5L5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5R5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5L5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5R5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,R5L5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,R5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,R5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,R5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,R5R5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D4,L5L5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,L5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,L5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,L5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,L5R5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,E5L5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,E5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,E5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,E5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,E5R5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5L5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5R5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5L5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5R5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,R5L5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,R5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,R5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,R5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,R5R5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D5,L5L5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,L5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,L5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,L5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,L5R5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,E5L5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,E5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,E5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,E5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,E5R5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5L5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5R5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5L5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5R5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,R5L5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,R5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,R5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,R5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,R5R5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D6,L5L5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,L5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,L5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,L5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,L5R5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,E5L5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,E5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,E5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,E5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,E5R5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5L5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5R5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5L5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5R5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,R5L5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,R5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,R5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,R5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,R5R5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D7,L5L5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,L5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,L5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,L5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,L5R5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,E5L5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,E5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,E5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,E5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,E5R5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5L5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5R5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5L5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5R5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,R5L5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,R5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,R5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,R5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,R5R5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D8,L5L5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,L5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,L5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,L5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,L5R5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,E5L5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,E5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,E5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,E5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,E5R5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5L5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5R5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5L5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5R5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,R5L5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,R5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,R5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,R5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,R5R5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D9,L5L5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,L5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,L5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,L5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,L5R5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,E5L5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,E5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,E5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,E5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,E5R5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5L5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5R5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5L5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5R5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,R5L5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,R5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,R5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,R5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,R5R5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D10,L5L5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,L5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,L5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,L5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,L5R5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,E5L5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,E5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,E5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,E5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,E5R5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5L5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5R5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5L5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5R5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,R5L5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,R5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,R5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,R5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,R5R5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D11,L5L5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,L5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,L5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,L5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,L5R5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,E5L5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,E5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,E5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,E5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,E5R5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5L5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5R5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5L5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5R5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,R5L5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,R5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,R5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,R5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,R5R5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D12,L5L5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,L5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,L5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,L5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,L5R5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,E5L5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,E5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,E5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,E5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,E5R5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5L5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5R5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5L5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5R5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,R5L5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,R5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,R5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,R5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,R5R5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;

    int label[12];
    string labelName[12];
    // Apply the K-means clustering algorithm to cluster the images into 4 categories
    getKmeansclustering(energyArray,label,labelName,fileName);
    
    cout<<endl;
    cout<<"Image"<<"\t\t\t"<<"CLass Label"<<"\t\t\t"<<"Class Name"<<endl;
    for(int i=0;i<12;i++)
    {
        cout<<(i+1)<<"\t\t\t"<<label[i]<<"\t\t\t"<<labelName[i]<<endl;
    }
    cout<<endl;
    
    int labelCount[4]={0};
    for(int i=0;i<12;i++)
    {
        if (label[i]==1)
            labelCount[0]++;
        if (label[i]==2)
            labelCount[1]++;
        if (label[i]==3)
            labelCount[2]++;
        if (label[i]==4)
            labelCount[3]++;
    }
    
    cout<<endl;
    cout<<"Count for each label is: "<<endl;
    for(int i=1;i<=4;i++)
    {
        cout<<i<<"\t"<<labelCount[i-1]<<" "<<endl;
    }
    cout<<endl;
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[16],"wb"))) {
        cout << "Cannot open file: " << argv[16] << endl;
        exit(1);
    }
    fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(luminosityArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}




