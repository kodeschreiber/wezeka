Uasge: mkblk [-i size|-p size] [-f format] [-b block-size] output-file

Sizing:
  You can denote the size bytes
  Using the `-i` flag means that the size should be applied to the output image
  Using the `-p` flag means that the size should be applied to the inner partition
  The `-i` and `-p` flags cannot be used together
  
Formatting:
  You may select a format for the partition within the block. Options are:
  - ext4 (default)
  - ext3
  - ntfs
  - vfat
  (More to come)
  
Block size:
  You can denote the size in bytes. The default is 4194304