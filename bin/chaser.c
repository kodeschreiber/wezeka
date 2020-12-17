#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[]) {
  char * line = NULL;
  size_t len = 0;
  struct winsize w;

  if(argc < 1)
    exit(EXIT_FAILURE);

  setvbuf(stdout, NULL, _IONBF, 0);
  ioctl(1, TIOCGWINSZ, &w);  // 1 replacing STDOUT_FILENO from unistd.h

  while(getline(&line, &len, stdin) != -1)
    printf("%*s\r%s%s\r", w.ws_col, "", line, argv[1]);

  printf("%s\n", argv[1]);
  if(line)
    free(line);

  exit(EXIT_SUCCESS);
}
