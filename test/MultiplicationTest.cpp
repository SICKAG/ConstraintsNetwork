// Copyright 2019 SICK AG. All rights reserved.

#include "Multiplication.h"
#include "Network.h"
#include "Wire.h"

#include "commonconstraintsMockWire.h"

#include <gtest/gtest.h>

namespace common { namespace constraints {

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

class MultiplicationTest : public ::testing::Test
{
protected:
  std::unique_ptr<Multiplication>
  createMultiplication(IWire& factorA, IWire& factorB, IWire& product)
  {
    return std::unique_ptr<Multiplication>(
      new Multiplication(factorA, factorB, product));
  }

  Range range(Multiplication& multiplier, Wire& wire)
  {
    return multiplier.range(wire);
  }

protected:
  Network mNetwork;
};

TEST_F(MultiplicationTest, twoTimesTwoEqualsFour)
{
  Wire& product = mNetwork.multiply(mNetwork.make(2), mNetwork.make(2));
  ASSERT_EQ(product.get(), 4);
}

TEST_F(MultiplicationTest, multiplyNegativeFraction)
{
  Wire& product = mNetwork.multiply(mNetwork.make(10), mNetwork.make(-0.1));
  ASSERT_NEAR(product.get(), -1.0, 1e-7);
}

TEST_F(MultiplicationTest, mutliplyThreeWires)
{
  Wire& product = mNetwork.multiply(
    mNetwork.make(4), mNetwork.multiply(mNetwork.make(3), mNetwork.make(2)));
  ASSERT_EQ(product.get(), 24);
}

TEST_F(MultiplicationTest, backpropagateInfiniteRangeToFactorA)
{
  StrictMock<MockWire> outWire;

  EXPECT_CALL(outWire, setDriver(_));
  EXPECT_CALL(outWire, set(_)).WillOnce(Return(Result(true)));
  EXPECT_CALL(outWire, range(_)).WillOnce(Return(Range::FULL));

  Wire& factorA = mNetwork.make(0);
  std::unique_ptr<Multiplication> multiplier =
    createMultiplication(factorA, mNetwork.make(1), outWire);

  ASSERT_EQ(range(*multiplier, factorA), Range::FULL);
}

TEST_F(MultiplicationTest, backpropagateLimitedRangeToFactorA)
{
  StrictMock<MockWire> outWire;

  EXPECT_CALL(outWire, setDriver(_));
  EXPECT_CALL(outWire, set(_)).WillOnce(Return(Result(true)));
  EXPECT_CALL(outWire, range(_)).WillOnce(Return(Range(0, 100)));

  Wire& factorA = mNetwork.make(0);
  std::unique_ptr<Multiplication> adder =
    createMultiplication(factorA, mNetwork.make(5), outWire);

  ASSERT_EQ(factorA.range(), Range(0, 100));
}

TEST_F(MultiplicationTest, backpropagateLimitedRangeToFactorB)
{
  StrictMock<MockWire> outWire;

  EXPECT_CALL(outWire, setDriver(_));
  EXPECT_CALL(outWire, set(_)).WillOnce(Return(Result(true)));
  EXPECT_CALL(outWire, range(_)).WillOnce(Return(Range(0, 100)));

  Wire& factorB = mNetwork.make(0);
  std::unique_ptr<Multiplication> adder =
    createMultiplication(mNetwork.make(5), factorB, outWire);

  ASSERT_EQ(factorB.range(), Range(0, 100));
}
}}
