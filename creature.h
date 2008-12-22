#ifndef EVOLUTION_creature_dot_h
#define EVOLUTION_creature_dot_h
//#include "col.h"

/// Class to handle creature individuals and their positions, life etc.
class creature
{
private:
  int x, y;
  unsigned char* col;
  unsigned int col_length;
  unsigned int pointer;
  unsigned char mem[512];
  int mempointer;
  int health;

  unsigned char charToNumber(unsigned char value)
  {
    if(value>='0'&&value<='9') return value-'0';
    if(value>='a'&&value<='f') return value-'a'+10;
    if(value>='A'&&value<='F') return value-'A'+10;
    return 255; // = error
  }
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
  /// Retrieve the COL for this creature
  /// @param col Pointer to assign the address of this creature's COL
  /// @param col_length Integer to assign the length of the creature's COL
  void getCOL(unsigned char*& col, unsigned int& col_length);
  /// Read the creature's position into two integers
  void getPosition(int& x, int& y);
  /// Returns the creature's health value
  /// @return The health level of the creature
  int getLife(){return health;} 
  /// Kills the creature, unsure if this will be needed, health<=0 might suffice
  void die(){health=0;}
  /// Execute the current command currently pointed at and increment the pointer
  void execute();
};
#endif
