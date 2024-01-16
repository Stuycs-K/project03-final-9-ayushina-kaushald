#include "queue.h"

struct queue *create_queue() {
    struct queue *q = malloc(sizeof(struct queue));
    q->capacity = MAX_CAPACITY;
    q->front = 0;
    q->size = 0;
    q->arr = malloc(q->capacity * sizeof(int));
    q->back = 0;
    return q;
}

void serialize(struct queue *q, int *data) {
    data[0] = q->capacity;
    data[1] = q->size;
    data[2] = q->front;
    data[3] = q->back;
    for (int i = 0; i < q->capacity; i++) {
        data[4 + i] = q->arr[i];
    }
    // printf("\nSerialize\n");
    // printf("SHM_SIZE: %ld\n", SHM_SIZE);
    // printf("[");
    // for (int i = 0; i < SHM_SIZE / sizeof(int); i++) {
    //     printf(" %d ", data[i]);
    // }
    // printf("]\n\n");
}

struct queue *deserialize(int *data) {
    struct queue *q = create_queue();
    q->capacity = data[0];
    q->size = data[1];
    q->front = data[2];
    q->back = data[3];
    for (int i = 0; i < q->capacity; i++) {
        q->arr[i] = data[4 + i];
    }
    // printf("\nDeserialize\n");
    // printf("[");
    // for (int i = 0; i < SHM_SIZE / sizeof(int); i++) {
    //     printf(" %d ", data[i]);
    // }
    // printf("]\n\n");
    // printf("SHM_SIZE: %ld\n", SHM_SIZE);
    // debug_print(q);
    // printf("End of deserialize\n");
    return q;
}

void enqueue(struct queue *q, int elem) {
    // printf("\nstarting enqueue\n");
    // debug_print(q);
    if (q->size == 0) {
        q->arr[q->back] = elem;
    }
    else {
        q->back = (q->back + 1) % q->capacity; 
        q->arr[q->back] = elem;
    }
    q->size += 1;

    // printf("elem %d\n", elem);
    // printf("ending enqueue\n");
    // debug_print(q);
    // printf("\n");
}

int dequeue(struct queue *q) {
    // printf("\nstarting dequeue\n");
    // debug_print(q);
    int elem = q->arr[q->front];
    q->arr[q->front] = 0;
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

void remove_plr(struct queue *q, int elem) {
    if (q->size == 0) return;
    int shift = 0;
    for (int i = 0; i < q->capacity; i++) {
        int index = (q->front + i) % q->capacity;
        if (shift) {
            int prevInd = (index - 1) % q->capacity;
            q->arr[prevInd] = q->arr[index];
            q->arr[index] = 0;
        }
        else {
            if (q->arr[index] == elem) {
                q->arr[index] = 0;
                shift = 1;
            }
        }
    }
    if (q->size > 1) {
        q->back = (q->back - 1) % q->capacity;
    }
    q->size -= 1;
}

void print_queue(struct queue *q) {
    printf("[");
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % q->capacity;
        printf(" %d ", q->arr[index] - PLR_OFFSET);
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
//     enqueue(q, 50);
//     // printf("%d\n", dequeue(q));
//     // printf("%d\n", dequeue(q));
//     print_queue(q);
//     remove_plr(q, 40);
//     print_queue(q);
//     remove_plr(q, 50);
//     print_queue(q);
//     remove_plr(q, 30);
//     print_queue(q);
// }
