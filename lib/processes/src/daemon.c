#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>



void daemonize(void)
{
  pid_t pid = fork();


  if (pid < 0) {
    printf("Error: Failed to create child process. Received error code: %d.\n", pid);
    exit(EXIT_FAILURE);
  }
  else if (pid > 0) {
    // Closing parent process
    exit(EXIT_SUCCESS);
  }

  // Child process
  // --------------------------------------
  if (setsid() < 0)
  {
    printf("Something when wrong when trying to create session!\n");
    exit(EXIT_FAILURE);
  }


  FILE *fp = fopen("/tmp/system-software-pid.txt", "w");
  fprintf(fp, "%d", getpid());
  fclose(fp);

  umask(0);

  if (chdir("/") < 0)
  {
    printf("Something went wrong when trying to change to root directory\n");
    exit(EXIT_FAILURE);
  }

  // Closing all open file descripters
  for (int i = sysconf(_SC_OPEN_MAX); i >= 0;  i--)
  {
    close(i);
  }
}
