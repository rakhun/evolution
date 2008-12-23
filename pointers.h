#ifndef EVOLUTION_pointers_dot_h
#define EVOLUTION_pointers_dot_h
#include <vector>

/// Structure to hold a pointer and its respective name
struct pointerInfo {
  /// The pointer
  void* pointer;
  /// The pointer's name
  const char* name;
  /// Whether the pointer is locked, for thread safety
  bool locked;
};

/// A globally available hub for pointers to instances of objects
class pointers {
private:
  static pointers* instance;
  std::vector<pointerInfo> pointerlist;
public:
  /// Retrieves the instanced pointers object or creates one if it does not yet exist
  static pointers* getInstance();
  /// Adds a new pointer to the pointerlist
  void addPointer(void* pointer, const char* name);
  /// Fetches a pointer based on its name in the list
  void* getPointer(const char* name);
  /// Fetches a pointer based on its name in the list and locks it
  void* getPointerLock(const char* name);
  /// Fetches a pointer based on its name in the list and locks it, waits if it's currently locked
  void* getPointerLockWait(const char* name);
  /// Return control of a previously locked pointer
  void unlockPointer(const char* name);
};
#endif
