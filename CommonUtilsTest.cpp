/**
 * @author huahang@me.com
 * @date 2015-10-15
 */

#include "CommonUtils.h"

#include "vector"
#include "string"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

TEST(CommonUtilsTest, TestSplit) {
  using co::zerus::thumbgen::CommonUtils;
  std::string input = ",,";
  std::vector<std::string> splits;
  CommonUtils::split(splits, input, ',');
  EXPECT_EQ(3, splits.size());
  EXPECT_EQ("", splits[0]);
  EXPECT_EQ("", splits[1]);
  EXPECT_EQ("", splits[2]);
  input = "";
  CommonUtils::split(splits, input, ',');
  EXPECT_EQ(1, splits.size());
  EXPECT_EQ("", splits[0]);
  input = ",";
  CommonUtils::split(splits, input, ',');
  EXPECT_EQ(2, splits.size());
  EXPECT_EQ("", splits[0]);
  EXPECT_EQ("", splits[1]);
  input = "a,,bBb,c";
  CommonUtils::split(splits, input.c_str(), ',');
  EXPECT_EQ(4, splits.size());
  EXPECT_EQ("a", splits[0]);
  EXPECT_EQ("", splits[1]);
  EXPECT_EQ("bBb", splits[2]);
  EXPECT_EQ("c", splits[3]);
  CommonUtils::split(splits, nullptr, ',');
  EXPECT_EQ(0, splits.size());
}
