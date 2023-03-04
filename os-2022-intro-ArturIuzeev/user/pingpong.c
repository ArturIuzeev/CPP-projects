#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int first[2];
  int second[2];
  int size;

  if (pipe(first) == -1) {
    exit(1);
  }

  if (pipe(second) == -1) {
    exit(1);
  }
  char *data = "ping";
  char data2[6];
  int id = fork();
  if (id == -1) {
    exit(1);
  }
  if (id != 0) {
    size = write(first[1], data, 5);
    if (size != 5) {
      exit(1);
    }
    close(first[1]);
    size = read(second[0], data2, 6);

    if (size < 0 || size >= 6) {
      exit(1);
    }
    printf("%d: got %s\n", getpid(), data2);
    close(second[0]);
    exit(0);
  } else {
    size = read(first[0], data2, 6);
    if (size < 0 || size >= 6) {
      exit(1);
    }
    close(first[0]);
    printf("%d: got %s\n", getpid(), data2);

    data = "pong";
    size = write(second[1], data, 5);
    if (size != 5) {
      exit(1);
    }
    close(second[1]);

    exit(0);
  }
}
