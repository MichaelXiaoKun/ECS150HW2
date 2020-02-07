#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>
#include "preempt.h"

int thread1(void* arg)
{
    for(int i = 0; i < 10000000; ++i) {
        printf("sayonara\n");
	}
	return 0;
}

int main(void)
{
    uthread_create(thread1, NULL);
    for(int i = 0; i < 1000000; ++i) {
        printf("Greetings my friend\n");
	}
    return 0;
}
