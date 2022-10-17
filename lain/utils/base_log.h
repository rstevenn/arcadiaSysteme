#ifndef __BASE_LOG__
#define __BASE_LOG__

#define INFO(...) {printf("[     INFO      ] %s::%s()::l%d: ", __FILE__, __FUNCTION__, __LINE__); \
                      printf(__VA_ARGS__); printf("\n"); }

#define ERROR(...) {printf("[     ERROR     ] %s::%s()::l%d: ", __FILE__, __FUNCTION__, __LINE__); \
                    printf(__VA_ARGS__); printf("\n"); \
                    exit(-1);}

#define WARNING(...) {printf("[    WARNING    ] %s::%s()::l%d: ", __FILE__, __FUNCTION__, __LINE__); \
                      printf(__VA_ARGS__); printf("\n"); }

#define NOT_IMPLEMENTED() {printf("[NOT IMPLEMENTED] %s::%s()::l%d\n", __FILE__, __FUNCTION__, __LINE__);}

#define CHECK_ALLOCATE(ptr, ...) if (ptr == NULL) ERROR(__VA_ARGS__)
#define CHECK_READ_WRITE(sizea, sizeb,...) if (sizea != sizeb) ERROR(__VA_ARGS__)


#endif