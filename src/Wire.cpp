// Copyright 2019 SICK AG. All rights reserved.
#include "Wire.h"

#include "Network.h"

#include <assert.h>
#include <sstream>

namespace {

std::string toString(double v)
{
  std::ostringstream s;
  s << v;
  return s.str();
}

}

namespace common { namespace constraints {

double Wire::get() const
{
  return mValue;
}

Result Wire::set(double value)
{
  mValue = value;
  return propagateValue();
}

Range Wire::range() const
{
  return range(*this);
}

WireExpression Wire::expression(const IWire& variable) const
{
  if (&variable == this)
  {
    return WireExpression::createLinear(1, 0);
  }
  else if (mDriver == nullptr)
  {
    return WireExpression::createLinear(0, get());
  }
  else
  {
    return mDriver->expression(variable);
  }
}

std::string Wire::dump(unsigned int indentationLevel) const
{
  std::ostringstream s;
  s << std::string(indentationLevel * 2, ' ') << getName();
  s << " with value " << mValue;
  s << " and range " << range() << std::endl;
  for (auto operation : mOperations)
  {
    s << operation->dump(indentationLevel + 1);
  }
  return s.str();
}


std::string Wire::getShortDescription() const
{
  std::ostringstream s;
  s << "(" << getName() << ")=" << mValue;
  return s.str();
}

std::string Wire::getName() const
{
  std::ostringstream s;
  if (!mName.empty())
  {
    s << mName;
    if (mDriver != nullptr)
    {
      s << ": ";
    }
  }
  if (mDriver != nullptr)
  {
    s << mDriver->getName();
  }
  return s.str();
}

Result Wire::operator=(double value)
{
  return set(value);
}

Wire& Wire::operator+(Wire& other)
{
  return mNetwork.add(*this, other);
}

Wire& Wire::operator+(double other)
{
  return *this + mNetwork.make(toString(other), other);
}

Wire& Wire::operator-(double other)
{
  return *this + (-other);
}

Wire& Wire::operator*(Wire& other)
{
  return mNetwork.multiply(*this, other);
}

Wire& Wire::operator*(double other)
{
  return *this * mNetwork.make(toString(other), other);
}


Wire& Wire::operator/(double other)
{
  return *this * (1 / other);
}

LessOrEqual& Wire::operator<=(Wire& other)
{
  return mNetwork.lessOrEqual(*this, other);
}

LessOrEqual& Wire::operator<=(double other)
{
  return *this <= mNetwork.make(toString(other), other);
}

LessOrEqual& Wire::operator>=(Wire& other)
{
  return other <= *this;
}

LessOrEqual& Wire::operator>=(double other)
{
  return *this >= mNetwork.make(toString(other), other);
}

// ----------------------------------------------------------------------------
// Private members

Wire::Wire(Network& network)
  : mNetwork(network)
  , mDriver(nullptr)
  , mValue(0.0)
{
}

Wire::Wire(Network& network, double value)
  : mNetwork(network)
  , mDriver(nullptr)
  , mValue(value)
{
}

Range Wire::range(const IWire& varyingWire) const
{
  Range r;
  for (auto operation : mOperations)
  {
    r = Range::intersect(r, operation->range(varyingWire));
  }
  return r;
}

void Wire::connect(IOperation* operation)
{
  mOperations.push_back(operation);
}


void Wire::setDriver(IOperation* operation)
{
  assert(mDriver == nullptr);
  mDriver = operation;
}

Network& Wire::getNetwork() const
{
  return mNetwork;
}

Result Wire::propagateValue()
{
  for (auto operation : mOperations)
  {
    Result result = operation->propagateValue();
    if (!result)
    {
      return result;
    }
  }
  return Result(true);
}

void Wire::setName(std::string name)
{
  mName = name;
}

Wire& operator+(double left, Wire& right)
{
  return right + left;
}

Wire& operator*(double left, Wire& right)
{
  return right * left;
}

LessOrEqual& operator<=(double left, Wire& right)
{
  return right >= left;
}

LessOrEqual& operator>=(double left, Wire& right)
{
  return right <= left;
}

}}
