IFIO (I'll Figure It Out)

Using a directory of templated scripts, attempts to run
each in a target directory

Usage: ifio [OPTIONS [ARG]]

OPTIONS:
  -c,--chdir <directory>
    Change to directory before building. Default is the current working directory

  -m,--mnemonic <file>
    Use this file to remember sucessful directory/build combinations

  -v,--verbose
    Allow all running scripts to output (stdout/stderr). Default is not verbose

  -q,--quiet
    Do not print IFIO messages. Default is not quiet

  -d,--dump <script>
    Print a list of the importatint environment variables defined in specified script

VARIABLES:

IFIO_SCRIPT_DIR
  Set to the directory containing your IFIO scripts

IFIO SCRIPTS:

Scripts should be writen in a way that they present user-accessible
variables easily, perform ealry tests to fail as quickly as possbile
(to allow finding the right 'fit' for the job quickly), and perform
a highly redundant task using a single script

IFIO Scripts must adhere to a few rules:
 - They must written a (POSIX compliant) SHELL language.
 - You must also use Linux magic (e.x. '#!/bin/sh') at the top
 - The 'set -e' flag must be set in the beginning
   - OR -
   Proper messures are taken to fail the script when possible
 - All variables used in the script that should be accessible
   to the end user must be prefaced with "$SOV" (immediately
   after the 'set -e') and follow with '$EOV'. This is done to
   combine documentation and functionality for the
   script creator/user and allow the '--dump' flag to present
   these variables to the end user for customization
 - They must be self executable (have their own execution bit set)

Here is an example script:

#!/bin/bash

set -e

$SOV
CC="gcc"
CFLAGS="${CFLAGS}"
$EOV

test -f ./configure

./configure
make CC="${CC}" CFLAGS="${CFLAGS}"
make install
