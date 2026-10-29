# Custom Analysis: Academic Risk Index (ARI)

## Purpose

A single pass mark only says who has already failed. The ARI also warns about
students who are close to failing, or who are falling behind the rest of their
course. It gives every student a score from 0 (no risk) to 100 (highest risk), so
a tutor can see who to help first.

## Formula

```
deficit  = max(0, WATCH_MARK - grade) / WATCH_MARK      (0 to 1)
peer_gap = max(0, course_average - grade) / 100         (0 to 1)
ARI      = 70 * deficit + 30 * peer_gap                 (0 to 100)
```

- `WATCH_MARK` is 60. It is 10 points above the pass mark (50), so students who
  only just pass still get a small warning.
- `course_average` is the mean grade of everyone in the same course. Courses are
  matched with `strcmp`.
- Weights: 70% for distance below the watch mark (absolute performance) and 30%
  for trailing classmates (relative performance).

## Bands

| ARI | Band | Meaning |
|---|---|---|
| 0 | None | At or above 60 and not behind the course average |
| above 0, below 10 | Low | Slightly behind; keep an eye on them |
| 10 to below 25 | Moderate | Near or below the pass mark; offer support |
| 25 and above | High | Well below the pass mark; act now |

Any failing grade (below 50) scores at least 70 * 10/60 = 11.7, so it is never
rated None or Low.

## Algorithm

1. For each student (pointer traversal over the array):
   1. Work out the average grade of their course.
   2. Work out their ARI with the formula and pick the band.
2. Count students per band, work out the class mean ARI, and remember the student
   with the highest ARI.

This is O(n^2) in the number of students, because each course average is a scan of
the list. That is fine for class-sized data; the performance test checks 2,000 records.

## Worked examples

| Grade | Course avg | deficit | peer_gap | ARI | Band |
|---|---|---|---|---|---|
| 90 | 70 | 0 | 0 | 0.0 | None |
| 75 | 80 | 0 | 0.05 | 1.5 | Low |
| 55 | 60 | 5/60 | 0.05 | 7.3 | Low |
| 45 | 50 | 15/60 | 0.05 | 19.0 | Moderate |
| 30 | 70 | 30/60 | 0.40 | 47.0 | High |

## Reading the results

- **Mean ARI**: the overall health of the class. Under 10 is a healthy class.
- **Band counts**: how many students need attention, and how urgently.
- **Most at risk**: the first student to contact.
