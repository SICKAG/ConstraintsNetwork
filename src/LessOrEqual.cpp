// Copyright 2019 SICK AG. All rights reserved.

#include "LessOrEqual.h"

#include "IWire.h"
#include "Network.h"

#include <assert.h>
#include <iostream>
#include <sstream>

namespace {

using namespace common::constraints;

/** Solve for x in the inequality a*x + b <= c*x + d
    \return the range of values of x that satisfy the inequality
 */
Range solveInequality(WireExpression left, WireExpression right)
{
  // Change "left <= right" to general form:
  // 0 <= difference = right - left
  WireExpression difference = right - left;

  if (difference.nonlinear)
  {
    std::stringstream message;
    message << "LessOrEqual cannot solve inequality with nonlinear expression: "
            << left << " < " << right;
    std::cerr << message.str() << std::endl;
    assert(false);
  }
  // The difference is an expression of form k*x + m
  const double k = difference.firstDegree;
  const double m = difference.constant;

  // When solving 0 <= k*x + m there are three cases:

  // Case I: k < 0: x <= -m/k
  if (k < 0)
  {
    return Range(Range::NEGATIVE_INFINITY, -m / k);
  }
  // Case II: k > 0: x >= -m/k
  else if (k > 0)
  {
    return Range(-m / k, Range::POSITIVE_INFINITY);
  }
  // Case III: k == 0: 0 >= -m
  else
  {
    // x can be any real number if m >= 0
    if (m >= 0)
    {
      return Range::FULL;
    }
    // otherwise no solutions for x
    else
    {
      return Range::EMPTY;
    }
  }
}

}


namespace common { namespace constraints {

std::string LessOrEqual::dump(unsigned int indentationLevel) const
{
  std::ostringstream s;
  s << std::string(indentationLevel * 2, ' ') << getShortDescription()
    << std::endl;
  return s.str();
}

std::string LessOrEqual::getShortDescription() const
{
  std::ostringstream name;
  name << mLeft.getShortDescription() << " <= " << mRight.getShortDescription();
  return name.str();
}

std::string LessOrEqual::getName() const
{
  std::ostringstream name;
  name << mLeft.getName() << " <= " << mRight.getName();
  return name.str();
}

// ----------------------------------------------------------------------------
// Private functions

LessOrEqual::LessOrEqual(IWire& left, IWire& right)
  : mLeft(left)
  , mRight(right)
{
  connect(mLeft);
  connect(mRight);
  bool valid = propagateValue();
  if (getNetwork(mLeft).isVerifyingSoundness())
  {
    assert(valid);
  }
}

Result LessOrEqual::propagateValue()
{
  Result r = Result(mLeft.get() <= mRight.get());
  r.push(this);
  return r;
}

Range LessOrEqual::range(const IWire& varyingWire) const
{
  // Solve for the valid values of varyingWire
  return solveInequality(mLeft.expression(varyingWire),
                         mRight.expression(varyingWire));
}

std::string LessOrEqual::getErrorMessage() const
{
  std::ostringstream s;
  s << getName() << " would fail";
  return s.str();
}


WireExpression LessOrEqual::expression(const IWire& varyingWire) const
{
  // Should never be called.
  (void)varyingWire;
  exit(1);
}

}}
