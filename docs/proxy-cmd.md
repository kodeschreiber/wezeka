Usage: ${0} COMMAND ARG1 ... ARGN

Commands:
$(echo "$PATH" | sed 's/:/\n/g' | while read dir; do find "$dir" | grep '${base}-' | sed 's@.*/'${base}'-\(.*\)@  \1@'; done)
