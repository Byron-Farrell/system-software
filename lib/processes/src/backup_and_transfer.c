#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../../utils/includes/file_lock.h"
#include "../../core/includes/config.h"
#include "../includes/backup.h"

void exec1(void);

void backup_and_transfer(void)
{
  while(1)
  {
    sleep(1);
    time_t epoch = time(NULL);
    struct tm now = *localtime(&epoch);

    if (now.tm_hour == 0 && now.tm_min == 0)
    {
      syslog(LOG_INFO, "Starting back and transfer process");
      exec1();
    }
  }
}

void exec1(void)
{
  int wstatus;
  size_t backup_pid = fork();
  lock_file(LIVE_SITE);

  if (backup_pid < 0)
  {
    syslog(LOG_ERR, "Error in fork: site_update backup");
  }
  else if (backup_pid == 0)
  {
    backup(LIVE_SITE, BACKUP_PATH);
    syslog(LOG_INFO, "backup complete!");
    exit(EXIT_SUCCESS);
  }


  waitpid(backup_pid, &wstatus, 0);

  size_t transfer_pid = fork();

  if (transfer_pid < 0)
  {
    syslog(LOG_ERR, "Error in fork: site_update transer");
  }
  else if (transfer_pid == 0)
  {
    // transfer
    syslog(LOG_INFO, "transfer complete!");
    exit(EXIT_SUCCESS);
  }

  waitpid(transfer_pid, &wstatus, 0);

  sleep(65);
  unlock_file(LIVE_SITE);
}
