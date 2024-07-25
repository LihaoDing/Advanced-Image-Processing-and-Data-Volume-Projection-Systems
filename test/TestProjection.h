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

#ifndef TEST_PROJECTION_H
#define TEST_PROJECTION_H
#include <vector>

class TestProjection {
public:
    std::vector<int> runTests();

private:
    bool testMIP();
    bool testMinIP();
    bool testAIP();
    bool testAIPMedian();
};

#endif
