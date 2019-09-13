// Copyright 2019 SICK AG. All rights reserved.

#include "Result.h"

#include "IOperation.h"

#include <sstream>

namespace common { namespace constraints {

Result::Result(bool success)
  : mSuccess(success)
{
}

Result::operator bool() const
{
  return mSuccess;
}

void Result::push(const IOperation* operation)
{
  mOperations.push_front(operation);
}

std::string Result::getErrorMessage() const
{
  std::ostringstream message;
  for (auto operation : mOperations)
  {
    message << operation->getErrorMessage();
  }
  message << ".";
  return message.str();
}

}}
