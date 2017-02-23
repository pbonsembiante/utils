#ifndef PNODE_H
#define PNODE_H

typedef struct plist_linked_node plist_linked_node;
struct plist_linked_node
{
	void *data;
	struct plist_linked_node *next;
};

typedef struct plist_double_linked_node plist_double_linked_node;
struct plist_double_linked_node
{
	void *data;
	struct plist_double_linked_node *previous;
	struct plist_double_linked_node *next;
};

typedef struct plist_hashmap_node plist_hashmap_node;
struct plist_hashmap_node
{
	char *key;
	unsigned int hashcode;
	void *data;
	struct plist_hashmap_node *next;
};

#endif // PNODE_H
