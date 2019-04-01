#ifndef QUEUE_H
#define QUEUE_H

typedef int elem;
typedef struct queue QUEUE;
typedef QUEUE* Queue;

Queue newQueue();
int putQueue(Queue q, elem x);
elem getQueue(Queue q);
int isEmptyQueue(Queue q);
void deleteQueue(Queue q);

#endif
