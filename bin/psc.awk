!/^\s*#func|^\s*#hfunc|^\s*#sfunc|^\s*#hsfunc|^\s*#file|^\s*#usage/ {
  print $0
}

/^\s*#func .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, file[1] "(){")
  print $1
  while(getline line<cmdarr[2]){print line}
  close(cmdarr[2])
  print "}"
}

/^\s*#hfunc .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, "_" file[1] "(){")
  print $1
  while(getline line<cmdarr[2]){print line}
  close(cmdarr[2])
  print "}"
}

/^\s*#sfunc .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, file[1] "()(")
  print $1
  while(getline line<cmdarr[2]){print line}
  close(cmdarr[2])
  print ")"
}

/^\s*#hsfunc .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, "_" file[1] "()(")
  print $1
  while(getline line<cmdarr[2]){print line}
  close(cmdarr[2])
  print ")"
}

/^\s*#file .*/ {
  split($0, cmdarr, " ")
  while(getline line<cmdarr[2]){print line}
  close(cmdarr[2])
}

/^\s*#usage .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  FS="@"
  gsub($0, "_usage(){\ncat <<'EOF'")
  print $1
  FS=" "
  while(getline line<cmdarr[2]){print line}
  close(cmdarr[2])
  print "EOF\n}"
}
