#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  // if(strlen(p) >= DIRSIZ)
  //  return p;
  //memmove(buf, p, strlen(p));
  // memcpy(buf, p, strlen(p));
  // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return p;
}

void 
find(char *path, char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      return;
    }

  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    if(st.type == T_DIR && (strcmp(fmtname(buf), ".") != 0) && (strcmp(fmtname(buf), "..") != 0)){
      // printf("Directory %s isn't `.` or `..`\n", fmtname(buf));
      // printf("Go to directory %s\n", buf);
      find(buf, name);
    }
    if (st.type == T_FILE){
      // printf("Check if %s equal to %s\n", fmtname(buf), name);
      if (!(strcmp(fmtname(buf), name)))    
        printf("%s\n", buf);
    }
    }

}

int
main (char argc, char *argv[])
{
  if(argc < 3) {
    printf("Usage: find directory filename\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}