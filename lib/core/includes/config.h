#ifndef CONFIG_H_
#define CONFIG_H_

// Message queue default attributes
#define FLAGS 0
#define MAX_MESSAGE 10
#define MESSAGE_BUFFER_SIZE 1024
#define CURRENT_MESSAGES 0

// queue name
#define QUEUE_NAME "/system-software-queue"

// Inotify buffer size
#define INOTIFY_BUFFER_SIZE 350

// Log identifier
#define LOG_ID "system-software"

// Site paths
#define LIVE_SITE "/var/www/html/live/"
#define INTERNAL_SITE "/var/www/html/internal/."

#define BACKUP_PATH "/var/lib/system-software/backups/"

#endif
