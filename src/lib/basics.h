#ifndef BASICS_H

#define TRUE 1
#define FALSE 0

#define typesize(x) _Generic((x), default: sizeof(x))

#define BASICS_H
typedef unsigned char bool;

// The attribute helps keep the enum's memory footprint low
enum __attribute__ ((__packed__)) {
  CHAR = 0,      SIGNED_CHAR,          UNSIGNED_CHAR,
  SHORT,         SIGNED_SHORT,         UNSIGNED_SHORT,
  INT,           SIGNED_INT,           UNSIGNED_INT,
  LONG,          SIGNED_LONG,          UNSIGNED_LONG,
  LONG_INT,      SIGNED_LONG_INT,      UNSIGNED_LONG_INT,
  LONG_LONG_INT, SIGNED_LONG_LONG_INT, UNSIGNED_LONG_LONG_INT,
  FLOAT, DOUBLE, LONG_DOUBLE,
  CHAR_PTR,      VOID_PTR
};

#define typeof(x) _Generic((x), \
char: CHAR,                   signed char: SIGNED_CHAR,                   unsigned char: UNSIGNED_CHAR, \
short: SHORT,                 signed short: SIGNED_SHORT,                 unsigned short: UNSIGNED_SHORT, \
int: INT,                     signed int: SIGNED_INT,                     unsigned int: UNSIGNED_INT, \
long: LONG,                   signed long: SIGNED_LONG,                   unsigned long: UNSIGNED_LONG, \
long int: LONG_INT,           signed long int: SIGNED_LONG_INT,           unsigned long int: UNSIGNED_LONG_INT, \
long long int: LONG_LONG_INT, signed long long int: SIGNED_LONG_LONG_INT, unsigned long long int: UNSIGNED_LONG_LONG_INT, \
float: FLOAT, double: DOUBLE, long double: LONG_DOUBLE, \
char*: CHAR_PTR,              void*: VOID_PTR \
)

int macrotypesize(char macro);
#endif
