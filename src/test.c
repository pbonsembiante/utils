/***************************************************************************
 * Copyright (C) Patricio Bonsembiante - Phanatos. All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plist.h"

int main(void)
{
	plist_list *lista = 0;
	plist_list *tmp = 0;

	lista = plist_create();
	tmp = plist_create();

	plist_append(lista, "1");
	plist_append(lista, "2");
	plist_append(lista, "3");
	plist_append(lista, "4");
	plist_append(lista, "5");
	plist_append(lista, "6");
	plist_append(lista, "7");

	plist_append(tmp, "8");
	plist_append(tmp, "9");
	plist_append(tmp, "10");
	plist_append(tmp, "11");
	plist_append(tmp, "12");
	plist_append(tmp, "13");
	plist_append(tmp, "14");

	plist_merge(lista, tmp);

	for(size_t i = 0; i < plist_size(lista); ++i){
		printf("%s\n", (char*)plist_get(lista,i));
	}

	plist_destroy(lista);
	plist_destroy(tmp);

	return 0;
}
