# Advanced Programming Group Project
Advanced Programming course group project assessment. Build a C++ program to apply a range of image filters and orthographic projections to an input 2D image or 3D data volume.

# CT Scans
Download CT Scan datasets here:
https://imperiallondon-my.sharepoint.com/:u:/g/personal/tmd02_ic_ac_uk/EafXMuNsbcNGnRpa8K62FjkBvIKvCswl1riz7hPDHpHdSQ

# LICENSE
MIT License

Copyright (c) 2024 ese-msc-2023

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

# Repository Structure

This repository contains the following files and directories:

- `documentation/html`: Contains HTML documentation.
- `executables`: Contains executables for both Mac users and Windows users.
- `src`: Source code files for the project.
- `test`: Contains test framework.
- `README.md`: The README file for the repository with instructions and information.

# Installation and Tests
## Install
Compile the main user interface.
```
cd src
g++ -std=c++17 -o project Filter.cpp Slice.cpp Projection.cpp Volume.cpp Image.cpp main.cpp
```

Run the project
```
./project
```
## Run the existed executables
For Mac users:
```
cd executables
./MacOS_project
```
Please do not run by click it directly!
## Test
Important: 
1. All the functionalities are implement in three classes: Filer, Projection, and Slice. As a result, only functions in these classes are tested. Other Class(Volume and Image) is designed as the interface for the main.cpp applying Facade design pattern. These interfaces are not going to be tested here.
2. Please note that the test is a MacOS exclusive feature.

Compile the test framework.
```
cd test
g++ -std=c++17 -o test ../src/Slice.cpp ../src/Projection.cpp ../src/Filter.cpp TestSlice.cpp TestProjection.cpp TestFilter.cpp mainTest.cpp
```

Run the test
```
./test
```

# User Instruction
## 1. START
### 1.1 Start - model choose
```
'3d' for 3D model
'2d' for 2D model
>>>2d
```
You will need to choose the model that you want to use first. Including 2d model to deal with 2d image operations and 3d model to deal with 3d images operations. And you will also be able to change the model after you choosing one.
### 1.2 Start - Upload data
```
Please add a photo(path) to start the canvas.
>>>../Images/gracehopper.png

Image loaded successfully.
```
For user who choose 2d model. You will need to enter the relative path to the image that you want to operate.
## 2. MENU
### 2.1 2D MENU
If you are in the 2d model, then you can see the following menu
```
============================MODEL2D MENU================================
|             For the current loaded Image:                            |
|             ENTER 'check' to look the data path & data info.         |
|             ENTER 'reload' to reload the image.                      |
|             ENTER 'save' to save the current image.                  |
|             ENTER '3d' to change to 3d model.                        |
|             ENTER 'exit' to quit the project.                        |
|----------------------------------------------------------------------|
|             ENTER 'gray' to apply grayscale.                         |
|             ENTER 'bright' to apply brightness.                      |
|             ENTER 'hist' to apply Histogram equalisation.            |
|             ENTER 'salt' to apply salt and pepper noise.             |
|             ENTER 'threshold' to apply threshold.                    |
|----------------------------------------------------------------------|
|             ENTER 'gaussian' to apply GaussianFilter.                |
|             ENTER 'median' to apply MedianFilter.                    |
|             ENTER 'box' to apply boxFilter.                          |
|----------------------------------------------------------------------|
|             ENTER 'sobel' to apply sobelDetection.                   |
|             ENTER 'prewitt' to apply prewittDetection.               |
|             ENTER 'scharr' to apply scharrDetection.                 |
|             ENTER 'roberts' to apply robertsCrossDetection.          |
========================================================================
```
#### 2.1.1 2D - check operation
Enter the "check" command, then you can see the information of the current loaded data.
```
>>>check

The current data path is: ../Images/gracehopper.png
The size of the image is: 512 * 600 * 4
```
#### 2.1.2 2D - reload
Enter the "reload" command, and then you can reload one image to replace the image that you current loaded to the program.
```
>>>reload

Please add a photo(path) to start the canvas.
>>>../images/tienshan.png

Image loaded successfully.
```
#### 2.1.3 2D - save
Enter the "save" command, then you can save the image that current in this program to the path that you specified.
```
>>>save

Please determine the output path for the current image.
>>>../myOutput/example.png

Image saved successfully.
```
#### 2.1.4 2D - 3d
Enter the "3d" command, then you will change to the 3d model. If you have not loaded data to the 3d model, it will ask you to enter the data folder that you want to load.
```
>>>3d

Please add a folder path to start the canvas.
>>>../confuciusornis
Loading images from directory: ../confuciusornis
Loaded image: "../confuciusornis/confuYZ0001.png"
Loaded image: "../confuciusornis/confuYZ0002.png"
Loaded image: "../confuciusornis/confuYZ0003.png"
...
Loaded image: "../confuciusornis/confuYZ0265.png"

Folder path loaded successfully.
```
Then you can see the 3d model menu.
```
============================MODEL3D MENU================================
|             For the current loaded Image:                            |
|             ENTER 'check' to look the data path & data info.         |
|             ENTER 'reload' to reload the image set.                  |
|             ENTER 'save' to save the current image set.              |
|             ENTER '2d' to change to 2d model.                        |
|             ENTER 'exit' to quit the project.                        |
|----------------------------------------------------------------------|
|             ENTER 'gaussian' to apply 3D GaussianFilter.             |
|             ENTER 'median' to apply 3D MedianFilter.                 |
|----------------------------------------------------------------------|
|             ENTER 'max' to apply MaxProjection.                      |
|             ENTER 'min' to apply MinProjection.                      |
|             ENTER 'average' to apply AverageProjection.              |
|             ENTER 'amedian' to apply AverageProjection with median.  |
|----------------------------------------------------------------------|
|             ENTER 'slice' to apply slicing operation.                |
========================================================================
```
#### 2.1.5 2D - exit
Enter "exit" command, then the program will quit.
#### 2.1.6 2D - gray
Enter "gray" command, then grayscale filter will be applied to the image saved in 2d model and replace the image.
```
>>>gray

Execution Time: 36741 microseconds

Apply grayscale successfully.
```
#### 2.1.7 2D - bright
Enter "bright" command, you will be asked to enter the value of brightness. Then brightness filter will be applied to the image saved in 2d model and replace the image.
```
>>>bright

Please enter the brightness value.
>>>50

Execution Time: 41412 microseconds

Apply brightness successfully.
```
#### 2.1.8 2D - hist
Enter "hist" command, you will be asked to enter whether to use HSL or not by enter 'n' or 'y'. Then histogram filter will be applied to the image saved in 2d model and replace the image.
```
>>>hist

Please enter whether to use HSL or not (y/n).
>>>y

Execution Time: 36468 microseconds

Apply histogram equalisation successfully.
```
#### 2.1.9 2D - salt
Enter "salt" command, you will be asked to enter the percentage of salt and pepper noise. For example, '10' for 10%. Then salt and pepper filter will be applied to the image saved in 2d model and replace the image.
```
>>>salt

Please enter the percentage of salt and pepper noise.
>>>10

Execution Time: 50564 microseconds

Apply salt and pepper noise successfully.
```
#### 2.1.10 2D - threshold
Enter "threshold" command, you will be asked to enter the threshold value and whether to use HsL. Then the threshold filter will be applied to the image saved in 2d model and replace the image.
```
>>>threshold

Please enter the threshold value.
>>>10

Please enter whether to use HSL or not (y/n).
>>>y

Execution Time: 23802 microseconds

Apply threshold successfully.
```
#### 2.1.11 2D - gaussian
Enter "gaussian" command, you will be asked to enter the kernal size. For example, enter '3' for 3*3 kernal. And you will also be asked to enter the sigma value, for example enter '2.0'. Then the gaussian blur will be applied to the image saved in 2d model and replace the image.
```
>>>gaussian

Please enter the filter size(>>>3 for size 3*3 kernal).
>>>3

Please enter the sigma value.
>>>2.0

Execution Time: 491210 microseconds

Apply Gaussian filter successfully.
```
#### 2.1.12 2D - median
Enter "median" command, you will be asked to enter the kernal size. For example, enter '3' for 3*3 kernal. Then the median blur will be applied to the image saved in 2d model and replace the image.
```
>>>median

Please enter the filter size(>>>3 for 3*3 size kernal).
>>>3

Execution Time: 1659028 microseconds

Apply Median filter successfully.
```
#### 2.1.13 2D - box
Enter "box" command, you will be asked to enter the kernal size. For example, enter '3' for 3*3 kernal. Then the box blur will be applied to the image saved in 2d model and replace the image.
```
>>>box

Please enter the filter size(>>>3 for size 3*3 kernal).
>>>3

Execution Time: 378615 microseconds

Apply Box filter successfully.
```
#### 2.1.14 2D - sobel
Enter "sobal" command, the sobel edge detection will be applied to the image saved in 2d model and replace the image.
```
>>>sobel

Execution Time: 1045827 microseconds

Apply Sobel detection successfully.
```
#### 2.1.15 2D - prewitt
Enter "prewitt" command, the prewitt edge detection will be applied to the image saved in 2d model and replace the image.
```
>>>prewitt

Execution Time: 1065161 microseconds

Apply Prewitt detection successfully.
```
#### 2.1.16 2D - scharr
Enter "scharr" command, the scharr edge detection will be applied to the image saved in 2d model and replace the image.
```
>>>scharr

Execution Time: 1069422 microseconds

Apply Scharr detection successfully.
```
#### 2.1.17 2D - roberts
Enter "roberts" command, the roberts edge detection will be applied to the image saved in 2d model and replace the image.
```
>>>roberts

Execution Time: 66343 microseconds

Apply Roberts Cross detection successfully.
```
### 2.2 3D MENU
If you are in the 3d model, then you can see the following menu
```
============================MODEL3D MENU================================
|             For the current loaded Image:                            |
|             ENTER 'check' to look the data path & data info.         |
|             ENTER 'reload' to reload the image set.                  |
|             ENTER 'save' to save the current image set.              |
|             ENTER '2d' to change to 2d model.                        |
|             ENTER 'exit' to quit the project.                        |
|----------------------------------------------------------------------|
|             ENTER 'gaussian' to apply 3D GaussianFilter.             |
|             ENTER 'median' to apply 3D MedianFilter.                 |
|----------------------------------------------------------------------|
|             ENTER 'max' to apply MaxProjection.                      |
|             ENTER 'min' to apply MinProjection.                      |
|             ENTER 'average' to apply AverageProjection.              |
|             ENTER 'amedian' to apply AverageProjection with median.  |
|----------------------------------------------------------------------|
|             ENTER 'slice' to apply slicing operation.                |
========================================================================
```
#### 2.2.1 3D - check operation
Enter the "check" command, then you can see the information of the current loaded data.
```
>>>check

The current data path is: ../confuciusornis
The size of the image set is: 265
The size of single image is: 996 * 1295 * 1
```
#### 2.2.2 3D - reload
Enter the "reload" command, and then you can reload one image to replace the image that you current loaded to the program.
```
>>>reload

Please add a folder path to start the canvas.
>>>../confuciusornis
Loading images from directory: ../confuciusornis
Loaded image: "../confuciusornis/confuYZ0001.png"
Loaded image: "../confuciusornis/confuYZ0002.png"
Loaded image: "../confuciusornis/confuYZ0003.png"
...
Loaded image: "../confuciusornis/confuYZ0265.png"

Folder path loaded successfully.
```
#### 2.2.3 3D - save
Enter the "save" command, then you can save the the 3D images that current in this program to the folder path that you specified.
```
>>>save

Please determine the output folder path for the current images.
>>>../examplefolder/
Saving image 0...
Saving image 1...
Saving image 2...
...
Saving image 264...

Images saved successfully.
```
#### 2.2.4 3D - 2d
Enter the "2d" command, then you will change to the 2d model. If you have not loaded data to the 2d model, it will ask you to enter the image path that you want to load.
```
>>>2d

Please add a photo(path) to start the canvas.
>>>../images/gracehopper.png

Image loaded successfully.
```
Then you can see the 2d model menu.
```
============================MODEL2D MENU================================
|             For the current loaded Image:                            |
|             ENTER 'check' to look the data path & data info.         |
|             ENTER 'reload' to reload the image.                      |
|             ENTER 'save' to save the current image.                  |
|             ENTER '3d' to change to 3d model.                        |
|             ENTER 'exit' to quit the project.                        |
|----------------------------------------------------------------------|
|             ENTER 'gray' to apply grayscale.                         |
|             ENTER 'bright' to apply brightness.                      |
|             ENTER 'hist' to apply Histogram equalisation.            |
|             ENTER 'salt' to apply salt and pepper noise.             |
|             ENTER 'threshold' to apply threshold.                    |
|----------------------------------------------------------------------|
|             ENTER 'gaussian' to apply GaussianFilter.                |
|             ENTER 'median' to apply MedianFilter.                    |
|             ENTER 'box' to apply boxFilter.                          |
|----------------------------------------------------------------------|
|             ENTER 'sobel' to apply sobelDetection.                   |
|             ENTER 'prewitt' to apply prewittDetection.               |
|             ENTER 'scharr' to apply scharrDetection.                 |
|             ENTER 'roberts' to apply robertsCrossDetection.          |
========================================================================
```
#### 2.2.5 3D - exit
Enter "exit" command, then the program will quit.
#### 2.2.6 3D - gaussian
Enter "gaussian" command, you will be asked to enter the kernal size. For example, enter '3' for 3*3*3 kernal. And you will also be asked to enter the sigma value, for example enter '2.0'. Then the 3D gaussian filter will be applied to the image set saved in 3d model and replace the images. The following is an example to apply 3D gaussian filter to the 10 images dataset.
```
>>>gaussian

Please enter the filter size(>>>3 for 3*3*3 size kernal).
>>>3

Please enter the sigma value.
>>>2.0
Applying 3D Gaussian filter...
Kernel created
Processing filter at index: 0...
Processing filter at index: 1...
Processing filter at index: 2...
...
Processing filter at index: 9...
3D Gaussian filter applied

Execution Time: 9384942 microseconds

Apply Gaussian filter successfully.
```
#### 2.2.7 3D - median
Enter "median" command, you will be asked to enter the kernal size. For example, enter '3' for 3*3*3 kernal. Then the 3D median filter will be applied to the image set saved in 3d model and replace the images dataset.
```
>>>median

Please enter the filter size(>>>3 for 3*3*3 size kernal).
>>>3
Processing filter at index: 0...
Processing filter at index: 1...
Processing filter at index: 2...
...
Processing filter at index: 9...
3D Median filter applied

Execution Time: 24770599 microseconds

Apply Median filter successfully.
```
#### 2.2.8 3D - max projection
Enter "max" command, you will be asked to enter whether to manually enter the start and end index. If you enter 'n', it will jump this step. If you enter 'y', you need to enter the start index and end index:
```
>>>max

Do you want to manually enter the start and end index? (y/n)
>>>y

Please enter the start index.
>>>3

Please enter the end index.
>>>6
```
Then you will be asked to enter the file path to generate the result. And then the maximum intensity projection will be applied to the image set saved in 3d model and the result will be saved in the give path.
```
Please enter the output image path for the Maximum Intensity Projection.
>>>../exampleOutput/example.png

Execution Time: 192775 microseconds

Projection generated successfully.
```
#### 2.2.8 3D - min projection
Enter "min" command, you will be asked to enter whether to manually enter the start and end index. If you enter 'n', it will jump this step. If you enter 'y', you need to enter the start index and end index:
```
>>>min

Do you want to manually enter the start and end index? (y/n)
>>>y

Please enter the start index.
>>>3

Please enter the end index.
>>>6
```
Then you will be asked to enter the file path to generate the result. And then the minimum intensity projection will be applied to the image set saved in 3d model and the result will be saved in the give path.
```
Please enter the output image path for the Minimum Intensity Projection.
>>>../exampleOutput/example2.png

Execution Time: 201425 microseconds

Projection generated successfully.
```
#### 2.2.9 3D - average projection
Enter "average" command, you will be asked to enter whether to manually enter the start and end index. If you enter 'n', it will jump this step. If you enter 'y', you need to enter the start index and end index:
```
>>>average

Do you want to manually enter the start and end index? (y/n)
>>>y

Please enter the start index.
>>>3

Please enter the end index.
>>>6
```
Then you will be asked to enter the file path to generate the result. And then the average intensity projection will be applied to the image set saved in 3d model and the result will be saved in the give path.
```
Please enter the output image path for the Average Intensity Projection.
>>>../exampleOutput/example3.png

Execution Time: 173371 microseconds

Projection generated successfully.
```
#### 2.2.10 3D - average projection with median
Enter "amedian" command, you will be asked to enter whether to manually enter the start and end index. If you enter 'n', it will jump this step. If you enter 'y', you need to enter the start index and end index:
```
>>>amedian

Do you want to manually enter the start and end index? (y/n)
>>>y

Please enter the start index.
>>>3

Please enter the end index.
>>>6
```
Then you will be asked to enter the file path to generate the result. And then the average intensity projection with median will be applied to the image set saved in 3d model and the result will be saved in the give path.
```
Please enter the output image path for the Average Intensity Projection with Median.
>>>../exampleOutput/example4.png

Execution Time: 981891 microseconds

Projection generated successfully.
```
#### 2.2.11 3D - slice
Enter "slice" command, you will be asked with plane that you want to apply the slice operation. Enter "YZ" or "XZ" for different plane. Then you will be asked the index to applying slice operation and the path to save the result image. Then the slice operation will be applied to the image set saved in 3d model and the result will be saved in the give path.
```
>>>slice

Please enter the slice plane (YZ, XZ).
>>>YZ

Please enter the slice index.
>>>400

Please enter the output image path for the slice.
>>>../exampleOutput/slice.png

Execution Time: 9335 microseconds

Slice generated successfully.
```
