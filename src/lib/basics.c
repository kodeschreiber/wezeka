#include "basics.h"

int macrotypesize(char macro) {
  switch(macro) {
    CHAR: return sizeof(char);
    SIGNED_CHAR: return sizeof(signed char);
    UNSIGNED_CHAR: return sizeof(unsigned char);
    SHORT: return sizeof(short);
    SIGNED_SHORT: return sizeof(signed short);
    UNSIGNED_SHORT: return sizeof(unsigned short);
    INT: return sizeof(int);
    SIGNED_INT: return sizeof(signed in);
    UNSIGNED_INT: return sizeof(unsigned int);
    LONG: return sizeof(long);
    SIGNED_LONG: return sizeof(signed long);
    UNSIGNED_LONG: return sizeof(unsigned long);
    LONG_INT: return sizeof(long int);
    SIGNED_LONG_INT: return sizeof(signed long int);
    UNSIGNED_LONG_INT: return sizeof(unsigned long int);
    LONG_LONG_INT: return sizeof(long long int);
    SIGNED_LONG_LONG_INT: return sizeof(signed long long int);
    UNSIGNED_LONG_LONG_INT: return sizeof(unsigned long long int);
    FLOAT: return sizeof(float);
    DOUBLE: return sizeof(double);
    LONG_DOUBLE: return sizeof(long double);
    CHAR_PTR: return sizeof(char*);
    VOID_PTR: return sizeof(void*);
  }
}
