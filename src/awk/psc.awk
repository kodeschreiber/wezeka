!/^\s*#func|^\s*#hfunc|^\s*#sfunc|^\s*#hsfunc|^\s*#file|^\s*#usage/ {
  print $0
}

/^\s*#func .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, file[1] "(){")
  print $1
  infile=ctx "/" cmdarr[2]
  while(getline line<infile){ if(substr(line,0,2) != "#!"){print line}}
  close(infile)
  print "}"
}

/^\s*#hfunc .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, "_" file[1] "(){")
  print $1
  infile=ctx "/" cmdarr[2]
  while(getline line<infile){ if(substr(line,0,2) != "#!"){print line}}
  close(infile)
  print "}"
}

/^\s*#sfunc .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, file[1] "()(")
  print $1
  infile=ctx "/" cmdarr[2]
  while(getline line<infile){ if(substr(line,0,2) != "#!"){print line}}
  close(infile)
  print ")"
}

/^\s*#hsfunc .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  split(nodes[n], file, ".")
  gsub($0, "_" file[1] "()(")
  print $1
  infile=ctx "/" cmdarr[2]
  while(getline line<infile){ if(substr(line,0,2) != "#!"){print line}}
  close(infile)
  print ")"
}

/^\s*#file .*/ {
  split($0, cmdarr, " ")
  infile=ctx "/" cmdarr[2]
  while(getline line<infile){print line}
  close(infile)
}

/^\s*#usage .*/ {
  split($0, cmdarr, " ")
  n=split(cmdarr[2], nodes, "/")
  FS="@"
  gsub($0, "_usage(){\ncat <<'EOF'")
  print $1
  FS=" "
  infile=ctx "/" cmdarr[2]
  while(getline line<infile){print line}
  close(infile)
  print "EOF\n}"
}
