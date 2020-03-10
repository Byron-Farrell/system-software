#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>

#include "../../core/includes/config.h"

void transfer(char *local_site, char *live_site)
{
  size_t size = sizeof(*local_site) + sizeof(".");
  char *source = malloc(size);

  if (strcpy(source, local_site) < 0)
  {
    syslog(LOG_ERR, "Error: failed copy local_site to source in file transfer.c");
    exit(EXIT_FAILURE);
  }

  if (strcat(source, ".\0") < 0)
  {
    syslog(LOG_ERR, "Error: failed copy . to source in file transfer.c");
    exit(EXIT_FAILURE);
  }

  if(execlp("cp", "cp", "-r", source, live_site, NULL) < 0)  {
    syslog(LOG_ERR, "Failed to transfer");
  }
}
