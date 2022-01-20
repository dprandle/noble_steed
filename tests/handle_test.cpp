#include "core/handle.h"
#include <gtest/gtest.h>

struct Simple
{
    double x;
    double y;
};
using namespace noble_steed;

TEST(HandleTest, Create) { 
    Handle<Simple> sh;
    EXPECT_EQ(sh.id(), uID());
    EXPECT_EQ(bool(sh), false) << "Made it here...";
    Handle<Simple> sh2("", new Simple);
    Handle<Simple> sh3(sh2);
    Handle<Simple> sh4("", make_shared<Simple>());
    EXPECT_NE(sh,sh2);
    EXPECT_EQ(sh2,sh3);
    EXPECT_NE(sh3,sh4);
}