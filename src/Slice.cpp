#ifndef SLICE_H
#define SLICE_H
#include "Slice.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

bool Slice::extractAndSaveSlice(std::vector<stbi_uc*>& images, int width, int height, int sliceIndex, SlicePlane plane, const std::string& outputFilename) {
    int depth = images.size(); // Total number of slices, representing the depth of the volume

    // Validate slice index based on the slicing plane
    if (sliceIndex < 1 || (plane == SlicePlane::YZ && sliceIndex > width) || (plane == SlicePlane::XZ && sliceIndex > height)) {
        std::cerr << "Slice index is out of range." << std::endl;
        return false; // Slice index is invalid
    }

    --sliceIndex; // Convert to zero-based indexing

    // Prepare a buffer for for both YZ and XZ slices
    std::vector<stbi_uc> slice(width * depth);

    // Extract the slice
    size_t lastSlashPos = outputFilename.find_last_of("/");
    if (lastSlashPos != std::string::npos) {
        std::string outPutDir = outputFilename.substr(0, lastSlashPos);
        namespace fs = std::filesystem;
        fs::create_directories(outPutDir); // Create the directory using C++17 filesystem library
    }

    if (plane == SlicePlane::YZ) {
        // Extract YZ slice
        for (int z = 0; z < depth; ++z) {
            for (int y = 0; y < height; ++y) {
                slice[z * height + y] = images[z][y * width + sliceIndex]; // Copy pixel value
            }
        }
        // Write the slice to a PNG file
        int success = stbi_write_png(outputFilename.c_str(), height, depth, 1, slice.data(), height);
        return success != 0; 
    }
    else { // XZ plane
        // Extract XZ slice
        for (int z = 0; z < depth; ++z) {
            for (int x = 0; x < width; ++x) {
                slice[z * width + x] = images[z][sliceIndex * width + x]; // Copy pixel value
            }
        }
        // Write the slice to a PNG file
        int success = stbi_write_png(outputFilename.c_str(), width, depth, 1, slice.data(), width);
        return success != 0; 
    }
    // If for some reason the function hasn't returned yet, return false
    return false;
}

#endif
