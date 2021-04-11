# INSTALLATION
Installing the program is simple:

```
make ARCH=<arch>
make PREFIX=<your_build_prefix> install
```

Alternatively, if you are cross-compiling:

```
make ARCH=<arch> CC=<cross_prefix>-gcc
make PREFIX=<your_build_prefix> install
```

The `ARCH` flag is used to determine the arch
of Busybox to download for your package.
Valid architectures for `ARCH` are as follows:
 - armv5l
 - armv7l
 - armv7m
 - armv7r
 - armv8l
 - i486
 - i686
 - microblaze
 - mips
 - mips64
 - mipsel
 - powerpc
 - powerpc64
 - s390x
 - sh2eb
 - sh4
 - x86_64

NOTE:
Busybox is dowloaded in an unmodified state (as a binary)
straight from thier website. You can find more information
and source code from their site at: https://busybox.net/
