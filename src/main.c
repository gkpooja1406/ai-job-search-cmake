#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "job_matcher.h"

#define MAX_TEXT 1024

static int read_first_line(const char *filename, char *buffer, size_t size) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: unable to open %s\n", filename);
        return 0;
    }

    if (fgets(buffer, (int)size, file) == NULL) {
        fprintf(stderr, "Error: unable to read %s\n", filename);
        fclose(file);
        return 0;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    fclose(file);
    return 1;
}

int main(int argc, char *argv[]) {
    const char *resume_file = "data/resume_skills.txt";
    const char *job_file = "data/job_skills.txt";
    char resume_skills[MAX_TEXT];
    char job_skills[MAX_TEXT];

    if (argc == 3) {
        resume_file = argv[1];
        job_file = argv[2];
    }

    printf("AI Job Search - Resume Skill Matcher\n");
    printf("====================================\n");

    if (!read_first_line(resume_file, resume_skills, sizeof(resume_skills))) {
        return EXIT_FAILURE;
    }

    if (!read_first_line(job_file, job_skills, sizeof(job_skills))) {
        return EXIT_FAILURE;
    }

    printf("Resume skills   : %s\n", resume_skills);
    printf("Required skills : %s\n", job_skills);

    MatchResult result = calculate_match(resume_skills, job_skills);
    print_matching_skills(resume_skills, job_skills);

    printf("\nResult:\n");
    printf("  Matched skills : %d\n", result.matched);
    printf("  Required skills: %d\n", result.total_required);
    printf("  Match score    : %.2f%%\n", result.percentage);

    if (result.percentage >= 70.0) {
        printf("  Recommendation : Strong match - apply for this job.\n");
    } else if (result.percentage >= 40.0) {
        printf("  Recommendation : Partial match - improve missing skills.\n");
    } else {
        printf("  Recommendation : Low match - consider another role.\n");
    }

    return EXIT_SUCCESS;
}
