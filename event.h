#ifndef EVOLUTION_event_dot_h
#define EVOLUTION_event_dot_h
#include <vector>

/// Simple structure to describe an event and any values it may contain
struct event
{
  /// The name to identify the event
  const char* name;
  /// Integer values contained in the event
  std::vector<int> integers;
  /// Floating point values contained in the event
  std::vector<float> floats;
  /// String values contained in the event
  std::vector<const char*> strings;
};
#endif
