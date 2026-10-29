#include "input.h"
#include "test.h"

#include <string.h>

int main(void)
{
    int i = 0;
    CHECK(parse_int("42", 0, 100, &i) && i == 42);
    CHECK(parse_int("  7  ", 0, 100, &i) && i == 7);
    CHECK(!parse_int("", 0, 100, &i));
    CHECK(!parse_int("abc", 0, 100, &i));
    CHECK(!parse_int("12abc", 0, 100, &i));
    CHECK(!parse_int("101", 0, 100, &i));
    CHECK(!parse_int("-1", 0, 100, &i));
    CHECK(!parse_int("99999999999999999999", 0, 100, &i));

    float f = 0.0f;
    CHECK(parse_float("88.5", 0.0f, 100.0f, &f) && f > 88.4f && f < 88.6f);
    CHECK(parse_float("0", 0.0f, 100.0f, &f) && f == 0.0f);
    CHECK(!parse_float("100.1", 0.0f, 100.0f, &f));
    CHECK(!parse_float("nan", 0.0f, 100.0f, &f));
    CHECK(!parse_float("inf", 0.0f, 100.0f, &f));
    CHECK(!parse_float("5,5", 0.0f, 100.0f, &f));

    char text[32] = "   hello world \t";
    CHECK(strcmp(trim(text), "hello world") == 0);
    char spaces[8] = "    ";
    CHECK(strcmp(trim(spaces), "") == 0);

    return TEST_DONE();
}
