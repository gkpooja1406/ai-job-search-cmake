#include <assert.h>
#include <stdio.h>
#include "job_matcher.h"

int main(void) {
    MatchResult result1 = calculate_match(
        "Python, SQL, AWS, Jenkins, Git, Linux",
        "Python, SQL, AWS, Docker, Jenkins"
    );

    assert(result1.matched == 4);
    assert(result1.total_required == 5);
    assert(result1.percentage > 79.0 && result1.percentage < 81.0);

    MatchResult result2 = calculate_match(
        "Java, Maven, Git",
        "Python, SQL, AWS"
    );

    assert(result2.matched == 0);
    assert(result2.total_required == 3);
    assert(result2.percentage == 0.0);

    printf("All tests passed successfully.\n");
    return 0;
}
