# setu-class-cpp
Cpp lib to load & use pytorch model of setu-class.

# Compile & Install

```bash
git clone https://github.com/fumiama/setu-class-cpp.git
cd setu-class-cpp
mkdir build
cd build
cmake ..
make
make install
```

# Usage

Use it as below.

```c
#include <stdio.h>
#include <setu.h>

#define MODULE_PATH argv[1]
#define IMAGE_PATH argv[2]

int main(int argc, const char* argv[]) {
    int i = load_module(MODULE_PATH);
    printf("[%d]: %d - %s\n", i, predict_file(IMAGE_PATH, i), IMAGE_PATH);
    return 0;
}
```

Then add `-lsetu` option to compile.
