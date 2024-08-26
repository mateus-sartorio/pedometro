#define MAX_INT_STR_SIZE 12 // 10 digits + sign + null terminator

#include <stdint.h>

int itoa(int n, char* str);
void delay(uint32_t delay);
uint64_t array_sum(uint64_t* array, int size);