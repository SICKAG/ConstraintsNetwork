// Copyright 2019 SICK AG. All rights reserved.

#include "Multiplication.h"

#include "Network.h"
#include "Wire.h"

#include <assert.h>
#include <sstream>

namespace common { namespace constraints {

std::string Multiplication::dump(unsigned int indentationLevel) const
{
  std::ostringstream s;
  s << std::string(indentationLevel * 2, ' ') << getShortDescription()
    << std::endl;
  s << mProduct.dump(indentationLevel + 1);
  return s.str();
}

std::string Multiplication::getShortDescription() const
{
  std::ostringstream name;
  name << mFactorA.getShortDescription() << " * "
       << mFactorB.getShortDescription();
  return name.str();
}

std::string Multiplication::getName() const
{
  std::ostringstream name;
  name << "(" << mFactorA.getName() << ") * (" << mFactorB.getName() << ")";
  return name.str();
}

// ----------------------------------------------------------------------------
// Private functions

Multiplication::Multiplication(IWire& factorA, IWire& factorB, IWire& product)
  : mFactorA(factorA)
  , mFactorB(factorB)
  , mProduct(product)
{
  connect(mFactorA);
  connect(mFactorB);
  drive(mProduct);
  bool valid = propagateValue();
  if (getNetwork(mFactorA).isVerifyingSoundness())
  {
    assert(valid);
  }
}

Result Multiplication::propagateValue()
{
  Result r = mProduct.set(mFactorA.get() * mFactorB.get());
  r.push(this);
  return r;
}

Range Multiplication::range(const IWire& varyingWire) const
{
  // Range is computed by the relation operator, just recurse
  return mProduct.range(varyingWire);
}

std::string Multiplication::getErrorMessage() const
{
  std::ostringstream s;
  s << getName() << " would fail because ";
  return s.str();
}

WireExpression Multiplication::expression(const IWire& varyingWire) const
{
  return mFactorA.expression(varyingWire) * mFactorB.expression(varyingWire);
}

}}
