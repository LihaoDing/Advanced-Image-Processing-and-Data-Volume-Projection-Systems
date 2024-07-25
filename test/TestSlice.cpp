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

#include "TestSlice.h"
#include "../src/Slice.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../src/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../src/stb_image_write.h"

std::vector<int> TestSlice::runTests() {
    std::vector<bool (TestSlice::*)()> tests = {
        &TestSlice::testExtractAndSaveSlice,
    };

    int successNum = 0;
    int failNum = 0;
    for (auto test : tests) {
        if ((this->*test)()) {
            successNum++;
        } else {
            failNum++;
        }
    }
    return {successNum, failNum};
}

bool TestSlice::testExtractAndSaveSlice() {
    try {
        int width = 4, height = 4, depth = 3;
        std::vector<stbi_uc*> images(depth);
        for (int z = 0; z < depth; ++z) {
            images[z] = new stbi_uc[width * height];
            for (int i = 0; i < width * height; ++i) {
                images[z][i] = static_cast<stbi_uc>((z + 1) * 25);
            }
        }

        Slice slicer;
        std::string outputFilename = "output_test_slice.png";

        bool success = slicer.extractAndSaveSlice(images, width, height, 2, SlicePlane::YZ, outputFilename);

        for (auto& img : images) {
            delete[] img;
        }

        assert(success && "Testcase Failed: (slicing function)Failed to produce an output file.");

        if (fileExists(outputFilename)) {
            int x, y, n;
            unsigned char *data = stbi_load(outputFilename.c_str(), &x, &y, &n, 0);
            assert(data != nullptr && "Testcase Failed: (slicing function)Failed to load the output image for verification.");

            bool valid = true;
            for (int z = 0; z < depth; ++z) {
                for (int y = 0; y < height; ++y) {
                    int pixelValue = data[z * height + y];
                    if (pixelValue != (z + 1) * 25) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) break;
            }

            stbi_image_free(data);
            assert(valid && "Testcase Failed: (slicing function)The output file content is not as expected.");

            std::filesystem::remove(outputFilename);

            std::cout << "Testcase Passed: Slicing function passed the test." << std::endl;
        } else {
            assert(false && "Testcase Failed: (slicing function)Output file does not exist.");
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Testcase Failed: (slicing function)Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestSlice::fileExists(const std::string& filename) {
    std::ifstream ifile(filename.c_str());
    return ifile.good();
}
