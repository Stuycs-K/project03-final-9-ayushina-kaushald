#include <stdio.h>
#include <stdlib.h>

#ifndef QUEUE_H
#define QUEUE_H
struct queue {
    int front, back, size, capacity;
    int *arr;
};
struct queue *create_queue(int max_capacity);
void enqueue(struct queue *q, int elem);
int dequeue(struct queue *q);
void remove_plr(struct queue *q, int elem);
void print_queue(struct queue *q);
#endif