#include <stdlib.h>
#include "utils.h"
#include "linkedList.h"

Link initializeList() {
	Link head = (Link)mallocSafe(sizeof(Node));
	head->total = 0;
	head->next = NULL;
	return head;
}

void insertItem(Link head, double altura) {
	Link novo = (Link) mallocSafe(sizeof(Node));
	novo->altura = altura;

	novo->next = head->next;
	head->next = novo;
	head->total++;
}

void freeAll(Link head) {
	Link lk = head, lixo;

	while (lk != NULL) {
		lixo = lk;
		lk = lk->next;
		free(lixo);
	}
}