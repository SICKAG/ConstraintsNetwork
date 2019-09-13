// Copyright 2019 SICK AG. All rights reserved.

#include "LessOrEqual.h"
#include "Network.h"
#include "commonconstraintsMockWire.h"

#include <gtest/gtest.h>

namespace common { namespace constraints {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

class LessOrEqualTest : public ::testing::Test
{
public:
  Range range(LessOrEqual& le, Wire& wire) { return le.range(wire); }
  IWire& leftWire(LessOrEqual& le) { return le.mLeft; }
  IWire& rightWire(LessOrEqual& le) { return le.mRight; }

  WireExpression createNonLinearExpression(double firstDegree, double constant)
  {
    return WireExpression(firstDegree, constant, true);
  }

protected:
  Network mNetwork;
};

TEST_F(LessOrEqualTest, wireLessOrEqualToSelf)
{
  Wire& w = mNetwork.make(0);

  // No assertion should fail
  mNetwork.lessOrEqual(w, w);
}

TEST_F(LessOrEqualTest, aLessThanOrEqualToB)
{
  Wire& a = mNetwork.make(0);
  Wire& b = mNetwork.make(1);
  mNetwork.lessOrEqual(a, b);
}

TEST_F(LessOrEqualTest, aNotLessThanOrEqualToB)
{
  Wire& a = mNetwork.make(42);
  Wire& b = mNetwork.make(1);
  ASSERT_DEATH(mNetwork.lessOrEqual(a, b), ".*");
}

TEST_F(LessOrEqualTest, trueOnValidChange)
{
  Wire& a = mNetwork.make(0);
  Wire& b = mNetwork.make(1);
  mNetwork.lessOrEqual(a, b);

  ASSERT_TRUE(b.set(0.1));
  ASSERT_TRUE(b.set(0));
}

TEST_F(LessOrEqualTest, falseOnInvalidChange)
{
  Wire& a = mNetwork.make(0);
  Wire& b = mNetwork.make(1);
  mNetwork.lessOrEqual(a, b);

  ASSERT_FALSE(b.set(-0.1));
}

TEST_F(LessOrEqualTest, leftRange)
{
  Wire& left = mNetwork.make(0);
  Wire& right = mNetwork.make(1);
  LessOrEqual& le = left <= right;
  ASSERT_EQ(range(le, left), Range(Range::NEGATIVE_INFINITY, 1));
  ASSERT_EQ(range(le, right), Range(0, Range::POSITIVE_INFINITY));
}

TEST_F(LessOrEqualTest, rangeOfVariableOnBothSidesIsInfinite)
{
  Wire& variable = mNetwork.make(0);
  Wire& constant = mNetwork.make(10);
  LessOrEqual& le = variable <= variable + constant;

  // 1*v + 0 on left hand side
  ASSERT_EQ(leftWire(le).expression(variable),
            WireExpression::createLinear(1, 0));
  // 1*v + 10 on right hand side
  ASSERT_EQ(rightWire(le).expression(variable),
            WireExpression::createLinear(1, 10));
  ASSERT_EQ(variable.range(),
            Range(Range::NEGATIVE_INFINITY, Range::POSITIVE_INFINITY));
  ASSERT_EQ(constant.range(), Range(0, Range::POSITIVE_INFINITY));
}

TEST_F(LessOrEqualTest, rangeOfVariableOnBothSidesIsLimitedDown)
{
  Wire& variable = mNetwork.make(0);
  Wire& constant = mNetwork.make(10);
  LessOrEqual& le = variable <= variable + variable + variable + constant;

  // 1*v + 0 on left hand side
  ASSERT_EQ(leftWire(le).expression(variable),
            WireExpression::createLinear(1, 0));
  // 3*v + 10 on right hand side
  ASSERT_EQ(rightWire(le).expression(variable),
            WireExpression::createLinear(3, 10));
  // v <= 3v + 10
  // ... is equivalent to 2v + 10 >= 0
  // ... is equivalent to v >= -10/2
  ASSERT_EQ(variable.range(), Range(-5, Range::POSITIVE_INFINITY));
  ASSERT_EQ(constant.range(), Range(0, Range::POSITIVE_INFINITY));
}

TEST_F(LessOrEqualTest, rangeOfVariableOnBothSidesIsLimitedUp)
{
  Wire& variable = mNetwork.make(0);
  Wire& constant = mNetwork.make(10);
  LessOrEqual& le = variable + variable + variable <= variable + constant;

  // 3*v + 0 on left hand side
  ASSERT_EQ(leftWire(le).expression(variable),
            WireExpression::createLinear(3, 0));
  // 1*v + 10 on right hand side
  ASSERT_EQ(rightWire(le).expression(variable),
            WireExpression::createLinear(1, 10));
  // 3v <= v + 10
  // ... is equivalent to -2v + 10 >= 0
  // ... is equivalent to v <= 10/2
  ASSERT_EQ(variable.range(), Range(Range::NEGATIVE_INFINITY, 5));
  ASSERT_EQ(constant.range(), Range(0, Range::POSITIVE_INFINITY));
}

TEST_F(LessOrEqualTest, rangeOfFactorIsScaled)
{
  Wire& coefficient = mNetwork.make(4);
  Wire& variable = mNetwork.make(0);
  Wire& constant = mNetwork.make(10);
  LessOrEqual& le = variable * coefficient <= constant;

  // 4*v on left hand side
  ASSERT_EQ(leftWire(le).expression(variable),
            WireExpression::createLinear(4, 0));
  // 10 on right hand side
  ASSERT_EQ(rightWire(le).expression(variable),
            WireExpression::createLinear(0, 10));
  // 4v <= 10
  // ... is equivalent to v <= 10/4
  ASSERT_EQ(variable.range(), Range(Range::NEGATIVE_INFINITY, 2.5));
  ASSERT_EQ(coefficient.range(), Range::FULL);
}

}}
