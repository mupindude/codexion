
#include "../../codexion.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define FAIL_CODER 1
#define FAIL_MONITOR 2
#define FAIL_SCHEDULER 3
#define FAIL_DONGLE 4
#define FAIL_COND 5

static int failure_mode;
static int coder_create_attempts;
static int scheduler_create_attempts;
static int monitor_create_attempts;

int __real_pthread_create(pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *), void *arg);

int __real_pthread_cond_init(pthread_cond_t *cond,
    const pthread_condattr_t *attr);

int __real_pthread_mutex_init(pthread_mutex_t *mutex,
    const pthread_mutexattr_t *attr);

int __wrap_pthread_create(pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *), void *arg)
{
    if (start_routine == coder_routine)
    {
        coder_create_attempts++;
        if (failure_mode == FAIL_CODER
            && coder_create_attempts == 3)
        {
            printf("TEST: Simulating coder thread creation failure\n");
            return (EAGAIN);
        }
    }
	if (start_routine == scheduler_routine)
	{
		scheduler_create_attempts++;
		if (failure_mode == FAIL_SCHEDULER)
		{
			printf("TEST: Simulating scheduler thread creation failure\n");
			return (EAGAIN);
		}
	}
    if (start_routine == monitor_routine)
    {
        monitor_create_attempts++;
        if (failure_mode == FAIL_MONITOR)
        {
            printf("TEST: Simulating monitor thread creation failure\n");
            return (EAGAIN);
        }
    }
    return (__real_pthread_create(thread, attr, start_routine, arg));
}

int __wrap_pthread_mutex_init(pthread_mutex_t *mutex,
    const pthread_mutexattr_t *attr)
{
    static int dongle_mutex_attempts;

    if (failure_mode == FAIL_DONGLE
        && dongle_mutex_attempts < 3)
    {
        dongle_mutex_attempts++;
        if (dongle_mutex_attempts == 3)
        {
            printf("TEST: Simulating dongle mutex initialization failure\n");
            return (EAGAIN);
        }
    }
    return (__real_pthread_mutex_init(mutex, attr));
}

int __wrap_pthread_cond_init(pthread_cond_t *cond,
    const pthread_condattr_t *attr)
{
    if (failure_mode == FAIL_COND)
    {
        printf("TEST: Simulating condition variable initialization failure\n");
        return (EAGAIN);
    }
    return (__real_pthread_cond_init(cond, attr));
}

static void set_test_args(t_args *args)
{
    args->nb_coders = 4;
    args->time_to_burnout = 10000;
    args->time_to_compile = 10;
    args->time_to_debug = 10;
    args->time_to_refactor = 10;
    args->nb_compiles = 5;
    args->dongle_cooldown = 10;
    args->scheduler = FIFO;
}

static int test_coder_failure(void)
{
    t_data data;
    t_args args;
    int created_coders;
    int result;

    failure_mode = FAIL_CODER;
    coder_create_attempts = 0;
    scheduler_create_attempts = 0;
    monitor_create_attempts = 0;
    created_coders = 0;
    set_test_args(&args);
    if (init_data(&data, &args) != 0)
        return (printf("FAIL: init_data failed\n"), 1);
    if (start_simulation(&data) != 0)
        return (printf("FAIL: start_simulation failed\n"), 1);
    result = start_coders(&data, &created_coders);
    if (result == 0 || created_coders != 2
        || coder_create_attempts != 3)
    {
        printf("FAIL: Unexpected coder creation results\n");
        join_threads(&data, created_coders);
        destroy_data(&data);
        return (1);
    }
    printf("TEST: Coder creation failed after 2 successful threads\n");
    join_threads(&data, created_coders);
    destroy_data(&data);
    printf("PASS: Coder creation failure handled\n");
    return (0);
}

static int test_monitor_failure(void)
{
    t_data data;
    t_args args;
    int result;

    failure_mode = FAIL_MONITOR;
    coder_create_attempts = 0;
    scheduler_create_attempts = 0;
    monitor_create_attempts = 0;
    set_test_args(&args);
    if (init_data(&data, &args) != 0)
        return (printf("FAIL: init_data failed\n"), 1);
    result = start_simulation(&data);
    if (result == 0 || scheduler_create_attempts != 1
        || monitor_create_attempts != 1)
    {
        printf("FAIL: Unexpected monitor failure results\n");
        return (1);
    }
    printf("TEST: Monitor creation failed after scheduler started\n");
    printf("PASS: Monitor creation failure handled\n");
    return (0);
}

static int test_scheduler_failure(void)
{
    t_data  data;
    t_args  args;
    int     result;

    failure_mode = FAIL_SCHEDULER;
    coder_create_attempts = 0;
    scheduler_create_attempts = 0;
    monitor_create_attempts = 0;
    set_test_args(&args);
    if (init_data(&data, &args) != 0)
        return (printf("FAIL: init_data failed\n"), 1);
    result = start_simulation(&data);
    if (result == 0 || scheduler_create_attempts != 1
        || monitor_create_attempts != 0)
    {
        printf("FAIL: Unexpected scheduler failure results\n");
        return (1);
    }
    printf("TEST: Scheduler creation failed as expected\n");
    printf("PASS: Scheduler creation failure handled\n");
    return (0);
}

static int test_dongle_failure(void)
{
    t_data  data;
    t_args  args;
    int     result;

    failure_mode = FAIL_DONGLE;
    coder_create_attempts = 0;
    scheduler_create_attempts = 0;
    monitor_create_attempts = 0;
    set_test_args(&args);
    result = init_data(&data, &args);
    if (result == 0)
    {
        printf("FAIL: Dongle initialization unexpectedly succeeded\n");
        destroy_data(&data);
        return (1);
    }
    printf("TEST: Dongle initialization failed as expected\n");
    printf("PASS: Dongle initialization failure handled\n");
    return (0);
}

static int test_cond_failure(void)
{
    t_data  data;
    t_args  args;
    int     result;

    failure_mode = FAIL_COND;
    set_test_args(&args);
    result = init_data(&data, &args);
    if (result == 0)
    {
        printf("FAIL: Condition initialization unexpectedly succeeded\n");
        destroy_data(&data);
        return (1);
    }
    printf("TEST: Condition initialization failed as expected\n");
    printf("PASS: Condition initialization failure handled\n");
    return (0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s coder|monitor|scheduler|dongle\n", argv[0]);
        return (1);
    }
    if (strcmp(argv[1], "coder") == 0)
        return (test_coder_failure());
    if (strcmp(argv[1], "monitor") == 0)
	{
        return (test_monitor_failure());
	}
	if (strcmp(argv[1], "scheduler") == 0)
    	return (test_scheduler_failure());
	if (strcmp(argv[1], "dongle") == 0)
    	return (test_dongle_failure());
	if (strcmp(argv[1], "cond") == 0)
    	return (test_cond_failure());
    printf("Unknown test scenario: %s\n", argv[1]);
    return (1);
}
