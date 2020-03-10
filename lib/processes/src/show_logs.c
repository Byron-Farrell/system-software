#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <sys/wait.h>


#include "../../core/includes/config.h"

void cat_exec(void);
void grep_exec(void);
void clean(void);

// fd[0] read
// fd[1] write
int fd[2];

void show_logs(void)
{
  openlog(LOG_ID, LOG_PID|LOG_CONS, LOG_DAEMON);
  if (pipe(fd) < 0)
  {
    syslog(LOG_ERR, "pipe failed.");
    closelog();
    exit(EXIT_FAILURE);
  }

  pid_t exec1_pid = fork();

  if (exec1_pid < 0)
  {
    syslog(LOG_ERR, "Error: failed to fork() in show_logs in file main.c");
  }

  // child process
  if (exec1_pid == 0)
  {
    cat_exec();
  }

  pid_t exec2_pid = fork();

  if (exec2_pid < 0)
  {
    syslog(LOG_ERR, "Error: failed to fork() in show_logs in file main.c");
  }

  // child process
  if (exec2_pid == 0)
  {
    printf("5");
    grep_exec();
  }

  sleep(1);
  closelog();
}

void cat_exec(void)
{
  dup2(fd[1], 1);

  close(fd[0]);
  close(fd[1]);
  closelog();
  execlp("cat", "cat", LOG_PATH, NULL);
}

void grep_exec(void)
{
  dup2(fd[0], 0);

  close(fd[0]);
  close(fd[1]);
  closelog();
  execlp("grep", "grep", LOG_ID, NULL);
}
