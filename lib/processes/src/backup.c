#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <stdio.h>


void backup(char *source, char *destination)
{
  syslog(LOG_DEBUG, "starting backup");
  char *file_name = " --- backup.zip\0";
  time_t t;
  struct tm * now;
  time(&t);
  now = localtime(&t);

  char *timestamp = asctime(now);
  size_t backup_path_size = sizeof(destination) + sizeof(timestamp) + sizeof(file_name) + sizeof("\"\"");

  char *backup_path = malloc(backup_path_size);

  if (backup_path == NULL)
  {
    syslog(LOG_ERR, "Error: failed to allocate space for variable backup_path in file backup.c");
    exit(EXIT_FAILURE);
  }

  if (strcpy(backup_path, destination) < 0)
  {
    syslog(LOG_ERR, "Error: failed copy backup_path to destination in file backup.c");
    exit(EXIT_FAILURE);
  }

  if (strcat(backup_path, "\"") < 0)
  {
    syslog(LOG_ERR, "Error: failed copy backup_path to \" in file backup.c");
    exit(EXIT_FAILURE);
  }

  if (strcat(backup_path, timestamp) < 0)
  {
    syslog(LOG_ERR, "Error: failed to concatinate backup_path and timestamp in file backup.c");
    exit(EXIT_FAILURE);
  }

  if (strcat(backup_path, file_name) < 0)
  {
    syslog(LOG_ERR, "Error: failed concatinate backup_path and file_name in file backup.c");
    exit(EXIT_FAILURE);
  }

  if (strcat(backup_path, "\"") < 0)
  {
    syslog(LOG_ERR, "Error: failed copy backup_path to \" in file backup.c");
    exit(EXIT_FAILURE);
  }

  syslog(LOG_DEBUG, "executing \"zip -r backup_path source\" in backup.c using execlp()");
  execlp("/usr/bin/zip", "zip", "-r", backup_path, source, NULL);
}
