#include "test.h"
#include <stdio.h>

int main(void)
{
    run_test(test_enqueue_first_task);
    run_test(test_enqueue_second_task);
    run_test(test_enqueue_inserts);
    run_test(test_enqueue_inserts_before_first);
    run_test(test_dequeue_returns_null_for_empty_queue);
    run_test(test_dequeue_returns_null_when_no_tasks_are_due);
    run_test(test_dequeue_returns_all_ready_tasks);

    printf("\n");
    printf("All tests passed.\n");
}


