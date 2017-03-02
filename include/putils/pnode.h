#ifndef _PNODE_H_
#define _PNODE_H_

typedef struct plinked_node plinked_node;
struct plinked_node
{
	void *data;
    struct plinked_node *next;
};

typedef struct pdouble_linked_node pdouble_linked_node;
struct pdouble_linked_node
{
	void *data;
    struct pdouble_linked_node *previous;
    struct pdouble_linked_node *next;
};

typedef struct phashmap_node phashmap_node;
struct phashmap_node
{
	char *key;
	unsigned int hashcode;
	void *data;
    struct phashmap_node *next;
};

#endif /* _PNODE_H_ */
