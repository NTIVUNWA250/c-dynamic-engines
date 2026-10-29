#include "analysis.h"
#include "log.h"

#include <string.h>

float course_average(const StudentList *list, const char *course)
{
    double total = 0.0;
    size_t members = 0;
    for (const Student *p = list->items, *end = p + list->count; p < end; p++) {
        if (strcmp(p->course, course) == 0) {
            total += p->grade;
            members++;
        }
    }
    return members == 0 ? 0.0f : (float)(total / (double)members);
}

float risk_index(float grade, float course_avg)
{
    float deficit = grade < WATCH_MARK ? (WATCH_MARK - grade) / WATCH_MARK : 0.0f;
    float peer_gap = course_avg > grade ? (course_avg - grade) / 100.0f : 0.0f;
    return DEFICIT_WEIGHT * deficit + PEER_WEIGHT * peer_gap;
}

RiskLevel risk_level(float index)
{
    if (index <= 0.0f)
        return RISK_NONE;
    if (index < 10.0f)
        return RISK_LOW;
    if (index < 25.0f)
        return RISK_MODERATE;
    return RISK_HIGH;
}

const char *risk_level_name(RiskLevel level)
{
    static const char *NAMES[RISK_LEVEL_COUNT] = {"None", "Low", "Moderate", "High"};
    return level < RISK_LEVEL_COUNT ? NAMES[level] : "?";
}

RiskSummary risk_summarise(const StudentList *list)
{
    RiskSummary summary = {0};
    double total = 0.0;

    for (const Student *p = list->items, *end = p + list->count; p < end; p++) {
        float index = risk_index(p->grade, course_average(list, p->course));
        summary.level_counts[risk_level(index)]++;
        total += index;
        if (summary.most_at_risk == NULL || index > summary.highest_index) {
            summary.most_at_risk = p;
            summary.highest_index = index;
        }
    }
    if (list->count > 0)
        summary.mean_index = (float)(total / (double)list->count);

    log_msg(LOG_INFO, "analysis", "risk index computed for %zu students, mean %.2f",
            list->count, summary.mean_index);
    return summary;
}
