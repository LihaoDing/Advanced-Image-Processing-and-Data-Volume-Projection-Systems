#include <string>
#include "Filter.h"

 /**
  * @class Image
  *
  * @brief Represents an image and provides functionalities for basic image processing.
  *
  * This class encapsulates an image's properties and operations, including loading, saving,
  * color correction, filtering, and edge detection. It supports various per-pixel modifications
  * and image blur functions, offering a comprehensive toolset for basic image manipulation.
  */
class Image {
public:
    /**
     * @brief Constructs a new Image object.
     */
    Image();

    // Getters and Setters

    /**
     * Retrieves the existence flag of the image.
     *
     * @return An integer indicating the existence of the image data.
     *         A value of 0 implies no image data is present; otherwise, image data exists.
     */
    int getExist();

    /**
     * Retrieves the file path of the image.
     *
     * @return A string representing the file path of the image.
     */
    std::string getPath();

    /**
     * Retrieves the width of the image in pixels.
     *
     * @return The width of the image.
     */
    int getWidth();

    /**
     * Retrieves the height of the image in pixels.
     *
     * @return The height of the image.
     */
    int getHeight();

    /**
     * Retrieves the number of color channels in the image.
     *
     * @return The number of channels in the image.
     */
    int getChannels();

    // Image processing functions

    /**
     * Loads an image from the specified file path.
     *
     * This method attempts to load an image from the given path using the stbi_load function. If an image is
     * already loaded, it frees the existing image data before loading the new image. It updates the image object's
     * properties, including the path, dimensions, and channel count, based on the loaded image.
     *
     * @param path The file path of the image to load.
     * @return A boolean value indicating the success of the image loading process. Returns true if the image
     *         was successfully loaded; otherwise, false.
     */
    bool loadImage(const std::string &path);

    /**
     * Saves the current image to the specified file path.
     *
     * This method saves the image data to a file at the given path using the stbi_write_png function. Before saving,
     * it checks if the specified directory exists and creates it if necessary. The method relies on the current image
     * data, width, height, and channel count to write the PNG file.
     *
     * @param path The file path where the image should be saved. If the directory does not exist, it will be created.
     * @return A boolean value indicating the success of the image saving process. Returns true if the image
     *         was successfully saved; otherwise, false.
     */
    bool saveImage(const std::string &path);

    //Colour correction and simple per-pixel modifiers

    /**
     * Applies a grayscale filter to the current image.
     *
     * This method converts the loaded image into grayscale by averaging the color channels or using a more complex
     * luminosity method, depending on the implementation of the `applyGrayscaleFilter` in the `Filter` class. It replaces
     * the current image data with the grayscale version and updates the number of channels to 1. The original image data
     * is freed to prevent memory leaks.
     *
     * @return A boolean value indicating the success of the grayscale conversion. Returns true if the conversion
     *         was successful and the image was loaded; otherwise, false.
     */
    bool Grayscale();

    /**
     * Applies a brightness filter to the current image.
     *
     * This method adjusts the brightness of the loaded image by adding the specified brightness value to each pixel.
     * The brightness adjustment is performed by the `applyBrightnessFilter` method of the `Filter` class.
     * After applying the filter, the original image data is freed to prevent memory leaks, and the image data
     * pointer is updated to the new, brightness-adjusted image data.
     *
     * @param brightness The amount to adjust the brightness by. Positive values make the image brighter,
     *                   while negative values make it darker.
     * @return A boolean value indicating the success of the brightness adjustment. Returns true if the adjustment
     *         was successful and the image was loaded; otherwise, false.
     */
    bool Brightness(int brightness);

    /**
     * Applies a Salt and Pepper Noise filter to the current image.
     *
     * This method introduces salt and pepper noise to the image, simulating a random occurrence of black and white
     * pixels across the image. The intensity of this effect is controlled by the specified percentage. The noise
     * application is handled by the `applySpFilter` method of the `Filter` class. After applying the noise, the
     * original image data is released to prevent memory leaks, and the image data pointer is updated to the new
     * image data with applied noise.
     *
     * @param percentage_sp The percentage of the image pixels that will be affected by salt and pepper noise.
     *                      This value should be between 0 and 100.
     * @return A boolean value indicating whether the noise application was successful. Returns true if the noise
     *         was successfully applied and the image was loaded; otherwise, false.
     */
    bool SaltAndPepper(int percentage_sp);

    /**
     * Applies a threshold filter to the current image.
     *
     * This method converts a grey scale or RGB image to a binary format based on the specified threshold value. Pixels above the
     * threshold are set to white, and those below are set to black. The method supports processing in either HSV or
     * HSL color spaces, as indicated by the `use_hsl` parameter. After applying the filter, the original image data
     * is released to avoid memory leaks, the image data pointer is updated to the thresholded image, and the channel
     * count is set to 1, reflecting the binary nature of the result.
     *
     * @param threshold The intensity threshold used to convert the image to binary. Pixel values above this threshold
     *                  will be set to white, and those below will be set to black.
     * @param use_hsl A boolean flag indicating whether to perform thresholding in the HSL color space instead of RGB.
     * @return A boolean value indicating the success of the threshold application. Returns true if the filter was
     *         successfully applied and the image was loaded; otherwise, false.
     */
    bool Threshold(int threshold, bool use_hsl);

    /**
     * Applies a Histogram Equalization filter to the current image.
     *
     * This method enhances the contrast of the image by equalizing its histogram. This process redistributes the
     * image's pixel intensities to cover the entire available range, improving visibility of details in both
     * bright and dark areas of the image. The filter can be applied in either the HSV or HSL color space, as
     * indicated by the `use_hsl` parameter. The method modifies the image data in place, without altering the
     * image's dimensions or channel count.
     *
     * @param use_hsl A boolean flag indicating whether to perform histogram equalization in the HSL color space
     *                instead of HSV.
     * @return A boolean value indicating the success of the histogram equalization process. Returns true if the
     *         filter was successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool HistogramEqualization(bool use_hsl);

    // Three Image blur functions

    /**
     * Applies a Gaussian blur filter to the current image.
     *
     * This method smooths the image using a Gaussian blur, characterized by the specified filter size and standard
     * deviation (sigma). Gaussian blur reduces image noise and detail by applying a Gaussian function to each pixel
     * and its neighbors. The effect is a soft blur that preserves edges better than some other types of blurring.
     * After applying the blur, the original image data is released, and the image data pointer is updated to point
     * to the new blurred image data.
     *
     * @param filterSize The size of the Gaussian kernel to use. Larger sizes result in more blur.
     * @param sigma The standard deviation of the Gaussian function. Higher values spread the blur over more pixels.
     * @return A boolean value indicating the success of the Gaussian blur application. Returns true if the filter
     *         was successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool GaussianFilter(int filterSize, double sigma);

    /**
     * Applies a Median blur filter to the current image.
     *
     * This method utilizes a Median blur, which is particularly effective at reducing "salt and pepper" noise
     * while preserving edges. The filter operates by sorting the values in each pixel's neighborhood and
     * replacing the pixel's value with the median of those values. The size of the neighborhood is determined
     * by the specified filter size. This process is applied to each pixel in the image, resulting in a
     * smoothed image. After applying the filter, the original image data is freed, and the image data pointer
     * is updated to the new, blurred image data.
     *
     * @param filterSize The size of the square kernel used for the Median blur. The kernel defines the
     *                   neighborhood size for each pixel's median calculation.
     * @return A boolean value indicating the success of the Median blur application. Returns true if the filter
     *         was successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool MedianFilter(int filterSize);

    /**
     * Applies a Box blur filter to the current image.
     *
     * This method uses a Box blur, also known as an averaging blur, which replaces each pixel's value with the average
     * value of its neighboring pixels. The size of the neighborhood is determined by the specified filter size, with
     * larger sizes resulting in a more pronounced blur effect. The Box blur is a simple and fast method to achieve
     * image smoothing. After applying the blur, the original image data is freed to avoid memory leaks, and the
     * image data pointer is updated to the new blurred image data.
     *
     * @param filterSize The size of the square kernel used for the Box blur. Larger sizes produce more blur.
     * @return A boolean value indicating the success of the Box blur application. Returns true if the filter was
     *         successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool boxFilter(int filterSize);

    // Edge detection functions

    /**
     * Applies a Sobel edge detection filter to the current image.
     *
     * The Sobel operator is used to highlight edges in the image by calculating the gradient magnitude at each pixel,
     * emphasizing regions of high spatial frequency that correspond to edges. This method is particularly effective
     * for edge detection in grayscale images. After applying the Sobel filter, the original image data is freed to
     * prevent memory leaks, and the image data pointer is updated to the edge-detected image data.
     *
     * @return A boolean value indicating the success of the Sobel edge detection application. Returns true if the filter
     *         was successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool sobelDetection();

    /**
     * Applies a Prewitt edge detection filter to the current image.
     *
     * The Prewitt operator is used for detecting vertical and horizontal edges in images by calculating the gradient
     * of the image intensity at each pixel. It's similar to the Sobel operator but uses a different kernel that
     * emphasizes edges slightly less aggressively. This method enhances the edges in the image, making them more
     * pronounced. After applying the Prewitt filter, the original image data is released to avoid memory leaks, and
     * the image data pointer is updated to the edge-enhanced image data.
     *
     * @return A boolean value indicating the success of the Prewitt edge detection application. Returns true if the
     *         filter was successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool prewittDetection();

    /**
     * Applies a Scharr edge detection filter to the current image.
     *
     * The Scharr operator is an edge detection method that offers a better rotation symmetry than Sobel and Prewitt
     * operators. It's particularly effective for capturing fine edge details and is often used in scenarios where
     * high accuracy edge detection is required. After applying the Scharr filter, the method releases the original
     * image data to manage memory efficiently and updates the image data pointer to the new image data emphasizing
     * edges.
     *
     * @return A boolean value indicating the success of the Scharr edge detection application. Returns true if the
     *         filter was successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool scharrDetection();

    /**
     * Applies a Roberts Cross edge detection filter to the current image.
     *
     * The Roberts Cross operator is one of the earliest edge detection operators and works by computing a simple,
     * quick to compute approximation of the gradient. It is particularly suited for detecting edges that are at
     * 45-degree angles. The filter highlights these edges by applying the Roberts Cross convolution kernels to the
     * image data. After the filter application, the original image data is freed to manage memory effectively, and
     * the image data pointer is updated to the new image data that emphasizes the detected edges.
     *
     * @return A boolean value indicating the success of the Roberts Cross edge detection. Returns true if the filter
     *         was successfully applied; otherwise, false, typically due to the absence of loaded image data.
     */
    bool robertsCrossDetection();

private:
    std::string path;      ///< File path of the image.
    int width;             ///< Width of the image in pixels.
    int height;            ///< Height of the image in pixels.
    int channels;          ///< Number of color channels in the image.
    unsigned char* data;   ///< Pointer to the raw image data.
    Filter filter;         ///< An instance of the Filter class for applying filters.
    int exist;             ///< Flag to check the existence of image data.
};
