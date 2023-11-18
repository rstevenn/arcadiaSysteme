#ifndef __BASE_LOG__
#define __BASE_LOG__

#include <stdio.h>
#include <stdlib.h>

#define INFO(...) {printf("[     INFO      ] %s::%s()::l%d: ", __FILE__, __FUNCTION__, __LINE__); \
                      printf(__VA_ARGS__); printf("\n"); }

#define ERROR(...) {printf("[     ERROR     ] %s::%s()::l%d: ", __FILE__, __FUNCTION__, __LINE__); \
                    printf(__VA_ARGS__); printf("\n"); \
                    exit(-1);}

#define WARNING(...) {printf("[    WARNING    ] %s::%s()::l%d: ", __FILE__, __FUNCTION__, __LINE__); \
                      printf(__VA_ARGS__); printf("\n"); }

#define NOT_IMPLEMENTED() {printf("[NOT IMPLEMENTED] %s::%s()::l%d\n", __FILE__, __FUNCTION__, __LINE__);}


#define SECURE_REALLOCATE(ptr, type, size, ...) { (ptr) = (type)realloc((ptr), size); CHECK_ALLOCATE((ptr), __VA_ARGS__) }
#define SECURE_ALLOCATE(ptr, type, size, ...) {   (ptr) = (type)malloc(size); CHECK_ALLOCATE(ptr, __VA_ARGS__)  }

#define CHECK_ALLOCATE(ptr, ...) if ((ptr) == NULL) ERROR(__VA_ARGS__)
#define CHECK_READ_WRITE(sizea, sizeb,...) if ((sizea) != (sizeb)) ERROR(__VA_ARGS__)


#endif