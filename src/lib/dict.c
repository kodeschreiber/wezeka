#include "dict.h"
#include "moremath.h"
#include <string.h>
#include <time.h>

int create_dict(struct dictionary* dict, size_t size) {
  dict->size = size;
  dict->count = 0;
  dict->first = NULL;
  dict->order = malloc(sizeof(uint64_t) * size);
  dict->data = calloc(size, sizeof(void));
  dict->seed = (unsigned long)time(NULL);
}

int set_dict_typed(struct dictionary* dict, void* key, char keytype, void* value, char valuetype) {
  int len = 0;
  if(keytype == CHAR_PTR)
    len = strlen((char*)key);
  else
    len = typesize(keytype);
  uint64_t hash = hash(key, macrotypesize(keytype), dict->seed);
}

int get_dict_typed(struct dictionary* dict, void* key, char keytype, void* value, char valuetype) {

}

int delete_dict(struct dictionary* dict) {

}
