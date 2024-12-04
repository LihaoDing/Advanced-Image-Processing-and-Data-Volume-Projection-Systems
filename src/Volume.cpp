#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <filesystem>
#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <regex>
#include <thread>
#include <cmath>
#include "Volume.h"
#define STB_IMAGE_IMPLEMENTATION_VOLUME
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION_VOLUME
#include "stb_image_write.h"

 /**
  * Default constructor for the Volume class.
  * Initializes the 'exist' flag to 0, indicating that no volume data is currently loaded.
  */
Volume::Volume() {
    this->exist = 0; // The constructor body is intentionally left empty.
}

/**
 * Retrieves the existence flag of the volume data.
 *
 * @return An integer indicating the existence of the volume data. A value of 0 implies no volume data is present;
 *         a non-zero value indicates that volume data exists.
 */
int Volume::getExist() {
    return this->exist;
}

/**
 * Retrieves the width of the volume in pixels.
 *
 * @return The width of the volume.
 */
int Volume::getWidth() {
    return this->width;
}

/**
 * Retrieves the height of the volume in pixels.
 *
 * @return The height of the volume.
 */
int Volume::getHeight() {
    return this->height;
}

/**
 * Retrieves the number of color channels in the volume.
 *
 * @return The number of channels in the volume.
 */
int Volume::getChannels() {
    return this->channels;
}

/**
 * Retrieves the folder path where the volume images are stored.
 *
 * @return A string representing the folder path of the volume.
 */
std::string Volume::getFolderPath() {
    return this->folderPath;
}

/**
 * Retrieves the volume data as a vector of image pointers.
 *
 * @return A vector containing pointers to the image data slices representing the 3D volume.
 */
std::vector<stbi_uc*> Volume::getImages() {
    return this->images;
}

/**
 * Sets the folder path where the volume images are stored.
 *
 * This method updates the `folderPath` attribute of the Volume object to the specified path. It first checks
 * if the provided folder path exists in the filesystem. If the path exists, the folder path is updated, and
 * the method returns true. If the path does not exist, an error message is displayed, and the method returns
 * false, indicating the update was unsuccessful.
 *
 * @param folderPath The new folder path where the volume images are to be stored.
 * @return A boolean value indicating the success of setting the folder path. Returns true if the folder path
 *         was successfully updated; otherwise, false.
 */
bool Volume::setFolderPath(const std::string& folderPath) {
    // Check if the provided folder path exists
    if (std::filesystem::exists(folderPath)) {
        this->folderPath = folderPath; // Update the folder path
        return true; // Indicate successful update
    }
    else {
        std::cerr << "Folder path does not exist" << std::endl;
        return false; // Indicate failure to update due to non-existent path
    }
}

/**
 * Sets the volume data by specifying a vector of image pointers.
 *
 * This method updates the `images` attribute of the Volume object with the provided vector of image pointers,
 * effectively setting the volume data. It checks if the provided vector contains any images. If it does, the
 * volume data is updated, and the method returns true, indicating success. If the vector is empty, indicating
 * no images are provided, an error message is displayed, and the method returns false, indicating failure.
 *
 * @param images A vector containing pointers to the image data slices that represent the 3D volume.
 * @return A boolean value indicating the success of setting the volume data. Returns true if the volume data
 *         was successfully updated with the provided images; otherwise, false.
 */
bool Volume::setImages(const std::vector<stbi_uc*>& images) {
    // Check if the provided vector contains any images
    if (images.size() > 0) {
        this->images = images; // Update the volume data with the provided images
        return true; // Indicate successful update
    }
    else {
        std::cerr << "No images to set" << std::endl;
        return false; // Indicate failure due to empty image vector
    }
}

/**
 * Loads all images from the specified directory, sorting them in order.
 *
 * This method iterates through the provided directory, loading all images into the volume data in a sorted order.
 * It first checks if the directory exists. If it does, the method iterates over each file in the directory,
 * loading only regular files as images. The images are sorted to maintain order, typically by filename. If
 * volume data already exists, it frees the existing images before loading new ones. Each successfully loaded
 * image is added to the volume's image list, and the existence flag is set.
 *
 * @param inputDir The path to the directory from which to load the images.
 * @return A boolean value indicating the success of the image loading process. Returns true if images were
 *         successfully loaded; otherwise, false, which could occur if the directory doesn't exist or an error
 *         is encountered during loading.
 */
bool Volume::loadImages(const std::string& inputDir) {
    std::cout << "Loading images from directory: " << inputDir << std::endl;

    try {
        if (!std::filesystem::exists(inputDir)) {
            std::cerr << "Directory does not exist" << std::endl;
            return false;
        }

        namespace fs = std::filesystem;
        std::vector<fs::path> paths;

        // Collect paths of all regular files in the directory
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file()) {
                paths.push_back(entry.path());
            }
        }

        // Sort the paths to ensure images are loaded in order
        quickSort(paths, 0, paths.size() - 1);

        // Free existing images if any
        if (this->exist) {
            for (auto img : images) {
                stbi_image_free(img);
            }
            images.clear();
        }

        // Load each image and add to the volume
        for (const auto& path : paths) {
            stbi_uc* img = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
            if (img) {
                images.push_back(img);
                std::cout << "Loaded image: " << path << std::endl;
                this->exist = 1;
            }
        }

        this->folderPath = inputDir; // Update the folder path
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading images: " << e.what() << std::endl;
        return false;
    }
}

/**
 * Saves all images in the volume to the specified directory.
 *
 * This method iterates through the images stored in the volume and saves each one as a PNG file in the
 * specified output directory. The directory is created if it does not already exist. Each image is saved with
 * a filename pattern "image_[index].png", where [index] is the zero-based index of the image in the volume.
 * The method prints a message to the console for each image it saves, indicating the progress.
 *
 * @param outputDir The path to the directory where the images will be saved. The directory will be created
 *                  if it doesn't exist.
 * @return A boolean value indicating the success of the image saving process. Always returns true, as failure
 *         to save an individual image does not halt the process or change the return value.
 */
bool Volume::saveImages(const std::string& outputDir) {
    namespace fs = std::filesystem;
    // Ensure the output directory exists, creating it if necessary
    fs::create_directories(outputDir);

    // Iterate through each image in the volume and save it
    for (size_t i = 0; i < images.size(); ++i) {
        std::cout << "Saving image " << i << "..." << std::endl;
        // Construct the output path for each image
        std::string outputPath = outputDir + "/image_" + std::to_string(i) + ".png";
        // Save the image as a PNG file
        stbi_write_png(outputPath.c_str(), width, height, 1, images[i], width);
    }
    return true; // Indicate successful completion of the saving process
}

/**
 * Applies a Gaussian filter to each image slice in the volume.
 *
 * This method enhances each slice by applying a Gaussian filter, which smooths the image using a Gaussian
 * kernel defined by the specified filter size and standard deviation (sigma). The Gaussian filter is effective
 * at reducing image noise and smoothing out variations while preserving edges to some extent.
 *
 * @param filterSize The size of the Gaussian kernel to use for the filter. Larger sizes result in more blur.
 * @param sigma The standard deviation of the Gaussian function, influencing the spread of the blur.
 * @return A boolean value indicating the success of the Gaussian filter application across the volume.
 *         Returns true if the filter was successfully applied to all slices; otherwise, false.
 */
bool Volume::applyGaussianFilter(int filterSize, double sigma) {
    return applyFilter(filterSize, 1, sigma = sigma);
}

/**
 * Applies a Median filter to each image slice in the volume.
 *
 * This method processes each slice with a Median filter, which reduces noise by replacing each pixel's value
 * with the median value of the intensities in its neighborhood. The filter size determines the size of the
 * neighborhood. Median filtering is particularly effective at removing 'salt and pepper' noise while preserving
 * edges.
 *
 * @param filterSize The size of the neighborhood around each pixel considered for finding the median value.
 * @return A boolean value indicating the success of the Median filter application across the volume.
 *         Returns true if the filter was successfully applied to all slices; otherwise, false.
 */
bool Volume::applyMedianFilter(int filterSize) {
    return applyFilter(filterSize, 0, 0);
}

/**
 * Generates a Maximum Intensity Projection (MIP) from a specified range of image slices in the volume.
 *
 * MIP is a volume rendering technique that projects the highest intensity value encountered along a viewing
 * direction onto a 2D plane. This method creates a MIP using either the entire set of images in the volume
 * or a specified subset defined by `startIndex` and `endIndex`. The result is saved to the specified output
 * path. If no range is specified (both indices are 0), the entire volume is used. The method validates the
 * specified range to ensure it is within the bounds of the available images.
 *
 * @param outputPath The file path where the resulting MIP image will be saved.
 * @param startIndex The index of the first image in the subset to be used for the MIP (1-based index).
 * @param endIndex The index of the last image in the subset to be used for the MIP (inclusive).
 * @return A boolean value indicating the success of the MIP generation. Returns true if the MIP was successfully
 *         created and saved; otherwise, false, which could occur due to missing images or invalid indices.
 */
bool Volume::MaxProjection(const std::string& outputPath, size_t startIndex, size_t endIndex) {
    size_t n = images.size();
    if (n == 0) {
        std::cerr << "No images to project" << std::endl;
        return false; // No images available for projection
    }

    // Use the entire volume if default indices are provided
    if (startIndex == 0 && endIndex == 0) {
        return projection.MIP(images, width, height, channels, outputPath);
    }

    // Validate the specified range of indices
    if (startIndex <= 0 || endIndex > n || startIndex > endIndex) {
        std::cerr << "Invalid range specified" << std::endl;
        return false; // Specified range is invalid
    }

    // Create a subset of images for the specified range
    std::vector<stbi_uc*> imageSubset(images.begin() + startIndex - 1, images.begin() + endIndex);

    // Generate and save the MIP from the subset
    return projection.MIP(imageSubset, width, height, channels, outputPath);
}

/**
 * Creates a Minimum Intensity Projection (MinIP) from a subset of the volume's images.
 *
 * MinIP emphasizes the darkest (minimum intensity) features within the volume by projecting the lowest intensity
 * values along the viewing direction onto a 2D plane. This method can operate on the entire volume or a specified
 * range of slices, defined by `startIndex` and `endIndex`. If the indices are set to their default values (0),
 * the entire volume is used. The projection is saved to the specified output path.
 *
 * @param outputPath The file path where the MinIP image should be saved.
 * @param startIndex The index of the first image slice to include in the projection. If 0, the projection starts
 *                   from the first image in the volume.
 * @param endIndex The index of the last image slice to include in the projection. If 0, the projection includes
 *                 all images up to the last one in the volume.
 * @return A boolean value indicating the success of the MinIP creation. Returns true if the MinIP was
 *         successfully created and saved; otherwise, false.
 */
bool Volume::MinProjection(const std::string& outputPath, size_t startIndex, size_t endIndex) {
    size_t n = images.size();
    if (n == 0) {
        std::cerr << "No images to project" << std::endl;
        return false;
    }

    // Use default indices to include all images if not specified
    if (startIndex == 0 && endIndex == 0) {
        return projection.MinIP(images, width, height, channels, outputPath);
    }

    // Validate the specified range
    if (startIndex <= 0 || endIndex > n || startIndex > endIndex) {
        std::cerr << "Invalid range specified" << std::endl;
        return false;
    }

    // Subset the images based on the specified range
    std::vector<stbi_uc*> imageSubset(images.begin() + startIndex - 1, images.begin() + endIndex);

    return projection.MinIP(imageSubset, width, height, channels, outputPath);
}

/**
 * Creates an Average Intensity Projection (AIP) from a subset of the volume's images.
 *
 * AIP represents the average intensity of the features within the volume by projecting the mean intensity
 * values along the viewing direction onto a 2D plane. This method can be applied to the entire volume or a
 * specified range of slices, determined by `startIndex` and `endIndex`. If the indices are default (0), the
 * entire volume is processed. The resulting projection is saved to the specified output path.
 *
 * @param outputPath The file path where the AIP image should be saved.
 * @param startIndex The index of the first image slice to include in the projection. If 0, the projection
 *                   includes all images from the beginning of the volume.
 * @param endIndex The index of the last image slice to include in the projection. If 0, the projection includes
 *                 all images up to the end of the volume.
 * @return A boolean value indicating the success of the AIP creation. Returns true if the AIP was successfully
 *         created and saved; otherwise, false.
 */
bool Volume::AverageProjection(const std::string& outputPath, size_t startIndex, size_t endIndex) {
    size_t n = images.size();
    if (n == 0) {
        std::cerr << "No images to project" << std::endl;
        return false;
    }

    // Use default indices to include all images if not specified
    if (startIndex == 0 && endIndex == 0) {
        return projection.AIP(images, width, height, channels, outputPath);
    }

    // Validate the specified range
    if (startIndex <= 0 || endIndex > n || startIndex > endIndex) {
        std::cerr << "Invalid range specified" << std::endl;
        return false;
    }

    // Subset the images based on the specified range
    std::vector<stbi_uc*> imageSubset(images.begin() + startIndex - 1, images.begin() + endIndex);

    return projection.AIP(imageSubset, width, height, channels, outputPath);
}

/**
 * Creates an Average Intensity Projection with Median (AIPMedian) from a subset of the volume's images.
 *
 * AIPMedian combines the concepts of average intensity projection and median filtering. It calculates the median
 * intensity value across the specified range of image slices and projects these median values onto a 2D plane.
 * This method can operate on the entire volume or a specified subset, defined by `startIndex` and `endIndex`.
 * If the indices are default (0), the entire volume is considered. The resulting projection, emphasizing median
 * intensity values, is saved to the specified output path.
 *
 * @param outputPath The file path where the AIPMedian image should be saved.
 * @param startIndex The index of the first image slice to include in the projection. If 0, the projection starts
 *                   from the first image in the volume.
 * @param endIndex The index of the last image slice to include in the projection. If 0, the projection includes
 *                 all images up to the last one in the volume.
 * @return A boolean value indicating the success of the AIPMedian creation. Returns true if the AIPMedian was
 *         successfully created and saved; otherwise, false.
 */
bool Volume::AverageProjectionMedian(const std::string& outputPath, size_t startIndex, size_t endIndex) {
    size_t n = images.size();
    if (n == 0) {
        std::cerr << "No images to project" << std::endl;
        return false;
    }

    // Use default indices to include all images if not specified
    if (startIndex == 0 && endIndex == 0) {
        return projection.AIPMedian(images, width, height, channels, outputPath);
    }

    // Validate the specified range
    if (startIndex <= 0 || endIndex > n || startIndex > endIndex) {
        std::cerr << "Invalid range specified" << std::endl;
        return false;
    }

    // Subset the images based on the specified range
    std::vector<stbi_uc*> imageSubset(images.begin() + startIndex - 1, images.begin() + endIndex);

    return projection.AIPMedian(imageSubset, width, height, channels, outputPath);
}

/**
 * Applies a specified filter to the entire volume of images.
 *
 * This method supports the application of either a 3D Median filter or a 3D Gaussian filter to the volume.
 * The type of filter to apply is determined by the `type` parameter. A 3D Median filter is useful for removing
 * noise while preserving edges, and a 3D Gaussian filter is effective at smoothing and reducing detail and noise.
 * The `filterSize` parameter determines the size of the kernel used for the filter, and the `sigma` parameter
 * (relevant only for the Gaussian filter) determines the standard deviation of the Gaussian function, affecting
 * the spread of the blur.
 *
 * @param filterSize The size of the kernel used for the filtering. Larger sizes produce a stronger effect.
 * @param type The type of filter to apply: 0 for 3D Median filter, 1 for 3D Gaussian filter.
 * @param sigma The standard deviation of the Gaussian function, applicable only for the Gaussian filter.
 *              Defaults to 1.0 if not specified.
 * @return A boolean value indicating the success of the filter application. Returns true if the filter was
 *         successfully applied; otherwise, false, typically due to the absence of volume data.
 */
bool Volume::applyFilter(int filterSize, int type, double sigma = 1.0) {
    if (images.empty()) {
        std::cerr << "No images to apply filter" << std::endl;
        return false;
    }

    int depth = images.size();
    // Apply the specified filter based on the 'type' parameter
    if (type == 0) {
        // Apply 3D Median filter
        filter.apply3DMedianFilter(images, width, height, depth, filterSize);
    }
    else if (type == 1) {
        // Apply 3D Gaussian filter
        filter.apply3DGaussianFilter(images, width, height, depth, filterSize, sigma);
    }

    // Log the applied filter type
    if (type == 0) {
        std::cout << "3D Median filter applied" << std::endl;
    }
    else if (type == 1) {
        std::cout << "3D Gaussian filter applied" << std::endl;
    }
    return true;
}

/**
 * Slices the 3D volume along the specified plane and saves the slice as an image.
 *
 * This method extracts a 2D slice from the 3D volume data along the specified plane (XY, YZ, or XZ) at the given
 * slice index. The extracted slice is then saved as an image file to the provided output filename. This functionality
 * is useful for visualizing cross-sections of the volume at specific depths or positions. It leverages the `Slice`
 * class to handle the extraction and saving process.
 *
 * @param plane The plane along which to slice the volume. The plane can be XY, YZ, or XZ, represented by an
 *              enumeration type `SlicePlane`.
 * @param sliceIndex The index of the slice to be extracted. This index corresponds to the position along the
 *                   axis perpendicular to the slicing plane.
 * @param outputFilename The path and filename where the extracted slice image should be saved.
 * @return A boolean value indicating the success of the slicing and saving operation. Returns true if the slice
 *         was successfully extracted and saved; otherwise, false.
 */
bool Volume::slice3DVolume(SlicePlane plane, int sliceIndex, const std::string& outputFilename) {
    if (images.size() == 0) {
        std::cerr << "No images to slice" << std::endl;
        return false;
    }

    Slice slice;
    return slice.extractAndSaveSlice(images, width, height, sliceIndex, plane, outputFilename);
}

/**
 * Partitions the array of filesystem paths based on the numerical part of the file stem.
 *
 * This method is part of the QuickSort algorithm and is used to rearrange the elements of the `paths` array
 * such that elements less than a chosen pivot are placed before it, and elements greater are placed after it.
 * The pivot is chosen to be the last element in the current segment defined by `low` and `high` indices. The
 * numerical part used for comparison is extracted from the file stem's last four characters, assuming a fixed
 * format for naming.
 *
 * @param paths A reference to the vector of filesystem paths to be partitioned.
 * @param low The starting index of the segment of the array to be partitioned.
 * @param high The ending index of the segment of the array to be partitioned.
 * @return The index position of the pivot after partitioning.
 */
int Volume::partition(std::vector<std::filesystem::path>& paths, int low, int high) {
    namespace fs = std::filesystem;
    fs::path pivot = paths[high];
    std::string namePivot = pivot.stem().string();
    // Assuming the numerical part is in the last four characters of the stem
    int numPivot = std::stoi(namePivot.substr(namePivot.length() - 4));
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        std::string nameJ = paths[j].stem().string();
        int numJ = std::stoi(nameJ.substr(nameJ.length() - 4));
        if (numJ < numPivot) {
            i++;
            std::swap(paths[i], paths[j]);
        }
    }
    std::swap(paths[i + 1], paths[high]);
    return (i + 1);
}

/**
 * Sorts a vector of filesystem paths using the QuickSort algorithm.
 *
 * This method recursively sorts the elements of the `paths` vector by dividing the vector into smaller
 * segments and then sorting those segments. The sorting is based on a numerical part extracted from the
 * file stem, assuming a specific naming format. The `partition` function is used to choose a pivot and
 * rearrange the elements such that those less than the pivot come before it and those greater come after.
 * The process is then repeated for the segments before and after the pivot until the entire vector is sorted.
 *
 * @param paths A reference to the vector of filesystem paths to be sorted.
 * @param low The starting index for the segment of the vector to be sorted.
 * @param high The ending index for the segment of the vector to be sorted.
 */
void Volume::quickSort(std::vector<std::filesystem::path>& paths, int low, int high) {
    if (low < high) {
        // pi is partitioning index, paths[pi] is now at right place
        int pi = partition(paths, low, high);

        // Recursively sort elements before partition and after partition
        quickSort(paths, low, pi - 1);
        quickSort(paths, pi + 1, high);
    }
}
