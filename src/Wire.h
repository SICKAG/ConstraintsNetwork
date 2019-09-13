// Copyright 2019 SICK AG. All rights reserved.
#pragma once

#include "IWire.h"

#include "IOperation.h"
#include "Range.h"
#include "Result.h"

#include <list>
#include <memory>


namespace common { namespace constraints {

class LessOrEqual;
class Network;
struct Range;

class Wire : public IWire
{
public:
  virtual double get() const override;
  virtual Result set(double value) override;
  virtual Range range() const override;
  virtual WireExpression expression(const IWire& variable) const override;
  virtual std::string dump(unsigned int indentationLevel = 0) const override;
  virtual std::string getShortDescription() const override;
  virtual std::string getName() const override;

  /** Assigns a value to the wire.
      \return a result object to know if the value was allowed
   */
  Result operator=(double value);
  /** Creates an addition operation between this wire and another.
      \return the output wire from the addition
   */
  Wire& operator+(Wire& other);
  Wire& operator+(double other);
  Wire& operator-(double other);
  /** Creates a multiplication operation between this wire and another.
      \return the output wire from the multiplication
 */
  Wire& operator*(Wire& other);
  Wire& operator*(double other);
  Wire& operator/(double other);
  /** Creates a less-than-or-equal-to relation operation between this wire and
      another.
      \return the operation object
   */
  LessOrEqual& operator<=(Wire& other);
  LessOrEqual& operator<=(double other);
  /** Creates a greater-than-or-equal-to relation operation between this wire
      and another.
      \return the operation object
   */
  LessOrEqual& operator>=(Wire& other);
  LessOrEqual& operator>=(double other);

private:
  Wire(const Wire&) = delete;
  void operator=(const Wire&) = delete;

  Wire(Network& network);
  Wire(Network& network, double value);

  virtual Range range(const IWire& wire) const override;
  virtual void connect(IOperation* operation) override;
  virtual void setDriver(IOperation* operation) override;
  virtual Network& getNetwork() const override;
  Result propagateValue();
  void setName(std::string name);

private:
  Network& mNetwork;
  IOperation* mDriver;
  double mValue;
  std::string mName;
  std::list<IOperation*> mOperations;

  // Allow network factory functions to create wires
  friend class Network;
  friend class WireTest;
};

/** Reversed variants for creating operations with literal values */
Wire& operator+(double left, Wire& right);
Wire& operator*(double left, Wire& right);
LessOrEqual& operator<=(double left, Wire& right);
LessOrEqual& operator>=(double left, Wire& right);

}}
