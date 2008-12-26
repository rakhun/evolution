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
#ifndef EVOLUTION_eventmanager_dot_h
#define EVOLUTION_eventmanager_dot_h
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
#endif
