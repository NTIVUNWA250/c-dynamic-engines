# Runs the shared helper tests, then every project's tests.
PROJECTS := $(wildcard project*/)

test:
	$(MAKE) -C common test
	@for p in $(PROJECTS); do $(MAKE) -C $$p test || exit 1; done

clean:
	$(MAKE) -C common clean
	@for p in $(PROJECTS); do $(MAKE) -C $$p clean; done

.PHONY: test clean
