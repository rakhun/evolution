#ifndef log
#define log(msg) \
  {event logmsg; \
  logmsg.name="log"; \
  logmsg.strings.push_back(msg); \
  ((eventManager*)pointers::getInstance()->getPointer("eventManager"))->triggerEvent(logmsg);}
#endif
