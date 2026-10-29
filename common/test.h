#ifndef TEST_H
#define TEST_H

#include <math.h>
#include <stdio.h>

// Tiny test helpers: count checks, report failures with file and line.
static int test_checks = 0;
static int test_failures = 0;

#define CHECK(cond)                                                       \
    do {                                                                  \
        test_checks++;                                                    \
        if (!(cond)) {                                                    \
            test_failures++;                                              \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                                 \
    } while (0)

#define CHECK_NEAR(a, b, eps) CHECK(fabs((double)(a) - (double)(b)) <= (eps))

// Prints the totals; use as the return value of main.
#define TEST_DONE()                                                        \
    (printf("%d checks, %d failed\n", test_checks, test_failures), test_failures != 0)

#endif
