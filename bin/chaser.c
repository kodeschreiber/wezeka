#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  struct winsize w;

  if(argc < 1)
    exit(EXIT_FAILURE);

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  while ((read = getline(&line, &len, stdin)) != -1) {
    for(int c = 0; c < w.ws_col; c++)
      printf(" ");
    printf("\r%s%s\r", line, argv[1]);
    fflush(stdout);
  }

  printf("\n");
  if (line)
    free(line);
  exit(EXIT_SUCCESS);
}
