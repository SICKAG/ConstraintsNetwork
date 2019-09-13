// Copyright 2019 SICK AG. All rights reserved.

#pragma once

#include "Range.h"
#include "Result.h"
#include "Wire.h"

#include <memory>
#include <string>

namespace common { namespace constraints {

/** Models an operation in a constraint network.

    There are two main types of operations, relation operations and operations
    that produce an output. Relations model the actual constraints in the
    network and do not have an output.

    \see \ref Addition
    \see \ref LessOrEqual
 */
class IOperation
{
public:
  virtual ~IOperation() {}

  virtual std::string dump(unsigned int indentationLevel) const = 0;
  virtual std::string getShortDescription() const = 0;
  virtual std::string getName() const = 0;

protected:
  virtual Result propagateValue() = 0;
  /** Given the network downstream of this operation, computes the range of
      varyingWire.
   */
  virtual Range range(const IWire& varyingWire) const = 0;
  /** Computes an expression for the value of this operation as a
      function of varyingWire.
   */
  virtual WireExpression expression(const IWire& varyingWire) const = 0;
  virtual std::string getErrorMessage() const = 0;

  /** Helper function to connect to a wire without having each subclass as a
      friend of IWire.
   */
  void connect(IWire& wire) { wire.connect(this); }
  /** Helper function to become the driver of a wire without having each
      subclass as a friend of IWire.
   */
  void drive(IWire& wire) { wire.setDriver(this); }
  /** Helper function to access the network that a wire belongs to without
      having each subclass as a friend of IWire.
   */
  Network& getNetwork(IWire& wire) { return wire.getNetwork(); }

  // Allow wire to propagate
  friend class Wire;
  // Allow result to read error message
  friend class Result;
};

}}
