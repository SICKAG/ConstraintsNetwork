// Copyright 2019 SICK AG. All rights reserved.
#pragma once

#include <limits>

// Forward-declares std::ostream
#include <iosfwd>

namespace common { namespace constraints {

/** A range models a contiguous set of values. */
struct Range
{
public:
  /** Helper constant for infinity to avoid std::numeric_limits. */
  static const double POSITIVE_INFINITY;
  /** Helper constant for infinity to avoid std::numeric_limits. */
  static const double NEGATIVE_INFINITY;
  /** The range of the full number line; from -infinity to +infinity */
  static const Range FULL;
  /** The empty range, containing no values */
  static const Range EMPTY;

  /** Creates a range from -infinity to +infinity. */
  Range();
  Range(double lower, double upper);
  Range(const Range& other) = default;
  Range(Range&& other) = default;

  /** Calculate an intersection range from two ranges, i.e., the values in both
      ranges.
   */
  static Range intersect(const Range& a, const Range& b);

  bool isEmpty() const;

  Range& operator=(const Range& other) = default;
  Range& operator=(Range&& other) = default;
  bool operator==(const Range& other) const;
  bool operator!=(const Range& other) const;

public:
  /** The lowest value in the set, inclusive. */
  double lower;
  /** The highest value in the set, inclusive. */
  double upper;
};

/** Helper function for stringifying a Range */
std::ostream& operator<<(std::ostream& s, const Range& range);

}}
