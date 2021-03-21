#!/bin/bash

_proxy_options() {
  echo "$PATH" | \
  sed 's/:/\n/g' | \
  while read dir; do
    find "$dir" | \
      grep "${base}-" | \
      sed 's@.*/'${base}'-\(.*\)@\1@' | \
      grep -v "${base}-" | \
      while read entry; do
        printf "${entry} "
      done
  done
}

for path in $(echo $PATH | sed 's/:/ /g'); do
  while read link; do
    base="$(basename "${link}")"
    complete -W "$(_proxy_options $base)" $base
  done < <(find -L $path -samefile %prefix%/bin/wzk-proxy 2>/dev/null)
done
