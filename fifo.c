#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

typedef struct _node {
	struct _node * next;
	void * e;
} node;

struct _queue {
	int capacity;
	int size;
	node * head;
	node * tail;
  deleteFn df;
};

static node * newNode(void * e)
{
	node * n = (node *) malloc(sizeof(struct _node));
	n->next = NULL;
	n->e = e;
	return n;
}

static void deleteNode(node * n)
{
	assert(n);
	free(n);
}

queue newQueue(int capacity, deleteFn df)
{
	queue q = (queue) malloc(sizeof(struct _queue));
	if(capacity <= 0) capacity = -1;
	q->capacity = capacity;
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;
  q->df = df;
	return q;
}

int isEmptyQueue(queue q)
{
	assert(q);
	return (q->size == 0);
}

int putQueue(queue q, void * e)
{
	node * n = NULL;
	assert (q);

	if (q->size == q->capacity) return -1;

	n = newNode(e);

	if (q->size == 0)
	{
		assert (!q->head);
		assert (!q->tail);
		q->head = n;
		q->tail = n;
	}
	else
	{
		assert(!q->tail->next);
		q->tail->next = n;
		q->tail = n;
	}

	q->size += 1;

	return 0;
}

int getQueue(queue q, void ** e)
{
	node * n;
	assert(q);

	if(!e) return -1;

	if(isEmptyQueue(q))
	{
		*e = NULL;
		return -2;
	}

	assert(q->head);
	
	n = q->head;
	*e = n->e;
	if(q->size == 1)
	{
		assert(!n->next);
		q->head = NULL;
		q->tail = NULL;
	}
	else
	{
		q->head = n->next;
	}
	deleteNode(n);
	q->size -= 1;

	return 0;
}
/*
int printQueue(queue q, printFn f)
{
	int cnt = 1;
	node * n = NULL;
	
	assert(q);
	if (!f) return -1;
	
	for(n = q->head; n != NULL; n = n->next)
	{
		printf(" %d:", cnt);
		f(n->e);
		cnt++;
	}
	printf("\n");
	
	return 0;
}
*/
void deleteQueue(queue q)
{
	assert(q);
	node * n = q->head;
	node * next = NULL;
	while(n != NULL)
	{
		next = n->next;
    if (q->df) q->df(n->e);
		deleteNode(n);
		n = next;
	}
	free(q);
}
