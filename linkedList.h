#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct no* Link;

typedef struct no {
	double altura;
	int total;
	Link next;
} Node;

Link initializeList();
void freeAll(Link head);
void insertItem(Link head, double altura);

#endif