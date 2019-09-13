// Copyright 2019 SICK AG. All rights reserved.
#pragma once

#include "Range.h"
#include "Result.h"
#include "WireExpression.h"

#include <memory>
#include <string>

namespace common { namespace constraints {

class IOperation;
class Network;

/** A wire models a value of a connection in a constraint network. It could
    either be a externally driven variable or the output of an operation in the
    network.
 */
class IWire
{
public:
  virtual ~IWire(){};

  virtual double get() const = 0;
  virtual Result set(double value) = 0;
  /** Computes the allowed range of values for this wire */
  virtual Range range() const = 0;
  /** Given the network downstream of this wire, computes the range of
      varyingWire. This member function is not intended to be visible to the
      user of a Network.
   */
  virtual Range range(const IWire& varyingWire) const = 0;

  /** Computes an expression for the value of this wire as a function of
      varyingWire.
   */
  virtual WireExpression expression(const IWire& varyingWire) const = 0;

  /** Generates a string showing a tree of downstream wires and operations with
      their respective values and ranges.
   */
  virtual std::string dump(unsigned int indentationLevel) const = 0;
  /** Generates a string of the name and current value of the wire. */
  virtual std::string getShortDescription() const = 0;
  virtual std::string getName() const = 0;

protected:
  /** Connects this wire to the input side of an operation. During propagation
      the operation is made aware of the new value of this wire.
   */
  virtual void connect(IOperation* operation) = 0;
  /** Connects this wire to the output side of an operation. The given operation
      becomes the driver of the wire, i.e. sets its value during
      propagation.
   */
  virtual void setDriver(IOperation* operation) = 0;

  /** Gets the network that this wire belongs to. */
  virtual Network& getNetwork() const = 0;

  // Allow operations to connect
  friend class IOperation;
};

}}
