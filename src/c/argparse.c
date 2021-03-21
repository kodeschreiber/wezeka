#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Current option input format:
  option=count  e.x. test=2

Future option input format:
  option=arg1 ... argN
  option=[arg1] ... [argN]

  Spaces are used to split the arguments
*/

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

#define NONE_MODE 0
#define WORD_MODE 1
#define SPACE_MODE 2

enum {
  OK = 0,
  INVALID_OPTION,
  ERROR_NO_MULTI,
  ERROR_REQUIRED
};

enum {
  REQUIRED = 0,
  MULTI,
  IS_PARGS
};

struct option {
  char* variable;
  char* shorthand;
  char* longhand;
  char* guide;
  char* values;
  int count;
  int argin;
  int required;
  int multi;
  int ispargs;
};
typedef struct option option;

void freeall(void* line[], size_t len) {
  for(int c = 0; c < len; c++)
    if(line[c] != NULL)
      free(line[c]);
}

int gen_opt(char* input, char* uniq, option* output) {
  output->required = -1;
  output->multi = -1;
  output->ispargs = -1;
  output->argin = 0;

  // Look for control characters
  while(input[0] == '!' || input[0] == '+' || input[0] == '?') {
    if(input[0] == '!')
      output->required = REQUIRED;
    else if(input[0] == '+')
      output->multi = MULTI;
    else if(input[0] == '?')
      output->ispargs = IS_PARGS;
    input++;
  }

  // Locate the equals colon, if present
  int index = -1;
  while(input[++index] != ':' && index < strlen(input)-1);

  // Determine the number of arguments
  output->count=0;
  int mode = NONE_MODE, prev_mode = NONE_MODE;
  for(int pt=index+1; pt < strlen(input); pt++) {
    prev_mode = mode;
    if(input[pt] == ' ')
      mode = SPACE_MODE;
    else if(input[pt] == '\n')
      break;
    else
      mode = WORD_MODE;
    if(prev_mode == WORD_MODE && mode == SPACE_MODE)
      output->count++;
  }
  if(mode == SPACE_MODE)
    output->count--;
  output->count++;

  // Allocating some variable
  output->variable = (char*)malloc(sizeof(char) * index + 1);
  output->longhand = (char*)malloc(sizeof(char) * index + 3);

  // If uniq is used
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

  // Compose the strings for the option
  snprintf(output->variable, index+1, "%s", input);
  snprintf(output->longhand, index+3, "--%s", input);

  // Allocate the values and guides
  size_t valsize = sizeof(char) * OPT_LEN_MAX * output->count + 1;
  output->values = (char*)malloc(valsize);
  output->guide = (char*)malloc(valsize);
  snprintf(output->guide, (strlen(input)-index)-1, "%s", (input+index+1));

  return OK;
}

int append_val(char* value, option* opt) {
  size_t savelen = sizeof(char) * opt->argin * OPT_LEN_MAX + 1;
  char* save = (char*)malloc(savelen);  // Used for switching out the string
  snprintf(save, savelen, "%s", opt->values);
  snprintf(opt->values, savelen+OPT_LEN_MAX, "%s%s ", save, value);
  free(save);
  opt->argin++;
}

int free_opt(option* input) {
  free(input->variable);
  free(input->longhand);
  free(input->guide);
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
    if(opts[c].ispargs != IS_PARGS) {
      if(opts[c].shorthand == NULL)
        fprintf(stderr, "      %s\t%s\n", opts[c].longhand, opts[c].guide);
      else
        fprintf(stderr, "  %s, %s\t%s\n", opts[c].shorthand, opts[c].longhand, opts[c].guide);
    }
    // TODO: Add listing for posargs
}

int main(int argc, char* argv[]) {
  char * line = NULL;    // For reading each line of stdin
  size_t len = 0;        // For the length of each line
  size_t optc = 0;       // Keeps track of the number of options
  int found = 0;         // Tracks if input option exists
  int res = 0;           // Used for string comparisons
  option* opts = malloc(sizeof(*opts) * OPT_MAX);  // Array to store all of the options
  char* uniq = (char*)calloc(256, sizeof(char)); // Tracks shorthand uniquity
  int pidx = -1;  // Used to find the option for pos arguments in the array
  char* pa_string = "?+posargs: [ANY]";  // The default posargs line
  void* freeline[8] = { line, opts, uniq };  // Used for easy memory cleanup
  size_t fcount = 3;  // Tracks the number of pointer in the array above

  // Process input options
  while(getline(&line, &len, stdin) != -1) {
    if(gen_opt(line, uniq, &opts[optc]) != OK) {
      fprintf(stderr, "Unable to generate an option for %s\n", line);
      exit(INVALID_OPTION);
    }
    if(opts[optc].ispargs == IS_PARGS)
      pidx = optc;
    optc++;
  }

  // // Add positional argument collector if not defined
  if(pidx == -1) {
    opts[optc].variable = (char*)malloc(sizeof(char) * 8);
    gen_opt(pa_string, NULL, &opts[optc]);
    pidx = optc;
    optc++;
  }

  // Process arguments
  for(int c = 1; c < argc; c++) {
    if(argv[c][0] == '-') {    // Is a flag
      found = 0;
      for(int d = 0; d < optc-1; d++) {
        if(argv[c][1] == '-')  // Is a verbose flag
          res = strcmp(argv[c], opts[d].longhand);
        else                   // Is a shorthand flag
          res = strcmp(argv[c], opts[d].shorthand);

        // If there is a match...
        if(res == 0) {
          // Check if multiple occurances is allowed
          if(strlen(opts[d].values) > 0 && opts[d].multi != MULTI) {
            fprintf(stderr, "Multiple calls to flag '%s' is not permitted\n", opts[d].longhand);
            print_usage(opts, optc);
            free_opt(opts);
            freeall(freeline, fcount);
            exit(ERROR_NO_MULTI);
          }

          if(opts[d].count < 1) {
            opts[d].values = "1";
            found = 1;
            continue;
          }
          for(int e = 0; e < opts[d].count && c < argc; e++) {
            append_val(argv[++c], &opts[d]);
            found = 1;
          }
        }
      }
      if(!found) {  // For when an option does not exist
        fprintf(stderr, "Could not locate option: %s\n", argv[c]);
        print_usage(opts, optc);
        for(int f = 0; f < optc; f++)
          free_opt(&opts[f]);
        freeall(freeline, fcount);
        exit(EXIT_FAILURE);
      }
    } else {  // Is a positional argument
      append_val(argv[c], &opts[pidx]);
    }
  }

  // Check and Print the results
  for(int c = 0; c < optc; c++) {
    if(strlen(opts[c].values) < 1 && opts[c].required == REQUIRED) {
      fprintf(stderr, "Flag '%s' is required, but undefined\n", opts[c].longhand);
      print_usage(opts, optc);
      free_opt(opts);
      freeall(freeline, fcount);
      exit(ERROR_REQUIRED);
    }
    output_opt(&opts[c]);
  }
  // output_opt(&opts[pidx]);

  // Release all the allocations
  free_opt(opts);
  freeall(freeline, fcount);

  exit(EXIT_SUCCESS);
}
