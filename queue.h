#include <stdio.h>
#include <stdlib.h>

#ifndef QUEUE_H
#define QUEUE_H
struct queue {
    int front, back, size, capacity;
    int *arr;
};
void create_queue(struct queue *q, int max_capacity);
void enqueue(struct queue *q, int elem);
int dequeue(struct queue *q);
void debug_print(struct queue *q);
void print_queue(struct queue *q);
int get_front(struct queue *q);
#endif
