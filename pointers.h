#include <vector>

/// Structure to hold a pointer and its respective name
struct pointerInfo {
  /// The pointer
  void* pointer;
  /// The pointer's name
  const char* name;
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
};
