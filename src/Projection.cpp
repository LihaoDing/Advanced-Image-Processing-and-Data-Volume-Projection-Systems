#include "Projection.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

Projection::Projection() {}

bool Projection::MIP(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath) {
    // Allocate memory for the final MIP image data
    unsigned char* finalImageData = new unsigned char[width * height * channels];
    // Initialize the image data with zeros
    std::fill(finalImageData, finalImageData + width * height * channels, 0);

    // Iterate over each pixel position
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Process each channel separately
            for (int c = 0; c < channels; ++c) {
                unsigned char maxVal = 0;
                // Find the maximum pixel value across all slices for this position and channel
                for (auto& imgData : images) {
                    unsigned char pixelVal = imgData[(y * width + x) * channels + c];
                    maxVal = std::max(maxVal, pixelVal);
                }
                // Assign the max value to the corresponding position in the final MIP image
                finalImageData[(y * width + x) * channels + c] = maxVal;
            }
        }
    }

    // Check if output directory exists, create it if not
    size_t lastSlashPos = outputPath.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        std::string outPutDir = outputPath.substr(0, lastSlashPos);
        namespace fs = std::filesystem;
        fs::create_directories(outPutDir);
    }
    // Write the final MIP image data to a PNG file
    int success = stbi_write_png(outputPath.c_str(), width, height, channels, finalImageData, 0);
    if (success) {
        // Free the allocated memory if the image was successfully saved
        stbi_image_free(finalImageData);
        return true;
    }
    return false; // Return false if saving the image failed
}

bool Projection::MinIP(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath) {
    // Allocate memory for the final MinIP image data, initializing with maximum possible values
    unsigned char* finalImageData = new unsigned char[width * height * channels];
    std::fill(finalImageData, finalImageData + width * height * channels, std::numeric_limits<unsigned char>::max());

    // Iterate over each pixel position in the images
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                unsigned char minVal = std::numeric_limits<unsigned char>::max();
                // Find the minimum pixel value across all slices for this position and channel
                for (auto& imgData : images) {
                    unsigned char pixelVal = imgData[(y * width + x) * channels + c];
                    minVal = std::min(minVal, pixelVal);
                }
                // Assign the min value to the corresponding position in the final MinIP image
                finalImageData[(y * width + x) * channels + c] = minVal;
            }
        }
    }

    // Create output directory if it doesn't exist
    size_t lastSlashPos = outputPath.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        std::string outPutDir = outputPath.substr(0, lastSlashPos);
        namespace fs = std::filesystem;
        fs::create_directories(outPutDir);
    }
    // Write the final MinIP image data to a PNG file
    int success = stbi_write_png(outputPath.c_str(), width, height, channels, finalImageData, 0);
    if (success) {
        // Free the allocated memory if the image was successfully saved
        stbi_image_free(finalImageData);
        return true;
    }
    return false; // Return false if saving the image failed
}

bool Projection::AIP(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath) {
    std::vector<unsigned long long> sum(width * height * channels, 0);
    int imageCount = images.size();

    // Sum pixel values across all images
    for (auto& imgData : images) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width * channels; ++x) {
                sum[y * width * channels + x] += imgData[y * width * channels + x];
            }
        }
    }

    // Create output directory if it doesn't exist
    size_t lastSlashPos = outputPath.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        std::string outPutDir = outputPath.substr(0, lastSlashPos);
        namespace fs = std::filesystem;
        fs::create_directories(outPutDir);
    }

    // Allocate memory for the final AIP image data
    unsigned char* finalImageData = new unsigned char[width * height * channels];
    // Compute the average and populate the final AIP image data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width * channels; ++x) {
            finalImageData[y * width * channels + x] = static_cast<unsigned char>(sum[y * width * channels + x] / imageCount);
        }
    }

    // Write the final AIP image data to a PNG file
    int success = stbi_write_png(outputPath.c_str(), width, height, channels, finalImageData, 0);
    if (success) {
        stbi_image_free(finalImageData);
        return true;
    } else {
        delete[] finalImageData;
        return false;
    }
}



// Average Intensity Projection with Median
bool Projection::AIPMedian(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath) {
    // Initialize a 3D vector to store pixel values across all images
    std::vector<std::vector<std::vector<unsigned char>>> pixels(height, std::vector<std::vector<unsigned char>>(width * channels));

    // Collect pixel values for each position across all images
    for (auto& imgData : images) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width * channels; ++x) {
                pixels[y][x].push_back(imgData[y * width * channels + x]);
            }
        }
    }

    // Create output directory if it doesn't exist
    size_t lastSlashPos = outputPath.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        std::string outPutDir = outputPath.substr(0, lastSlashPos);
        namespace fs = std::filesystem;
        fs::create_directories(outPutDir);
    }

    // Allocate memory for the final AIPMedian image data
    unsigned char* finalImageData = new unsigned char[width * height * channels];
    int n = pixels[0][0].size(); // Number of values to consider for median

    // Calculate the median value for each pixel position
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto& values = pixels[y][x];
            unsigned char median;
            quickSort(values, 0, n - 1); // Sort the pixel values
            // Compute median
            if (n % 2 == 0) {
                median = (values[n / 2] + values[n / 2 + 1]) / 2;
            }
            else {
                median = values[n / 2];
            }
            // Assign the median value to all channels of the pixel
            for (int c = 0; c < channels; ++c) {
                finalImageData[(y * width + x) * channels + c] = median;
            }
        }
    }

    // Write the final AIPMedian image data to a PNG file
    int success = stbi_write_png(outputPath.c_str(), width, height, channels, finalImageData, 0);
    if (success) {
        stbi_image_free(finalImageData); // Free the memory allocated for the AIPMedian image data
        return true; // Return true if the image was successfully saved
    }
    return false; // Return false if saving the image failed
}

// helper functions for finding the median

int Projection::partition(std::vector<unsigned char>& vec, int low, int high) {
    unsigned char pivot = vec[high]; // Choose the last element as the pivot
    int i = (low - 1); // Index of the smaller element

    for (int j = low; j <= high - 1; j++) {
        // If the current element is smaller than the pivot, swap it with the element at index i
        if (vec[j] < pivot) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    // Swap the pivot element with the element at index i+1
    std::swap(vec[i + 1], vec[high]);
    return (i + 1); // Return the partition index
}

void Projection::quickSort(std::vector<unsigned char>& vec, int low, int high) {
    if (low < high) {
        // pi is partitioning index, vec[pi] is now at right place
        int pi = partition(vec, low, high);

        // Recursively sort elements before and after partition
        quickSort(vec, low, pi - 1);
        quickSort(vec, pi + 1, high);
    }
}
