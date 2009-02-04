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
#include "object.h"

Object* Object::iter=0;

void Object::getPosition(float& x, float& y)
{
  x=this->x;
  y=this->y;
}

void Object::setPosition(float x, float y)
{
  this->x=x;
  this->y=y;
}
