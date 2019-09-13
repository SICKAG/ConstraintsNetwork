// Copyright 2019 SICK AG. All rights reserved.

#pragma once

#include <ostream>

namespace common { namespace constraints {

/** Models an expression firstDegree * x + constant + a_nonlinear_function(x) of
    some variable x.
 */
struct WireExpression
{
public:
  /** Factory function to create a linear expression. */
  static WireExpression createLinear(double firstDegree, double constant);

  /** Forms a new expression s(x) which represents the sum of two expressions
      e1(x) and e2(x) such that s(x) = e1(x) + e2(x) */
  WireExpression operator+(const WireExpression& other) const;
  /** Forms a new expression d(x) which represents the difference of two
      expressions e1(x) and e2(x) such that d(x) = e1(x) - e2(x)
   */
  WireExpression operator-(const WireExpression& other) const;
  /** Forms a new expression d(x) which represents the product of two
      expressions e1(x) and e2(x) such that d(x) = e1(x) * e2(x)
   */
  WireExpression operator*(const WireExpression& other) const;
  /** Checks for expression equality, i.e., if all coefficients are equal.
   */
  bool operator==(const WireExpression& other) const;

public:
  double constant;
  double firstDegree;
  bool nonlinear;

private:
  WireExpression(double firstDegree, double constant, bool nonlinear);

  friend class WireExpressionTest;
  friend class LessOrEqualTest;
};

/** Helper function for stringifying a WireExpression */
std::ostream& operator<<(std::ostream& s, const WireExpression& expression);

}}
