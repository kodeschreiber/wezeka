#include "moremath.h"

/*
  Four optimizations:
  - Odds only
  - Start at half the input
  - Start with smaller numbers
  - Start at 3
  - Return upon non prime
*/
bool isprime(int input) {
  if(!(input % 2))
    return 0;
  for(int c = 3; c < input/2; c+=2)
    if(!(input % c))
      return 0;
  return 1;
}

int nextprime(int target) {
  int res = target;
  while(!isprime(res++));
  return res - 1;
}

// Uses the Murmur2 algorithmm 64bit
// Code from: https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c
// Algorithm licensed as public domain
uint64_t hash(const void* key, int len, uint64_t seed) {
  const uint64_t m = 0xc6a4a7935bd1e995;
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  const uint64_t * data = (const uint64_t *)key;
  const uint64_t * end = data + (len/8);

  while(data != end)
  {
    uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  const unsigned char * data2 = (const unsigned char*)data;

  switch(len & 7)
  {
  case 7: h ^= ((uint64_t) data2[6]) << 48;
  case 6: h ^= ((uint64_t) data2[5]) << 40;
  case 5: h ^= ((uint64_t) data2[4]) << 32;
  case 4: h ^= ((uint64_t) data2[3]) << 24;
  case 3: h ^= ((uint64_t) data2[2]) << 16;
  case 2: h ^= ((uint64_t) data2[1]) << 8;
  case 1: h ^= ((uint64_t) data2[0]);
          h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}
