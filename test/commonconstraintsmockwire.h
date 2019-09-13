// Copyright 2019 SICK AG. All rights reserved.

#pragma once

#include "IWire.h"
#include <gmock/gmock.h>

namespace common { namespace constraints {

class MockWire : public IWire
{
public:
  MOCK_CONST_METHOD0(get, double());
  MOCK_METHOD1(set, Result(double value));
  MOCK_CONST_METHOD0(range, Range());
  MOCK_CONST_METHOD1(range, Range(const IWire& varyingWire));
  MOCK_CONST_METHOD1(expression, WireExpression(const IWire& varyingWire));
  MOCK_CONST_METHOD1(dump, std::string(unsigned int indentationLevel));
  MOCK_CONST_METHOD0(getShortDescription, std::string());
  MOCK_CONST_METHOD0(getName, std::string());
  MOCK_METHOD1(connect, void(IOperation* operation));
  MOCK_METHOD1(setDriver, void(IOperation* operation));
  MOCK_CONST_METHOD0(getNetwork, Network&());
};

} // namespace constraints
} // namespace common
