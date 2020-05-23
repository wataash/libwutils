# libwutils

<!--
sync_libwutils.bash
-->

wataash's single-file utility library. Very WIP.

## Usage (incorporate source)

### In C++ (or C/C++ mixing) project

In single c++ source in your project (usually the one containing `main()`):

```c++
#define WUTILS_IMPL
#include "path/to/wutils.cc"
```

And other files should include me without `WUTILS_IMPL`:

```c++
#include "path/to/wutils.cc"
```

### In C project

One `c++` (`g++`/`clang++`) invocation is needed. In your Makefile:

```Makefile
SOURCES += path/to/wutils.cc
CPPFLAGS += -include path/to/wutils.cc  # TODO: test
CPPFLAGS += -DWUTILS_IMPL_IF_CXX
```

<!--
CPPFLAGS += -DWUTILS_IMPL_IF_CXX
or
CMAKE_CXXFLAGS -DWUTILS_IMPL
-->

Then let your C sources `#include "path/to/wutils.cc"`.

<!--
Some build system may beed -x c++ ?

compile all files with c++ is also ok if cc/c++ is from same distribution because ABI TODO
-->

## Usage (as library)

TODO

```
-lwutils
-lwutils -lstdc++  # for C project
```

## Why not single _header_ library?

- In order to reduce compilation time. (avoid compiling `static inline`
  functions from all files including `wutils.cc`)
- Making shared/static library would be easy. (TODO)

<!--
- Some build systems (e.g. NetBSD's Makefile) can't compile

If hpp, -x c++ is needed (maybe)
  c++ -x c++ path/to/wutils.hpp
-->
