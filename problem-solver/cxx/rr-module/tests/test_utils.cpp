#include <sc-memory/test/sc_test.hpp>
#include "utils/utils.hpp"


TEST(TestUtils1, TestUtilsSplit){
    std::string splitData1 = "ArtemDjDj,dBurbas,zh_kulik,suYEATolog";
    const auto & splitRes1 = Utils::SplitData(splitData1, ",");
    std::string splitData2 = "";
    const auto & splitRes2 = Utils::SplitData(splitData2, ",");

    EXPECT_EQ(splitRes1.size(), 4);
    EXPECT_EQ(splitRes1[0], "ArtemDjDj");
    EXPECT_EQ(splitRes1[1], "dBurbas");
    EXPECT_EQ(splitRes1[2], "zh_kulik");
    EXPECT_EQ(splitRes1[3], "suYEATolog");

    EXPECT_EQ(splitRes2.size(), 0);
}

TEST(TestUtils2, TestUtilsPageIndexCorrect){
    std::string pageIdtf = "page_9";
    int pageIndex = Utils::GetPageIndex(pageIdtf);
    EXPECT_EQ(pageIndex, 9);
}

TEST(TestUtils3, TestUtilsPageIndexIncorrect){
    std::string pageIdtf = "page";
    EXPECT_THROW(Utils::GetPageIndex(pageIdtf), std::invalid_argument);
}