#include "event.h"

/// Structure to describe a callback and which events it should be called for
struct eventCallback {
  /// The name of events to trigger upon
  const char* name;
  /// Pointer to the function to call
  bool (*callback)(event);
};

/// Class to handle events in the program
class eventManager {
private:
  std::vector<eventCallback> callbacks;
public:
  /// Register a callback to an event
  void registerEvent(const char* name, bool (*callback)(event));
  /// Trigger an event and call all callbacks registered to it
  bool triggerEvent(event eventobj);
};
