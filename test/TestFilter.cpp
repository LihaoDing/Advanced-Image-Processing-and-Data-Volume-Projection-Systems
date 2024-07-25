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

#include "TestFilter.h"
#include "../src/Filter.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <cassert>
#include <cstring>

std::vector<int> TestFilter::runTests() {
    std::vector<bool (TestFilter::*)()> tests = {
        &TestFilter::testApplyGrayscaleFilter,
        &TestFilter::testApplyBrightnessFilter,
        &TestFilter::testApplyHistogramEqualization,
        &TestFilter::testApplyThresholdFilter,
        &TestFilter::testApplySpFilter,
        &TestFilter::testApply2DGaussianFilter,
        &TestFilter::testApply2DMedianBlurFilter,
        &TestFilter::testApplyBoxBlur,
        &TestFilter::testSobelFilter,
        &TestFilter::testPrewittFilter,
        &TestFilter::testScharrFilter,
        &TestFilter::testRobertsCrossFilter,
        &TestFilter::testApply3DMedianFilter,
        &TestFilter::testApply3DGaussianFilter
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

bool TestFilter::testApplyGrayscaleFilter() {
    try {
        unsigned char imageData[] = {255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255};
        int w = 2, h = 2, c = 3;
        Filter filter;
        
        unsigned char* grayData = filter.applyGrayscaleFilter(imageData, w, h, c);
        
        assert(grayData[0] == 54 && grayData[1] == 182 && grayData[2] == 18 && grayData[3] == 255);

        delete[] grayData;
        std::cout << "Testcase Passed: Grayscale Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testApplyBrightnessFilter() {
    try {
        unsigned char imageData[] = {50, 100, 150, 200};
        int w = 2, h = 2, c = 1, brightness = 50;
        Filter filter;
        
        unsigned char* brightData = filter.applyBrightnessFilter(imageData, w, h, c, brightness);
        
        assert(brightData[0] == 100 && brightData[1] == 150 && brightData[2] == 200 && brightData[3] == 250 && "Testcase Failed: Brightness Filter function did not produce expected output.");

        delete[] brightData;
        std::cout << "Testcase Passed: Brightness Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testApplyHistogramEqualization() {
    try {
        unsigned char imageData[] = {50, 100, 150, 200};
        unsigned char imageDataCopy[] = {50, 100, 150, 200};
        int w = 2, h = 2, channels = 1;

        Filter filter;
        unsigned char* eqData = filter.applyHistogramEqualization(imageData, w, h, channels, true); // 非HSL模式

        bool allDifferent = true;
        for (int i = 0; i < w*h*channels; ++i) {
            if (eqData[i] == imageDataCopy[i]) {
                allDifferent = false;
                break;
            }
        }
        
        assert(allDifferent && "Testcase Failed: Histogram Equalization function did not produce different output.");
        std::cout << "Testcase Passed: Histogram Equalization function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}


bool TestFilter::testApplyThresholdFilter() {
    try {
        unsigned char imageData[] = {50, 120, 180, 240};
        int w = 4, h = 1, channels = 1, threshold = 100;

        Filter filter;
        unsigned char* threshData = filter.applyThresholdFilter(imageData, w, h, channels, threshold, false);

        assert(threshData[0] == 0 && threshData[1] == 255 && threshData[2] == 255 && threshData[3] == 255 && "Testcase Failed: Threshold Filter function did not produce expected output.");

        delete[] threshData;
        std::cout << "Testcase Passed: Threshold Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testApplySpFilter() {
    try {
        unsigned char imageData[] = {
            50, 50, 50, 50,
            50, 50, 50, 50,
            50, 50, 50, 50,
            50, 50, 50, 50
        };
        int w = 4, h = 4, c = 1, percentage_sp = 50;
        Filter filter;
        
        unsigned char* spData = filter.applySpFilter(imageData, w, h, c, percentage_sp);

        int noisePixels = 0;
        int totalPixels = w * h;
        for (int i = 0; i < totalPixels; ++i) {
            if (spData[i] == 0 || spData[i] == 255) {
                noisePixels++;
            }
        }

        float noisePercentage = static_cast<float>(noisePixels) / static_cast<float>(totalPixels) * 100.0f;
        assert(noisePercentage >= percentage_sp - 10 && noisePercentage <= percentage_sp + 10);

        delete[] spData;
        std::cout << "Testcase Passed: Salt and Pepper Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
    
}

bool TestFilter::testApply2DGaussianFilter() {
    try {
        unsigned char imageData[] = {100, 150, 100, 150, 200, 150, 100, 150, 100};
        int w = 3, h = 3, c = 1;
        Filter filter;
        int kernelSize = 3;
        float sigma = 1.0f;

        unsigned char* filteredData = filter.apply2DGaussianFilter(imageData, w, h, c, kernelSize, sigma);

        assert(filteredData[4] > 140 && filteredData[4] < 200);
        delete[] filteredData;
        std::cout << "Testcase Passed: 2D Gaussian Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testApply2DMedianBlurFilter() {
    try {
        unsigned char imageData[] = {255, 150, 100, 150, 200, 150, 100, 150, 100};
        int w = 3, h = 3, c = 1;
        Filter filter;
        int kernelSize = 3;

        unsigned char* filteredData = new unsigned char[w * h * c];
        filter.apply2DMedianBlurFilter(imageData, filteredData, w, h, c, kernelSize);
        
        assert(filteredData[4] == 150);
        delete[] filteredData;
        std::cout << "Testcase Passed: 2D Median Blur Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testApplyBoxBlur() {
    try {
        unsigned char imageData[] = {60, 90, 120, 90, 120, 150, 120, 150, 180};
        int w = 3, h = 3, c = 1;
        int kernelSize = 3;
        Filter filter;

        unsigned char* blurredData = filter.applyBoxBlur(imageData, w, h, c, kernelSize);

        int expected = (60 + 90 + 120 + 90 + 120 + 150 + 120 + 150 + 180) / 9;
        assert(blurredData[4] == expected && "Testcase Failed: Box Blur function did not produce expected output.");

        delete[] blurredData;
        std::cout << "Testcase Passed: Box Blur function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testSobelFilter() {
    try {
        unsigned char imageData[] = {
            255, 255, 255, 255, 255,
            255, 255, 255, 255, 255,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0
        };
        int w = 5, h = 5;
        Filter filter;

        unsigned char* edgeData = filter.sobelFilter(imageData, w, h);

        assert(edgeData[2 * w + 2] > 0);

        delete[] edgeData;
        std::cout << "Testcase Passed: Sobel Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testPrewittFilter() {
    try {
        unsigned char imageData[] = {
            255, 255, 255, 255, 255,
            255, 255, 255, 255, 255,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0
        };
        int w = 5, h = 5;
        Filter filter;

        unsigned char* edgeData = filter.prewittFilter(imageData, w, h);

        assert(edgeData[2 * w + 2] > 0);

        delete[] edgeData;
        std::cout << "Testcase Passed: Prewitt Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testScharrFilter() {
    try {
        unsigned char imageData[] = {
            255, 255, 255, 255, 255,
            255, 255, 255, 255, 255,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0
        };
        int w = 5, h = 5;
        Filter filter;

        unsigned char* edgeData = filter.scharrFilter(imageData, w, h);

        assert(edgeData[2 * w + 2] > 0);

        delete[] edgeData;
        std::cout << "Testcase Passed: Scharr Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testRobertsCrossFilter() {
    try {
        unsigned char imageData[] = {
            255, 255, 255, 255, 255,
            255, 255, 255, 255, 255,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,
            0,   0,   0,   0,   0
        };
        int w = 5, h = 5;
        Filter filter;

        unsigned char* edgeData = filter.robertsCrossFilter(imageData, w, h);

        assert(edgeData[(2 * w) + 1] > 0 || edgeData[(2 * w) + 2] > 0 || edgeData[(1 * w) + 2] > 0);

        delete[] edgeData;
        std::cout << "Testcase Passed: Roberts Cross Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testApply3DMedianFilter() {
    try {
        std::vector<unsigned char*> imageData;
        unsigned char* layer1 = new unsigned char[4]{1, 2, 3, 4};
        unsigned char* layer2 = new unsigned char[4]{5, 6, 7, 8};
        imageData.push_back(layer1);
        imageData.push_back(layer2);
        int width = 2, height = 2, depth = 2;

        Filter filter;
        std::streambuf* orig_buf = std::cout.rdbuf();
        std::ofstream ofs("/dev/null");
        std::cout.rdbuf(ofs.rdbuf());
        filter.apply3DMedianFilter(imageData, width, height, depth, 2);
        std::cout.rdbuf(orig_buf);

        assert(imageData[0][0] >= 1 && imageData[0][0] <= 8);
        std::cout << "Testcase Passed: 3D Median Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: Exception occurred: " << e.what() << std::endl;
        return false;
    }
}

bool TestFilter::testApply3DGaussianFilter() {
    try {
        std::vector<unsigned char*> imageData(3);
        unsigned char* layer1 = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
        unsigned char* layer2 = new unsigned char[9]{9, 8, 7, 6, 5, 4, 3, 2, 1};
        unsigned char* layer3 = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
        imageData[0] = layer1;
        imageData[1] = layer2;
        imageData[2] = layer3;
        int width = 3, height = 3, depth = 3;

        Filter filter;
        int kernelSize = 3;
        double sigma = 1.0;
        std::streambuf* orig_buf = std::cout.rdbuf();
        std::ofstream ofs("/dev/null");
        std::cout.rdbuf(ofs.rdbuf());
        filter.apply3DGaussianFilter(imageData, width, height, depth, kernelSize, sigma);
        std::cout.rdbuf(orig_buf);

        unsigned char centerValue = imageData[1][4];
        bool isSmoothed = centerValue > 1 && centerValue < 9;
        assert(isSmoothed && "Testcase Failed: 3D Gaussian Filter function did not produce expected output.");

        std::cout << "Testcase Passed: 3D Gaussian Filter function pass the test." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "apply3DGaussianFilter test failed: " << e.what() << std::endl;
        return false;
    }
}

