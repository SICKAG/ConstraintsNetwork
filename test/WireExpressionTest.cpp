// Copyright 2019 SICK AG. All rights reserved.

#include "WireExpression.h"

#include <gtest/gtest.h>

namespace common { namespace constraints {

class WireExpressionTest : public ::testing::Test
{
public:
  WireExpression createNonLinear(double firstDegree, double constant)
  {
    return WireExpression(firstDegree, constant, true);
  }
};

TEST_F(WireExpressionTest, zeroDifference)
{
  WireExpression a = WireExpression::createLinear(3.14, 1.23);

  ASSERT_EQ(a - a, WireExpression::createLinear(0, 0));
}

TEST_F(WireExpressionTest, nonZeroDifference)
{
  // x + 3
  WireExpression a = WireExpression::createLinear(1, 3);
  // 2x + 10
  WireExpression b = WireExpression::createLinear(2, 10);

  // Shall be -x - 7
  WireExpression difference = a - b;
  ASSERT_EQ(difference, WireExpression::createLinear(-1, -7));
}

TEST_F(WireExpressionTest, sum)
{
  // 2x + 27
  WireExpression a = WireExpression::createLinear(2, 27);
  // x + 10
  WireExpression b = WireExpression::createLinear(1, 10);

  // Shall be 3x + 37
  WireExpression sum = a + b;
  ASSERT_EQ(sum, WireExpression::createLinear(3, 37));
}

TEST_F(WireExpressionTest, addInfinity)
{
  double inf = std::numeric_limits<double>::infinity();
  WireExpression a = WireExpression::createLinear(47, inf);
  WireExpression b = WireExpression::createLinear(-inf, 11);

  WireExpression sum = a + b;
  ASSERT_EQ(sum, WireExpression::createLinear(-inf, inf));
}

TEST_F(WireExpressionTest, setNonlinear)
{
  ASSERT_FALSE(WireExpression::createLinear(1, 2).nonlinear);
  ASSERT_TRUE(createNonLinear(1, 2).nonlinear);
}

TEST_F(WireExpressionTest, propagateNonlinear)
{
  WireExpression a = createNonLinear(1, 2);
  WireExpression b = WireExpression::createLinear(3, 4);

  WireExpression sum = a + b;
  ASSERT_TRUE(sum.nonlinear);

  WireExpression difference = a - b;
  ASSERT_TRUE(difference.nonlinear);

  WireExpression zeroDifference = a - a;
  ASSERT_TRUE(zeroDifference.nonlinear);
}

TEST_F(WireExpressionTest, multiplyToLinear)
{
  // 1x + 2
  WireExpression a = WireExpression::createLinear(1, 2);
  // -3
  WireExpression b = WireExpression::createLinear(0, -3);

  // Shall be -3x -6
  WireExpression product = a * b;
  ASSERT_EQ(product, WireExpression::createLinear(-3, -6));
}

TEST_F(WireExpressionTest, multiplyToNonlinear)
{
  // 1x + 2
  WireExpression a = WireExpression::createLinear(1, 2);
  // 4x - 3
  WireExpression b = WireExpression::createLinear(4, -3);

  // Shall be 5x -6 plus a nonlinear term
  WireExpression product = a * b;
  ASSERT_EQ(product, createNonLinear(5, -6));
}

TEST_F(WireExpressionTest, commutativeMultiplication)
{
  WireExpression a = WireExpression::createLinear(0, 2);
  WireExpression b = WireExpression::createLinear(3, 0);
  WireExpression c = WireExpression::createLinear(5, 4);
  WireExpression d = createNonLinear(7, 6);

  ASSERT_EQ(a * b, b * a);
  ASSERT_EQ(a * c, c * a);
  ASSERT_EQ(b * c, c * b);
  ASSERT_EQ(a * d, d * a);
  ASSERT_EQ(b * d, d * b);
  ASSERT_EQ(c * d, d * c);
}

TEST_F(WireExpressionTest, negativeConstantToString)
{
  WireExpression e = WireExpression::createLinear(5, -3);

  std::ostringstream s;
  s << e;
  ASSERT_EQ(s.str(), "5*x - 3");
}

TEST_F(WireExpressionTest, zeroConstantToString)
{
  WireExpression e = WireExpression::createLinear(5, 0);

  std::ostringstream s;
  s << e;
  ASSERT_EQ(s.str(), "5*x + 0");
}

TEST_F(WireExpressionTest, positiveConstantToString)
{
  WireExpression e = WireExpression::createLinear(5, 3);

  std::ostringstream s;
  s << e;
  ASSERT_EQ(s.str(), "5*x + 3");
}

TEST_F(WireExpressionTest, fractionalToString)
{
  WireExpression e = WireExpression::createLinear(5.2, -3.999);

  std::ostringstream s;
  s << e;
  ASSERT_EQ(s.str(), "5.2*x - 3.999");
}

TEST_F(WireExpressionTest, nonlinearToString)
{
  WireExpression e = createNonLinear(-2, -3);

  std::ostringstream s;
  s << e;
  ASSERT_EQ(s.str(), "-2*x - 3 + a_nonlinear_term");
}

TEST_F(WireExpressionTest, infToString)
{
  WireExpression e =
    WireExpression::createLinear(std::numeric_limits<double>::infinity(),
                                 -std::numeric_limits<double>::infinity());

  std::ostringstream s;
  s << e;
  ASSERT_EQ(s.str(), "inf*x - inf");
}

}}
