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

#include "TestProjection.h"
#include "../src/Projection.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION_2
#include "../src/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION_2
#include "../src/stb_image_write.h"

std::vector<int> TestProjection::runTests() {
    std::vector<bool (TestProjection::*)()> tests = {
        &TestProjection::testMIP,
        &TestProjection::testMinIP,
        &TestProjection::testAIP,
        &TestProjection::testAIPMedian,
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

bool TestProjection::testMIP() {
    try {
        int width = 2, height = 2, channels = 1;
        std::vector<unsigned char*> images;
        unsigned char img1[] = {0, 50, 50, 0};
        unsigned char img2[] = {50, 0, 0, 50};
        images.push_back(img1);
        images.push_back(img2);

        Projection projection;
        std::string outputPath = "mip_test.png";

        bool success = projection.MIP(images, width, height, channels, outputPath);
        assert(success && "Testcase Failed: Maximum Projection function failed.");

        int x, y, n;
        unsigned char* data = stbi_load(outputPath.c_str(), &x, &y, &n, 0);
        assert(data != nullptr && "Testcase Failed: Maximum Projection function failed to load the output image for verification.");

        unsigned char expected[] = {50, 50, 50, 50};

        for (int i = 0; i < 4; ++i) {
            assert(data[i] == expected[i] && "Testcase Failed: Maximum Projection function output does not match expected result.");
        }
        std::filesystem::remove(outputPath);
        stbi_image_free(data);
        std::cout << "Testcase Passed: Maximum Projection function passed the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Testcase Failed: (Maximum Projection function)Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestProjection::testMinIP() {
    try {
        int width = 2, height = 2, channels = 1;
        std::vector<unsigned char*> images;
        unsigned char img1[] = {100, 30, 60, 80};
        unsigned char img2[] = {30, 100, 80, 60};
        images.push_back(img1);
        images.push_back(img2);

        Projection projection;
        std::string outputPath = "minip_test.png";

        bool success = projection.MinIP(images, width, height, channels, outputPath);
        assert(success && "Testcase Failed: Minimum Projection function failed.");

        int x, y, n;
        unsigned char* data = stbi_load(outputPath.c_str(), &x, &y, &n, 0);
        assert(data != nullptr && "Testcase Failed: Minimum Projection function failed to load the output image for verification.");

        unsigned char expected[] = {30, 30, 60, 60};

        for (int i = 0; i < 4; ++i) {
            assert(data[i] == expected[i] && "Testcase Failed: Minimum Projection function output does not match expected result.");
        }
        std::filesystem::remove(outputPath);
        stbi_image_free(data);
        std::cout << "Testcase Passed: Minimum Projection function passed the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Testcase Failed: (Minimum Projection function)Exception occurred: " << e.what() << std::endl;
        return false;
    }
}


bool TestProjection::testAIP() {
    try {
        int width = 2, height = 2, channels = 1;
        std::vector<unsigned char*> images;
        unsigned char img1[] = {10, 20, 30, 40};
        unsigned char img2[] = {20, 30, 40, 50};
        unsigned char img3[] = {30, 40, 50, 60};
        images.push_back(img1);
        images.push_back(img2);
        images.push_back(img3);

        Projection projection;
        std::string outputPath = "aip_test.png";

        bool success = projection.AIP(images, width, height, channels, outputPath);
        assert(success && "Testcase Failed: Average Projection function failed.");

        int x, y, n;
        unsigned char* data = stbi_load(outputPath.c_str(), &x, &y, &n, 0);
        assert(data != nullptr && "Testcase Failed: AIP function failed to load the output image for verification.");

        unsigned char expected[] = {20, 30, 40, 50};

        for (int i = 0; i < 4; ++i) {
            assert(data[i] == expected[i] && "Testcase Failed: Average Projection output does not match expected result.");
        }
        std::filesystem::remove(outputPath);
        stbi_image_free(data);
        std::cout << "Testcase Passed: Average Projection function passed the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Testcase Failed: (Average Projection function)Exception occurred: " << e.what() << std::endl;
        return false;
    }
}


bool TestProjection::testAIPMedian() {
    try {
        int width = 2, height = 2, channels = 1;
        std::vector<unsigned char*> images;
        unsigned char img1[] = {10, 60, 30, 40};
        unsigned char img2[] = {20, 50, 40, 30};
        unsigned char img3[] = {30, 40, 50, 20};
        images.push_back(img1);
        images.push_back(img2);
        images.push_back(img3);

        Projection projection;
        std::string outputPath = "aipmedian_test.png";

        bool success = projection.AIPMedian(images, width, height, channels, outputPath);
        assert(success && "Testcase Failed: AIPMedian method failed.");

        int x, y, n;
        unsigned char* data = stbi_load(outputPath.c_str(), &x, &y, &n, 0);
        assert(data != nullptr && "Testcase Failed: AIPMedian method failed to load the output image for verification.");

        unsigned char expected[] = {20, 50, 40, 30};

        for (int i = 0; i < 4; ++i) {
            assert(data[i] == expected[i] && "Testcase Failed: AIPMedian method output does not match expected result.");
        }
        std::filesystem::remove(outputPath);
        stbi_image_free(data);
        std::cout << "Testcase Passed: Average Projection with median function passed the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}


