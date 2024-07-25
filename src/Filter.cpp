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

#define _USE_MATH_DEFINES 
#include "Filter.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>
#include <cstring>

Filter::Filter() {}

// Colour Correction and Per-Pixel Modifiers

unsigned char* Filter::applyGrayscaleFilter(unsigned char* data, int w, int h, int c) {
    if (data == nullptr) {
        std::cerr << "Error loading image" << std::endl; // Error handling: data pointer is null
        return nullptr;
    }
    // if c == 1, create a copy of the image data and return
    if (c == 1) {
        unsigned char* grayData = new unsigned char[w * h]; // Allocate memory for grayscale image data
        std::memcpy(grayData, data, w * h); // Copy the data
        return grayData; // Return the grayscale image data
    }
    else
    if (c >= 3) { // Ensure the image is RGB or RGBA
        unsigned char* grayData = new unsigned char[w * h]; // Allocate memory for grayscale image data
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                unsigned char* pixel = data + (j * w + i) * c; // Pointer to the current pixel
                // Calculate luminance using the Rec. 709 formula
                float luminance = 0.2126f * pixel[0] + 0.7152f * pixel[1] + 0.0722f * pixel[2];
                unsigned char grayValue = static_cast<unsigned char>(luminance); // Convert to unsigned char
                grayData[j * w + i] = grayValue; // Set the gray value for the current pixel in the grayscale image
            }
        }
        return grayData; // Return the grayscale image data
    }
    else {
        std::cerr << "Image must be RGB or RGBA to convert to grayscale" << std::endl; // Error handling: incorrect color channels
        return nullptr;
    }
}

unsigned char* Filter::applyBrightnessFilter(unsigned char* data, int w, int h, int c, int brightness) {
    if (data == nullptr) {
        std::cerr << "Error: Image data is null" << std::endl; // Error handling: data pointer is null
        return nullptr;
    }

    // A unique case where if brightness is 0, calculate an average brightness from the image.
    if (brightness == 0) {
        int sum = 0;
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                unsigned char* pixel = data + (j * w + i) * c;
                for (int k = 0; k < c; ++k) {
                    sum += pixel[k]; // Sum all pixel values
                }
            }
        }
        brightness = sum / (w * h * c); // Calculate average brightness
    }

    unsigned char* brightData = new unsigned char[w * h * c]; // Allocate memory for brightened image data
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            unsigned char* pixel = data + (j * w + i) * c; // Pointer to the current pixel
            for (int k = 0; k < c; ++k) {
                int value = pixel[k] + brightness; // Adjust brightness
                // Clamp the value to the 0-255 range
                if (value < 0) {
                    value = 0;
                }
                else if (value > 255) {
                    value = 255;
                }
                brightData[(j * w + i) * c + k] = value; // Set the adjusted value
            }
        }
    }
    return brightData; // Return the brightened image data
}

unsigned char* Filter::applyHistogramEqualization(unsigned char* data, int w, int h, int channels, bool use_hsl) {
    if (data == nullptr || channels < 1 || channels > 4) {
        std::cerr << "Unsupported number of channels or null data." << std::endl;
        return nullptr;
    }

    // For grayscale images, directly apply equalization
    if (channels == 1) {
        auto histogram = computeHistogram(data, w * h, channels);
        auto equalized_values = equalizeHistogram(histogram, w * h);
        applyEqualization(data, equalized_values, w * h, channels);
    }
    // For RGB and RGBA, process without altering the alpha channel (if present)
    else if (channels == 3 || channels == 4) {
        applyRGBEqualization(data, w, h, channels, use_hsl);
        // If there's an alpha channel (4th channel in RGBA), ignore it during equalization
    }
    else {
        std::cerr << "Unsupported number of channels for histogram equalization." << std::endl;
    }

    return data;
}

unsigned char* Filter::applyThresholdFilter(unsigned char* data, int w, int h, int c, int threshold, bool use_hsl) {
    unsigned char* thresholdData = new unsigned char[w * h]; // Allocate memory for the thresholded image

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float lv; // Will hold the luminance or value component

            if (c == 1) { // Greyscale image
                lv = data[y * w + x] / 255.0f; // Normalize the pixel value to [0, 1]
            }
            else { // RGB image
                float r = data[(y * w + x) * c];
                float g = data[(y * w + x) * c + 1];
                float b = data[(y * w + x) * c + 2];
                float h, s; // Hue and saturation components, unused here
                if (use_hsl) {
                    RGBtoHSL(r, g, b, h, s, lv); // Convert to HSL to get luminance
                }
                else {
                    RGBtoHSV(r, g, b, h, s, lv); // Convert to HSV to get value
                }
            }

            lv *= 255.0f; // Scale the luminance/value back to [0, 255]
            thresholdData[y * w + x] = (lv < threshold) ? 0 : 255; // Apply threshold
        }
    }

    return thresholdData; 
}

unsigned char* Filter::applySpFilter(unsigned char* data, int w, int h, int c, int percentage_sp) {
    unsigned char* modifiedData = new unsigned char[w * h * c]; // Allocate memory for the noisy image
    std::memcpy(modifiedData, data, w * h * c); // Copy original image data to the new array

    int totalPixels = w * h;
    int pixelsToModify = (totalPixels * percentage_sp) / 100; // Calculate the number of pixels to be affected by noise

    std::vector<int> pixelIndices(totalPixels);
    std::iota(pixelIndices.begin(), pixelIndices.end(), 0); // Fill with 0, 1, ..., totalPixels-1

    std::random_device rd; // Seed for generating random numbers
    std::mt19937 gen(rd()); // Mersenne Twister: Good quality random number generator
    std::shuffle(pixelIndices.begin(), pixelIndices.end(), gen); // Randomly shuffle pixel index array
    std::uniform_int_distribution<> disValue(0, 1);

    for (int i = 0; i < pixelsToModify; ++i) {
        int pixelIndex = pixelIndices[i]; // Choose a random but non-duplicate pixel index
        int value = disValue(gen) * 255; // Randomly choose between black (0) and white (255)

        // Apply noise to the selected pixel, ensuring it affects all channels (to maintain grayscale appearance)
        for (int channel = 0; channel < c; ++channel) {
            modifiedData[pixelIndex * c + channel] = value;
        }
    }

    return modifiedData;
}


// 2D Image Blur

unsigned char* Filter::apply2DGaussianFilter(unsigned char* data, int width, int height, int channels, int kernelSize, float sigma) {
    if (data == nullptr) {
        std::cerr << "Error loading image" << std::endl;
        return nullptr;
    }

    // Generate the Gaussian kernel for the specified size and sigma
    std::vector<std::vector<float>> kernel = generate2DGaussianKernel(kernelSize, sigma);
    int halfSize = kernelSize / 2; // Calculate the kernel's radius
    unsigned char* output = new unsigned char[width * height * channels]; // Allocate memory for the output image

    // Apply Gaussian blur to each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) { // Process each channel independently
                float blurredPixel = 0.0; // Accumulator for the blurred pixel value

                // Convolve the kernel over the pixel's neighborhood
                for (int kx = -halfSize; kx <= halfSize; kx++) {
                    for (int ky = -halfSize; ky <= halfSize; ky++) {
                        // Handle edge pixels by clamping indices to the image boundaries
                        int ix = std::min(std::max(x + kx, 0), width - 1);
                        int iy = std::min(std::max(y + ky, 0), height - 1);
                        // Accumulate the weighted sum of pixel values
                        blurredPixel += data[(iy * width + ix) * channels + c] * kernel[ky + halfSize][kx + halfSize];
                    }
                }

                // Assign the blurred value to the output image, clamping to valid [0, 255] range
                output[(y * width + x) * channels + c] = std::min(std::max(int(blurredPixel), 0), 255);
            }
        }
    }

    return output; 
}


void Filter::apply2DMedianBlurFilter(unsigned char* data, unsigned char* output, int w, int h, int c, int kernelSize) {
    int edge = kernelSize / 2; // Half the kernel size, used to calculate the neighborhood bounds
    std::vector<unsigned char> window; // Window of pixel values for median calculation

    // Iterate through each pixel in the image
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // Process each channel of the pixel independently
            for (int channel = 0; channel < c; ++channel) {
                window.clear(); // Clear the window for the new pixel

                // Collect pixel values from the neighborhood defined by the kernel size
                for (int fx = -edge; fx <= edge; ++fx) {
                    for (int fy = -edge; fy <= edge; ++fy) {
                        // Calculate the coordinates of the neighboring pixel, clamping to image boundaries
                        int ix = std::min(std::max(x + fx, 0), w - 1);
                        int iy = std::min(std::max(y + fy, 0), h - 1);
                        // Add the neighboring pixel's value to the window
                        window.push_back(data[(iy * w + ix) * c + channel]);
                    }
                }
                // Set the current pixel's value to the median of the window values
                output[(y * w + x) * c + channel] = getMedian(window);
            }
        }
    }
}

unsigned char* Filter::applyBoxBlur(unsigned char* data, int w, int h, int c, int kernelSize) {
    if (data == nullptr) {
        std::cerr << "Error loading image for box blur" << std::endl;
        return nullptr;
    }

    unsigned char* output = new unsigned char[w * h * c]; // Allocate memory for the blurred image
    int edge = kernelSize / 2; // Calculate the half-size of the kernel to determine the neighborhood bounds
    int area = kernelSize * kernelSize; // Total number of pixels within the kernel

    // Iterate through each pixel in the image
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // Apply the blur to each channel independently
            for (int channel = 0; channel < c; ++channel) {
                int sum = 0; // Sum of pixel values within the kernel

                // Collect pixel values from the neighborhood defined by the kernel size
                for (int ky = -edge; ky <= edge; ++ky) {
                    for (int kx = -edge; kx <= edge; ++kx) {
                        // Calculate the coordinates of the neighboring pixel, clamping to image boundaries
                        int px = std::min(std::max(x + kx, 0), w - 1);
                        int py = std::min(std::max(y + ky, 0), h - 1);
                        // Accumulate the pixel values
                        sum += data[(py * w + px) * c + channel];
                    }
                }
                // Calculate the average value and assign it to the current pixel
                output[(y * w + x) * c + channel] = sum / area;
            }
        }
    }

    return output; 
}

// 2D Edge Detection

unsigned char* Filter::sobelFilter(unsigned char* image, int width, int height) {
    // Sobel kernels for horizontal and vertical edge detection
    std::vector<std::vector<int>> gx = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
    std::vector<std::vector<int>> gy = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

    unsigned char* output = new unsigned char[width * height];

    // Apply Sobel operator to each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Convolve Sobel kernels with the image
            float sx = applyKernel(image, width, height, x, y, gx);
            float sy = applyKernel(image, width, height, x, y, gy);
            // Compute gradient magnitude
            float gradient = std::sqrt(sx * sx + sy * sy);
            // Assign gradient magnitude to output, clipping to valid range
            output[y * width + x] = std::min(std::max(int(gradient), 0), 255);
        }
    }
    return output;
}

unsigned char* Filter::prewittFilter(unsigned char* image, int width, int height) {
    // Prewitt kernels for horizontal and vertical edge detection
    std::vector<std::vector<int>> gx = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
    std::vector<std::vector<int>> gy = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };

    unsigned char* output = new unsigned char[width * height];

    // Apply Prewitt operator to each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Convolve Prewitt kernels with the image
            float sx = applyKernel(image, width, height, x, y, gx);
            float sy = applyKernel(image, width, height, x, y, gy);
            // Compute gradient magnitude
            float gradient = std::sqrt(sx * sx + sy * sy);
            // Assign gradient magnitude to output, clipping to valid range
            output[y * width + x] = std::min(std::max(int(gradient), 0), 255);
        }
    }
    return output;
}

unsigned char* Filter::scharrFilter(unsigned char* image, int width, int height) {
    // Scharr kernels for more accurate edge detection
    std::vector<std::vector<int>> gx = { {-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3} };
    std::vector<std::vector<int>> gy = { {-3, -10, -3}, {0, 0, 0}, {3, 10, 3} };

    unsigned char* output = new unsigned char[width * height];

    // Apply Scharr operator to each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Convolve Scharr kernels with the image
            float sx = applyKernel(image, width, height, x, y, gx);
            float sy = applyKernel(image, width, height, x, y, gy);
            // Compute gradient magnitude
            float gradient = std::sqrt(sx * sx + sy * sy);
            // Assign gradient magnitude to output, clipping to valid range
            output[y * width + x] = std::min(std::max(int(gradient), 0), 255);
        }
    }
    return output;
}

unsigned char* Filter::robertsCrossFilter(unsigned char* image, int width, int height) {
    unsigned char* output = new unsigned char[width * height]; // Allocate memory for the output image

    // Iterate over the image, excluding the last row and column to avoid boundary issues
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            // Calculate the gradient components using Roberts' Cross approximation
            float sx = image[y * width + x] - image[(y + 1) * width + (x + 1)];
            float sy = -image[(y + 1) * width + x] + image[y * width + (x + 1)];
            // Compute the gradient magnitude
            float gradient = std::sqrt(sx * sx + sy * sy);
            // Assign the gradient magnitude to the output, clamping the value to the [0, 255] range
            output[y * width + x] = std::min(std::max(int(gradient), 0), 255);
        }
    }

    return output; // Return the output image with highlighted edges
}

// 3D Image Blur

void Filter::apply3DMedianFilter(std::vector<unsigned char*>& images, int width, int height, int depth, int filterSize) {
    std::vector<unsigned char*> outputImages(depth);
    int halfSize = filterSize / 2;
    std::vector<unsigned char> neighborhood;
    neighborhood.reserve(filterSize * filterSize * filterSize);

    // Initialize output images
    for (int d = 0; d < depth; ++d) {
        outputImages[d] = new unsigned char[width * height]; // Allocate memory for each output slice
    }

    // Apply median filter to each voxel
    for (int z = 0; z < depth; ++z) {
        std::cout << "Processing filter at index: " << z << "..." << std::endl;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                neighborhood.clear(); // Clear the neighborhood for the new voxel

                // Collect neighborhood values from the 3D kernel
                for (int dz = -halfSize; dz <= halfSize; ++dz) {
                    int zz = std::min(std::max(z + dz, 0), depth - 1); // Clamping z index
                    for (int dy = -halfSize; dy <= halfSize; ++dy) {
                        int yy = std::min(std::max(y + dy, 0), height - 1); // Clamping y index
                        for (int dx = -halfSize; dx <= halfSize; ++dx) {
                            int xx = std::min(std::max(x + dx, 0), width - 1); // Clamping x index
                            neighborhood.push_back(images[zz][yy * width + xx]); // Add voxel value to the neighborhood
                        }
                    }
                }
                outputImages[z][y * width + x] = getMedian(neighborhood);
            }
        }
    }

    // Replace input images with filtered images and free old images
    for (int d = 0; d < depth; ++d) {
        delete[] images[d]; // Free memory of the original slice
        images[d] = outputImages[d]; // Update pointer to the new, filtered slice
    }
}

void Filter::apply3DGaussianFilter(std::vector<unsigned char*>& images, int width, int height, int depth, int filterSize, double sigma) {
    std::cout << "Applying 3D Gaussian filter..." << std::endl;
    auto gaussianKernel = generate3DGaussianKernel(filterSize, sigma); // Generate the Gaussian kernel
    std::cout << "Kernel created" << std::endl;
    int halfSize = filterSize / 2; // Half the kernel size, for indexing
    std::vector<unsigned char*> outputImages(depth); // Output images after Gaussian filtering

    // Initialize output images
    for (int i = 0; i < depth; i++) {
        outputImages[i] = new unsigned char[width * height]; // Allocate memory for each output slice
    }

    // Apply Gaussian filter to each voxel
    for (int z = 0; z < depth; z++) {
        std::cout << "Processing filter at index: " << z << "..." << std::endl;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double filteredValue = 0.0; // Accumulator for the Gaussian weighted sum

                // Convolve the Gaussian kernel over the voxel's neighborhood
                for (int dz = -halfSize; dz <= halfSize; dz++) {
                    for (int dy = -halfSize; dy <= halfSize; dy++) {
                        for (int dx = -halfSize; dx <= halfSize; dx++) {
                            int newX = std::min(std::max(x + dx, 0), width - 1); // Clamp x coordinate
                            int newY = std::min(std::max(y + dy, 0), height - 1); // Clamp y coordinate
                            int newZ = std::min(std::max(z + dz, 0), depth - 1); // Clamp z coordinate
                            // Accumulate the weighted voxel values
                            filteredValue += images[newZ][newY * width + newX] * gaussianKernel[dz + halfSize][dy + halfSize][dx + halfSize];
                        }
                    }
                }
                // Assign the computed Gaussian weighted sum to the output voxel
                outputImages[z][y * width + x] = static_cast<unsigned char>(std::min(std::max(int(filteredValue), 0), 255));
            }
        }
    }

    // Replace input images with filtered images and free old images
    for (int i = 0; i < depth; i++) {
        delete[] images[i]; // Free memory of the original slice
        images[i] = outputImages[i]; // Update pointer to the new, filtered slice
    }
}

// Color Space Conversion

void Filter::RGBtoHSV(float r, float g, float b, float& h, float& s, float& v) {
    r /= 255.0f; // Normalize RGB values to [0, 1]
    g /= 255.0f;
    b /= 255.0f;

    float max_val = std::max({ r, g, b }); // Find the maximum RGB component
    float min_val = std::min({ r, g, b }); // Find the minimum RGB component
    float delta = max_val - min_val; // Calculate the difference

    // Hue calculation
    if (delta == 0) {
        h = 0; // Hue is undefined when all RGB values are equal, conventionally set to 0
    }
    else if (max_val == r) {
        h = fmod((g - b) / delta, 6); // between yellow & magenta
    }
    else if (max_val == g) {
        h = (b - r) / delta + 2; // between cyan & yellow
    }
    else {
        h = (r - g) / delta + 4; // between magenta & cyan
    }

    h *= 60; // Convert from [0, 6) to [0, 360) degrees
    if (h < 0) h += 360; // Ensure hue is non-negative

    // Saturation calculation
    s = (max_val == 0) ? 0 : (delta / max_val);

    // Value calculation
    v = max_val; // The value component is simply the maximum RGB component
}

void Filter::HSVtoRGB(float h, float s, float v, float& r, float& g, float& b) {
    // Check for achromatic case (no saturation)
    if (s <= 0.0) {
        r = g = b = v * 255.0f; // All components are equal to value component
        return;
    }

    float hh = h / 60; // Sector division for color wheel
    long i = static_cast<long>(hh); // Integer part of 'hh', determining the sector
    float ff = hh - i; // Fractional part of 'hh', used in interpolation

    // Precompute values for interpolation
    float p = v * (1.0 - s);
    float q = v * (1.0 - (s * ff));
    float t = v * (1.0 - (s * (1.0 - ff)));

    // Apply the color conversion based on the HSV sector
    switch (i) {
    case 0:
        r = v; g = t; b = p; break;
    case 1:
        r = q; g = v; b = p; break;
    case 2:
        r = p; g = v; b = t; break;
    case 3:
        r = p; g = q; b = v; break;
    case 4:
        r = t; g = p; b = v; break;
    case 5:
    default: // Loop around the color wheel for values >= 360
        r = v; g = p; b = q; break;
    }

    // Convert RGB components back to the range [0, 255]
    r *= 255.0f;
    g *= 255.0f;
    b *= 255.0f;
}

void Filter::RGBtoHSL(float r, float g, float b, float& h, float& s, float& l) {
    // Normalize RGB values to the range [0, 1]
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;

    // Determine the maximum and minimum values among R, G, and B
    float max_color = std::max({ r, g, b });
    float min_color = std::min({ r, g, b });
    float delta = max_color - min_color; // Calculate the difference

    // Lightness is the average of the max and min color values
    l = (max_color + min_color) / 2;

    // Saturation calculation depends on lightness
    if (delta == 0) {
        h = s = 0; // Achromatic case (no saturation, hue is undefined)
    }
    else {
        // Saturation calculation varies depending on the lightness
        s = l > 0.5 ? delta / (2 - max_color - min_color) : delta / (max_color + min_color);

        // Hue calculation based on which RGB component is max
        // The additional conditions handle the wrap-around the color wheel
        if (max_color == r) {
            h = (g - b) / delta + (g < b ? 6 : 0);
        }
        else if (max_color == g) {
            h = (b - r) / delta + 2;
        }
        else {
            h = (r - g) / delta + 4;
        }

        h /= 6; // Normalize hue to the range [0, 1]
    }
}

void Filter::HSLtoRGB(float h, float s, float l, float& r, float& g, float& b) {
    // Achromatic case (no saturation)
    if (s == 0) {
        r = g = b = l * 255.0f; // Directly scale lightness to RGB range
    }
    else {
        // Helper function to convert hue to RGB
        auto hue2rgb = [](float p, float q, float t) -> float {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1 / 6.0) return p + (q - p) * 6 * t; // Increasing red
            if (t < 1 / 2.0) return q; // Full green
            if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6; // Decreasing blue
            return p; // Full red
            };

        // Calculate q based on lightness and saturation
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        // p is calculated to assist in the conversion
        float p = 2 * l - q;

        // Convert HSL to RGB using the helper function
        r = hue2rgb(p, q, h + 1 / 3.0) * 255.0f; // Red component
        g = hue2rgb(p, q, h) * 255.0f; // Green component
        b = hue2rgb(p, q, h - 1 / 3.0) * 255.0f; // Blue component
    }
}

// Histogram Computation and Equalization

std::vector<unsigned int> Filter::computeHistogram(unsigned char* data, int size, int channels) {
    std::vector<unsigned int> histogram(256, 0); // Initialize histogram bins

    for (int i = 0; i < size * channels; i += channels) {
        // Assuming the grayscale value or the first channel in RGB to compute histogram
        histogram[data[i]]++;
    }

    return histogram; // Return the computed histogram
}

std::vector<unsigned int> Filter::computeHistogram(const std::vector<float>& data) {
    std::vector<unsigned int> histogram(256, 0); // Initialize histogram bins

    for (auto value : data) {
        // Ensure the float value is within the expected range and increment the corresponding histogram bin
        int index = static_cast<int>(value);
        histogram[index]++;
    }

    return histogram; // Return the computed histogram
}

std::vector<unsigned char> Filter::equalizeHistogram(const std::vector<unsigned int>& histogram, int pixels) {
    std::vector<unsigned int> cdf(histogram.size()); // Initialize the Cumulative Distribution Function (CDF) array
    std::partial_sum(histogram.begin(), histogram.end(), cdf.begin()); // Compute the CDF from the histogram

    // Normalize the CDF
    float cdf_min = *std::min_element(cdf.begin(), cdf.end());

    std::vector<unsigned char> equalized_values(cdf.size()); // Initialize the array for equalized pixel values
    for (size_t i = 0; i < cdf.size(); i++) {
        // Apply histogram equalization formula to each value
        equalized_values[i] = static_cast<unsigned char>(round((cdf[i] - cdf_min) / (float)(pixels - cdf_min) * 255));
    }

    return equalized_values; // Return the array of equalized pixel values
}

void Filter::applyEqualization(unsigned char* data, const std::vector<unsigned char>& equalized_values, int size, int channels) {
    for (int i = 0; i < size; ++i) {
        // Apply the equalized value to the first channel
        data[i * channels] = equalized_values[data[i * channels]];

        // For RGB images, replicate the equalized first channel value to the other two channels
        if (channels == 3) {
            data[i * channels + 1] = data[i * channels]; // Copy to the Green channel
            data[i * channels + 2] = data[i * channels]; // Copy to the Blue channel
        }
    }
}

void Filter::applyRGBEqualization(unsigned char* data, int w, int h, int channels, bool use_hsl) {
    if (channels != 3 && channels != 4) {
        std::cerr << "RGB equalization requires an image with 3 or 4 (RGBA) channels." << std::endl;
        return;
    }

    // Arrays to hold the HSL/HSV components
    std::vector<float> h_channel(w * h), s_channel(w * h), lv_channel(w * h);

    // Convert RGB/RGBA to HSL/HSV while ignoring the alpha channel, store components in separate arrays
    for (int i = 0; i < w * h; ++i) {
        float r = data[i * channels];
        float g = data[i * channels + 1];
        float b = data[i * channels + 2];
        if (use_hsl) {
            RGBtoHSL(r, g, b, h_channel[i], s_channel[i], lv_channel[i]);
        }
        else {
            RGBtoHSV(r, g, b, h_channel[i], s_channel[i], lv_channel[i]);
        }
    }

    // Prepare the L/V channel for histogram computation
    for (float& value : lv_channel) {
        value *= 255.0f;
    }

    // Compute and equalize histogram on the L/V channel
    auto histogram = computeHistogram(lv_channel);
    auto equalized_values = equalizeHistogram(histogram, w * h);

    // Map original L/V values to equalized values
    for (int i = 0; i < w * h; ++i) {
        lv_channel[i] = equalized_values[static_cast<int>(lv_channel[i])];
    }

    // Convert back to RGB/RGBA from equalized HSL/HSV
    for (int i = 0; i < w * h; ++i) {
        float r, g, b;
        if (use_hsl) {
            HSLtoRGB(h_channel[i], s_channel[i], lv_channel[i] / 255.0f, r, g, b);
        }
        else {
            HSVtoRGB(h_channel[i], s_channel[i], lv_channel[i] / 255.0f, r, g, b);
        }

        // Update image data with equalized RGB values
        data[i * channels] = static_cast<unsigned char>(r);
        data[i * channels + 1] = static_cast<unsigned char>(g);
        data[i * channels + 2] = static_cast<unsigned char>(b);
        // Alpha channel (if present) is left unchanged
    }
}

// Helper function for the QuickSort algorithm
int Filter::partition(std::vector<unsigned char>& vec, int low, int high) {
    unsigned char pivot = vec[high]; // Choosing the last element as the pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot, swap it with the element at i
        if (vec[j] < pivot) {
            i++; // Increment index of smaller element
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]); // Swap the pivot element with the element at i+1
    return (i + 1); // Return the partitioning index
}

// Implements the QuickSort algorithm
void Filter::quickSort(std::vector<unsigned char>& vec, int low, int high) {
    if (low < high) {
        int pi = partition(vec, low, high); // Partitioning index
        quickSort(vec, low, pi - 1); // Recursively sort elements before partition
        quickSort(vec, pi + 1, high); // Recursively sort elements after partition
    }
}

unsigned char Filter::getMedian(std::vector<unsigned char>& vec) {
    int n = vec.size(); // Size of the vector
    quickSort(vec, 0, n - 1); // Sort the vector
    return vec[n / 2]; // Return the median element
}

// Gaussian Kernel Generation

std::vector<std::vector<float>> Filter::generate2DGaussianKernel(int kernelSize, float sigma) {
    std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize));
    float sum = 0.0; // Used for normalization
    int halfSize = kernelSize / 2; // Calculate the midpoint of the kernel

    // Populate the kernel with Gaussian distribution values
    for (int i = -halfSize; i <= halfSize; i++) {
        for (int j = -halfSize; j <= halfSize; j++) {
            float value = exp(-(i * i + j * j) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma); // Gaussian function
            kernel[i + halfSize][j + halfSize] = value; // Assign value to the kernel
            sum += value; // Add to the normalization sum
        }
    }

    // Normalize the kernel to ensure the sum of all elements equals 1
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }

    return kernel; // Return the normalized Gaussian kernel
}

std::vector<std::vector<std::vector<double>>> Filter::generate3DGaussianKernel(int size, double sigma) {
    std::vector<std::vector<std::vector<double>>> kernel(size, std::vector<std::vector<double>>(size, std::vector<double>(size)));
    int halfSize = size / 2; // Calculate the kernel's midpoint
    double sum = 0.0; // Sum of all kernel values for normalization

    // Populate the kernel with Gaussian distribution values
    for (int x = -halfSize; x <= halfSize; x++) {
        for (int y = -halfSize; y <= halfSize; y++) {
            for (int z = -halfSize; z <= halfSize; z++) {
                double exponent = -(x * x + y * y + z * z) / (2 * sigma * sigma); // Gaussian function exponent
                kernel[x + halfSize][y + halfSize][z + halfSize] = std::exp(exponent); // Calculate kernel value
                sum += kernel[x + halfSize][y + halfSize][z + halfSize]; // Add to the normalization sum
            }
        }
    }

    // Normalize the kernel to ensure the sum of all elements equals 1
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            for (int z = 0; z < size; z++) {
                kernel[x][y][z] /= sum; // Normalize each element
            }
        }
    }

    return kernel; // Return the normalized 3D Gaussian kernel
}

// Convolution and Edge Detection Helpers

unsigned char Filter::getPixel(unsigned char* image, int x, int y, int width, int height) {
    x = std::max(0, std::min(width - 1, x)); // Clamp x-coordinate to image boundaries
    y = std::max(0, std::min(height - 1, y)); // Clamp y-coordinate to image boundaries
    return image[y * width + x]; // Return the pixel value
}

float Filter::applyKernel(unsigned char* image, int width, int height, int x, int y, const std::vector<std::vector<int>>& kernel) {
    float sum = 0.0; // Accumulator for the sum of the kernel application
    int kernelSize = kernel.size(); // Size of the kernel (assumed to be square)
    int halfSize = kernelSize / 2; // Half the size of the kernel, used to calculate bounds

    // Iterate over the kernel's elements
    for (int i = -halfSize; i <= halfSize; i++) {
        for (int j = -halfSize; j <= halfSize; j++) {
            // Retrieve the corresponding image pixel with edge padding
            unsigned char pixel = getPixel(image, x + j, y + i, width, height);
            // Apply the kernel element-wise and accumulate the sum
            sum += pixel * kernel[i + halfSize][j + halfSize];
        }
    }

    return sum;
}