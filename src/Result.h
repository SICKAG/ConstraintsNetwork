// Copyright 2019 SICK AG. All rights reserved.
#pragma once

#include <list>
#include <string>

namespace common { namespace constraints {

class IOperation;

/** Holds information about the outcome of setting the value of a Wire. */
class Result
{
public:
  /** Creates a result with a success value
      \param success true if the setting was successful
   */
  Result(bool success);
  Result(Result&& result) = default;
  Result(const Result& result) = default;

  /** Adds to the chain of operations that are affected */
  void push(const IOperation* operation);
  /** Returns a message detailing why the setting was not successful in terms of
      which operations are affected
   */
  std::string getErrorMessage() const;

  /** Returns the success value of this result */
  operator bool() const;

private:
  bool mSuccess;
  std::list<const IOperation*> mOperations;
};

}}
