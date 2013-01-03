#include "pqueue.h"

struct pqueue * pqueue_create()
{
    struct pqueue * queue = malloc(sizeof(struct pqueue));
    pthread_mutex_init(&queue->lock, NULL);
    queue->length = 0;
    return queue;
}

struct task * task_create(PtTimestamp start)
{
    struct task *t = malloc(sizeof(struct task));
    t->start = start;
    t->next  = NULL;
    t->lua_function = NULL;
    t->c_function = NULL;
    t->fn_data = NULL;
    return t;
}

void enqueue(struct pqueue *queue, struct task *t)
{
    pthread_mutex_lock(&queue->lock);

    if (queue->length == 0) {
        queue->head = t;

    } else {
        if (queue->head->start > t->start) {
            struct task *oldhead = queue->head;
            queue->head = t;
            t->next = oldhead;
        } else {
            struct task *current = queue->head;
            struct task *prev    = queue->head;

            do {
                prev    = current;
                current = current->next;

            } while (current != NULL && current->start <= t->start);

            prev->next = t;
            if (current != NULL) {
                t->next = current;
            }
        }

    }
    queue->length++;
    pthread_mutex_unlock(&queue->lock);
}

struct task * dequeue(struct pqueue *queue, PtTimestamp now)
{
    pthread_mutex_lock(&queue->lock);
    struct task *ready;

    if (queue->length == 0 || now < queue->head->start) {
        ready = NULL;

    } else {
        int removed = 0;
        ready = queue->head;
        struct task *current = queue->head;
        struct task *prev    = queue->head;

        do {
            prev    = current;
            current = current->next;
            removed++;
        } while (current != NULL && current->start <= now);

        prev->next = NULL;
        if (current != NULL) {
            queue->head = current;
        }

        queue->length -= removed;
    }

    pthread_mutex_unlock(&queue->lock);
    return ready;
}
