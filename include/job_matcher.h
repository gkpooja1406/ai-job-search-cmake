#ifndef JOB_MATCHER_H
#define JOB_MATCHER_H

typedef struct {
    int matched;
    int total_required;
    double percentage;
} MatchResult;

MatchResult calculate_match(const char *resume_skills, const char *job_skills);
void print_matching_skills(const char *resume_skills, const char *job_skills);

#endif
