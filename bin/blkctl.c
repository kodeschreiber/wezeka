#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <linux/loop.h>

#define EXT4_INODE_SIZE 256.0
#define EXT4_INODE_RATIO 4096.0
#define EXT4_RESERVED 0.05

enum {
  OK = 0,
  ERROR_BAD_ARGCOUNT,
  ERROR_LOOP_CONTROL,
  ERROR_IMAGE_OPEN,
  ERROR_LOOP_DEVICE,
  ERROR_IMAGE_LOOP,
  ERROR_LOOP_MOUNT,
  ERROR_LOOP_UMOUNT,
  ERROR_UNLOOP,
  ERROR_FREE_LOOP,
  ERROR_NO_LOOP
};

// Returns loop number
int get_loop(char* dire) {
  FILE* fp = fopen("/proc/mounts", "r");
  char* line = NULL;
  size_t len = 0;
  char capture[256];
  int res = -1;

  while(getline(&line, &len, fp) != -1) {
    int idx = -1, idxend;
    while(line[++idx] != ' ' && idx < strlen(line)-1);
    idxend = idx+1;
    while(line[++idxend] != ' ' && idxend < strlen(line)-1);

    snprintf(capture, strlen("/dev/loop") + 1, "%s", line);
    if(strcmp(capture, "/dev/loop") == 0) {
      snprintf(capture, idxend-idx, "%s", (line+idx+1));
      if(strcmp(capture, dire) == 0) {
        snprintf(capture, idx-8, "%s", (line+9));
        res = atoi(capture);
      }
    }
  }

  fclose(fp);
  free(line);
  return res;
}

int mount_image(char* imgpath, char* dire, int ro) {
  int ctlfd, imgfd, devfd, loopid;
  char loopdev[12];

  if((ctlfd = open("/dev/loop-control", O_RDWR)) < 0)
    return ERROR_LOOP_CONTROL;

  loopid = ioctl(ctlfd, LOOP_CTL_GET_FREE);
  sprintf(loopdev, "/dev/loop%d", loopid);
  close(ctlfd);

  if((imgfd = open(imgpath, O_RDWR)) < 0)
    return ERROR_IMAGE_OPEN;

  if((devfd = open(loopdev, O_RDWR)) < 0) {
    close(imgfd);
    return ERROR_LOOP_DEVICE;
  }

  if(ioctl(devfd, LOOP_SET_FD, imgfd) < 0) {
    close(imgfd);
    close(devfd);
    return ERROR_IMAGE_LOOP;
  }

  close(imgfd);

  if(mount(loopdev, dire, "ext4", ro, "") < 0) {
    ioctl(devfd, LOOP_CLR_FD, 0);
    close(devfd);
    return ERROR_LOOP_MOUNT;
  }

  // ioctl(devfd, LOOP_CLR_FD, 0);
  close(devfd);
}

int umount_image(char* dire) {
  int devfd, ctlfd;
  char loopdev[16];
  char realdire[256];
  realpath(dire, realdire);
  int loopid;
  if((loopid=get_loop(realdire)) < 0)
    return ERROR_NO_LOOP;

  snprintf(loopdev, 15, "/dev/loop%i", loopid);
  devfd = open(loopdev, O_RDWR);

  if((ctlfd = open("/dev/loop-control", O_RDWR)) < 0)
    return ERROR_LOOP_CONTROL;

  if(umount(realdire) < 0)
    return ERROR_LOOP_UMOUNT;

  if(ioctl(devfd, LOOP_CLR_FD, 0) < 0)
    return ERROR_UNLOOP;

  if(ioctl(ctlfd, LOOP_CTL_REMOVE, loopid) < 0)
    return ERROR_FREE_LOOP;
}

int make_image(size_t image_size, char* file) {
  int block_size = 4194304;
  int iter = image_size / block_size;
  int zerofd = open("/dev/zero", O_RDONLY);
  int imgfd = open(file, O_RDWR|O_CREAT);
  char* buff = (char*)malloc(sizeof(char) * block_size);
  char cmd[267];
  size_t count;

  for(int c = 0; c < iter; c++) {
    read(zerofd, buff, count);
    write(imgfd, buff, block_size);
  }

  close(zerofd);
  close(imgfd);

  int nfd = open("/dev/null", O_RDWR);
  chmod(file, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  snprintf(cmd, 267, "mkfs.ext4 %s", file);
  dup2(nfd, 1);
  dup2(nfd, 2);
  FILE* proc = popen(cmd, "w");
  fprintf(proc, "y\n");
  pclose(proc);
  close(nfd);
}

int main(int argc, char* argv[]) {
  int ro = 0;
  // Using the pointer adjustment accounts for prefixes i.e. wzk-mkblk will match mkblk
  if(strcmp((argv[0]+(strlen(argv[0])-6)+1), "mkblk") == 0) {
    if(argc != 4) {
      fprintf(stderr, "Usage: mkblk filename <i|p> size\n");
      exit(ERROR_BAD_ARGCOUNT);
    }
    int image_size = atoi(argv[3]);
    int part_size = image_size;

    if(argv[2][0] == 'p')
      image_size = (int)(part_size+(part_size - (part_size * (1.0-((EXT4_INODE_SIZE/EXT4_INODE_RATIO)+EXT4_RESERVED)))));

    exit(make_image(image_size, argv[1]));

  } else if(strcmp((argv[0]+(strlen(argv[0])-8)+1), "umntblk") == 0) {
    if(argc != 2) {
      fprintf(stderr, "Usage: umntblk mount_directory\n");
      exit(ERROR_BAD_ARGCOUNT);
    }
    exit(umount_image(argv[1]));
  } else if(strcmp((argv[0]+(strlen(argv[0])-7)+1), "mntblk") == 0) {
    if(argc > 4 || argc < 3) {
      fprintf(stderr, "Usage: mntblk [--ro] image_path mount_directory\n");
      exit(ERROR_BAD_ARGCOUNT);
    }
    if(strcmp(argv[1], "--ro") == 0) {
      ro = MS_RDONLY;
      argv++;  // Shift args
    }
    exit(mount_image(argv[1], argv[2], ro));
  } else if(strcmp((argv[0]+(strlen(argv[0])-6)), "blkctl") == 0) {
    printf("This command is indended to be ran by specialy-named symlinks\n");
    printf("which specfiy the desired function. Command options are:\n");
    printf("mkblk - create an EXT4 image file\n");
    printf("mntblk - mount an EXT4 image file to a directory\n");
    printf("umntblk - unmount an EXT4 image file from a directory\n");
  } else {
    fprintf(stderr, "Unknown command: %s\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}
