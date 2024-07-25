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
#include "TestProjection.h"
#include "TestFilter.h"
#include <chrono>
#include <iostream>
#include <string>

void printLineWithText(const std::string& text, int totalLength = 80) {
    int textLength = text.length();
    if (textLength >= totalLength) {
        std::cout << text << std::endl;
        return;
    }
    int totalEquals = totalLength - textLength;
    int equalsPerSide = totalEquals / 2;
    std::string leftEquals(equalsPerSide, '=');
    std::string rightEquals = totalEquals % 2 == 0 ? leftEquals : leftEquals + "=";
    std::cout << leftEquals << text << rightEquals << std::endl;
}

template<typename TestClass>
void runTestSuite(const std::string& testName) {
    std::cout << std::endl;
    printLineWithText("Test for class " + testName + " start");
    auto startTime = std::chrono::high_resolution_clock::now();
    TestClass testInstance;
    auto resultArray = testInstance.runTests();
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    std::string result = std::to_string(resultArray[0] + resultArray[1]) + " tests, " + std::to_string(resultArray[0]) + " passed, " + std::to_string(resultArray[1]) + " failed in " + std::to_string(duration.count()) + "s";
    printLineWithText(result);
}

int main() {
    runTestSuite<TestSlice>("Slice");
    runTestSuite<TestProjection>("Projection");
    runTestSuite<TestFilter>("Filter");
    return 0;
}
