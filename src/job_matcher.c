#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "job_matcher.h"

#define MAX_SKILLS 50
#define MAX_SKILL_LENGTH 64

static void trim(char *text) {
    char *start = text;
    char *end;

    while (isspace((unsigned char)*start)) {
        start++;
    }

    if (start != text) {
        memmove(text, start, strlen(start) + 1);
    }

    end = text + strlen(text);
    while (end > text && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    *end = '\0';
}

static void to_lowercase(char *text) {
    while (*text) {
        *text = (char)tolower((unsigned char)*text);
        text++;
    }
}

static int contains_skill(const char *skills_text, const char *skill) {
    char text_copy[1024];
    char skill_copy[MAX_SKILL_LENGTH];

    snprintf(text_copy, sizeof(text_copy), "%s", skills_text);
    snprintf(skill_copy, sizeof(skill_copy), "%s", skill);

    to_lowercase(text_copy);
    to_lowercase(skill_copy);

    return strstr(text_copy, skill_copy) != NULL;
}

MatchResult calculate_match(const char *resume_skills, const char *job_skills) {
    MatchResult result = {0, 0, 0.0};
    char job_copy[1024];
    char *token;

    snprintf(job_copy, sizeof(job_copy), "%s", job_skills);

    token = strtok(job_copy, ",");
    while (token != NULL && result.total_required < MAX_SKILLS) {
        trim(token);

        if (strlen(token) > 0) {
            result.total_required++;
            if (contains_skill(resume_skills, token)) {
                result.matched++;
            }
        }

        token = strtok(NULL, ",");
    }

    if (result.total_required > 0) {
        result.percentage =
            ((double)result.matched / (double)result.total_required) * 100.0;
    }

    return result;
}

void print_matching_skills(const char *resume_skills, const char *job_skills) {
    char job_copy[1024];
    char *token;
    int found = 0;

    snprintf(job_copy, sizeof(job_copy), "%s", job_skills);
    printf("\nMatching skills:\n");

    token = strtok(job_copy, ",");
    while (token != NULL) {
        trim(token);

        if (strlen(token) > 0 && contains_skill(resume_skills, token)) {
            printf("  - %s\n", token);
            found = 1;
        }

        token = strtok(NULL, ",");
    }

    if (!found) {
        printf("  No matching skills found.\n");
    }
}
