#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "student.h"

// Academic Risk Index (ARI): a 0-100 score, higher means more at risk.
//   deficit  = max(0, WATCH_MARK - grade) / WATCH_MARK
//   peer_gap = max(0, course_average - grade) / 100
//   ARI      = 70 * deficit + 30 * peer_gap
#define WATCH_MARK 60.0f      // grades below this start to count as risk
#define DEFICIT_WEIGHT 70.0f  // share of the score from distance below WATCH_MARK
#define PEER_WEIGHT 30.0f     // share of the score from trailing the course average

typedef enum { RISK_NONE, RISK_LOW, RISK_MODERATE, RISK_HIGH, RISK_LEVEL_COUNT } RiskLevel;

// Class-wide results of the analysis.
typedef struct RiskSummary {
    size_t level_counts[RISK_LEVEL_COUNT];
    float mean_index;
    const Student *most_at_risk; // NULL when the list is empty
    float highest_index;
} RiskSummary;

// Average grade of everyone in the same course (matched with strcmp).
float course_average(const StudentList *list, const char *course);

// The ARI for one grade, given the average of that student's course.
float risk_index(float grade, float course_avg);

// Band for a score: 0 None, below 10 Low, below 25 Moderate, otherwise High.
RiskLevel risk_level(float index);
const char *risk_level_name(RiskLevel level);

// Scores every student and totals the bands.
RiskSummary risk_summarise(const StudentList *list);

#endif
