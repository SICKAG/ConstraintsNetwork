// Copyright 2019 SICK AG. All rights reserved.

#include "Wire.h"

#include <list>

namespace common { namespace constraints {

class IOperation;
class LessOrEqual;

/** Holds a forward-propagating constraint network.

    A constraint network can be used to calculate allowed ranges of variables
    with given relations between them. The network can also verify that the
    actual values of the variables are within these ranges.

    The network consists of wires and operations. A wire can be a variable that
    is connected as an input to an operation. Wires can also be output from an
    operation, e.g., an addition or multiplication. Some operations model
    relations and have no output, e.g., comparison.

    <pre>
         Height  Width  <- Wires
             |     |
              \   /
              [ * ]   <- Operation
                |
               Area  1234  <- Wires
                \    /
                [ <= ]  <- Relation operation
    </pre>

    Example code with constraint network:
    \code{.cpp}
      Network network;
      Wire& height = network.make(0);
      Wire& width = network.make(0);
      Wire& area = height * width;
      area <= 1234;

      assert(height.set(10));
      assert(width.set(5));
      double maxHeight = height.range().upper;
    \endcode

    \section memory Memory management
    The Network owns all the Wires and IOperations that are part of the network.
    Destroying the Network object will invalidate all references to these.
    There is no guarantee that combining wires from different Networks will
    work as intended.

    \see \ref IWire
    \see \ref IOperation
 */
class Network
{
public:
  /** Creates a constraints network. Will assert when a constraint that does not
      hold is added.
   */
  Network() = default;

  /** Creates a network, specifying whether to assert when a constraint that
      does not hold is added. Disabling it allows the network to be built
      without regard for what the current values of the wires currently are.
      This error checking does not affect the results returned when setting
      wire values.

      \see \ref activateSoundnessVerification
   */
  Network(bool verifySoundness);

  /** Creates a wire modeling a double variable. */
  Wire& make(double value);
  /** Creates a wire modeling a double variable, with a name for easier
      debugging.
   */
  Wire& make(std::string name, double value);

  /** Creates an addition operation between two wires.
      \return the output wire for the sum
      \note Prefer using the \ref Wire::operator+()
   */
  Wire& add(Wire& termA, Wire& termB);

  /** Creates a multiplication operation between two wires.
      \return the output wire for the product
      \note Prefer using the \ref Wire::operator*()
   */
  Wire& multiply(Wire& factorA, Wire& factorB);

  /** Creates an less-than-or-equal-to relation operation between two wires.
      \return the actual operation object
      \note Prefer using the \ref Wire::operator<=()
  */
  LessOrEqual& lessOrEqual(IWire& left, IWire& right);

  bool isVerifyingSoundness();

  /** Activates immediate verification that new constraints are sound when they
      are added. Runs a propagation of all wires and returns if all are in a
      valid state before activating checking which asserts when a constraint
      that does not hold is added. This is meant to be used after the network
      has been built and if error checking was disabled before. This error
      checking does not affect the results returned when setting wire values.
   */
  bool activateSoundnessVerification();

private:
  Network(const Network&) = delete;
  void operator=(const Network&) = delete;

private:
  std::list<std::unique_ptr<Wire>> mWires;
  std::list<std::unique_ptr<IOperation>> mOperations;

  bool mVerifySoundness = true;
};
}}
