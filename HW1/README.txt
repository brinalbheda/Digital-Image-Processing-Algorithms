#EE 569 - Homework Assignment #1
#Date - 01/22/2019
#Name - Brinal Chimanlal Bheda
#USC ID - 8470655102
#Email - bbheda@usc.edu

Version of OS - macOS Mojave (version 10.14.2)
IDE - Visual Studio Code for C++
Compiler- g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

-->How to run the codes?
1.  Open terminal.
2.  Go to the directory where the source code files are stored along with the input image files in .cpp and .raw format respectively
2a. cd "path"
3.  Compile the codes using:
3a. g++ -o program source_code.cpp
4.  Run and execute the code and the output images are generated and stored in the same folder as the source codes and input images.
4a. ./program input_image.raw output_image.raw 1 or ./program input_image.raw output_image.raw 3
4b. Last argument denotes the bytes_per_pixel

-->How to choose arguments for compilation?
input_image.raw output_image.raw bytes_per_pixel
bytes_per_pixel is 1 for grayscale image and 2 for color images

-->File name of output images
output_image.raw

