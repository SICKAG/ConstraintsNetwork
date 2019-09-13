// Copyright 2019 SICK AG. All rights reserved.

#include "Network.h"

#include "LessOrEqual.h"

#include <gtest/gtest.h>

namespace common { namespace constraints {

class NetworkTest : public ::testing::Test
{
protected:
  void addInvalid(Network& network)
  {
    Wire& one = network.make(1);
    one <= 0;
  }

  void addValid(Network& network)
  {
    Wire& one = network.make(1);
    0 <= one;
  }
};

TEST_F(NetworkTest, addingInvalidConstraintAssertsWithErrorChecking)
{
  Network network(true);
  Wire& one = network.make(1);

  // Invalid constraint
  ASSERT_DEATH(one <= 0, ".*");
}

TEST_F(NetworkTest, addingValidConstraintDoesNotAssertWithErrorChecking)
{
  Network network(true);
  // Shall not assert
  addValid(network);
}

TEST_F(NetworkTest, addingInvalidConstraintDoesNotAssertWithoutErrorChecking)
{
  Network network(false);
  // Shall not assert
  addInvalid(network);
}

TEST_F(NetworkTest, addingValidConstraintDoesNotAssertWithoutErrorChecking)
{
  Network network(false);
  // Shall not assert
  addValid(network);
}

TEST_F(NetworkTest, activatingErrorCheckingWithInvalidConstraints)
{
  Network network(false);
  addInvalid(network);

  ASSERT_FALSE(network.activateSoundnessVerification());
}

TEST_F(NetworkTest, activatingErrorCheckingWithValidConstraints)
{
  Network network(false);
  addValid(network);

  ASSERT_TRUE(network.activateSoundnessVerification());
}

TEST_F(NetworkTest, activatingErrorCheckingTwiceInvalid)
{
  Network network(false);
  addInvalid(network);

  ASSERT_FALSE(network.activateSoundnessVerification());
  ASSERT_FALSE(network.activateSoundnessVerification());
}

TEST_F(NetworkTest, activatingErrorCheckingTwiceValid)
{
  Network network(false);
  addValid(network);

  ASSERT_TRUE(network.activateSoundnessVerification());
  ASSERT_TRUE(network.activateSoundnessVerification());
}


TEST_F(NetworkTest, errorCheckingStatusIsCorrectInitially)
{
  Network network(true);

  ASSERT_TRUE(network.isVerifyingSoundness());
}


TEST_F(NetworkTest, errorCheckingStatusIsUpdatedAfterActivating)
{
  Network network(false);

  ASSERT_FALSE(network.isVerifyingSoundness());

  network.activateSoundnessVerification();

  ASSERT_TRUE(network.isVerifyingSoundness());
}

TEST_F(NetworkTest, emptyNetworkIsValidWhenActivated)
{
  Network network(false);

  ASSERT_TRUE(network.activateSoundnessVerification());
}

TEST_F(NetworkTest, setValueFailsWhenInvalid)
{
  Network network(false);
  Wire& w = network.make(0);

  // Constraints that have no solution for w
  w <= 0;
  w >= 1;

  ASSERT_FALSE(network.activateSoundnessVerification());

  ASSERT_FALSE(w.set(0));
  ASSERT_FALSE(w.set(Range::POSITIVE_INFINITY));
}

TEST_F(NetworkTest, nonlinearConstraintAssertsOnRange)
{
  Network network(true);
  Wire& x = network.make(1);

  x* x <= 2;
  // Cannot solve with nonlinear expressions present
  ASSERT_DEATH(x.range(), ".*");
}

}}
