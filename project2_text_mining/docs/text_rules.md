# How the analyzer reads text

| Case | Rule | Example in | Words out |
|---|---|---|---|
| Letters and digits | Make up words | `In 1851` | `in`, `1851` |
| Upper/lower case | Folded to lowercase, so they count as one word | `The THE the` | `the` x3 |
| Spaces, tabs, newlines, repeated whitespace | Separate words | `a   b\n\nc` | `a`, `b`, `c` |
| Punctuation `. , ; : ! ? ( ) " ...` | Separates words and is dropped | `Ishmael.` | `ishmael` |
| Apostrophe inside a word | Kept | `people's`, `don't` | `people's`, `don't` |
| Apostrophe at the start or end | Dropped | `'quoted'`, `dogs'` | `quoted`, `dogs` |
| Single hyphen between letters | Kept (compound word) | `well-known` | `well-known` |
| Double hyphen `--` (a dash) | Separates | `ago--never` | `ago`, `never` |
| Hyphen at the start or end | Dropped | `-dash-` | `dash` |
| Non-ASCII bytes (curly quotes, accented letters) | Treated as separators | `"Hello"` (curly) | `hello` |

## Long words

`struct wordStat` stores a name in `char name[40]`, so it holds at most 39
characters plus the closing `'\0'`.

- A longer word is **truncated** to its first 39 characters for storage and
  counting. Two long words that share the same first 39 characters count as one word.
- Its **full length** is still used for the longest-word result and the average
  word length.
- The summary reports how many words were truncated.

## Word length

- **Longest word**: the full length, including `'` and `-`.
- **Average word length**: letters and digits only, so `people's` counts as 7.
