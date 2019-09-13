// Copyright 2019 SICK AG. All rights reserved.

#include "Range.h"

#include <algorithm>
#include <ostream>

namespace common { namespace constraints {

const double Range::POSITIVE_INFINITY = std::numeric_limits<double>::infinity();
const double Range::NEGATIVE_INFINITY =
  -std::numeric_limits<double>::infinity();
const Range Range::FULL = Range();
const Range Range::EMPTY = Range(1, 0);

Range::Range()
  : lower(NEGATIVE_INFINITY)
  , upper(POSITIVE_INFINITY)
{
}

Range::Range(double lower, double upper)
  : lower(lower)
  , upper(upper)
{
}

Range Range::intersect(const Range& a, const Range& b)
{
  Range r(a);
  r.lower = std::max(r.lower, b.lower);
  r.upper = std::min(r.upper, b.upper);
  return r;
}

bool Range::isEmpty() const
{
  return lower > upper;
}

bool Range::operator==(const Range& other) const
{
  if (isEmpty() && other.isEmpty())
  {
    return true;
  }
  return lower == other.lower && upper == other.upper;
}

bool Range::operator!=(const Range& other) const
{
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& s, const Range& range)
{
  if (range.isEmpty())
  {
    s << "Empty range";
  }
  else
  {
    s << "[" << range.lower << ", " << range.upper << "]";
  }
  return s;
}

}}
