#include "Slice.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

bool fileExists(const std::string& filename) {
    std::ifstream ifile(filename.c_str());
    return ifile.good();
}

int main() {
    int width = 4, height = 4, depth = 3;
    std::vector<stbi_uc*> images(depth);
    for (int z = 0; z < depth; ++z) {
        images[z] = new stbi_uc[width * height];
        for (int i = 0; i < width * height; ++i) {
            images[z][i] = static_cast<stbi_uc>((z + 1) * 25); // 假设每层图像的像素值不同
        }
    }

    Slice slicer;
    std::string outputFilename = "output_test_slice.png";

    // 调用函数尝试提取一个切片并保存到文件
    bool success = slicer.extractAndSaveSlice(images, width, height, 2, SlicePlane::YZ, outputFilename);

    // 清理图像数据
    for (auto& img : images) {
        delete[] img;
    }

    if (success && fileExists(outputFilename)) {
        std::cout << "Output file created successfully." << std::endl;

        int x, y, n;
        unsigned char *data = stbi_load(outputFilename.c_str(), &x, &y, &n, 0);
        if (data != nullptr) {
            // 假设我们知道期望的像素值，这里进行验证
            // 由于这是一个YZ平面的切片，我们可以检查特定的z值和y值上的像素
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

            if (valid) {
                std::cout << "Test passed: The output file content is correct." << std::endl;
            } else {
                std::cerr << "Test failed: The output file content is not correct." << std::endl;
            }
        } else {
            std::cerr << "Failed to load the output image for verification." << std::endl;
        }
    } else {
        std::cerr << "Test failed: Output file not created." << std::endl;
    }

    return 0;
}
