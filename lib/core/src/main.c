#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>

#include "../includes/service.h"
#include "../includes/config.h"
#include "../../processes/includes/show_logs.h"

// ---------------------------------------------------------------------------
// Function Prototypes
// ---------------------------------------------------------------------------
void display_commands(void);
void send_message(char *message);
int process_exist(void);

int main(int argc, char **argv)
{
    if (argc == 1)
    {
      printf("error: no commands found.\n\n");
      display_commands();
    }
    else if (argc == 2)
    {
      if (!strncmp(argv[1], "start", strlen("start")))
      {
        if (process_exist())
        {
          printf("error: process is already running.\n\n");
          display_commands();
        }
        else
        {
          printf("process initiated.\n");
          start_service();
        }
      }
      else if (!strncmp(argv[1], "start-force", strlen("start-force")))
      {
        // forcing start by clearing PID file
        FILE *fp = fopen(PID_TEMP_FILE, "w+");
        fclose(fp);
        printf("process initiated.\n");
        start_service();
      }
      else if (!strncmp(argv[1], "logs", strlen("logs")))
      {
        printf("0");
        show_logs();
      }
      else if (!strncmp(argv[1], "restart", strlen("restart")))
      {
        send_message("stop");
        start_service();
      }
      else if (!strncmp(argv[1], "stop", strlen("stop")) ||
               !strncmp(argv[1], "backup", strlen("backup")) ||
               !strncmp(argv[1], "transfer", strlen("transfer")))
      {
        send_message(argv[1]);
      }
      else if (!strncmp(argv[1], "help", strlen("help")))
      {
        display_commands();
      }
      else
      {
        printf("error: unknown commands.\n\n");
        display_commands();
      }
    }
    else
    {
      printf("error: unknown commands.\n\n");
      display_commands();
    }

    return 0;
}

void display_commands(void)
{
  printf("Commands\n");
  printf("----------------------------------------------------------------------------------------\n");
  printf("start:\t\tStarts the service.\n");
  printf("start-force:\tForce start the service (not recomended).\n");
  printf("stop:\t\tStops the service.\n");
  printf("restart:\tRestarts the service service.\n");
  printf("transfer:\tTransfer all files from interanet to live site.\n");
  printf("backup:\t\tBackup all files being watched by service.\n");
  printf("logs:\t\tView all logs associated with this program.\n");
  printf("help:\t\tShow this menu.\n");

  printf("\n\nNote: All commands should be run with sudo to ensure the processes have root access.\n");

}

void send_message(char *message)
{
  mqd_t mq = mq_open(QUEUE_NAME, O_WRONLY);

  mq_send(mq, message, sizeof(message), 0);

  mq_close(mq);
}

int process_exist(void)
{
  FILE *fp = fopen(PID_TEMP_FILE, "r+");

  if (fp == NULL)
  {
    return 0;
  }

  char buffer[100];

  if (fgets(buffer, sizeof(buffer), fp) == NULL)
  {
    fclose(fp);
    return 0;
  }
  else
  {
    fclose(fp);
    return 1;
  }

}
