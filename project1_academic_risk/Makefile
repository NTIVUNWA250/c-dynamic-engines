CC        ?= gcc
CFLAGS    := -std=c11 -Wall -Wextra -Wpedantic -Iinclude -I../common
TESTFLAGS := -g -fsanitize=address,undefined -fno-omit-frame-pointer
WRAPFLAGS := -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc
LDLIBS    := -lm

APP     := bin/academic_engine
HEADERS := $(wildcard include/*.h ../common/*.h)
COMMON  := ../common/log.c ../common/input.c
LIB_SRC := $(filter-out src/main.c,$(wildcard src/*.c)) $(COMMON)

UNIT_TESTS := $(patsubst tests/%.c,bin/%,$(wildcard tests/test_*.c))
PERF_TESTS := $(patsubst perf/%.c,bin/%,$(wildcard perf/perf_*.c))

all: $(APP)

$(APP): src/main.c $(LIB_SRC) $(HEADERS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -O2 $(filter %.c,$^) -o $@ $(LDLIBS)

bin/test_%: tests/test_%.c $(LIB_SRC) $(HEADERS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(TESTFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS)

bin/test_alloc_failure: tests/test_alloc_failure.c $(LIB_SRC) ../common/test_alloc.c $(HEADERS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(TESTFLAGS) $(WRAPFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS)

bin/app_checked: src/main.c $(LIB_SRC) $(HEADERS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(TESTFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS)

bin/perf_%: perf/perf_%.c $(LIB_SRC) $(HEADERS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -O2 $(filter %.c,$^) -o $@ $(LDLIBS)

test: unit integration perf

unit: $(UNIT_TESTS)
	@for t in $(UNIT_TESTS); do echo "== $$t"; ./$$t || exit 1; done

integration: bin/app_checked
	@if [ -f tests/integration.sh ]; then sh tests/integration.sh bin/app_checked; fi

perf: $(PERF_TESTS)
	@for t in $(PERF_TESTS); do echo "== $$t"; ./$$t || exit 1; done

clean:
	rm -rf bin

.PHONY: all test unit integration perf clean
