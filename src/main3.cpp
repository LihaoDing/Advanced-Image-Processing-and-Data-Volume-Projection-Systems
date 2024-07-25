#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include "Filter.h"
#include "Projection.h"
#include "Image.h"

int main() {
    // int w, h, c;
    // unsigned char* tienshan_data;
    // unsigned char* gracehopper_data;
    // unsigned char* stinkbug_data;
    // unsigned char* vh_anatomy_sp15_data;
    // Filter filter;

    // // Apply grayscale filter
    // // 1.tienshan.png
    // tienshan_data = stbi_load("../Images/tienshan.png", &w, &h, &c, 0);
    // std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
    // unsigned char* grayData = filter.apply_grayscale_filter(tienshan_data, w, h, c);
    // int gray_success = stbi_write_png("../Output/grayscale/tienshan.png", w, h, 1, grayData, 0);
    // if (gray_success) {
    //     stbi_image_free(tienshan_data);
    //     stbi_image_free(grayData);
    // }
    // // 2.gracehopper.png
    // gracehopper_data = stbi_load("../Images/gracehopper.png", &w, &h, &c, 0);
    // std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
    // unsigned char* grayData2 = filter.apply_grayscale_filter(gracehopper_data, w, h, c);
    // gray_success = stbi_write_png("../Output/grayscale/gracehopper.png", w, h, 1, grayData2, 0);
    // if (gray_success) {
    //     stbi_image_free(grayData2);
    // }

    // // Apply salt and pepper filter
    // gracehopper_data = stbi_load("../Images/gracehopper.png", &w, &h, &c, 0);
    // std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
    // unsigned char* sp1 = filter.apply_sp_filter(gracehopper_data, w, h, c, 10);
    // int sp_success = stbi_write_png("../Output/SaltAndPepper/gracehopper_10.png", w, h, c, sp1, w * c);
    // if (sp_success) {
    //     stbi_image_free(sp1);
    // }
    // Paths for the input images
    // const char* imagePath = "../Images/vh_anatomy.png";
    // const char* imagePathGrayscale = "../Images/vh_ct.png"; // Assuming you want to use a different image for grayscale processing

    // // Load the RGB image
    // unsigned char* data = stbi_load(imagePath, &w, &h, &c, 0);
    // if (data == nullptr) {
    //     std::cerr << "Error loading image from " << imagePath << "." << std::endl;
    //     return 1;
    // }
    // std::cout << "Loaded RGB image with dimensions " << w << "x" << h << " and " << c << " channels." << std::endl;

    // // Process and save HSV equalized image
    // if (c == 3) {
    //     filter.applyHistogramEqualization(data, w, h, c, false); // false for HSV
    //     stbi_write_png("../Output/histogram/vh_anatomy_HSV.png", w, h, c, data, w * c);
    // } else {
    //     std::cerr << "RGB image is not in expected format, found " << c << " channels." << std::endl;
    // }

    // // Process and save HSL equalized image
    // if (c == 3) {
    //     filter.applyHistogramEqualization(data, w, h, c, true); // true for HSL
    //     stbi_write_png("../Output/histogram/vh_anatomy_HSL.png", w, h, c, data, w * c);
    // }

    // stbi_image_free(data); // Free the RGB image memory
    // // Apply salt and pepper filter
    // gracehopper_data = stbi_load("../Images/gracehopper.png", &w, &h, &c, 0);
    // std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
    // unsigned char* sp1 = filter.applySpFilter(gracehopper_data, w, h, c, 10);
    // int sp_success = stbi_write_png("../Output/SaltAndPepper/gracehopper_10.png", w, h, c, sp1, w * c);
    // if (sp_success) {
    //     stbi_image_free(sp1);
    // }

    // // Apply gaussian filter
    // // 1.stinkbug.png
    // // kernelsize 3x3, 5x5, sigma=3.0
    // stinkbug_data = stbi_load("../Images/stinkbug.png", &w, &h, &c, 0);
    // std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
    // unsigned char* stinkbugOutput3x3 = filter.apply_2D_Gaussian_filter(stinkbug_data, w, h, c, 3, 1.0);
    // unsigned char* stinkbugOutput5x5 = filter.apply_2D_Gaussian_filter(stinkbug_data, w, h, c, 5, 1.0);
    // int stinkbug3x3_success = stbi_write_png("../Output/blur/gaussian/stinkbug_3x3.png", w, h, c, stinkbugOutput3x3, 0);
    // int stinkbug5x5_success = stbi_write_png("../Output/blur/gaussian/stinkbug_5x5.png", w, h, c, stinkbugOutput5x5, 0);
    // if (stinkbug3x3_success && stinkbug5x5_success) {
    //     std::cout << "Stinkbug images written successfully" << std::endl;
    //     stbi_image_free(stinkbug_data);
    //     stbi_image_free(stinkbugOutput3x3);
    //     stbi_image_free(stinkbugOutput5x5);
    // } else {
    //     std::cerr << "Error writing stinkbug images" << std::endl;
    // }
    // stinkbug_data = stbi_load("../Images/stinkbug.png", &w, &h, &c, 0);
    // std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
    // unsigned char* sp3 = filter.apply_sp_filter(stinkbug_data, w, h, c, 10);
    // int sp_success_3 = stbi_write_png("../Output/SaltAndPepper/stinkbug_10.png", w, h, c, sp3, w * c);
    // if (sp_success_3) {
    //     stbi_image_free(sp3);
    // }

    // // Load the grayscale image (if different from the RGB image)
    // unsigned char* grayscaleData = stbi_load(imagePathGrayscale, &w, &h, &c, 0);
    // if (grayscaleData == nullptr) {
    //     std::cerr << "Error loading grayscale image from " << imagePathGrayscale << "." << std::endl;
    //     return 1;
    // }
    // std::cout << "Loaded grayscale image with dimensions " << w << "x" << h << " and " << c << " channels." << std::endl;

    // // Apply histogram equalization on the grayscale image
    // filter.applyHistogramEqualization(grayscaleData, w, h, 1, false); // The last parameter doesn't matter for grayscale
    // stbi_write_png("../Output/histogram/vh_ct.png", w, h, 1, grayscaleData, w);

    // stbi_image_free(grayscaleData); // Free the grayscale image memory

    // std::cout << "Histogram equalization complete. Output images saved in ../Output/histogram/" << std::endl;

    // // Apply 2D Box filter
    // unsigned char* boxBlurredData = filter.applyBoxBlur(imageData, width, height, channels, 5);
    // if (boxBlurredData != nullptr) {
    //     // Save the blurred image (adjust the filename and format if needed)
    //     stbi_write_png("../Output/blur/box/vh_anatomy_sp15_5x5.png", width, height, channels, boxBlurredData, width * channels);
    //     delete[] boxBlurredData;
    //     std::cout << "Box blurred image saved.\n";
    // } else {
    //     std::cerr << "Failed to apply box blur filter.\n";
    // }

    // return 0;

    // Apply grayscale filter
    // 1.tienshan.png
    // tienshan_data = stbi_load("../Images/tienshan.png", &w, &h, &c, 0);
    // std::cout << "Image loaded with size " << w << " x " << h << " with " << c << " channel(s)." << std::endl;
    // unsigned char* grayData = filter.applyGrayscaleFilter(tienshan_data, w, h, c);
    // // int gray_success = stbi_write_png("../Output/grayscale/tienshan.png", w, h, 1, grayData, 0);
    // // if (gray_success) {
    // //     stbi_image_free(tienshan_data);
    // //     stbi_image_free(grayData);
    // // }
    // unsigned char* guassianData = filter.apply2DGaussianFilter(grayData, w, h, c, 5, 1.0);

    Image img;
    img.loadImage("../Images/gracehopper.png");
    img.Grayscale();
    img.GaussianFilter(5,1.0);
    img.sobelDetection();
    img.saveImage("../Output/test.png");
}