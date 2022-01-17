/***************************************************************************
 * Copyright (C) 2016 - 2022 Patricio Bonsembiante. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef _PNODE_H_
#define _PNODE_H_

typedef struct plinked_node plinked_node;
struct plinked_node {
  void *data;
  struct plinked_node *next;
};

typedef struct pdouble_linked_node pdouble_linked_node;
struct pdouble_linked_node {
  void *data;
  struct pdouble_linked_node *previous;
  struct pdouble_linked_node *next;
};

typedef struct phashmap_node phashmap_node;
struct phashmap_node {
  char *key;
  unsigned int hashcode;
  void *data;
  struct phashmap_node *next;
};

#endif /* _PNODE_H_ */
