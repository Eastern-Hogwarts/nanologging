NanoLogging
============
A light and header-only logging library for debug propose based on C++17.

Usage
-----

```c++
#include "nanologging\nanologging.h"

int main() {
  LOG_INFO("This is LOG_INFO with value ", 123);
  LOG_WARN("This is LOG_WARN with value ", 0.01);
  // LOG_ERROR("This is LOG_ERROR with value ", 'c', " and throw");

  CHECK_EQ(1, 1);
  // CHECK_EQ(1, 2);

  return 0;
}
```
