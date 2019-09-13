// Copyright 2019 SICK AG. All rights reserved.

#include "Addition.h"

#include "Network.h"
#include "Wire.h"

#include <assert.h>
#include <sstream>

namespace common { namespace constraints {

std::string Addition::dump(unsigned int indentationLevel) const
{
  std::ostringstream s;
  s << std::string(indentationLevel * 2, ' ') << getShortDescription()
    << std::endl;
  s << mSum.dump(indentationLevel + 1);
  return s.str();
}

std::string Addition::getShortDescription() const
{
  std::ostringstream name;
  name << mTermA.getShortDescription() << " + " << mTermB.getShortDescription();
  return name.str();
}

std::string Addition::getName() const
{
  std::ostringstream name;
  name << mTermA.getName() << " + " << mTermB.getName();
  return name.str();
}

// ----------------------------------------------------------------------------
// Private functions

Addition::Addition(IWire& termA, IWire& termB, IWire& sum)
  : mTermA(termA)
  , mTermB(termB)
  , mSum(sum)
{
  connect(mTermA);
  connect(mTermB);
  drive(mSum);
  bool valid = propagateValue();
  if (getNetwork(mTermA).isVerifyingSoundness())
  {
    assert(valid);
  }
}

Result Addition::propagateValue()
{
  Result r = mSum.set(mTermA.get() + mTermB.get());
  r.push(this);
  return r;
}

Range Addition::range(const IWire& varyingWire) const
{
  // Range is computed by the relation operator, just recurse
  return mSum.range(varyingWire);
}

std::string Addition::getErrorMessage() const
{
  std::ostringstream s;
  s << getName() << " would fail because ";
  return s.str();
}

WireExpression Addition::expression(const IWire& varyingWire) const
{
  return mTermA.expression(varyingWire) + mTermB.expression(varyingWire);
}

}}
