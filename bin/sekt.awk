($0~"\["name"\]"),($0!~"\["name"\]")&&/^\s*\[[a-zA-Z0-9_-]*\]/ {
  if(NR<1 || /^\s*\[[a-zA-Z0-9_-]*\]/) { next; }
  if(norem == "true" && $0~/^\s*#[^\!]/) { next; }
  if(env == "true") {
    if($0~/^\s*#[^\!]/ || $0~/^[ \t]*$/) { next; }
    gsub(/^[ \t]*/,"",$0); gsub(/[ \t]*\$/,"",$0)
    if(safe == "true" && $0!~/=['"]/) {
      split($0, parts, "=")
      $0=parts[1] "='" parts[2] "'"
    }
  }
  if(length(prefix) != 0) { $0=prefix $0; }
  print $0
}
