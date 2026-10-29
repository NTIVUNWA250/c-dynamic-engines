#!/bin/sh
APP="${1:-bin/text_analyzer}"
OUT="bin/integration_output.txt"

"$APP" < samples/demo_input.txt > "$OUT" 2> bin/integration_stderr.txt
status=$?
if [ "$status" -ne 0 ]; then
    echo "FAIL: program exited with status $status"
    cat bin/integration_stderr.txt
    exit 1
fi

failures=0
expect() {
    if grep -qF -- "$1" "$OUT"; then
        echo "ok   $1"
    else
        echo "FAIL missing: $1"
        failures=$((failures + 1))
    fi
}

expect "Project: Dynamic Text Mining and Linguistic Pattern Analyzer"
expect "Source: From an African Oral Tale to an English Picture Book"
expect "Loaded the built-in paragraph: 328 words, 167 distinct."
expect "Longest word: \"rwanda-college\" (14 characters)"
expect "Most frequent word: \"the\", 18 times"
expect "Word statistics sorted By frequency (most frequent first)."
expect "Word statistics sorted Alphabetically (a to z)."
expect "\"moby\" does not appear in the text."
expect "\"kinyarwanda\" appears 5 time(s)"
expect "\"!!!\" contains no letters or digits."
expect "Lexical Diversity Score: 50.9 / 100 -> Moderate"
expect "Text buffer address:"
expect "Word statistics array:"
expect "Selected word-stat entry:"

expect "(read 741 characters)"
expect "Loaded \"Punctuation Test\": 119 words, 15 distinct."
expect "Note: this text has fewer than 150 words."
expect "Most frequent word: \"again\" (100 times)"
expect "Words truncated to 39 characters: 1"
expect "Longest word: \"pneumonoultramicroscopicsilicovolcanoco\" (45 characters)"

expect "Empty text, keeping the previous paragraph."
expect "Source: Punctuation Test"
expect "All memory released. Goodbye."

for word in "well-known" "cat's" "dog's"; do
    if grep -qE "^[0-9]+ +$word +[0-9]+$" "$OUT"; then
        echo "ok   word kept whole: $word"
    else
        echo "FAIL word not kept whole: $word"
        failures=$((failures + 1))
    fi
done

if [ "$failures" -ne 0 ]; then
    echo "$failures integration check(s) failed. Output is in $OUT"
    exit 1
fi
echo "integration: all checks passed"
