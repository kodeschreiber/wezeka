BASH DICT

Usage: dict [optional: BEHAVIOR] dict_file1 ... dict_fileN

BEHAVIORS:

-se, --stubborn-exclusive
  The first value set for the key is permanent; no new keys allowed

-sn, --stubborn-nonexclusive
  The first value set for the key is permanent; new keys allowed
  
-oe, --overwrite-exclusive
  New values for any key override their predecessor; no new keys allowed
  
-on, --overwrite-nonexculsive (default)
  New values for any key override their predecessor; new keys allowed

FORMATTING:

The format of a dictionary file must be:

key=value

where the key:
  - Starts from the beginning of the line
  - Matches the regex [a-zA-Z0-9_]*
  - Has no space between it and the equals sign

and the value:
  - Has no space between it and the equals sign
  - Ends at the end-of-line
