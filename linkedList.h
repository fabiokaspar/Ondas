#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct no* Link;

typedef struct no {
	float angle;
	int total;
	Link next;
} Node;

Link initialize();
void freeAll(Link head);
void insertItem(Link head, float angle);
int searchItem(Link head, float angle);

#endif