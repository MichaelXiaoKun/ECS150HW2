#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

void signal_handler(int signum) {
    signum = signum;
    uthread_yield();
}

sigset_t signal_set;
struct sigaction sa;
struct itimerval timer;

void preempt_disable(void)
{
	/* TODO Phase 4 */
	sigaddset(&signal_set, (unsigned int)SIGVTALRM);
	sigprocmask(SIG_BLOCK, &signal_set, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
    sigaddset(&signal_set, (unsigned int)SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
}

void preempt_start(void)
{
	/* TODO Phase 4 */
	sigemptyset(&signal_set);
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &signal_handler;
    sigaction(SIGVTALRM, &sa, NULL);
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100000 / HZ;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100000 / HZ;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

