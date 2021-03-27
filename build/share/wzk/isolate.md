ISOLATE

Create an overlayed envionment and execute in a
chroot-like environment

Usage: isolate [OPTION [ARG]] CMD ARG1 ... ARGN

OPTIONS:

-r, --rootfs <dir>
  Add a folder (rootfs) to be merged readonly in
  the isloated environment

-b, --bind <dir>
  Add a folder to bind to the rootfs in the
  isolated environment

-d, --diff <dir>
  Directory for all changes made in isolation to
  be saved to

--dev
  Bind the /dev directory

--proc
  Bind the /proc directory

-c, --chdir
  Specify the working directory in isolation
