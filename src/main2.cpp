// #include "Volume.h"
#include "Image.h"
#include <iostream>
#include <string>

int main() {
    // 3D OPERATIONS EXAMPLES
    // Volume volume;


    // Two filters Examples

    //Check Gaussian Filter
    // volume.loadImages("../Image/");
    // volume.applyGaussianFilter(3, 1.0);
    // volume.saveImages("../Gua/");

    //Check Median Filter
    // volume.loadImages("../Image/");
    // volume.applyMedianFilter(3);
    // volume.saveImages("../Med/");



    // All three projections Examples

    // Check Maximum Intensity Projection
    // volume.loadImages("../Image/");
    // volume.MaxProjection("qweqwe/123/MIP_result.png");

    // Check Minimum Intensity Projection
    // volume.loadImages("../Image/");
    // volume.MinProjection("MinIP_result.png");

    // Check Average Intensity Projection
    // volume.loadImages("../Image/");
    // volume.AverageProjection("AIP_result.png");

    // Check Average Intensity Projection with Median
    // volume.loadImages("../Image/");
    // volume.AverageProjectionMedian("AIP_Median_result.png");



    // Slice Example

    // volume.loadImages("../Image/");
    // volume.slice3DVolume(SlicePlane::YZ, 400, "123/slice_yz_x400__new.png");
    // volume.slice3DVolume(SlicePlane::XZ, 420, "slice_xz_y420__new.png");



    // Example to generate:
    // average intensity projection, 3x3x3 gaussian filter
    // volume.loadImages("../Image/");
    // volume.applyGaussianFilter(3, 1.0);
    // volume.AverageProjection("AIP_Gaussian_result.png");




    // 2D OPERATIONS EXAMPLES
    Image image;


    // Five Color correction filters Examples

    // Check Grayscale
    // image.loadImage("../Images/gracehopper.png");
    // image.Grayscale();
    // image.saveImage("223/test_grayscale.png");

    // Check Brightness
    // image.loadImage("../Images/gracehopper.png");
    // image.Brightness(50);
    // image.saveImage("test_brightness.png");

    // Check Salt and Pepper Noise
    // image.loadImage("../Images/gracehopper.png");
    // image.SaltAndPepper(10);// means 10% of the pixels will be affected
    // image.saveImage("test_salt_and_pepper_noise.png");

    //!!!!!!!!!!!!!!!!!Need to be fixed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Check Threshold
    // image.loadImage("../Images/gracehopper.png");
    // image.Threshold(100);
    // image.saveImage("test_threshold.png");

    //!!!!!!!!!!!!!!!!!Need to be fixed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Check Histogram Equalization
    // image.loadImage("../Images/gracehopper.png");
    // image.HistogramEqualization(true);
    // image.saveImage("test_histogram_equalization.png");



    // Three Image blur filters Examples
    
    // Check Gaussian Filter
    // image.loadImage("../Images/gracehopper.png");
    // image.GaussianFilter(3, 1.0);
    // image.saveImage("test_gaussian_filter.png");

    // Check Median Filter
    // image.loadImage("../Images/gracehopper.png");
    // image.MedianFilter(3);
    // image.saveImage("test_median_filter.png");

    // Check Box Filter
    // image.loadImage("../Images/gracehopper.png");
    // image.boxFilter(3);
    // image.saveImage("test_box_filter.png");


    // Combine the two filters
    // image.loadImage("../Images/gracehopper.png");
    // image.SaltAndPepper(10);
    // image.MedianFilter(3);
    // image.saveImage("test_salt_and_pepper_median_filter.png");


    // NEED TO BE FIXED!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // // Four Edge detection filters Examples

    // // Check Sobel Detection
    // image.loadImage("../Images/gracehopper.png");
    // image.Grayscale();
    // image.GaussianFilter(3, 1.0);
    // image.sobelDetection();
    // image.saveImage("test_sobel_detection.png");

    // // Check Prewitt Detection
    // image.loadImage("../Images/gracehopper.png");
    // image.Grayscale();
    // image.prewittDetection();
    // image.saveImage("test_prewitt_detection.png");

    // // Check Scharr Detection
    // image.loadImage("../Images/gracehopper.png");
    // image.Grayscale();
    // image.scharrDetection();
    // image.saveImage("test_scharr_detection.png");

    // // Check Roberts Cross Detection
    // image.loadImage("../Images/gracehopper.png");
    // image.Grayscale();
    // image.robertsCrossDetection();
    // image.saveImage("test_roberts_cross_detection.png");


    
    Image img;
    img.loadImage("../Images/gracehopper.png");
    img.Grayscale();
    img.GaussianFilter(5,1.0);
    img.sobelDetection();
    img.saveImage("../Output/test.png");
    return 0;
}
