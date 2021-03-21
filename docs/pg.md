PG - Process Grid

Create a TMUX session designed to break up STDOUT, STDIN and milestones
into different panes

Usage: pg <workdir> <output_dir> <session_name>

OPTIONS:

<workdir>
  Working directory for editing

<output_dir>
  This directory will be populated with the files 'stdout' and 'stderr'
  along with the pipe 'progress'

<session_name>
  Name for tmux session
