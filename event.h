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
