// Copyright 2019 SICK AG. All rights reserved.

#pragma once

#include "IOperation.h"
#include "IWire.h"

namespace common { namespace constraints {

/** Models a multiplication operation between two wires. */
class Multiplication : public IOperation
{
public:
  virtual ~Multiplication(){};

  virtual std::string dump(unsigned int indentationLevel) const override;
  virtual std::string getShortDescription() const override;
  virtual std::string getName() const override;

private:
  Multiplication(const Multiplication&) = delete;
  void operator=(const Multiplication&) = delete;
  Multiplication(IWire& factorA, IWire& factorB, IWire& product);

  virtual Result propagateValue() override;
  virtual Range range(const IWire& wire) const override;
  virtual WireExpression expression(const IWire& variable) const override;
  virtual std::string getErrorMessage() const override;

private:
  IWire& mFactorA;
  IWire& mFactorB;
  IWire& mProduct;

  // Allow network factory functions to create multiplication objects
  friend class Network;
  friend class MultiplicationTest;
};

}}
