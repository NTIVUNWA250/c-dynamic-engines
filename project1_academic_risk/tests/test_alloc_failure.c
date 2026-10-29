#include "records.h"
#include "test.h"
#include "test_alloc.h"

#include <string.h>

// When malloc/calloc/realloc fail, the list reports it and stays usable.
int main(void)
{
    StudentList list;

    test_alloc_fail_after(0);
    CHECK(!list_init(&list, 10));
    CHECK(list.items == NULL && list.count == 0);

    CHECK(list_init(&list, 1));
    Student s = {0};
    s.id = 1;
    strcpy(s.name, "First");
    strcpy(s.course, "C");
    s.grade = 70.0f;
    CHECK(list_add(&list, &s) == REC_OK);

    // The array is full, so the next add needs realloc, which we make fail.
    s.id = 2;
    test_alloc_fail_after(0);
    CHECK(list_add(&list, &s) == REC_NO_MEMORY);
    CHECK(list.count == 1 && list.items[0].id == 1);

    // Once memory is available again the same add works.
    CHECK(list_add(&list, &s) == REC_OK);
    CHECK(list.count == 2);

    list_free(&list);
    return TEST_DONE();
}
