 - PRE-SHELL COMPILER -

Usage: psc [shell_script] [output_file]

Options:
  Both the input and output targets may be undefined,
   defaulting to stdin and stout, repectively

Syntax:
  All options are prefixed with a pound sign (#):

  #func filepath
  - Pulls in shell script as a function.

  #hfunc filepath
  - Pulls in shell script as a hidden function
    (prefixed with an underscore)

  #sfunc filepath
  - Pulls in shell script as a subshell function
    (using parenthesis instead of curly braces)

  #hsfunc filepath
  - Pulls in shell script as a hidden subshell function
    (using parenthesis instead of curly braces; underscore prefix)

  #file filepath
  - Inserts contents of file over this reference

  #usage filepath
  - Creates a function called '_usage' that will print
    the scripts usage information to stderr
