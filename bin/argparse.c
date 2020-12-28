#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Current option input format:
  option=count  e.x. test=2

Future option input format:
  option [arg1 ... argN] Description
*/

// #ifndef USAGE_LINK
// #define USAGE_LINK "usage"
// #endif

#ifndef OPT_MAX
#define OPT_MAX 128
#endif

// This size includes the space between args
#ifndef OPT_LEN_MAX
#define OPT_LEN_MAX 256
#endif

#ifndef POS_ARG_COUNT_MAX
#define POS_ARG_COUNT_MAX 64
#endif

enum {
  OK = 0,
  INVALID_OPTION
};

struct option {
  char* variable;
  char* shorthand;
  char* longhand;
  char* values;
  int count;
  int argin;
};
typedef struct option option;

int gen_opt(char* input, char* uniq, option* output) {
  int index = -1;
  while(input[++index] != '=');
  if(index >= strlen(input))
    return INVALID_OPTION;

  output->variable = (char*)malloc(sizeof(char) * index + 1);
  output->longhand = (char*)malloc(sizeof(char) * index + 3);

  if(uniq != NULL) {
    if(!uniq[input[0]]) {
      output->shorthand = (char*)malloc(sizeof(char) * 3);
      snprintf(output->shorthand, index+1, "-%c", input[0]);
      uniq[input[0]] = 1;
    } else {
      output->shorthand = NULL;
    }
  } else {
    output->shorthand = NULL;
  }

  snprintf(output->variable, index+1, "%s", input);
  snprintf(output->longhand, index+3, "--%s", input);

  char number[4];
  snprintf(number, 3, "%s", (input+index+1));
  output->count = atoi(number);
  output->values = (char*)malloc(sizeof(char) * OPT_LEN_MAX * output->count + 1);
}

int append_val(char* value, option* opt) {
  size_t savelen = sizeof(char) * opt->argin * OPT_LEN_MAX + 1;
  char* save = (char*)malloc(savelen);
  snprintf(save, savelen, "%s", opt->values);
  snprintf(opt->values, savelen+OPT_LEN_MAX, "%s%s ", save, value);
  free(save);
  opt->argin++;
}

int free_opt(option* input) {
  free(input->variable);
  free(input->longhand);
  free(input->values);
  if(input->shorthand);
    free(input->shorthand);
  return OK;
}

int output_opt(option* input) {
  input->values[strlen(input->values)-1] = '\0';
  printf("%s=\"%s\"\n", input->variable, input->values);
}

int print_usage(option opts[], size_t optc) {
  fprintf(stderr, "Usage:\n");
  for(int c = 0; c < optc; c++)
    if(opts[c].shorthand == NULL)
      fprintf(stderr, "      %s\tTakes %i following arguments\n", opts[c].longhand, opts[c].count);
    else
      fprintf(stderr, "  %s, %s\tTakes %i following arguments\n", opts[c].shorthand, opts[c].longhand, opts[c].count);
}

int main(int argc, char* argv[]) {
  char * line = NULL;
  size_t len = 0;
  size_t optc = 0;
  int found = 0;
  int res = 0;
  option opts[OPT_MAX];
  char* uniq = (char*)calloc(256, sizeof(char));

  option posargs;
  char* pa_string = (char*)malloc(sizeof(char) * 12);
  snprintf(pa_string, 12, "posargs=%i", POS_ARG_COUNT_MAX);
  gen_opt(pa_string, NULL, &posargs);
  free(pa_string);

  while(getline(&line, &len, stdin) != -1)
    if(!gen_opt(line, uniq, &opts[optc++])) {
      fprintf(stderr, "Unable to generate an option for %s\n", line);
      exit(INVALID_OPTION);
    }

  for(int c = 1; c < argc; c++) {
    if(argv[c][0] == '-') {    // Is a flag
      found = 0;
      for(int d = 0; d < optc; d++) {
        if(argv[c][1] == '-')  // Is a verbose flag
          res = strcmp(argv[c], opts[d].longhand);
        else                   // Is a shorthand flag
          res = strcmp(argv[c], opts[d].shorthand);

        if(res == 0) {
          if(opts[d].count < 1) {
            opts[d].values = "1";
            found = 1;
            continue;
          }
          for(int e = 0; e < opts[d].count && c < argc; e++) {
            append_val(argv[++c], &opts[d]);
            found = 1;
          }
          // c++;
        }
      }
      if(!found) {
        fprintf(stderr, "Could not locate option: %s\n", argv[c]);
        print_usage(opts, optc);
        free(uniq);
        for(int f = 0; f < optc; f++)
          free_opt(&opts[f]);
        if(line)
          free(line);
        exit(EXIT_FAILURE);
      }
    } else {  // Is a positional argument
      append_val(argv[c], &posargs);
    }
  }

  for(int c = 0; c < optc; c++)
    output_opt(&opts[c]);
  output_opt(&posargs);

  free(uniq);
  free_opt(opts);
  if(line)
    free(line);

  exit(EXIT_SUCCESS);
}
