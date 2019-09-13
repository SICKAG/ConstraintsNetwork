// Copyright 2019 SICK AG. All rights reserved.

#include "Range.h"

#include <gtest/gtest.h>

namespace common { namespace constraints {

TEST(RangeTest, defaultRangeIsUnlimited)
{
  Range r;
  ASSERT_EQ(r.lower, -std::numeric_limits<double>::infinity());
  ASSERT_EQ(r.upper, std::numeric_limits<double>::infinity());
}

TEST(RangeTest, selfIntersectEqualSelf)
{
  Range r(42, 100);
  ASSERT_EQ(r, Range::intersect(r, r));
}

TEST(RangeTest, intersectCommonLower)
{
  Range rangeA(42, 100);
  Range rangeB(42, 50);
  ASSERT_EQ(Range(42, 50), Range::intersect(rangeA, rangeB));
}

TEST(RangeTest, intersectCommonUpper)
{
  Range rangeA(0, 50);
  Range rangeB(42, 50);
  ASSERT_EQ(Range(42, 50), Range::intersect(rangeA, rangeB));
}

TEST(RangeTest, isEmpty)
{
  Range r(1, 0);
  ASSERT_TRUE(r.isEmpty());
}

TEST(RangeTest, pointRangeIsNotEmpty)
{
  Range r(1, 1);
  ASSERT_FALSE(r.isEmpty());
}

TEST(RangeTest, equalNonEmpty)
{
  Range a(3, 6.1);
  Range b(3, 6.1);
  ASSERT_EQ(a, b);
}

TEST(RangeTest, nonEqualNonEmpty)
{
  Range a(3, 6.1);
  Range b(3, 16.1);
  ASSERT_NE(a, b);
}

TEST(RangeTest, nonEqualOneEmpty)
{
  Range a(3, 6.1);
  Range b(3, -6.1); // Empty
  ASSERT_NE(a, b);
}

TEST(RangeTest, equalEmpty)
{
  Range a(3, -6.1); // Empty
  Range b(3, -6.1); // Empty
  ASSERT_EQ(a, b);
}

TEST(RangeTest, nonEqualEmpty)
{
  Range a(10, -1); // Empty
  Range b(3, 0); // Empty
  ASSERT_EQ(a, b);
}

TEST(RangeTest, equalBecauseEmptyDespiteDifferentRanges)
{
  Range a(100, 0);
  Range b(-1, -42);
  ASSERT_EQ(a, b);
}
}}
