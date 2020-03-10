main = lib/core/src/main.c
core = lib/core/src/service.c
processes = lib/processes/src/show_logs.c lib/processes/src/daemon.c lib/processes/src/backup.c lib/processes/src/backup_and_transfer.c lib/processes/src/transfer.c
utils = lib/utils/src/file_lock.c lib/utils/src/create_queue.c

manager:
	gcc -Wall $(main) $(core) $(processes) $(utils) -o bin/$@ -lrt
