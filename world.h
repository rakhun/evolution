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
#ifndef EVOLUTION_world_dot_h
#define EVOLUTION_world_dot_h
/// Small struct to provide basic world information, needed for plugins like sdl_display and network
struct world {
  /// Width of the world in pixels
  int width;
  /// Height of the world in pixels
  int height;
};
#endif
