#include <stdio.h>
#include "setu.h"

#define MODULE_PATH argv[1]
#define IMAGE_PATH argv[2]

int main(int argc, const char* argv[]) {
    load_module(MODULE_PATH);
    printf("%d\n", predict_file(IMAGE_PATH));
    return 0;
}