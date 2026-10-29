#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "student.h"

#define WATCH_MARK 60.0f
#define DEFICIT_WEIGHT 70.0f
#define PEER_WEIGHT 30.0f

typedef enum { RISK_NONE, RISK_LOW, RISK_MODERATE, RISK_HIGH, RISK_LEVEL_COUNT } RiskLevel;

typedef struct RiskSummary {
    size_t level_counts[RISK_LEVEL_COUNT];
    float mean_index;
    const Student *most_at_risk;
    float highest_index;
} RiskSummary;

float course_average(const StudentList *list, const char *course);

float risk_index(float grade, float course_avg);

RiskLevel risk_level(float index);
const char *risk_level_name(RiskLevel level);

RiskSummary risk_summarise(const StudentList *list);

#endif
