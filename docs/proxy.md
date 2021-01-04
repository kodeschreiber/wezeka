- PROXY -

Usage:
  Proxy is designed to call subcommands similarly to GIT.
  To use Proxy, simply symlink to this command, located
  at $(which proxy), with a name for your custom command.
  For example, if you create the symlink:

    /usr/bin/example -> $(which proxy)

  Your sub commands would be prefixed 'example-' and be
  located somewhere in your path. To call these commands,
  you would do 'example <command> <arg1> ... <argN>'
