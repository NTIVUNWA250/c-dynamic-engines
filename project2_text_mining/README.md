# Project 2: Dynamic Text Mining and Linguistic Pattern Analyzer

A menu-driven C program that stores a paragraph in a growing heap buffer, extracts
words with pointer traversal, counts them in a growing array of `struct wordStat`,
and scores the text with a **Lexical Diversity Score**.

## Build and run

```
make                         # builds bin/text_analyzer
./bin/text_analyzer
```

The program starts with a built-in paragraph: the abstract (328 words) of a paper
by five University of Rwanda authors.

- **Source:** Niyibizi, Sibomana, Uwizeyemariya, Ntabajyana and Niyomugabo,
  "From an African Oral Tale to an English Picture Book: Rwandan Teachers'
  Experience with Online Translation of South African Institute of Distance
  Education's African Storybooks".
- **Published in:** *Teacher Education through Flexible Learning in Africa*,
  Vol 1 No 1, 2019, DOI 10.35293/tetfle.v1i1.66.
- **Licence:** CC BY 4.0, so it may be reused with credit.

Use menu option 1 to paste your own paragraph.

Make a sample output from the scripted session:

```
./bin/text_analyzer < samples/demo_input.txt > samples/sample_output.txt
```

## Test

```
make test        # unit + integration + performance
```

## Menu

| # | Operation | Function |
|---|---|---|
| 1 | Enter or paste a paragraph | `enter_paragraph()` |
| 2 | Count words | `count_words()` |
| 3 | Longest word | `longest_word()` |
| 4 | Most frequent word | `most_frequent()` |
| 5 | Word statistics | `display_word_statistics()` |
| 6 | Sort (a to z, or by frequency) | `sort_word_statistics()` |
| 7 | Search a word | `search_word()` |
| 8 | Text summary | `generate_text_summary()` |
| 9 | Lexical Diversity Score | `custom_analysis()` |
| 10 | Memory inspection | `memory_inspection()` |
| 11 | Show paragraph and source | `show_paragraph()` |
| 12 | Exit | frees memory and quits |

## Where each requirement lives

| Requirement | File |
|---|---|
| `struct wordStat { char name[40]; int count; }` | `include/wordstat.h` |
| Text buffer: `malloc`, grows with `realloc` | `src/textbuf.c` |
| Word array: `calloc`, grows with `realloc` | `src/wordtable.c` |
| Total and distinct word counts | `src/wordtable.c` |
| Pointer-based word extraction, lowercasing | `src/tokenizer.c` |
| `strcmp` (duplicates, sorting, stop words), `strlen`, `strcpy` | `src/wordtable.c`, `src/text_stats.c` |
| Function-pointer menu table (`AnalysisOperation`) | `src/menu.c` |
| Function-pointer sort strategies (`WordCompare`) | `src/operations.c`, `src/text_stats.c` |
| Memory addresses with `%p` and `(void *)` | `src/inspect.c` |
| Pasted text of any length (read in 255-byte chunks) | `src/textio.c` |
| Empty input and allocation failures | `src/load_text.c`, `tests/test_alloc_failure.c` |
| Words over 39 characters | `docs/text_rules.md` |
| Custom analysis | `src/text_stats.c`, `src/inspect.c`, `docs/custom_analysis.md` |

## Text rules

Punctuation, apostrophes, hyphens, case and long words: see `docs/text_rules.md`.

## Evidence in the sample session

`samples/demo_input.txt` covers:
- every operation on the built-in abstract;
- a pasted paragraph with `well-known`, `cat's`, `--` dashes, `!!!`, mixed case
  (`the`, `CAT`), a 45-letter word and a 599-character line, which shows the
  buffer growing;
- an empty paste, which keeps the previous text.
