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

#ifndef TEST_FILTER_H
#define TEST_FILTER_H
#include <vector>

class TestFilter {
public:
    std::vector<int> runTests();

private:
    bool testApplyGrayscaleFilter();
    bool testApplyBrightnessFilter();
    bool testApplyHistogramEqualization();
    bool testApplyThresholdFilter();
    bool testApplySpFilter();
    bool testApply2DGaussianFilter();
    bool testApply2DMedianBlurFilter();
    bool testApplyBoxBlur();
    bool testSobelFilter();
    bool testPrewittFilter();
    bool testScharrFilter();
    bool testRobertsCrossFilter();
    bool testApply3DMedianFilter();
    bool testApply3DGaussianFilter();
};

#endif
