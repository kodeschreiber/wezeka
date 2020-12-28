# DICTIONARY
---
Dictionary is an environment filter made to help control
the sourcing of environment variables in an expected
behavior
---
Usage: dict <filter> <envfile> [delim]

##Filters:
Filters are defined by the behavior when attempting to set,
set and unset variables:

| Filter      | set     | unset | flag |
|======================================|
| overwriting | sets    | sets  | -o   |
| stubborn    |  X      | sets  | -s   |
| exclusive   | sets    |  X    | -e   |
| unwritable  |  X      |  X    |      |
| appending   | appends | sets  | -a   |

The resulting usage can be any of the following:
dict -o
dict -s
dict -e
dict -a <delim>
(Where <delim> is the separator between each appendage;
  default is a space)

The comparison of what is set and not set is based on
the variables that exist in the parent shell's environment
and the values from the envfile (or stdin if not defined)
