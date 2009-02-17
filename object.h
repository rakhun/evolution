/*
 * Evolution - An experiment with evolving scripts through natural selection
 * Copyright (C) 2008-2009 Henrik Nilsson
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.
 */
#ifndef EVOLUTION_object_dot_h
#define EVOLUTION_object_dot_h

/// Base object class which creatures and inanimate objects inherit from
class Object
{
protected:
  float x; ///< X position of the object
  float y; ///< Y position of the object
  unsigned char objType; ///< Type of object, will be used as the "color" of the object
public:
  static Object* iter; ///< Pointer to the first object in the iterator list
  Object* next; ///< Pointer to the next object in the list

  /// Read the object's position into two integers
  void getPosition(float& x, float& y);
  /// Moves the object to the position specified
  void setPosition(float x, float y);
  /// Getting what type of object it is
  unsigned char getType();
};
#endif
