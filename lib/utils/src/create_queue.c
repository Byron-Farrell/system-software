#include <mqueue.h>
#include "../../core/includes/config.h"

mqd_t create_queue(char *name)
{
  struct mq_attr queue_attributes;

  queue_attributes.mq_flags = FLAGS;
  queue_attributes.mq_maxmsg = MAX_MESSAGE;
  queue_attributes.mq_msgsize = MESSAGE_BUFFER_SIZE;
  queue_attributes.mq_curmsgs = CURRENT_MESSAGES;

  return mq_open(name, O_CREAT | O_RDONLY, 0644, &queue_attributes);
}
