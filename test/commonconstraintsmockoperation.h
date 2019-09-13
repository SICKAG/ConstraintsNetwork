// Copyright 2019 SICK AG. All rights reserved.

#pragma once

#include "IOperation.h"
#include <gmock/gmock.h>

namespace common { namespace constraints {

class MockOperation : public IOperation
{
public:
  MOCK_CONST_METHOD1(dump, std::string(unsigned int indentationLevel));
  MOCK_CONST_METHOD0(getShortDescription, std::string());
  MOCK_CONST_METHOD0(getName, std::string());
  MOCK_METHOD0(propagateValue, Result());
  MOCK_CONST_METHOD1(range, Range(const IWire& varyingWire));
  MOCK_CONST_METHOD1(expression, WireExpression(const IWire& varyingWire));
  MOCK_CONST_METHOD0(getErrorMessage, std::string());
};

} // namespace constraints
} // namespace common
