
#ifndef DICT_H

#include <stdlib.h>
#include <stdint.h>
#define DICT_SIZE_TYPE short

#define DICT_H
typedef unsigned DICT_SIZE_TYPE dsize_t;

struct __attribute__ ((__packed__)) kvp {
  void* key;
  void* value;
  char keytype;
  char valuetype;
};

// Cascading-Hashmap Dictionary
struct dictionary {
  struct kvp* first;
  void* data;
  uint64_t* order;
  size_t size;
  size_t count;
  uint64_t seed;
};

int create_dict(struct dictionary* dict, size_t size);
int set_dict_typed(struct dictionary* dict, void* key, char keytype, void* value, char valuetype);
int get_dict_typed(struct dictionary* dict, void* key, char keytype, void* value, char valuetype);
int delete_dict(struct dictionary* dict);

#define set_dict(k, v) _Generic((k), \
  default: _Generic((v), \
    default: set_dict_typed))(struct dictionary dict, (void*)k, typeof(k), (void*)v, typeof(v))

#define get_dict(k, v) _Generic((k), \
  default: _Generic((v), \
    default: get_dict_typed))(struct dictionary dict, (void*)k, typeof(k), (void*)v, typeof(v))

#endif
