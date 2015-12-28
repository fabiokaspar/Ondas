#include <stdlib.h>
#include "utils.h"
#include "linkedList.h"

Link initializeList() {
	Link head = (Link)mallocSafe(sizeof(Node));
	head->total = 0;
	head->next = NULL;
	return head;
}

void insertItem(Link head, float altura) {
	Link novo = (Link) mallocSafe(sizeof(Node));
	novo->altura = altura;
	Link lk;

	for (lk = head; lk->next != NULL; lk = lk->next) {
		if (lk->next->altura > altura) break;
	}

	novo->next = lk->next;
	lk->next = novo;
	head->total++;
}

int searchItem(Link head, float altura) {
	Link lk;

	if (head->total) {
		for (lk = head->next; lk != NULL; lk = lk->next) {
			if (lk->altura == altura) return 1;
		}
	}
	return 0;
}

void freeAll(Link head) {
	Link lk = head, lixo;

	while (lk != NULL) {
		lixo = lk;
		lk = lk->next;
		free(lixo);
	}
}