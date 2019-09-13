// Copyright 2019 SICK AG. All rights reserved.

#include "Addition.h"
#include "Network.h"
#include "Wire.h"

#include "commonconstraintsMockWire.h"

#include <gtest/gtest.h>

namespace common { namespace constraints {

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

class AdditionTest : public ::testing::Test
{
protected:
  std::unique_ptr<Addition>
  createAddition(IWire& termA, IWire& termB, IWire& sum)
  {
    return std::unique_ptr<Addition>(new Addition(termA, termB, sum));
  }

  Range range(Addition& adder, Wire& wire) { return adder.range(wire); }

protected:
  Network mNetwork;
};

TEST_F(AdditionTest, onePlusOneEqualsTwo)
{
  Wire& sum = mNetwork.add(mNetwork.make(1), mNetwork.make(1));
  ASSERT_EQ(sum.get(), 2);
}

TEST_F(AdditionTest, addNegativeFraction)
{
  Wire& sum = mNetwork.add(mNetwork.make(2), mNetwork.make(-1.7));
  ASSERT_NEAR(sum.get(), 0.3, 1e-7);
}

TEST_F(AdditionTest, addThreeWires)
{
  Wire& sum = mNetwork.add(mNetwork.make(3),
                           mNetwork.add(mNetwork.make(2), mNetwork.make(1)));
  ASSERT_EQ(sum.get(), 6);
}

TEST_F(AdditionTest, backpropagateInfiniteRangeToTermA)
{
  StrictMock<MockWire> outWire;

  EXPECT_CALL(outWire, setDriver(_));
  EXPECT_CALL(outWire, set(_)).WillOnce(Return(Result(true)));
  EXPECT_CALL(outWire, range(_)).WillOnce(Return(Range::FULL));

  Wire& termA = mNetwork.make(0);
  std::unique_ptr<Addition> adder =
    createAddition(termA, mNetwork.make(1), outWire);

  ASSERT_EQ(range(*adder, termA), Range::FULL);
}

TEST_F(AdditionTest, backpropagateLimitedRangeToTermA)
{
  StrictMock<MockWire> outWire;

  EXPECT_CALL(outWire, setDriver(_));
  EXPECT_CALL(outWire, set(_)).WillOnce(Return(Result(true)));
  EXPECT_CALL(outWire, range(_)).WillOnce(Return(Range(0, 100)));

  Wire& termA = mNetwork.make(0);
  std::unique_ptr<Addition> adder =
    createAddition(termA, mNetwork.make(10), outWire);

  ASSERT_EQ(termA.range(), Range(0, 100));
}

TEST_F(AdditionTest, backpropagateLimitedRangeToTermB)
{
  StrictMock<MockWire> outWire;

  EXPECT_CALL(outWire, setDriver(_));
  EXPECT_CALL(outWire, set(_)).WillOnce(Return(Result(true)));
  EXPECT_CALL(outWire, range(_)).WillOnce(Return(Range(0, 100)));

  Wire& termB = mNetwork.make(0);
  std::unique_ptr<Addition> adder =
    createAddition(mNetwork.make(10), termB, outWire);

  ASSERT_EQ(termB.range(), Range(0, 100));
}
}}
