// Copyright 2019 SICK AG. All rights reserved.

#include "WireExpression.h"

namespace common { namespace constraints {

WireExpression WireExpression::createLinear(double firstDegree, double constant)
{
  return WireExpression(firstDegree, constant, false);
}

WireExpression WireExpression::operator+(const WireExpression& other) const
{
  return WireExpression(firstDegree + other.firstDegree,
                        constant + other.constant,
                        nonlinear || other.nonlinear);
}

WireExpression WireExpression::operator-(const WireExpression& other) const
{
  return WireExpression(firstDegree - other.firstDegree,
                        constant - other.constant,
                        nonlinear || other.nonlinear);
}

WireExpression WireExpression::operator*(const WireExpression& other) const
{
  double secondDegree = firstDegree * other.firstDegree;
  return WireExpression(firstDegree * other.constant
                          + constant * other.firstDegree,
                        constant * other.constant,
                        nonlinear || other.nonlinear || secondDegree != 0.0);
}

bool WireExpression::operator==(const WireExpression& other) const
{
  return firstDegree == other.firstDegree && constant == other.constant
         && nonlinear == other.nonlinear;
}

WireExpression::WireExpression(double firstDegree,
                               double constant,
                               bool nonlinear)
  : constant(constant)
  , firstDegree(firstDegree)
  , nonlinear(nonlinear)
{
}

std::ostream& operator<<(std::ostream& s, const WireExpression& expression)
{
  s << expression.firstDegree << "*x";
  if (expression.constant >= 0)
  {
    s << " + " << expression.constant;
  }
  else
  {
    s << " - " << -expression.constant;
  }
  if (expression.nonlinear)
  {
    s << " + a_nonlinear_term";
  }
  return s;
}

}}
