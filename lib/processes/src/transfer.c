#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdlib.h>

#include "../../core/includes/config.h"

void transfer(char *local_site, char *live_site)
{
  if(execlp("cp", "cp", "-r", INTERNAL_SITE, LIVE_SITE, NULL) < 0)  {
    syslog(LOG_ERR, "Failed to transfer");
  };
}
