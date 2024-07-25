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

#ifndef FILTER_H
#define FILTER_H

#include <vector>

 /**
  * @class Filter
  *
  * @brief Implements various image processing filters and operations.
  *
  * This class provides functionalities for applying various filters and image processing operations such as color correction,
  * blurring, edge detection, and histogram equalization. It supports operations on both 2D and 3D images.
  */
class Filter {
public:
    /**
     * @brief Construct a new Filter object.
     */
    Filter();

    // Colour Correction and Per-Pixel Modifiers

    /**
     * Applies a grayscale filter to the provided image data.
     *
     * This function converts an RGB or RGBA image to grayscale by calculating the luminance of each pixel
     * using the formula 0.2126*R + 0.7152*G + 0.0722*B and sets this value for the corresponding pixel in the
     * grayscale image. This function assumes a linear color space.
     *
     * @param data A pointer to the image data array. Each pixel is expected to have 'c' components (RGB/RGBA).
     * @param w The width of the image in pixels.
     * @param h The height of the image in pixels.
     * @param c The number of color channels per pixel (3 for RGB, 4 for RGBA).
     * @return A pointer to the newly allocated grayscale image data, or nullptr on failure.
     *         The caller is responsible for freeing this memory.
     *
     * Note: The function will print an error message to stderr if the input data is null or if the image
     *       is not in RGB/RGBA format.
     */
    unsigned char* applyGrayscaleFilter(unsigned char* data, int w, int h, int c);
    
    /**
     * Applies a brightness adjustment to the provided image data.
     *
     * This function adjusts the brightness of an image by adding a specified brightness value to each component
     * (R, G, B, and optionally A) of every pixel. If the brightness adjustment results in a value outside the
     * 0-255 range, it is clamped to fit within this range to avoid overflow.
     *
     * @param data A pointer to the image data array. Each pixel is expected to have 'c' components.
     * @param w The width of the image in pixels.
     * @param h The height of the image in pixels.
     * @param c The number of color channels per pixel (e.g., 3 for RGB, 4 for RGBA).
     * @param brightness The brightness adjustment value. Positive values increase brightness, negative values decrease it.
     * @return A pointer to the newly allocated brightened image data, or nullptr on failure.
     *         The caller is responsible for freeing this memory.
     *
     * Note: The function will print an error message to stderr if the input data is null.
     *       If the brightness parameter is 0, the function calculates an average brightness value based on the
     *       sum of all pixel values and uses this as the adjustment value, which is not a typical use case for brightness adjustment.
     */
    unsigned char* applyBrightnessFilter(unsigned char* data, int w, int h, int c, int brightness = 0);
    
    /**
     * Applies histogram equalization to an image with support for 1, 3, or 4 channels.
     *
     * This function applies histogram equalization to an image's data in-place. It supports grayscale images
     * (1 channel), RGB images (3 channels), and RGBA images (4 channels). For grayscale images, equalization
     * is applied directly. For RGB(A) images, the function can optionally use HSL/HSV color space for equalization,
     * which typically yields more visually pleasing results, though it's computationally more intensive.
     * The alpha channel in RGBA images, if present, is preserved and not altered by the equalization process.
     *
     * @param data Pointer to the image data to be equalized. This data is modified in-place.
     * @param w Width of the image in pixels.
     * @param h Height of the image in pixels.
     * @param channels Number of color channels per pixel (1, 3, or 4).
     * @param use_hsl A boolean flag indicating whether to use HSL color space for equalization (applies only to RGB(A) images).
     * @return Pointer to the modified image data. If the input data is null or the number of channels is unsupported, returns nullptr.
     */
    unsigned char* applyHistogramEqualization(unsigned char* data, int w, int h, int channels, bool use_hsl);
    
    /**
     * Applies a threshold filter to an image.
     *
     * This function processes an image and applies a threshold filter, converting the image into a binary
     * image where each pixel is set to black or white depending on whether its luminance or value (for greyscale
     * or color images, respectively) is below or above a specified threshold. For color images, the function
     * can operate in either the HSL or HSV color space to determine the luminance or value.
     *
     * @param data Pointer to the original image data.
     * @param w Width of the image in pixels.
     * @param h Height of the image in pixels.
     * @param c Number of channels per pixel (1 for grayscale, 3 for RGB).
     * @param threshold The luminance/value level below which pixels are set to black and above which to white.
     * @param use_hsl Boolean flag to choose between HSL (true) and HSV (false) for color space conversion in color images.
     * @return Pointer to the new image data after applying the threshold filter. The caller is responsible for freeing this memory.
     */
    unsigned char* applyThresholdFilter(unsigned char* data, int w, int h, int c, int threshold, bool use_hsl);
    
    /**
     * Applies a salt and pepper noise filter to an image.
     *
     * This function creates and returns a new image with salt and pepper noise added. Salt and pepper noise
     * consists of randomly distributed black (pepper) and white (salt) pixels. The percentage of pixels affected
     * by the noise is determined by the `percentage_sp` parameter. The function works on images with any number
     * of channels, applying the same noise value across all channels of a pixel to maintain the noise's grayscale
     * nature.
     *
     * @param data Pointer to the original image data.
     * @param w Width of the image in pixels.
     * @param h Height of the image in pixels.
     * @param c Number of channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param percentage_sp Percentage of total pixels that will be affected by noise, between 0 and 100.
     * @return Pointer to the new image data with salt and pepper noise. The caller is responsible for freeing this memory.
     */
    unsigned char* applySpFilter(unsigned char* data, int w, int h, int c, int percentage_sp);
   
    // 2D Image Blur

    /**
     * Applies a 2D Gaussian blur to an image using a generated Gaussian kernel.
     *
     * This function blurs an image by convolving it with a Gaussian kernel. The kernel is generated based on
     * the specified size and sigma (standard deviation), which determine the amount of blur. The function
     * supports images with multiple channels (e.g., RGB or RGBA) and applies the blur to each channel independently.
     * Edge pixels are handled using the 'copy edge' padding method, where pixels outside the image boundaries
     * are assumed to have the same value as the nearest edge pixel.
     *
     * @param data Pointer to the original image data.
     * @param width Width of the image in pixels.
     * @param height Height of the image in pixels.
     * @param channels Number of channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param kernelSize The size of the Gaussian kernel. It influences the blur amount and must be an odd number.
     * @param sigma The standard deviation of the Gaussian distribution. Higher values result in more blur.
     * @return Pointer to the new image data after applying the Gaussian blur. The caller is responsible for freeing this memory.
     */
    unsigned char* apply2DGaussianFilter(unsigned char* data, int width, int height, int channels, int size, float sigma);

    /**
     * Applies a 2D median blur filter to an image.
     *
     * This function processes an image by applying a median filter, which replaces each pixel's value with the median
     * value of the intensities in the pixel's neighborhood, defined by the kernel size. The median filter is applied
     * independently to each channel of the image, making it suitable for both grayscale and multi-channel images (e.g., RGB).
     * This technique is effective at reducing "salt and pepper" noise while preserving edges in the image.
     *
     * @param data Pointer to the original image data.
     * @param output Pointer to the memory where the filtered image data should be stored. This memory should be
     *               pre-allocated with the same size as the input data.
     * @param w Width of the image in pixels.
     * @param h Height of the image in pixels.
     * @param c Number of channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param kernelSize Size of the square kernel used for the median calculation. Must be an odd number.
     */
    void apply2DMedianBlurFilter(unsigned char* data, unsigned char* output, int w, int h, int c, int kernelSize);
    
    /**
     * Applies a box blur to an image.
     *
     * Box blur, also known as average blur, is a simple image blurring technique that smooths an image by
     * replacing each pixel's value with the average value of the pixel and its neighbors. This function applies
     * box blur to each channel of the image independently, making it suitable for both grayscale and multi-channel
     * images (e.g., RGB). The function uses a square kernel of the specified size to determine the neighborhood
     * of each pixel.
     *
     * @param data Pointer to the original image data.
     * @param w Width of the image in pixels.
     * @param h Height of the image in pixels.
     * @param c Number of channels per pixel (e.g., 1 for grayscale, 3 for RGB).
     * @param kernelSize Size of the square kernel used for the blur. Larger kernel sizes result in more blur.
     * @return Pointer to the new image data after applying the box blur. The caller is responsible for freeing this memory.
     */
    unsigned char* applyBoxBlur(unsigned char* data, int w, int h, int c, int kernelSize);

    // 2D Edge Detection

    /**
     * Applies the Sobel operator to an image to detect edges.
     *
     * The Sobel operator calculates the gradient of the image intensity at each pixel, providing the direction
     * of the largest possible increase from light to dark and the rate of change in that direction. This method
     * highlights high-frequency components in the image, marking the edges. It uses two 3x3 kernels, one estimating
     * the gradient in the x-direction (horizontal) and the other in the y-direction (vertical).
     *
     * @param image Pointer to the original grayscale image data.
     * @param width Width of the image in pixels.
     * @param height Height of the image in pixels.
     * @return Pointer to the new image data after applying the Sobel filter. The caller is responsible for freeing this memory.
     */
    unsigned char* sobelFilter(unsigned char* data, int width, int height);
    
    /**
     * Applies the Prewitt operator to an image for edge detection.
     *
     * Similar to the Sobel operator, the Prewitt operator is used for detecting vertical and horizontal edges
     * in images. It employs two 3x3 kernels to estimate gradients in the x and y directions. The magnitude of
     * the gradient at each pixel gives the edge strength. The Prewitt operator is simpler and less sensitive to
     * image noise compared to Sobel.
     *
     * @param image Pointer to the grayscale image data.
     * @param width Width of the image.
     * @param height Height of the image.
     * @return Pointer to the edge-detected image data.
     */
    unsigned char* prewittFilter(unsigned char* data, int width, int height);

    // Scharr operator implementation for edge detection
    /**
     * Applies the Scharr operator to an image to detect edges.
     *
     * The Scharr operator is similar to the Sobel and Prewitt operators but uses a different convolution kernel
     * that provides better rotation symmetry. This can lead to improved edge detection performance in certain
     * images. Like Sobel and Prewitt, Scharr calculates the gradient magnitude at each pixel to highlight edges.
     *
     * @param image Pointer to the grayscale image data.
     * @param width Width of the image.
     * @param height Height of the image.
     * @return Pointer to the edge-detected image data.
     */
    unsigned char* scharrFilter(unsigned char* data, int width, int height);
    
    /**
     * Applies Roberts' Cross operator to an image for edge detection.
     *
     * This function uses Roberts' Cross operator to identify edges in an image. The operator uses a pair of
     * 2x2 convolution kernels to calculate the gradient of an image at each pixel. The gradient magnitude
     * is used to highlight edges. Due to the small size of the kernel, this operator is highly sensitive to
     * noise and is less commonly used for precise edge detection in modern applications.
     *
     * @param image Pointer to the original grayscale image data.
     * @param width Width of the image in pixels.
     * @param height Height of the image in pixels.
     * @return Pointer to the new image data with edges highlighted. The caller is responsible for freeing this memory.
     */
    unsigned char* robertsCrossFilter(unsigned char* data, int width, int height);

    // 3D Image Blur

    /**
     * Applies a 3D median filter to a sequence of 2D images, treating them as slices of a 3D volume.
     *
     * This function processes a 3D volume represented as a vector of 2D image slices. It applies a median filter
     * in a 3D neighborhood around each voxel (3D pixel), replacing the voxel's value with the median value in its
     * local 3D neighborhood. The function is useful for reducing noise in 3D image data (e.g., medical imaging)
     * while preserving edges. The filtered images replace the original images, and the memory for the old images
     * is freed.
     *
     * @param images A vector of pointers to the image data for each slice of the 3D volume.
     * @param width The width of each 2D image slice in pixels.
     * @param height The height of each 2D image slice in pixels.
     * @param depth The number of slices in the 3D volume.
     * @param filterSize The size of the cubic kernel used for the median calculation. Must be an odd number.
     */
    void apply3DMedianFilter(std::vector<unsigned char*>& images, int width, int height, int depth, int filterSize);
    
    /**
     * Applies a 3D Gaussian filter to a sequence of 2D image slices, treating them as a 3D volume.
     *
     * This function smooths a 3D volume, represented as a stack of 2D image slices, using a Gaussian kernel.
     * The smoothing is performed in all three dimensions, making it particularly useful for volumetric data,
     * such as medical images or 3D simulations. Each voxel's new value is computed as the weighted sum of its
     * neighbors, with weights defined by the Gaussian kernel. This process reduces noise and smooths transitions
     * between regions in the volume.
     *
     * @param images A vector of pointers to the image data for each slice of the 3D volume.
     * @param width The width of each 2D image slice in pixels.
     * @param height The height of each 2D image slice in pixels.
     * @param depth The number of slices in the 3D volume.
     * @param filterSize The size of the cubic Gaussian kernel. Determines the extent of smoothing.
     * @param sigma The standard deviation of the Gaussian distribution. Controls the spread of the blur.
     */
    void apply3DGaussianFilter(std::vector<unsigned char*>& images, int width, int height, int depth, int filterSize, double sigma);

private:
    // Color Space Conversion

    /**
     * Converts RGB color space to HSV color space.
     *
     * This function takes individual RGB components (with values from 0 to 255), converts them to HSV format,
     * and outputs the results as references to the input parameters for hue, saturation, and value.
     *
     * @param r Red component of the color, value range from 0 to 255.
     * @param g Green component of the color, value range from 0 to 255.
     * @param b Blue component of the color, value range from 0 to 255.
     * @param h Reference to output hue component, value range from 0 to 360 degrees.
     * @param s Reference to output saturation component, value range from 0 to 1.
     * @param v Reference to output value component, value range from 0 to 1.
     */
    void RGBtoHSV(float r, float g, float b, float& h, float& s, float& v);
    
    /**
     * Converts HSV color space to RGB color space.
     *
     * This function takes hue (h), saturation (s), and value (v) in HSV format, converts them to RGB format,
     * and outputs the RGB components as references to the input parameters for red, green, and blue.
     * It handles conversion across the entire color wheel and accounts for different sectors.
     *
     * @param h Hue component of the color, value range from 0 to 360 degrees.
     * @param s Saturation component of the color, value range from 0 to 1.
     * @param v Value component of the color, value range from 0 to 1.
     * @param r Reference to output red component, value range from 0 to 255.
     * @param g Reference to output green component, value range from 0 to 255.
     * @param b Reference to output blue component, value range from 0 to 255.
     */
    void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b);
    
    /**
     * Converts RGB color space to HSL color space.
     *
     * This function takes individual RGB components (with values from 0 to 255), converts them to HSL format,
     * and outputs the HSL components as references to the input parameters for hue, saturation, and lightness.
     * The hue is calculated based on which RGB component is the max and adjusted for the HSL color wheel.
     *
     * @param r Red component of the color, value range from 0 to 255.
     * @param g Green component of the color, value range from 0 to 255.
     * @param b Blue component of the color, value range from 0 to 255.
     * @param h Reference to output hue component, value range from 0 to 1 (where 1 represents 360 degrees).
     * @param s Reference to output saturation component, value range from 0 to 1.
     * @param l Reference to output lightness component, value range from 0 to 1.
     */
    void RGBtoHSL(float r, float g, float b, float& h, float& s, float& l);
    
    /**
     * Converts HSL color space to RGB color space.
     *
     * This function takes hue (h), saturation (s), and lightness (l) in HSL format, converts them to RGB format,
     * and outputs the RGB components as references to the input parameters for red, green, and blue.
     * The conversion involves using an intermediate helper function 'hue2rgb' for the conversion logic.
     *
     * @param h Hue component of the color, value range from 0 to 1 (where 1 represents 360 degrees).
     * @param s Saturation component of the color, value range from 0 to 1.
     * @param l Lightness component of the color, value range from 0 to 1.
     * @param r Reference to output red component, value range from 0 to 255.
     * @param g Reference to output green component, value range from 0 to 255.
     * @param b Reference to output blue component, value range from 0 to 255.
     */
    void HSLtoRGB(float h, float s, float l, float& r, float& g, float& b);

    // Histogram Computation and Equalization

    /**
     * Computes the histogram of an image using raw pixel data.
     *
     * This function creates a histogram with 256 bins for an image represented as a raw data array. It works by
     * iterating through the image data, incrementing the count in the corresponding bin for each pixel's value.
     * The function is designed to work with grayscale images or the first channel of multi-channel images.
     *
     * @param data Pointer to the image data. For multi-channel images, only the first channel is considered.
     * @param size The total number of pixels in the image.
     * @param channels The number of channels per pixel (e.g., 3 for RGB, 1 for grayscale).
     * @return A std::vector<unsigned int> representing the histogram, where the index corresponds to the pixel
     *         value, and the value at that index is the count of pixels with that value.
     */
    std::vector<unsigned int> computeHistogram(unsigned char* data, int size, int channels);
    
    /**
     * Computes the histogram for a L/V-channel of the image represented as a vector of float values.
     *
     * This variant of the computeHistogram function is intended for use with data where pixel values are stored
     * as floats, such as the Luminance or Value channel in HSL or HSV color spaces, respectively. It assumes the
     * data has already been scaled to the range [0, 255].
     *
     * @param data A std::vector<float> containing the image data, where each element represents a pixel's value.
     * @return A std::vector<unsigned int> representing the histogram, similar to the unsigned char* data version.
     */
    std::vector<unsigned int> computeHistogram(const std::vector<float>& data);
    
    /**
     * Performs histogram equalization on a given histogram.
     *
     * Histogram equalization is a technique used to improve the contrast in an image. This function calculates
     * the Cumulative Distribution Function (CDF) from the given histogram and uses it to map the original pixel
     * values to new values that spread out more evenly across the available range. This typically enhances the
     * overall contrast of the image.
     *
     * @param histogram A std::vector<unsigned int> representing the original histogram of the image. Each element
     *                  in the vector corresponds to the count of pixels with a specific intensity value.
     * @param pixels The total number of pixels in the image.
     * @return A std::vector<unsigned char> containing the new pixel values after equalization. The index in the
     *         vector corresponds to the original pixel value, and the value at that index is the new pixel value.
     */
    std::vector<unsigned char> equalizeHistogram(const std::vector<unsigned int>& histogram, int pixels);
    
    /**
     * Applies histogram equalization mapping to image data.
     *
     * This function modifies the original image data in-place by mapping each pixel's value according to the
     * equalized_values vector obtained from histogram equalization. The function supports both grayscale and
     * RGB images. For RGB images, the equalization is applied only to the first channel (assumed to be luminance
     * or a similar concept), and the same equalized value is copied to the other two channels, which may not be
     * visually accurate but preserves the overall brightness transformation.
     *
     * @param data Pointer to the original image data, which will be modified in-place.
     * @param equalized_values A std::vector<unsigned char> containing the mapping from original to equalized
     *                         pixel values, as obtained from histogram equalization.
     * @param size The total number of pixels in the image.
     * @param channels The number of channels per pixel (1 for grayscale, 3 for RGB).
     */
    void applyEqualization(unsigned char* data, const std::vector<unsigned char>& equalized_values, int size, int channels);
    
    /**
     * Applies RGB equalization to an image, optionally handling an alpha channel.
     *
     * This function performs histogram equalization on RGB or RGBA images. It can optionally convert RGB to HSL
     * (or HSV), perform equalization on the Luminance (or Value) channel, and then convert back to RGB. This
     * approach can yield more visually pleasing results. The function preserves the alpha channel in RGBA images
     * without modification.
     *
     * @param data Pointer to the image data. This data is modified in-place.
     * @param w Width of the image in pixels.
     * @param h Height of the image in pixels.
     * @param channels Number of color channels per pixel. Must be 3 (RGB) or 4 (RGBA).
     * @param use_hsl If true, uses HSL for equalization; otherwise, uses HSV (or directly manipulates RGB).
     */
    void applyRGBEqualization(unsigned char* data, int w, int h, int channels, bool use_hsl);

    // Gaussian Kernel Generation

    /**
     * A helper function generates a 2D Gaussian kernel used for applying Gaussian blur to an image.
     *
     * This function creates a square Gaussian kernel matrix of the specified size and standard deviation (sigma).
     * The kernel is used to apply a Gaussian blur, which smooths an image by weighting nearby pixels according
     * to the Gaussian distribution. The kernel values decrease with distance from the center, and the kernel is
     * normalized so that the sum of all its elements equals 1, ensuring the overall brightness of the image is
     * maintained after blurring.
     *
     * @param kernelSize The size of one side of the square kernel. It must be an odd number to have a central pixel.
     * @param sigma The standard deviation of the Gaussian distribution. A higher sigma value results in more blur.
     * @return A 2D vector of floats representing the Gaussian kernel. The dimensions of the kernel are kernelSize x kernelSize.
     */
    std::vector<std::vector<float>> generate2DGaussianKernel(int kernelSize, float sigma);
    
    /**
     * Generates a 3D Gaussian kernel for use in 3D Gaussian filtering.
     *
     * This function constructs a cubic Gaussian kernel with the specified size and standard deviation (sigma).
     * The kernel values are calculated based on the Gaussian distribution formula and are normalized so that
     * the sum of all elements equals 1. This normalization ensures that the overall brightness of the 3D volume
     * remains consistent after applying the Gaussian filter. The 3D Gaussian kernel is used to apply smoothing
     * in three dimensions, which is particularly useful for volumetric data such as medical images (MRI, CT scans).
     *
     * @param size The side length of the cubic kernel. Must be an odd number to ensure a central element.
     * @param sigma The standard deviation of the Gaussian distribution. Controls the amount of blur.
     * @return A 3D vector of doubles representing the Gaussian kernel. The dimensions of the kernel are size x size x size.
     */
    std::vector<std::vector<std::vector<double>>> generate3DGaussianKernel(int size, double sigma);

    // Median Calculation Helpers

    /**
     * Partitions the vector around a pivot for QuickSort.
     *
     * This function selects the last element in the vector as the pivot, then rearranges the elements so that
     * all elements smaller than the pivot come before it, and all elements greater come after. It's a helper
     * function used by the QuickSort algorithm to sort the vector.
     *
     * @param vec The vector of unsigned chars to be partitioned.
     * @param low The starting index of the segment of the vector to be partitioned.
     * @param high The ending index of the segment of the vector to be partitioned.
     * @return The partitioning index after which the pivot is placed.
     */
    int partition(std::vector<unsigned char>& vec, int low, int high);
    
    /**
     * Sorts a vector using the QuickSort algorithm.
     *
     * QuickSort is a divide-and-conquer algorithm that picks an element as a pivot and partitions the given
     * array around the picked pivot. This function recursively applies QuickSort to sub-arrays.
     *
     * @param vec The vector of unsigned chars to be sorted.
     * @param low The starting index of the segment of the vector to be sorted.
     * @param high The ending index of the segment of the vector to be sorted.
     */
    void quickSort(std::vector<unsigned char>& vec, int low, int high);
    
    // Retrieves the median value from a vector
    /**
     * Finds the median of a vector.
     *
     * This function sorts the vector using QuickSort and then returns the median value. The median is the
     * middle value in the sorted vector, or the average of the two middle values if the vector has an even
     * number of elements.
     *
     * @param vec The vector from which to find the median value.
     * @return The median value from the sorted vector.
     */
    unsigned char getMedian(std::vector<unsigned char>& vec);

    // Convolution and Edge Detection Helpers

    /**
     * Retrieves the value of a pixel from an image, applying edge padding.
     *
     * This helper function is used to safely access pixel values from an image, applying padding at the edges
     * to handle boundary conditions. If the requested pixel lies outside the image boundaries, the function
     * returns the value of the nearest edge pixel, effectively replicating the edge pixels.
     *
     * @param image Pointer to the image data.
     * @param x The x-coordinate of the desired pixel.
     * @param y The y-coordinate of the desired pixel.
     * @param width The width of the image in pixels.
     * @param height The height of the image in pixels.
     * @return The value of the pixel at (x, y), or the nearest edge pixel if (x, y) is outside the image boundaries.
     */
    unsigned char getPixel(unsigned char* image, int x, int y, int width, int height);
    
    /**
     * Applies a convolution kernel to a specific pixel in an image.
     *
     * This function performs convolution by overlaying a kernel on top of the image centered at a specific
     * pixel, multiplying corresponding elements, and summing the results. It's a core operation in many image
     * processing tasks, such as blurring, sharpening, and edge detection. The function safely handles edge cases
     * by using the `getPixel` helper, which applies edge padding when the kernel extends beyond the image bounds.
     *
     * @param image Pointer to the image data.
     * @param width The width of the image in pixels.
     * @param height The height of the image in pixels.
     * @param x The x-coordinate of the target pixel for convolution.
     * @param y The y-coordinate of the target pixel for convolution.
     * @param kernel The convolution kernel, a 2D vector of integers. The kernel dimensions should be odd to have a central element.
     * @return The convolution result as a float, which is the sum of the element-wise product of the kernel and the underlying image region.
     */
    float applyKernel(unsigned char* image, int width, int height, int x, int y, const std::vector<std::vector<int>>& kernel);
};

#endif // FILTER_H

