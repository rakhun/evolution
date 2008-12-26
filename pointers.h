/*
 * Evolution - An experiment with evolving scripts through natural selection
 * Copyright (C) 2008 Henrik Nilsson
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.
 */
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
