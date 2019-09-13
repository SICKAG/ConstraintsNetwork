// Copyright 2019 SICK AG. All rights reserved.

#include "LessOrEqual.h"
#include "Network.h"

#include <gtest/gtest.h>

// This test is for integration of the parts in the constraints namespace.

namespace common { namespace constraints {

class ConstraintsTest : public ::testing::Test
{
protected:
  Network mNetwork;
};

TEST_F(ConstraintsTest, sumShouldBeLessThanLimit)
{
  Wire& a = mNetwork.make(15);
  Wire& b = mNetwork.make(35);

  Wire& sum = mNetwork.add(a, b);
  Wire& upperLimit = mNetwork.make(100);

  mNetwork.lessOrEqual(sum, upperLimit);

  // Change within the limit
  ASSERT_TRUE(a.set(50));

  // Change so that sum is above limit
  ASSERT_FALSE(b.set(51));
}

TEST_F(ConstraintsTest, sumShouldBeLessThanLimitWithOperators)
{
  Wire& a = mNetwork.make(15);
  Wire& b = mNetwork.make(35);
  Wire& upperLimit = mNetwork.make(100);
  a + b <= upperLimit;
  ASSERT_TRUE(a = 50);
  ASSERT_FALSE(b = 51);
}

TEST_F(ConstraintsTest, failureMessageWhenInvalid)
{
  Wire& a = mNetwork.make(50);
  Wire& b = mNetwork.make(35);
  Wire& c = mNetwork.make(1);
  Wire& upperLimit = mNetwork.make(100);
  a + (b * c) <= upperLimit;
  Result r = (b = 51);
  ASSERT_FALSE(r);
  ASSERT_EQ(r.getErrorMessage(),
            "(Wire2) * (Wire3) would fail because "
            "Wire1 + (Wire2) * (Wire3) would fail because "
            "Wire1 + (Wire2) * (Wire3) <= Wire4 would fail.");
}

TEST_F(ConstraintsTest, nameOfLessOrEqual)
{
  Wire& a = mNetwork.make("Foo", 50);
  Wire& b = mNetwork.make("Bar", 35);
  Wire& c = mNetwork.make("Baz", 5);
  Wire& upperLimit = mNetwork.make("Limit", 100);
  LessOrEqual& le = (a + b + c <= upperLimit);
  ASSERT_EQ(le.getName(), "Foo + Bar + Baz <= Limit");
}

TEST_F(ConstraintsTest, dumpMessage)
{
  Wire& a = mNetwork.make(50);
  Wire& b = mNetwork.make(35);
  Wire& c = mNetwork.make(1);
  Wire& upperLimit = mNetwork.make(100);
  a + b* c <= upperLimit;
  ASSERT_EQ(
    a.dump(),
    "Wire1 with value 50 and range [-inf, 65]\n"
    "  (Wire1)=50 + ((Wire2) * (Wire3))=35\n"
    "    Wire1 + (Wire2) * (Wire3) with value 85 and range [-inf, 100]\n"
    "      (Wire1 + (Wire2) * (Wire3))=85 <= (Wire4)=100\n");
}

TEST_F(ConstraintsTest, unambiguousName)
{
  Wire& a = mNetwork.make(50);
  Wire& b = mNetwork.make(35);
  Wire& c = mNetwork.make(1);
  Wire& product = (a + b) * c;
  ASSERT_EQ(product.getName(), "(Wire1 + Wire2) * (Wire3)");
}

TEST_F(ConstraintsTest, dumpMessageMultiplication)
{
  Wire& a = mNetwork.make("A", 4);
  Wire& b = mNetwork.make("B", 5);
  a* b;

  ASSERT_EQ(a.dump(),
            "A with value 4 and range [-inf, inf]\n"
            "  (A)=4 * (B)=5\n"
            "    (A) * (B) with value 20 and range [-inf, inf]\n");
}

TEST_F(ConstraintsTest, expressionOfWire)
{
  Wire& a = mNetwork.make(50);
  ASSERT_EQ(a.expression(a), WireExpression::createLinear(1, 0));
}

TEST_F(ConstraintsTest, expressionOfWirePlusConstant)
{
  Wire& a = mNetwork.make(50);
  Wire& sum = a + mNetwork.make(20);
  ASSERT_EQ(sum.expression(a), WireExpression::createLinear(1, 20));
}

TEST_F(ConstraintsTest, expressionOfThreeTimesWirePlusConstant)
{
  Wire& a = mNetwork.make(50);
  Wire& polynomial = a * mNetwork.make(3) + mNetwork.make(20);
  ASSERT_EQ(polynomial.expression(a), WireExpression::createLinear(3, 20));
}

}}
