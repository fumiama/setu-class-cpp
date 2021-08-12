#ifndef _SETU_H_
#define _SETU_H_
    // #define DEBUG
    #ifndef MAX_FILENAME_LEN
        #define MAX_FILENAME_LEN 512
    #endif

    // return module index
    int load_module(const char* path);
    int predict_file(const char* path, int index);
#endif // _SETU_H_
