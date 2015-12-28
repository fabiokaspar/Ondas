#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct no* Link;

typedef struct no {
	float altura;
	int total;
	Link next;
} Node;

Link initializeList();
void freeAll(Link head);
void insertItem(Link head, float altura);
int searchItem(Link head, float altura);

#endif