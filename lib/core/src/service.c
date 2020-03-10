#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../../processes/includes/daemon.h"
#include "../../processes/includes/backup_and_transfer.h"
#include "../../processes/includes/backup.h"
#include "../../processes/includes/transfer.h"
#include "../../utils/includes/create_queue.h"
#include "../includes/config.h"


// ---------------------------------------------------------------------------
// Function Prototypes
// ---------------------------------------------------------------------------
void setup(void);
void clean(void);
void handle_message(char *message, short int *terminate);


// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
mqd_t mq;
pid_t backup_and_transfer_pid;

int start_service(void)
{
	// ---------------------------------------------------------------------------
	// Variables
	// ---------------------------------------------------------------------------

	// buffers
	char message_buffer[MESSAGE_BUFFER_SIZE + 1];

	// sentinel for while loop
	short int terminate = 0;


	// ---------------------------------------------------------------------------
	// Setup
	// ---------------------------------------------------------------------------

	setup();

	backup_and_transfer_pid = fork();

	// Child process
	if (backup_and_transfer_pid == 0)
	{
		backup_and_transfer();
	}

	// ---------------------------------------------------------------------------
	// Error Checking
	// ---------------------------------------------------------------------------

	if (backup_and_transfer_pid < 0)
	{
		syslog(LOG_ERR, "fork failed: site_update.");
		clean();
		exit(EXIT_FAILURE);
	}

	// ---------------------------------------------------------------------------
	// Main Loop
	// ---------------------------------------------------------------------------

	syslog(LOG_DEBUG, "Starting service.c main loop.");

	while(!terminate)
	{
		// reading messages from queue
		size_t message_bytes_read = mq_receive(mq, message_buffer, MESSAGE_BUFFER_SIZE, NULL);
		message_buffer[message_bytes_read] = '\0';

		if (message_bytes_read > 0)
		{
			handle_message(message_buffer, &terminate);
		}
	}

	clean();
	exit(EXIT_SUCCESS);
}


void handle_message(char *message, short int *terminate)
{
	if (!strncmp(message, "stop", strlen("stop")))
	{
		*terminate = 1;
		FILE *fp = fopen(PID_TEMP_FILE, "w+");
		fclose(fp);
		syslog(LOG_INFO, "terminating process.");
	}
	if (!strncmp(message, "backup", strlen("backup")))
	{
		syslog(LOG_INFO, "backing up files.");

		size_t pid = fork();

		if (pid < 0)
		{
			syslog(LOG_ERR, "Error: fork() failed for backup() process in service.c");
		}
		// child process
		else if (pid == 0)
		{
			backup(LIVE_SITE, BACKUP_PATH);
			exit(EXIT_SUCCESS);
		}
		// parent process
		else {
			int wstatus;
			waitpid(pid, &wstatus, 0);
			syslog(LOG_DEBUG, "backup process ended.");
		}


	}
	if (!strncmp(message, "transfer", strlen("transfer")))
	{
		syslog(LOG_INFO, "transfering files.");

		size_t pid = fork();

		if (pid < 0)
		{
			syslog(LOG_ERR, "Error: fork() failed for transfer() process in service.c");
		}
		// child process
		else if (pid == 0)
		{
			transfer(INTERNAL_SITE, LIVE_SITE);
			exit(EXIT_SUCCESS);
		}
		// parent process
		else {
			int wstatus;
			waitpid(pid, &wstatus, 0);
			syslog(LOG_DEBUG, "transfer process ended.");
		}

	}
}


void setup(void)
{
	daemonize();
	mq = create_queue(QUEUE_NAME);
	openlog(LOG_ID, LOG_PID|LOG_CONS, LOG_DAEMON);
}


void clean(void)
{
	// Clean up
	kill(backup_and_transfer_pid, SIGKILL);
	mq_close(mq);
	mq_unlink(QUEUE_NAME);
	closelog();
}
