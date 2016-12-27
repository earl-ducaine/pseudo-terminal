#define _XOPEN_SOURCE 800
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#define __USE_MISC
#include <termios.h>

int main(void) {
  int fdm,
    fds,
    rc;
  char input[150];
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
  while (1) {
    // Operator's entry (standard input = terminal)
    write(1, "Input : ", sizeof("Input : "));
    rc = read(0, input, sizeof(input));
    if (rc > 0) {
      // Send the input to the child process through the PTY
      write(fdm, input, rc);
      // Get the child's answer through the PTY
      rc = read(fdm, input, sizeof(input) - 1);
      if (rc > 0) {
	// Make the answer NUL terminated to display it as a string
	input[rc] = '\0';
	fprintf(stderr, "%s", input);
      } else {
	break;
      }
    } else {
      break;
    }
  }
  return 1;
}
