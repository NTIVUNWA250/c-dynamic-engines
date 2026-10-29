#include "records.h"
#include "test.h"

#include <string.h>

static Student make(int id, const char *name, const char *course, float grade)
{
    Student s = {0};
    s.id = id;
    strncpy(s.name, name, NAME_LEN - 1);
    strncpy(s.course, course, COURSE_LEN - 1);
    s.grade = grade;
    return s;
}

// Adding, rejecting, finding and removing records, including resizing.
int main(void)
{
    StudentList list;
    CHECK(list_init(&list, 2));
    CHECK(list.capacity == 2 && list.count == 0);

    Student a = make(10, "Amina Uwase", "Embedded C", 72.0f);
    Student b = make(11, "Brian Otieno", "Embedded C", 45.5f);
    Student c = make(12, "Chloe Mensah", "Web Design", 90.0f);
    CHECK(list_add(&list, &a) == REC_OK);
    CHECK(list_add(&list, &b) == REC_OK);
    CHECK(list_add(&list, &c) == REC_OK);
    CHECK(list.count == 3);
    CHECK(list.capacity >= 3);

    // Validation.
    CHECK(list_add(&list, &a) == REC_DUPLICATE_ID);
    Student bad_grade = make(20, "X", "Y", 100.5f);
    CHECK(list_add(&list, &bad_grade) == REC_INVALID_GRADE);
    bad_grade.grade = -1.0f;
    CHECK(list_add(&list, &bad_grade) == REC_INVALID_GRADE);
    Student bad_id = make(0, "X", "Y", 50.0f);
    CHECK(list_add(&list, &bad_id) == REC_INVALID_ID);
    Student empty = make(21, "", "Y", 50.0f);
    CHECK(list_add(&list, &empty) == REC_EMPTY_TEXT);
    CHECK(list.count == 3);

    // Search by id and by name (whole name or one word, any case).
    CHECK(list_find_id(&list, 11) != NULL && list_find_id(&list, 11)->grade == 45.5f);
    CHECK(list_find_id(&list, 99) == NULL);
    const Student *found = list_find_name(&list, "chloe mensah", NULL);
    CHECK(found != NULL && found->id == 12);
    found = list_find_name(&list, "OTIENO", NULL);
    CHECK(found != NULL && found->id == 11);
    CHECK(list_find_name(&list, "Otie", NULL) == NULL);

    // Two students with the same first name are both found.
    Student a2 = make(13, "Amina Kato", "Web Design", 60.0f);
    CHECK(list_add(&list, &a2) == REC_OK);
    found = list_find_name(&list, "amina", NULL);
    CHECK(found != NULL && found->id == 10);
    found = list_find_name(&list, "amina", found);
    CHECK(found != NULL && found->id == 13);
    CHECK(list_find_name(&list, "amina", found) == NULL);

    // Removing keeps the other records in order.
    CHECK(list_remove(&list, 11));
    CHECK(!list_remove(&list, 11));
    CHECK(list.count == 3);
    CHECK(list.items[0].id == 10 && list.items[1].id == 12 && list.items[2].id == 13);

    // Growing to many records and shrinking back down.
    for (int id = 100; id < 140; id++) {
        Student s = make(id, "Test Student", "Load", 50.0f);
        CHECK(list_add(&list, &s) == REC_OK);
    }
    size_t big = list.capacity;
    for (int id = 100; id < 140; id++)
        CHECK(list_remove(&list, id));
    CHECK(list.capacity < big);
    CHECK(list.count == 3);

    // Removing everything releases the array.
    CHECK(list_remove(&list, 10) && list_remove(&list, 12) && list_remove(&list, 13));
    CHECK(list.items == NULL && list.capacity == 0);
    CHECK(list_add(&list, &a) == REC_OK);

    list_free(&list);
    CHECK(list.items == NULL && list.count == 0);
    return TEST_DONE();
}
