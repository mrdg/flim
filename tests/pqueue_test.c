#include "../src/pqueue.h"
#include "test.h"

void test_enqueue_first_task()
{
    struct pqueue *queue =  pqueue_create();
    struct task *t = task_create(Pt_Time(NULL));
    enqueue(queue, t);

    assert_equals(queue->head, t);
}

void test_enqueue_second_task()
{
    struct pqueue *queue =  pqueue_create();
    PtTimestamp time = Pt_Time(NULL);
    struct task *t1 = task_create(time);
    struct task *t2 = task_create(time + 500);
    enqueue(queue, t1);
    enqueue(queue, t2);

    assert_equals(queue->head->next, t2);
}

void test_enqueue_inserts()
{
    struct pqueue *queue =  pqueue_create();
    PtTimestamp time = Pt_Time(NULL);
    struct task *t1 = task_create(time);
    struct task *t2 = task_create(time + 500);
    struct task *t3 = task_create(time + 250);
    enqueue(queue, t1);
    enqueue(queue, t2);
    enqueue(queue, t3);

    assert_equals(queue->head->next, t3);
    
}

void test_enqueue_inserts_before_first()
{
    PtTimestamp time = Pt_Time(NULL);
    struct pqueue *queue =  pqueue_create();
    struct task *t1 = task_create(time);
    struct task *t0 = task_create(time - 500);
    enqueue(queue, t1);
    enqueue(queue, t0);

    assert_equals(queue->head, t0);
}

void test_dequeue_returns_null_for_empty_queue()
{
    struct pqueue *queue = pqueue_create();
    PtTimestamp now = Pt_Time(NULL);
    struct task *t = dequeue(queue, now);

    assert_equals(NULL, t);
}

void test_dequeue_returns_null_when_no_tasks_are_due()
{
    struct pqueue *queue = pqueue_create();
    PtTimestamp now = Pt_Time(NULL);
    struct task *t0 = task_create(now + 500);
    enqueue(queue, t0);
    struct task *t1 = dequeue(queue, now);

    assert_equals(NULL, t1);
}

void test_dequeue_returns_all_ready_tasks()
{
    struct pqueue *queue = pqueue_create();
    PtTimestamp now = Pt_Time(NULL);

    struct task *t0 = task_create(now - 500);
    struct task *t1 = task_create(now);
    struct task *t2 = task_create(now + 500);
    enqueue(queue, t0);
    enqueue(queue, t1);
    enqueue(queue, t2);

    assert_equals(queue->length, 3);

    struct task *first  = dequeue(queue, now);
    struct task *second = first->next;

    assert_equals(first, t0); // < now
    assert_equals(second, t1);  // == now
    assert_equals(second->next, NULL); // no third task
    assert_equals(queue->head, t2);
    assert_equals(queue->length, 1);
}
