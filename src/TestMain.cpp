#include <gtest\gtest.h>

#ifdef _MSC_VER
#  include <Windows.h>
#endif

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
#ifdef _MSC_VER
  if (IsDebuggerPresent() && result != 0)
  {
    // Wait to allow user to see stdout/err in window before it disappears.
    system("PAUSE");
  }
#endif
  return result;
}
