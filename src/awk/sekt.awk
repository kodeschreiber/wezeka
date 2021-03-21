($0~"\\["name"\\]"),($0!~"\\["name"\\]")&&/^\s*\[[a-zA-Z0-9 _-]*\]/ {
  if($0~"\\["name"\\]" || /^\s*\[[a-zA-Z0-9 _-]*\]/) { next; }
  if(norem == "true" && $0~/^\s*#[^\!]/) { next; }
  if(env == "true") {
    if($0~/^\s*#[^\!]/ || $0~/^[ \t]*$/) { next; }
    gsub(/^[[:space:]]*/,"",$0); gsub(/[[:space:]]*$/,"",$0)
    if(safe == "true" && $0!~/=['"]/) {
      split($0, parts, "=")
      $0=parts[1] "='" parts[2] "'"
    }
  }
  if(length(prefix) != 0) { $0=prefix $0; }
  print $0
}
