#define _XOPEN_SOURCE 800
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#define __USE_MISC
#include <termios.h>



char** write_to_lisp (char* input[]) {
}


int initialize_lib(void) {
  int fdm,
    fds,
    rc;
  fdm = posix_openpt(O_RDWR);
  if (fdm < 0) {
    fprintf(stderr, "Error %d on posix_openpt()\n", errno);
    return 1;
  }
  printf("Terminal can be found: %s\n", ptsname(fdm));
  rc = grantpt(fdm);
  if (rc != 0) {
    fprintf(stderr, "Error %d on grantpt()\n", errno);
    return 1;
  }
  rc = unlockpt(fdm);
  if (rc != 0) {
    fprintf(stderr, "Error %d on unlockpt()\n", errno);
    return 1;
  }
  return 0;
}

int pump_input (int fdm) {
  char** output;
  char input[150];
// Operator's entry (standard input = terminal)
  write(1, "Input : ", sizeof("Input : "));
  int rc = read(fdm, input, sizeof(input) - 1);
  if (rc > 0) {
    // Send the input to lisp
    output = write_to_lisp(input, rc);
    write(fdm, output, 100);
    // Get the child's answer through the PTY
  }
  return 0;
}
