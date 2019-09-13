// Copyright 2019 SICK AG. All rights reserved.

#include "Network.h"

#include "Addition.h"
#include "LessOrEqual.h"
#include "Multiplication.h"
#include "Wire.h"

#include <algorithm>
#include <sstream>

namespace common { namespace constraints {


Network::Network(bool verifySoundness)
  : mVerifySoundness(verifySoundness)
{
}

Wire& Network::make(double value)
{
  Wire* wire = new Wire(*this, value);
  mWires.emplace_back(wire);
  std::ostringstream name;
  name << "Wire" << mWires.size();
  wire->setName(name.str());
  return *wire;
}

Wire& Network::make(std::string name, double value)
{
  Wire& w = make(value);
  w.setName(name);
  return w;
}

Wire& Network::add(Wire& termA, Wire& termB)
{
  Wire* sum = new Wire(*this);
  mWires.emplace_back(sum);
  mOperations.emplace_back(new Addition(termA, termB, *sum));
  return *sum;
}

Wire& Network::multiply(Wire& factorA, Wire& factorB)
{
  Wire* product = new Wire(*this);
  mWires.emplace_back(product);
  mOperations.emplace_back(new Multiplication(factorA, factorB, *product));
  return *product;
}

LessOrEqual& Network::lessOrEqual(IWire& left, IWire& right)
{
  LessOrEqual* pointer = new LessOrEqual(left, right);
  mOperations.emplace_back(pointer);
  return *pointer;
}

bool Network::isVerifyingSoundness()
{
  return mVerifySoundness;
}

bool Network::activateSoundnessVerification()
{
  mVerifySoundness = true;
  return std::all_of(
    mWires.begin(), mWires.end(), [&](std::unique_ptr<Wire>& w) {
      return w->propagateValue();
    });
}
}}
