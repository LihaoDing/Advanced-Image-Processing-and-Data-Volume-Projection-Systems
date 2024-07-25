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

#include <string>
#include <vector>
#include "stb_image.h"
#include "stb_image_write.h"

 /**
  * @enum SlicePlane
  *
  * @brief Represents the planes along which a 3D volume can be sliced.
  */
enum class SlicePlane {
    YZ, ///< Slice along the YZ plane, keeping the X-coordinate constant.
    XZ  ///< Slice along the XZ plane, keeping the Y-coordinate constant.
};

/**
 * @class Slice
 *
 * @brief Provides functionality to extract and save 2D slices from a 3D image volume.
 *
 * This class is designed to handle the extraction of specific 2D slices from a 3D volume of images
 * based on a given plane. It supports slicing along the YZ and XZ planes and saving the extracted slice as an image file.
 */
class Slice {
public:
    /**
     * @brief Extracts a 2D slice from a 3D image volume and saves it as an image file.
     *
     * This method takes a stack of 2D images representing a 3D volume and extracts a 2D slice from it. The slice can be taken
     * along the YZ or XZ plane, depending on the specified 'SlicePlane'. The extracted slice is then saved to a file with the
     * specified output filename.
     *
     * @param images A vector of pointers to the image data, representing the 3D volume.
     * @param width The width of each image in the volume.
     * @param height The height of each image in the volume.
     * @param sliceIndex The index of the slice to extract. This corresponds to the constant coordinate in the selected slice plane.
     * @param plane The plane along which to extract the slice (YZ or XZ).
     * @param outputFilename The path and filename where the extracted slice will be saved.
     * @return bool True if the slice is extracted and saved successfully, false otherwise.
     */
    bool extractAndSaveSlice(std::vector<stbi_uc*>& images, int width, int height, int sliceIndex, SlicePlane plane, const std::string& outputFilename);
};