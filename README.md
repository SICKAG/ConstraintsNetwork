# ConstraintsNetwork

This is a library implementing a constraints network and is aimed for embedded C++ platforms.

A constraint network can be used to calculate allowed ranges of variables
with given relations between them. The network can also verify that the
actual values of the variables are within these ranges.

More information is available in [Network.h](src/Network.h)

Some examples are implemented as unit tests in [ConstraintsTest.cpp](test/ConstraintsTest.cpp)

### Setup

- Download [Google Test 1.8.0](https://github.com/google/googletest/releases/tag/release-1.8.0)
  and extract as folder googletest-release-1.8.0 in the root directory.
- Build Google Test:
  - Open googletest-release-1.8.0/googletest/msvc/gtest.sln in Visual Studio 2015.
  - When asked to do a one-way upgrade of the project files, click `OK`.
  - Create a new solution platform: x64
    - In the Visual Studio menu select `Build`, then `Configuration Manager`.
    - In the dropdown for *Active solution platform*, select `<New...>`
    - In the window *New Solution Platform* type `x64` in the first field, then click `OK`
    - Close the *Configuration Manager* window.
  - Ensure that the active solution configuration is Debug x64.
  - Build solution
- Build Google Mock:
  - Open googletest-release-1.8.0/googlemock/msvc/2015/gmock.sln in Visual Studio 2015
  - Create a new solution platform: x64. Follow the same steps as for Google Test above.
  - Ensure that the active solution configuration is Debug x64.
  - Build solution
- In the ConstraintsNetwork solution set Test as the startup project
- Start a debugging session, which executes the unit tests.

### License

Apache License Version 2.0, see [LICENSE](LICENSE)
