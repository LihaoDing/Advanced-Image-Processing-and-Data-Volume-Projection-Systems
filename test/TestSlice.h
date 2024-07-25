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

#ifndef TEST_SLICE_H
#define TEST_SLICE_H
#include <string>
#include <vector>

class TestSlice {
public:
    std::vector<int> runTests();

private:
    bool testExtractAndSaveSlice();
    bool fileExists(const std::string& filename);
};

#endif
