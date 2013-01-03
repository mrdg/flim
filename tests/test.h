#define assert_equals(expected, actual)\
    if (expected != actual) {\
        printf("FAIL: %#Lx != %#Lx\n", (uint64_t)expected, (uint64_t)actual);\
        exit(1); \
    }

#define run_test(name)\
    printf("%s\n", #name);\
    name();

void test_enqueue_first_task();
void test_enqueue_second_task();
void test_enqueue_inserts();
void test_enqueue_inserts_before_first();
void test_dequeue_returns_null_for_empty_queue();
void test_dequeue_returns_null_when_no_tasks_are_due();
void test_dequeue_returns_all_ready_tasks();
