#include <stdio.h>
#include <stdlib.h>

#ifndef QUEUE_H
#define QUEUE_H
#define MAX_CAPACITY 20
#define SHM_SIZE ((MAX_CAPACITY + 4) * sizeof(int))
struct queue {
    int front, back, size, capacity;
    int *arr;
};
void serialize(struct queue *q, int *data);
struct queue *deserialize(int *data);
struct queue *create_queue();
void enqueue(struct queue *q, int elem);
int dequeue(struct queue *q);
void remove_plr(struct queue *q, int elem);
void debug_print(struct queue *q);
void print_queue(struct queue *q);
int get_front(struct queue *q);
#endif
