// Copyright 2019 SICK AG. All rights reserved.

#pragma once

#include "IOperation.h"
#include "IWire.h"

namespace common { namespace constraints {

/** Models an addition operation between two wires. */
class Addition : public IOperation
{
public:
  virtual ~Addition(){};

  virtual std::string dump(unsigned int indentationLevel) const override;
  virtual std::string getShortDescription() const override;
  virtual std::string getName() const override;

private:
  Addition(const Addition&) = delete;
  void operator=(const Addition&) = delete;
  Addition(IWire& termA, IWire& termB, IWire& sum);

  virtual Result propagateValue() override;
  virtual Range range(const IWire& wire) const override;
  virtual WireExpression expression(const IWire& variable) const override;
  virtual std::string getErrorMessage() const override;

private:
  IWire& mTermA;
  IWire& mTermB;
  IWire& mSum;

  // Allow network factory functions to create addition objects
  friend class Network;
  friend class AdditionTest;
};

}}
