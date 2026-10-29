CC       ?= gcc
CFLAGS   := -std=c11 -Wall -Wextra -Wpedantic -I.
TESTFLAGS := -g -fsanitize=address,undefined -fno-omit-frame-pointer

# Builds and runs the shared helper tests with sanitizers on.
test: bin/test_input
	./bin/test_input

bin/test_input: tests/test_input.c input.c log.c $(wildcard *.h)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(TESTFLAGS) $(filter %.c,$^) -o $@ -lm

clean:
	rm -rf bin

.PHONY: test clean
