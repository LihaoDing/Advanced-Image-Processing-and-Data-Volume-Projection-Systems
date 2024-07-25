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

#ifndef PROJECTION_H
#define PROJECTION_H

#include <string>
#include <vector>

 /**
  * @class Projection
  *
  * @brief Provides functionality for creating various intensity projections from a stack of images.
  *
  * This class supports Maximum Intensity Projection (MIP), Minimum Intensity Projection (MinIP),
  * Average Intensity Projection (AIP), and Average Intensity Projection with Median (AIPMedian).
  * These projections are useful for visualizing specific features across a stack of 2D images.
  */
class Projection {
public:
    /**
     * @brief Constructs a new Projection object.
     */
    Projection();

    /**
     * Generates a Maximum Intensity Projection (MIP) from a series of image slices.
     *
     * MIP is a volume rendering method that projects the voxel with the highest intensity value along the
     * viewing direction onto a 2D plane. This function iterates through a stack of 2D images (slices), finds the
     * maximum pixel value across all images for each pixel position, and compiles these max values into a single
     * 2D image. The resulting 2D image highlights the highest intensity structures in the volume.
     *
     * @param images A vector of pointers to the image data slices.
     * @param width The width of the images in pixels.
     * @param height The height of the images in pixels.
     * @param channels The number of color channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param outputPath The file path where the resulting MIP image should be saved.
     * @return true if the MIP image was successfully saved; false otherwise.
     */
    bool MIP(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath);

    /**
     * Generates a Minimum Intensity Projection (MinIP) from a series of image slices.
     *
     * MinIP is a volume rendering technique that projects the voxel with the lowest intensity value along the
     * viewing direction onto a 2D plane. This function iterates through a stack of 2D images, finds the minimum
     * pixel value across all images for each pixel position, and compiles these min values into a single 2D image.
     * The resulting image emphasizes the lowest intensity structures in the volume.
     *
     * @param images A vector of pointers to the image data slices representing the 3D volume.
     * @param width The width of the images in pixels.
     * @param height The height of the images in pixels.
     * @param channels The number of color channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param outputPath The file path where the resulting MinIP image should be saved.
     * @return true if the MinIP image was successfully saved; false otherwise.
     */
    bool MinIP(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath);

    /**
     * Generates an Average Intensity Projection (AIP) from a series of image slices.
     *
     * AIP is a volume rendering technique that projects the average voxel intensity value along the viewing
     * direction onto a 2D plane. This function calculates the average pixel value across a stack of 2D images
     * for each pixel position and compiles these average values into a single 2D image. The resulting image
     * represents the mean intensity of structures in the volume, providing an overview of the entire volume's
     * content.
     *
     * @param images A vector of pointers to the image data slices representing the 3D volume.
     * @param width The width of the images in pixels.
     * @param height The height of the images in pixels.
     * @param channels The number of color channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param outputPath The file path where the resulting AIP image should be saved.
     * @return true if the AIP image was successfully saved; false otherwise.
     */
    bool AIP(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath);

    /**
     * Generates an Average Intensity Projection with Median (AIPMedian) from a series of image slices.
     *
     * This function combines the concept of Average Intensity Projection (AIP) with median filtering. Instead of
     * simply averaging pixel values across the stack of images, it gathers all pixel values for each position
     * across the stack, sorts them, and then selects the median value. This approach is particularly useful for
     * reducing the influence of outliers (such as noise) on the final projection, providing a clearer representation
     * of the underlying structures.
     *
     * @param images A vector of pointers to the image data slices representing the 3D volume.
     * @param width The width of the images in pixels.
     * @param height The height of the images in pixels.
     * @param channels The number of color channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param outputPath The file path where the resulting AIPMedian image should be saved.
     * @return true if the AIPMedian image was successfully saved; false otherwise.
     */
    bool AIPMedian(std::vector<unsigned char*>& images, int& width, int& height, int& channels, const std::string& outputPath);

private:
    /**
     * Partitions the vector around a pivot for the QuickSort algorithm.
     *
     * This function selects the last element in the vector as the pivot and rearranges the elements such that
     * all elements smaller than the pivot come before it, while all elements greater come after. It's a helper
     * function used by the QuickSort algorithm to sort the vector.
     *
     * @param vec The vector of unsigned chars to be partitioned.
     * @param low The starting index of the segment of the vector to be partitioned.
     * @param high The ending index of the segment of the vector to be partitioned.
     * @return The index where the pivot element is finally placed.
     */
    int partition(std::vector<unsigned char>& vec, int low, int high);

    /**
     * Sorts a vector using the QuickSort algorithm.
     *
     * QuickSort is a divide-and-conquer algorithm that picks an element as a pivot and partitions the given
     * array around the picked pivot. This function recursively applies QuickSort to sub-arrays to achieve
     * a fully sorted vector.
     *
     * @param vec The vector of unsigned chars to be sorted.
     * @param low The starting index of the segment of the vector to be sorted.
     * @param high The ending index of the segment of the vector to be sorted.
     */
    void quickSort(std::vector<unsigned char>& vec, int low, int high);
};

#endif // PROJECTION_H