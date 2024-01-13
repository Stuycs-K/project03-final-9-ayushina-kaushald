#include "queue.h"

void create_queue(struct queue *q, int max_capacity) {
    q->capacity = max_capacity;
    q->front = 0;
    q->size = 0;
    q->arr = malloc(max_capacity * sizeof(int));
    q->back = max_capacity - 1;
}

void enqueue(struct queue *q, int elem) {
    // printf("\nstarting enqueue\n");
    // debug_print(q);
    q->back = (q->back + 1) % q->capacity; 
    q->arr[q->back] = elem;
    q->size += 1;
    //printf("elem %d", elem);
    // printf("ending enqueue\n");
    // debug_print(q);
    // printf("\n");
}

int dequeue(struct queue *q) {
    // printf("starting dequeue\n");
    // debug_print(q);
    int elem = q->arr[q->front];
    q->size -= 1;
    if (q->size > 0) {
        q->front = (q->front + 1) % q->capacity;
    }
    // printf("ending dequeue\n");
    // debug_print(q);
    // printf("\n");
    return elem;
}

int get_front(struct queue *q) {
    return q->arr[q->front];
}

void remove_plr(int *arr, int elem) {
    for (int i = 0; i < sizeof(arr)/sizeof(int); i += sizeof(int)) {
        if (arr[i] == elem) {
            
        }
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

void debug_print(struct queue *q) {
    printf("debug [");
    for (int i = 0; i < q->capacity; i++) {
        printf(" %d ", q->arr[i]);
        // if (q->arr[i]) {
        //     printf(" %d ", q->arr[i]);
        // }
        // else {
        //     printf(" - ");
        // }
    }
    printf("] front %d back %d\n", q->front, q->back);
}

// int main() {
//     struct queue *q = create_queue(20);
//     enqueue(q, 30);
//     enqueue(q, 40);
//     printf("%d\n", dequeue(q));
//     printf("%d\n", dequeue(q));
// }
