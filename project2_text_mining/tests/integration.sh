#!/bin/sh
# Runs a scripted session (samples/demo_input.txt) through the program and checks the output.
# Usage: sh tests/integration.sh <program>
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

# Built-in paragraph.
expect "Project: Dynamic Text Mining and Linguistic Pattern Analyzer"
expect "Source: Moby-Dick; or, The Whale"
expect "Loaded the built-in paragraph: 201 words, 134 distinct."
expect "Longest word: \"involuntarily\" (13 characters)"
expect "Most frequent word: \"the\", 10 times"
expect "Word statistics sorted By frequency (most frequent first)."
expect "Word statistics sorted Alphabetically (a to z)."
expect "\"whale\" does not appear in the text."
expect "\"i\" appears 9 time(s)"
expect "\"!!!\" contains no letters or digits."
expect "Lexical Diversity Score: 66.7 / 100 -> High (varied)"
expect "Text buffer address:"
expect "Word statistics array:"
expect "Selected word-stat entry:"

# Pasted paragraph: punctuation, repeated words, a long word and a 599-character line.
expect "(read 741 characters)"
expect "Loaded \"Punctuation Test\": 119 words, 15 distinct."
expect "Note: this text has fewer than 150 words."
expect "Most frequent word: \"again\" (100 times)"
expect "Words truncated to 39 characters: 1"
expect "Longest word: \"pneumonoultramicroscopicsilicovolcanoco\" (45 characters)"

# Empty paste keeps the previous text.
expect "Empty text, keeping the previous paragraph."
expect "Source: Punctuation Test"
expect "All memory released. Goodbye."

# Word rules: these must be single words in the statistics table.
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
