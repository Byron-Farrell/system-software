#include <sys/stat.h>

void lock_file(char *path)
{
  chmod(path, 000);
}

void unlock_file(char *path)
{
  chmod(path, 777);
}
