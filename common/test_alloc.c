#include <stddef.h>

#include "test_alloc.h"

void *__real_malloc(size_t size);
void *__real_calloc(size_t count, size_t size);
void *__real_realloc(void *ptr, size_t size);

void *__wrap_malloc(size_t size);
void *__wrap_calloc(size_t count, size_t size);
void *__wrap_realloc(void *ptr, size_t size);

static int countdown = -1;

void test_alloc_fail_after(int successes)
{
    countdown = successes;
}

static int should_fail(void)
{
    if (countdown < 0)
        return 0;
    if (countdown == 0) {
        countdown = -1;
        return 1;
    }
    countdown--;
    return 0;
}

void *__wrap_malloc(size_t size)
{
    return should_fail() ? NULL : __real_malloc(size);
}

void *__wrap_calloc(size_t count, size_t size)
{
    return should_fail() ? NULL : __real_calloc(count, size);
}

void *__wrap_realloc(void *ptr, size_t size)
{
    return should_fail() ? NULL : __real_realloc(ptr, size);
}
