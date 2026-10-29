#include "report.h"
#include "analysis.h"
#include "stats.h"

#include <stdio.h>

static void print_risk_totals(const RiskSummary *summary)
{
    printf("Risk bands: None %zu | Low %zu | Moderate %zu | High %zu\n",
           summary->level_counts[RISK_NONE], summary->level_counts[RISK_LOW],
           summary->level_counts[RISK_MODERATE], summary->level_counts[RISK_HIGH]);
    printf("Mean risk index: %.1f\n", summary->mean_index);
    if (summary->most_at_risk != NULL)
        printf("Most at risk: %s (ID %d), index %.1f\n", summary->most_at_risk->name,
               summary->most_at_risk->id, summary->highest_index);
}

void print_risk_report(const StudentList *list)
{
    printf("\nAcademic Risk Index (0 = no risk, 100 = highest risk)\n");
    printf("ARI = 70 x max(0, %.0f - grade)/%.0f + 30 x max(0, course avg - grade)/100\n",
           WATCH_MARK, WATCH_MARK);
    if (list->count == 0) {
        printf("No student records to analyse.\n");
        return;
    }

    printf("%-7s %-24s %6s %10s %6s  %-8s\n", "ID", "Name", "Grade", "Course avg", "ARI",
           "Band");
    for (const Student *p = list->items, *end = p + list->count; p < end; p++) {
        float avg = course_average(list, p->course);
        float index = risk_index(p->grade, avg);
        printf("%-7d %-24.24s %6.1f %10.1f %6.1f  %-8s\n", p->id, p->name, p->grade, avg,
               index, risk_level_name(risk_level(index)));
    }

    RiskSummary summary = risk_summarise(list);
    print_risk_totals(&summary);
    printf("Bands: None = 0, Low < 10, Moderate < 25, High >= 25.\n");
}

void print_summary_report(const StudentList *list)
{
    printf("\n===== Summary Report =====\n");
    printf("Total student records: %zu\n", list->count);
    if (list->count == 0) {
        printf("No records, so there is nothing more to report.\n");
        return;
    }

    const Student *top = stats_top(list);
    const Student *low = stats_lowest(list);
    size_t passing = stats_count_passing(list, PASS_MARK);

    printf("Average grade: %.2f\n", stats_average(list));
    printf("Highest grade: %.1f (%s, ID %d)\n", top->grade, top->name, top->id);
    printf("Lowest grade: %.1f (%s, ID %d)\n", low->grade, low->name, low->id);
    printf("Pass mark: %.0f\n", PASS_MARK);
    printf("Students passing (>= %.0f): %zu\n", PASS_MARK, passing);
    printf("Students below pass mark: %zu\n", list->count - passing);

    printf("\nCustom analysis - Academic Risk Index\n");
    RiskSummary summary = risk_summarise(list);
    print_risk_totals(&summary);
    printf("==========================\n");
}
