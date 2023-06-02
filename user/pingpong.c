#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc > 1){
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

  int fd[2];
  // fd[0] -- read, fd[1] -- write
  if (pipe(fd) == -1)
    exit(1);

  int id = fork();
  if (id != 0) {
    char x;
    write(fd[1], &x, sizeof(char));
    read(fd[0], &x, sizeof(char));
    printf("%d: received pong\n", getpid());
  } else {
    char x;
    read(fd[0], &x, sizeof(char));
    printf("%d: received ping\n", getpid());
    write(fd[1], &x, sizeof(char));
  }
  close(fd[0]);
  close(fd[1]);
  exit(0);
}