#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CHECK(cmd) if (cmd) { perror(#cmd); return 1; }

extern char **environ;

struct stats {
    long average;
    long min;
    long max;
};

struct stats stats(long values[], int n) {
    long min = values[0];
    long max = values[0];
    long sum = 0L;
    for (int i = 0; i < n; i++) {
        long value = values[i];
        if (value < min)
            min = value;
        if (value > max)
            max = value;
        sum += value;
    }
    struct stats s = {sum/n, min, max};
    return s;
}

int main(int argc, char *const argv[]) {
    char *self = argv[0];
    argc--;
    argv++;

    int n = 1;
    if (argc > 0 && argv[0][0] >= '1' && argv[0][0] <= '9') {
        n = atoi(argv[0]);
        argc--;
        argv++;
    }

    if (argc < 1) {
        printf("Usage:\n");
        printf("  %s [REPEAT=1] /full/path/to/command ARGS...\n", self);
        printf("  %s [REPEAT=1] `which command` ARGS...\n", self);
        return 1;
    }

    struct rusage usage;
    struct timespec before, after;
    long realtime[n];
    long memory[n];
    pid_t pid;
    int status;
    long ns_to_ms = 1000000L;

    /* run once for warming caches */
    CHECK(posix_spawn(&pid, argv[0], NULL, NULL, argv, environ));
    CHECK(waitpid(pid, &status, 0) == -1);

    for (int i = 0; i < n; i++) {
        CHECK(clock_gettime(CLOCK_MONOTONIC, &before));

        CHECK(posix_spawn(&pid, argv[0], NULL, NULL, argv, environ));
        CHECK(waitpid(pid, &status, 0) == -1);

        CHECK(clock_gettime(CLOCK_MONOTONIC, &after));
        CHECK(getrusage(RUSAGE_CHILDREN, &usage));

        realtime[i] = (after.tv_sec-before.tv_sec) * 1000000000L +
                      (long) (after.tv_nsec-before.tv_nsec);
        printf("%ld ", (realtime[i]/ns_to_ms));

        memory[i] = usage.ru_maxrss;
    }
    printf("\n");

    struct stats ts = stats(realtime, n);
    struct stats ms = stats(memory, n);

#ifdef __APPLE__
    long rss_to_mb = 1024L * 1024L;
#else
    long rss_to_mb = 1024L;
#endif

    printf("%ld [%ld-%ld] ms\n", (ts.average/ns_to_ms), (ts.min/ns_to_ms), (ts.max/ns_to_ms));
    printf("%ld [%ld-%ld] MB\n", (ms.average/rss_to_mb), (ms.min/rss_to_mb), (ms.max/rss_to_mb));

    return 0;
}
