#ifndef log
#define log \
  {event logmsg; \
  logmsg.name="log"; \
  char* log_msg; \
  asprintf(&log_msg,
#define endlog );\
  logmsg.strings.push_back(log_msg); \
  ((eventManager*)pointers::getInstance()->getPointer("eventManager"))->triggerEvent(logmsg);}
#endif
