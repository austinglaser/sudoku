#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

typedef struct _node {
	struct _node * prev;
	void * e;
} node;

struct _queue {
	int capacity;
	int size;
  deleteFn df;
	node * head;
};

static node * newNode(void * e)
{
	node * n = (node *) malloc(sizeof(struct _node));
	n->prev = NULL;
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

	n->prev = q->head;
	q->head = n;

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
		assert(!n->prev);
		q->head = NULL;
	}
	else q->head = n->prev;

	deleteNode(n);
	q->size -= 1;

	return 0;
}
/*
int printQueue(queue q, printFn f)
{
	int cnt = q->size - 1;
	node * n = NULL;
	void * content[q->size];
	
	assert(q);
	if (!f) return -1;
	
	for(n = q->head; n != NULL; n = n->prev)
	{
		content[cnt] = n->e;
		cnt--;
	}
	for(cnt = 0; cnt < q->size; ++cnt)
	{
		printf(" %d:", cnt+1);
		f(content[cnt]);
	}	
	printf("\n");
	
	return 0;
}
*/
void deleteQueue(queue q)
{
	assert(q);
	node * n = q->head;
	node * prev = NULL;
	while(n != NULL)
	{
		prev = n->prev;
    if (q->df) q->df(n->e);
		deleteNode(n);
		n = prev;
	}
	free(q);
}
