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
#ifndef EVOLUTION_creature_dot_h
#define EVOLUTION_creature_dot_h
#include "object.h"

/// Class to handle creature individuals and their positions, life etc.
class creature: public Object
{
private:
  float angle;
  unsigned char* col;
  unsigned int col_length;
  unsigned int pointer;
  unsigned char mem[512];
  int mempointer;
  float health;

  unsigned char charToNumber(unsigned char value)
  {
    if(value>='0'&&value<='9') return value-'0';
    if(value>='a'&&value<='f') return value-'a'+10;
    if(value>='A'&&value<='F') return value-'A'+10;
    return 255; // = error
  }

  void writeVisual(unsigned char* mem, int mempointer, int resolution);

public:
  /// Create a new creature with pre-set data
  /// @param x The new creature's X coordinate
  /// @param y The new creature's Y coordinate
  /// @param code The COL code for the new creature
  creature(int x, int y, const unsigned char* code);
  /// Create a new creature based on two parents
  /// @param parent_a One parent
  /// @param parent_b Another parent
  creature(creature* parent_a, creature* parent_b);
  /// Create a new creature based on precise information
  creature(float x, float y, float angle, unsigned char* col, unsigned int col_length, unsigned int pointer, unsigned char* mem, int mempointer, float health);
  /// Retrieve the COL for this creature
  /// @param col Pointer to assign the address of this creature's COL
  /// @param col_length Integer to assign the length of the creature's COL
  void getCOL(unsigned char*& col, unsigned int& col_length);
  /// Returns which angle the creature is facing
  float getAngle();
  /// Returns the creature's health value
  /// @return The health level of the creature
  float getLife(){return health;} 
  /// Kills the creature, unsure if this will be needed, health<=0 might suffice
  void die(){health=0;}
  /// Execute the current command currently pointed at and increment the pointer
  void execute();
  /// Return what value/"color" creatures will see this creature as
  unsigned char getType();
  unsigned int getPointer();
  int getMemPointer();
};
#endif
