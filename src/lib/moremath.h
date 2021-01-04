#ifndef MOREMATH_H
#define MOREMATH_H

#include "basics.h"
#include <stdint.h>

bool isprime(int input);
int nextprime(int target);
uint64_t hash(const void* key, int len, uint64_t seed);

#endif
