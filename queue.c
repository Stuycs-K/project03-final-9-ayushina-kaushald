#include "queue.h"

struct queue *create_queue(int max_capacity) {
    struct queue *q = malloc(sizeof(struct queue));
    q->capacity = max_capacity;
    q->front = 0;
    q->size = 0;
    q->arr = malloc(max_capacity * sizeof(int));
    q->back = max_capacity - 1;
    return q;
}

void enqueue(struct queue *q, int elem) {
    q->back = (q->back + 1) % q->capacity; 
    q->arr[q->back] = elem;
    q->size += 1;
}

int dequeue(struct queue *q) {
    int elem = q->arr[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size -= 1;
    return elem;
}

int main() {
    struct queue *q = create_queue(20);
    enqueue(q, 30);
    enqueue(q, 40);
    printf("%d\n", dequeue(q));
    printf("%d\n", dequeue(q));
}
