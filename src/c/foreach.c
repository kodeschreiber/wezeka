#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef CMD_BUFFER
#define CMD_BUFFER 1024
#endif

#ifndef STDIN_BUFFER
#define STDIN_BUFFER 8192
#endif

#ifndef DELIM
#define DELIM "%s"
#endif

/*
"threading" support? Later
input quoting support? Maybe
multi-refs Done
*/

#define TOTAL_BUFFER (CMD_BUFFER + STDIN_BUFFER)

int substitute(int idx_count, char* result, char* cmd, char* line) {
  int leng = strlen(cmd) + (strlen(line)*idx_count) - (2 * idx_count);
  switch(idx_count) {
    case 0: snprintf(result, leng, cmd, ""); break;
    case 1: snprintf(result, leng, cmd, line); break;
    case 2: snprintf(result, leng, cmd, line, line); break;
    case 3: snprintf(result, leng, cmd, line, line, line); break;
    case 4: snprintf(result, leng, cmd, line, line, line, line); break;
    case 5: snprintf(result, leng, cmd, line, line, line, line, line); break;
    case 6: snprintf(result, leng, cmd, line, line, line, line, line, line); break;
    case 7: snprintf(result, leng, cmd, line, line, line, line, line, line, line); break;
    case 8: snprintf(result, leng, cmd, line, line, line, line, line, line, line, line); break;
    default: printf("%i is an unsupported number of substitutions", idx_count); exit(2);
  }
}

int main(int argc, char** argv) {
  size_t cmd_buffer = CMD_BUFFER;
  size_t stdin_buffer = STDIN_BUFFER;
  size_t total_buffer = TOTAL_BUFFER;
  char* cmdbuff = malloc(CMD_BUFFER);
  char* stdbuff = malloc(STDIN_BUFFER);
  char* combuff = malloc(TOTAL_BUFFER);
  //size_t subcount = 0;
  u_int8_t startpos = 1;
  //size_t threads = 1;
  size_t idx_count = 0;
  //size_t child_count = 0;
  //int wtype = 0;
  //int mutex = 1;

  // Check for foreach-specific args
  //while(argv[startpos][0] == '-') {
    //if(strcmp(argv[startpos++], "-t") == 0) {
    //  threads = atoi(argv[startpos++]);
    //}
    /*else if(strcmp(argv[startpos++], "-q") == 0) {
      quoting = '\'';
    }
    else if(strcmp(argv[startpos++], "-Q") == 0) {
      quoting = '\"';
    }*/
  //}

  // Compose the command
  for(int x = startpos; x < argc; x++) {
    strncat(cmdbuff, argv[x], strlen(argv[x]));
    strncat(cmdbuff, " ", 2);
  }

  //Get Indexes
  for(int d = 0; d < strlen(cmdbuff); d++)
    if(cmdbuff[d] == '%' && cmdbuff[d+1] == 's')
      idx_count++;

  // Iterate the input lines through the command
  while(getline(&stdbuff, &stdin_buffer, stdin) >= 0) {
    //if(child_count < threads) {
    //  if(fork() == 0) {
    //    if(mutex) {
        *(stdbuff + (strlen(stdbuff)-1)) = '\0';
        substitute(idx_count, combuff, cmdbuff, stdbuff);
        system(combuff);
     //   }
     // }
    //  else {
    //    child_count++;
    //  }
    //}
    //else {
    //  wait(NULL);
    //  child_count--;
    //}
  }

  free(cmdbuff);
  free(stdbuff);
  free(combuff);
}
