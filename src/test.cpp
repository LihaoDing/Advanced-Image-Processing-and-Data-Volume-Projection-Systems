#include "Slice.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// 一个简单的帮助函数来检查文件是否存在
bool fileExists(const std::string& filename) {
    std::ifstream ifile(filename.c_str());
    return ifile.good();
}

int main() {
    // 模拟一些图像数据
    int width = 4, height = 4, depth = 3; // 举例用的尺寸
    std::vector<stbi_uc*> images(depth);
    for (int z = 0; z < depth; ++z) {
        images[z] = new stbi_uc[width * height];
        for (int i = 0; i < width * height; ++i) {
            images[z][i] = static_cast<stbi_uc>(z + 1); // 填充每个像素，简单模拟
        }
    }

    Slice slicer;
    std::string outputFilename = "output_test_slice.png";

    // 调用 extractAndSaveSlice 函数进行测试
    bool success = slicer.extractAndSaveSlice(images, width, height, 2, SlicePlane::YZ, outputFilename);

    // 清理图像数据
    for (auto& img : images) {
        delete[] img;
    }

    // 检查是否成功并且文件是否存在
    if (success && fileExists(outputFilename)) {
        std::cout << "Test passed: Output file created successfully." << std::endl;
    } else {
        std::cerr << "Test failed: Output file not created." << std::endl;
    }

    // 可选：如果你想在测试后删除输出文件
    // std::filesystem::remove(outputFilename);

    return 0;
}
