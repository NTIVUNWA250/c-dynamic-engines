# Working rules for this repo

These are the machine-wide session rules, plus the details that apply to C work here.
Anyone (person or assistant) working in this repo follows them.

## Attribution

- Never list an assistant as a contributor. No `Co-Authored-By` trailers, no
  "Generated with" lines in commits or PRs.
- Commits are made under the owner's git identity.

## Commits

- Many small commits, each one a meaningful, working unit of change.
- One-line commit messages in plain language. No body unless truly needed.
- PR descriptions are brief and clear.

## Branches

- Always branch off `main`, unless continuing the same task on its branch.
- Name branches by type: `feat/...`, `fix/...`, `refactor/...`, `test/...`, `chore/...`.
- Never merge into `main` and never open a PR unless the owner says so.
- A branch that has diverged from `main` is deleted, not repaired.
- Never squash-merge. Real merges only.
- Keep long-lived branches current with `main` (weekly remerge at minimum).

## Code

- C11, compiled with `gcc -std=c11 -Wall -Wextra -Wpedantic`. No warnings allowed.
- One short comment per change, saying what the code does in plain words.
- Keep files small. When a file grows large, split it into files that each hold one
  coherent set of functions. Splitting must never change behaviour.
- Log through `common/log.h` (`log_msg`). Logs go to stderr, so program output stays
  clean. Set `APP_LOG=info` or `APP_LOG=debug` to see them.
- Every allocation is checked. Everything allocated is freed before exit.
- Program text and docs use plain ASCII characters only.

## Testing

Every change ships with tests.

- Unit tests: `tests/test_*.c` in each project (they use `common/test.h`).
- Integration tests: `tests/integration.sh`. It pipes a scripted session into the
  program and checks the output.
- Performance tests: `perf/perf_*.c`, with a time budget for large inputs.
  (k6 is for web services, so C projects use timed C programs instead.)
- Tests build with AddressSanitizer and UBSan, so leaks, overflows and
  use-after-free fail the run.
- Allocation failures are tested by wrapping `malloc`/`calloc`/`realloc` at link
  time (`common/test_alloc.c`).

## What may be run

- Allowed: test commands only (`make test`, `make -C <project> test`).
- Not allowed: anything else (deploys, release builds), unless the owner asks.
- Always print the exact commands the owner can run to check a change.

## Reporting

- Always list the folders and files that changed.
- Write-ups and reports are printed in the terminal.
