// Copyright 2019 SICK AG. All rights reserved.

#include "Wire.h"

#include "Network.h"

#include "commonconstraintsMockOperation.h"

#include <gtest/gtest.h>

namespace common { namespace constraints {

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

class WireTest : public ::testing::Test
{
public:
  void connect(Wire& wire, IOperation& operation) { wire.connect(&operation); }
  void setDriver(Wire& wire, IOperation& operation)
  {
    wire.setDriver(&operation);
  }

protected:
  Network mNetwork;
};

TEST_F(WireTest, getValue)
{
  Wire& w = mNetwork.make(3.14);
  ASSERT_EQ(w.get(), 3.14);
}

TEST_F(WireTest, setValue)
{
  Wire& w = mNetwork.make(3.14);
  w.set(5);
  ASSERT_EQ(w.get(), 5);
}

TEST_F(WireTest, getRangeValue)
{
  Wire& w = mNetwork.make(3.14);
  ASSERT_EQ(w.range(), Range::FULL);
}

TEST_F(WireTest, getRangeFromOneListener)
{
  Wire& w = mNetwork.make(42);
  StrictMock<MockOperation> operation;

  EXPECT_CALL(operation, range(_)).WillOnce(Return(Range(20, 100)));
  connect(w, operation);

  ASSERT_EQ(w.range(), Range(20, 100));
}

TEST_F(WireTest, getRangeFromTwoListenersOverlapping)
{
  Wire& w = mNetwork.make(42);
  StrictMock<MockOperation> operationA;
  StrictMock<MockOperation> operationB;

  EXPECT_CALL(operationA, range(_)).WillOnce(Return(Range(20, 100)));
  connect(w, operationA);
  EXPECT_CALL(operationB, range(_)).WillOnce(Return(Range(-30, 50)));
  connect(w, operationB);

  ASSERT_EQ(w.range(), Range(20, 50));
}

TEST_F(WireTest, getRangeFromTwoListenersNotOverlapping)
{
  Wire& w = mNetwork.make(42);
  StrictMock<MockOperation> operationA;
  StrictMock<MockOperation> operationB;

  EXPECT_CALL(operationA, range(_)).WillOnce(Return(Range(100, 200)));
  connect(w, operationA);
  EXPECT_CALL(operationB, range(_)).WillOnce(Return(Range(-30, 0)));
  connect(w, operationB);

  ASSERT_TRUE(w.range().isEmpty());
}

TEST_F(WireTest, nonNegativeRange)
{
  Wire& w = mNetwork.make(99);
  0 <= w;

  ASSERT_EQ(w.range(), Range(0, std::numeric_limits<double>::infinity()));
}

TEST_F(WireTest, constructedWithName)
{
  Wire& w = mNetwork.make("Height", 42);
  ASSERT_EQ(w.getName(), "Height");
}

TEST_F(WireTest, defaultName)
{
  Wire& w = mNetwork.make(42);
  ASSERT_EQ(w.getName(), "Wire1");
}

TEST_F(WireTest, nameFromDriver)
{
  StrictMock<MockOperation> driver;
  Wire& w = mNetwork.make(42);
  setDriver(w, driver);
  EXPECT_CALL(driver, getName()).WillOnce(Return("A + B"));

  ASSERT_EQ(w.getName(), "Wire1: A + B");
}

TEST_F(WireTest, nameGivenAndFromDriver)
{
  StrictMock<MockOperation> driver;
  Wire& w = mNetwork.make("Sum", 41);
  setDriver(w, driver);
  EXPECT_CALL(driver, getName()).WillOnce(Return("A + B"));

  ASSERT_EQ(w.getName(), "Sum: A + B");
}

TEST_F(WireTest, descriptionWithNameGivenAndFromDriver)
{
  StrictMock<MockOperation> driver;
  Wire& w = mNetwork.make("Sum", 41);
  setDriver(w, driver);
  EXPECT_CALL(driver, getName()).WillOnce(Return("A + B"));

  ASSERT_EQ(w.getShortDescription(), "(Sum: A + B)=41");
}

TEST_F(WireTest, valueAfterInvalidDoesNotRollBack)
{
  Wire& w = mNetwork.make("Height", 42);
  StrictMock<MockOperation> operation;

  EXPECT_CALL(operation, propagateValue()).WillOnce(Return(Result(false)));
  connect(w, operation);
  ASSERT_FALSE(w = 150);

  // Wire keeps the invalid value
  ASSERT_EQ(w.get(), 150);
}

TEST_F(WireTest, implicitWireFromLiteral)
{
  Wire& a = mNetwork.make(42);

  // Not yet supported
  // a - a;
  a - 4;
  a + 3;
  a * 2;
  a / 3;
  a <= 100;
  a >= 0;
}

TEST_F(WireTest, reversedOperators)
{
  Wire& a = mNetwork.make(42);

  // Not yet supported
  // 4 - a;
  3 + a;
  2 * a;
  0 <= a;
  100 >= a;
}

}}
