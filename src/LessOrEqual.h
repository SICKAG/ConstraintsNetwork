// Copyright 2019 SICK AG. All rights reserved.
#pragma once

#include "IOperation.h"
#include "IWire.h"

namespace common { namespace constraints {

/** Models a less-than-or-equal-to relation between two wires. */
class LessOrEqual : public IOperation
{
public:
  virtual ~LessOrEqual(){};

  virtual std::string dump(unsigned int indentationLevel) const override;
  virtual std::string getShortDescription() const override;
  virtual std::string getName() const override;

private:
  LessOrEqual(const LessOrEqual&) = delete;
  void operator=(const LessOrEqual&) = delete;
  LessOrEqual(IWire& left, IWire& right);

  virtual Result propagateValue() override;
  virtual Range range(const IWire& varyingWire) const override;
  virtual WireExpression expression(const IWire& varyingWire) const override;
  virtual std::string getErrorMessage() const override;

private:
  IWire& mLeft;
  IWire& mRight;

  // Allow network factory functions to create comparison objects
  friend class Network;
  friend class LessOrEqualTest;
};
}}
