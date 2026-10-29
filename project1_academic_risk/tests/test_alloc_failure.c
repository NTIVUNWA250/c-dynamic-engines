#include "records.h"
#include "test.h"
#include "test_alloc.h"

#include <string.h>

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

    s.id = 2;
    test_alloc_fail_after(0);
    CHECK(list_add(&list, &s) == REC_NO_MEMORY);
    CHECK(list.count == 1 && list.items[0].id == 1);

    CHECK(list_add(&list, &s) == REC_OK);
    CHECK(list.count == 2);

    list_free(&list);
    return TEST_DONE();
}
