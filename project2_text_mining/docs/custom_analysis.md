# Custom Analysis: Lexical Diversity Score

## Purpose

Shows how varied a writer's vocabulary is. A text that keeps reusing the same
words scores low; a text that keeps introducing new words scores high. A second
score leaves out small "function" words such as *the*, *and* and *of*, to show
whether the repetition is in meaningful words or just in grammar.

## Formulas

```
Lexical Diversity Score  = 100 x distinct words / total words
Hapax share              = 100 x (words used exactly once) / distinct words
Content-word diversity   = 100 x distinct content words / total content words
```

A **content word** is any word that is not in the stop-word list in
`src/text_stats.c` (48 common English words, such as "the", "and", "of", "i", "my").

## Algorithm

1. The word table already holds every distinct word and its count (`struct wordStat`).
2. Walk the table once with a pointer:
   - if `count == 1`, add one to the hapax total;
   - if the word is not a stop word (checked with `strcmp` against the list),
     add 1 to the content-distinct total and `count` to the content-total total.
3. Apply the formulas, then pick the band from the main score.

This is one pass over the distinct words, plus a short stop-word lookup for each.

## Bands

| Score | Band |
|---|---|
| below 40 | Low (repetitive) |
| 40 to 59.9 | Moderate |
| 60 and above | High (varied) |

## Interpretation

- If **content-word diversity is higher than the main score**, the repetition comes
  mostly from function words, and the meaningful vocabulary is rich.
- If it is **equal or lower**, the text keeps returning to the same key ideas.
- The score drops naturally as texts get longer, because common words keep
  coming back. Only compare texts of similar length.

## Result for the built-in paragraph (University of Rwanda abstract, 328 words)

| Measure | Value |
|---|---|
| Distinct / total | 167 / 328 |
| Lexical Diversity Score | 50.9, Moderate |
| Words used once | 111 (66.5% of distinct) |
| Content words | 144 distinct / 220 total |
| Content-word diversity | 65.5 |

An academic abstract repeats its key terms on purpose: *african* appears 11
times, *reading* 7 and *kinyarwanda* 5. That keeps the raw score Moderate. Once
function words are removed, the content-word score rises to 65.5, so the
vocabulary around those key terms is still varied.
