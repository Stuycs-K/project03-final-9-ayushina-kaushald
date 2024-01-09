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

}

int dequeue(struct queue *q) {
    return 0;
}

int main() {
    
}
