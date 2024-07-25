/*
 * Group Name: Ukkonen
 * Members:
 * - Zeyu Zhao (@edsml-zz2123)
 * - Ark Saini (@acse-as12123)
 * - Lihao Ding (@acse-ld823)
 * - Geyu JI (@acse-gj23)
 * - Yanan Wang (@acse-yy3123)
 * - Chandrasekhar Gudipati (@edsml-cg1123)
 */

#include "Image.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image() {
    this->exist = 0; // The constructor body is intentionally left empty.
}

// Getters and Setters

int Image::getExist() {
    return this->exist;
}

std::string Image::getPath() {
    return this->path;
}

int Image::getWidth() {
    return this->width;
}

int Image::getHeight() {
    return this->height;
}

int Image::getChannels() {
    return this->channels;
}

// Image processing functions

bool Image::loadImage(const std::string& path) {
    // Free existing image data if present
    if (this->exist) {
        stbi_image_free(this->data);
    }

    // Update the image path
    this->path = path;
    // Load the image and update dimensions and channel count
    this->data = stbi_load(path.c_str(), &this->width, &this->height, &this->channels, 0);

    // Check if the image was successfully loaded
    if (this->data == NULL) {
        std::cerr << "Error in loading image" << std::endl;
        return false; // Loading failed
    }
    else {
        this->exist = 1; // Mark the image as existing
        return true; // Loading succeeded
    }
}

bool Image::saveImage(const std::string& path) {
    // Check and create the output directory if it does not exist
    size_t lastSlashPos = path.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        std::string outPutDir = path.substr(0, lastSlashPos);
        namespace fs = std::filesystem;
        fs::create_directories(outPutDir);
    }

    // Save the image using stbi_write_png
    int success = stbi_write_png(path.c_str(), this->width, this->height, this->channels, this->data, 0);

    // Check for successful image saving
    if (success) {
        return true; // Image saved successfully
    }
    else {
        std::cerr << "Error in saving image" << std::endl;
        return false; // Image saving failed
    }
}

//Colour correction and simple per-pixel modifiers

bool Image::Grayscale() {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the grayscale filter to the image data
    unsigned char* filterData = filter.applyGrayscaleFilter(this->data, this->width, this->height, this->channels);

    // Update image properties to reflect the grayscale conversion
    this->channels = 1; // Update the number of channels to 1 for grayscale
    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the grayscale image data

    return true; // Return true indicating successful grayscale conversion
}

bool Image::Brightness(int brightness) {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the brightness filter to the image data
    unsigned char* filterData = filter.applyBrightnessFilter(this->data, this->width, this->height, this->channels, brightness);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the brightness-adjusted image data

    return true; // Return true indicating successful brightness adjustment
}

bool Image::SaltAndPepper(int percentage_sp) {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Salt and Pepper Noise filter to the image data
    unsigned char* filterData = filter.applySpFilter(this->data, this->width, this->height, this->channels, percentage_sp);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the noised image data

    return true; // Return true indicating successful noise application
}

bool Image::Threshold(int threshold, bool use_hsl) {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the threshold filter to the image data
    unsigned char* filterData = filter.applyThresholdFilter(this->data, this->width, this->height, this->channels, threshold, use_hsl);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the thresholded image data
    this->channels = 1; // Update the number of channels to 1, as the result is a binary image

    return true; // Return true indicating successful threshold application
}

bool Image::HistogramEqualization(bool use_hsl) {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Histogram Equalization filter to the image data
    filter.applyHistogramEqualization(this->data, this->width, this->height, this->channels, use_hsl);

    return true; // Return true indicating successful histogram equalization
}

// Image blur functions

bool Image::GaussianFilter(int filterSize, double sigma) {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Gaussian blur filter to the image data
    unsigned char* filterData = filter.apply2DGaussianFilter(this->data, this->width, this->height, this->channels, filterSize, sigma);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the blurred image data

    return true; // Return true indicating successful Gaussian blur application
}

bool Image::MedianFilter(int filterSize) {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Allocate memory for the output image data
    unsigned char* output = static_cast<unsigned char*>(malloc(this->width * this->height * this->channels));
    if (!output) {
        std::cerr << "Memory allocation failed for Median filter output" << std::endl;
        return false;
    }

    // Apply the Median blur filter to the image data
    filter.apply2DMedianBlurFilter(this->data, output, this->width, this->height, this->channels, filterSize);

    stbi_image_free(this->data); // Free the original image data
    this->data = output; // Update the image data pointer to the blurred image data

    return true; // Return true indicating successful Median blur application
}

bool Image::boxFilter(int filterSize) {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Box blur filter to the image data
    unsigned char* filterData = filter.applyBoxBlur(this->data, this->width, this->height, this->channels, filterSize);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the blurred image data

    return true; // Return true indicating successful Box blur application
}

// Edge detection functions

bool Image::sobelDetection() {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Sobel edge detection filter to the image data
    unsigned char* filterData = filter.sobelFilter(this->data, this->width, this->height);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the edge-detected image data

    return true; // Return true indicating successful Sobel edge detection
}

bool Image::prewittDetection() {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Prewitt edge detection filter to the image data
    unsigned char* filterData = filter.prewittFilter(this->data, this->width, this->height);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the edge-enhanced image data

    return true; // Return true indicating successful Prewitt edge detection
}

bool Image::scharrDetection() {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Scharr edge detection filter to the image data
    unsigned char* filterData = filter.scharrFilter(this->data, this->width, this->height);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the edge-enhanced image data

    return true; // Return true indicating successful Scharr edge detection
}

bool Image::robertsCrossDetection() {
    // Check if image data exists
    if (this->data == nullptr) {
        std::cerr << "No image loaded" << std::endl;
        return false; // Return false if no image data is loaded
    }

    // Apply the Roberts Cross edge detection filter to the image data
    unsigned char* filterData = filter.robertsCrossFilter(this->data, this->width, this->height);

    stbi_image_free(this->data); // Free the original image data
    this->data = filterData; // Update the image data pointer to the edge-detected image data

    return true; // Return true indicating successful Roberts Cross edge detection
}