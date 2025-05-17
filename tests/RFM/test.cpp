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

TEST(TestRFM, GeneralMultiple) {
    employee e{.num = 1, .name = "Ilya", .hours = 5};
    employee g{.num = 2, .name = "Ilya", .hours = 6};
    std::ifstream in("test");
    std::ofstream out("test");
    RFM rfm(in, out);

    rfm.add(e);
    rfm.add(g);
    employee ec = rfm.read(1);
    employee gc = rfm.read(2);
    EXPECT_EQ(e, ec);
    EXPECT_EQ(g, gc);
    employee gq{.num = 1, .name = "Ilya2", .hours = 6};
    rfm.edit(1, gq);
    EXPECT_EQ(gq, rfm.read(1));
}