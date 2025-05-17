#include <gtest/gtest.h>
#include "RFM.h"

TEST(TestRFM, General) {
    employee e{.num = 1, .name = "Ilya", .hours = 5};
    std::ifstream in("test");
    std::ofstream out("test");
    RFM rfm(in, out);

    rfm.add(e);
    employee ec = rfm.read(1);
    EXPECT_EQ(e, ec);
    employee g{.num = 1, .name = "Ilya", .hours = 6};
    rfm.edit(1, g);
    EXPECT_EQ(g, rfm.read(1));
}