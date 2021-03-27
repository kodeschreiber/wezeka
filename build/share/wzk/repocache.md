REPOCACHE

Create an intermediate (local) git repository host

Usage: repocache [ACTION] <URL> [OPTS...]

VARIABLES:

RC_DIR
  Set to the directory that you wish to use for caching

ACTION:

fetch
  Pull down a repository into the cache

clone
  Clone from local cache. Treat just like the "git clone" command (including extra flags)

remove
  Remove an entry from the cache
