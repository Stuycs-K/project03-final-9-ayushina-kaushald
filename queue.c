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

void remove_plr(struct queue *q, int elem) {
    int i = 0;
    int shift = 0;
    while (i < q->size) {
        int index = (q->front + i) % q->capacity;
        if (q->arr[index] == elem) {
            q->arr[index] = 0;
            shift = 1;
        }
        else if (shift) {
            q->arr[index - 1] = q->arr[index];
            q->arr[index] = 0;
        }
        i++;
    }
}

void print_queue(struct queue *q) {
    printf("[");
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % q->capacity;
        printf(" %d ", q->arr[index]);
    }
    printf("]\n");
}

// int main() {
//     struct queue *q = create_queue(20);
//     enqueue(q, 30);
//     enqueue(q, 40);
//     printf("%d\n", dequeue(q));
//     printf("%d\n", dequeue(q));
// }
