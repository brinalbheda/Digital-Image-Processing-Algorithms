%EE 569: Homework 2 - By Brinal Bheda
%USC ID - 8470655102
%Email - bbheda@usc.edu
%Submission Date - 02/12/2019


% Open the file where the input image is stored by providing the path name, and read the input .jpg image by using imread()
% Convert the rgb image into the gray scale image
% Apply the canny edge detector using the edge() function
% Edge() function inputs the parameters gray scale input image, name of the edge detector being used i.e. Canny, threshold applied to the image to display the edge map i.e. 0.05, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80
% Display the output of the image showing the edge map using imshow() Write the output in the Canny output image and store it in the same folder naming output_canny for different thresholds using imwrite()

% Filter image with the derivative of Gaussian
% Find magnitude and orientation of gradient
% Non-max suppression (NMS) - performs edge thinning
% Double Thresholding (Hysteresis Thresholding) - by selecting high and low threshold values
% If an edge pixel’s gradient value is higher than the high threshold value, it is marked as a strong edge pixel. If an edge pixel’s gradient value is smaller than the high threshold value and larger than the low threshold value, it is marked as a weak edge pixel. If an edge pixels value is smaller than the low threshold value, it will be suppressed.


%Canny edge detector

%load and read the input tiger and pig image from the path
filename = '/Users/brinalbheda/Desktop/DIP/HW2/1b/Pig.jpg';
I = imread(filename);
%imshow(I)

%convert the rgb image to gray image
gray_I = rgb2gray(I);
%imshow(gray_I)

%apply the canny edge function on the gray image for different thresholds
canny_I = edge(gray_I,'Canny',0.20);

%display the final output image
figure, imshow(canny_I)

