#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;    //文件名
  size_t size;  //文件大小
  size_t disk_offset; //文件在ramdisk中的偏移
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);

char buf[4096];

//打开文件并返回对应的文件描述符
int fs_open(const char *pathname, int flags, int mode) {
  //如何找到pathname? 在file_table中找
  //忽略权限位 flag
  //使用ramdisk_read 进行真正的读取
  //偏移量不要越界
  
  // Finfo * ff;
  bool find = false;
  int sum = sizeof(file_table)/sizeof(file_table[0]);
  //printf("%d\n",sum);
  //to fix : printf can not use %sum
  int i = 0;
  for(i = 0;i < sum;i++)
  printf("%s\n",file_table[i].name);
  printf("open in\n");
  printf("open %s\n",pathname);

  for(int i=0;i<sum;i++) {
    printf("%s\n",file_table[i].name);

    if(strcmp(pathname,file_table[i].name)==0) {
      // ff->disk_offset = file_table[i].disk_offset;
      // ff->size = file_table[i].size;
      find = true;
      break;
    }
  }
  assert(find==true);
  // memset(buf,0,4096);
  // int len = ramdisk_read(buf,ff->disk_offset,ff->size);
  if(i==sum) return -1;
  //返回文件描述符
  return i;
}

//从文件描述符中进行读取
size_t fs_read(int fd, void *buf, size_t len) {
  Finfo * ff = NULL;
  ff->disk_offset = file_table[fd].disk_offset;
  ff->size = file_table[fd].size;
  return ramdisk_read(buf,ff->disk_offset,ff->size);
}


int fs_close(int fd) {
  return 0;
}

/* write `len' bytes starting from `buf' into the `offset' of ramdisk */
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
// buf -> fd
size_t fs_write(int fd, const void *buf, size_t len) {
  Finfo * ff = NULL;
  ff->disk_offset = file_table[fd].disk_offset;
  ff->size = file_table[fd].size;
  size_t ret = ramdisk_write(buf, ff->disk_offset, len);

  return ret;
}

//根据whence修改offset
size_t fs_lseek(int fd, size_t offset, int whence) {
  Finfo * ff = NULL;
  ff->disk_offset = file_table[fd].disk_offset;
  ff->size = file_table[fd].size;
  switch(whence) {
    case SEEK_SET: ff->disk_offset = offset;break;
    case SEEK_CUR: ff->disk_offset +=offset;break;
    case SEEK_END: ff->disk_offset = ff->disk_offset + ff->size + offset;break;
    default: return -1;
  }
  return ff->disk_offset;
}