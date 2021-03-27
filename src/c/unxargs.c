#include <stdio.h>

int main(int argc, char* argv[]) {
  for(int c = 1; c < argc; c++)
    printf("%s\n", argv[c]);
}
