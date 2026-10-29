#ifndef TEST_ALLOC_H
#define TEST_ALLOC_H

// Lets the given number of allocations succeed, then makes the next one fail once.
// Pass -1 to turn failures off.
void test_alloc_fail_after(int successes);

#endif
