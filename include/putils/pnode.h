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

#include <stdlib.h>

typedef struct plist_node plist_node;
struct plist_node {
  void *data;
  struct plist_node *next;
};

typedef struct plist_double_node plist_double_node;
struct plist_double_node {
  void *data;
  struct plist_double_node *previous;
  struct plist_double_node *next;
};

typedef struct pdict_node pdict_node;
struct pdict_node {
  char *key;
  size_t key_len;
  unsigned int hashcode;
  void *data;
  struct pdict_node *next;
};

#endif /* _PNODE_H_ */
