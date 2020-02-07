#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)			\
do {						\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {				\
		printf("PASS\n");		\
	} else	{				\
		printf("FAIL\n");		\
		exit(1);			\
	}					\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stdout, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stdout, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

void test_queue_destory(void)
{
    fprintf(stdout, "*** TEST queue_destory ***\n");
    queue_t q;

    //test deallocating empty queue.
    q = queue_create();
    TEST_ASSERT(queue_destroy(q) == 0);

    //test deallocating non-empty queue.
    int data = 3;
    q = queue_create();
    queue_enqueue(q, &data);
    TEST_ASSERT(queue_destroy(q) == -1);

    //test deallocating NULL;
    q = NULL;
    TEST_ASSERT(queue_destroy(q) == -1);
}

void test_queue_enqueue(void)
{
    fprintf(stdout, "*** TEST queue_enqueue ***\n");
    queue_t q;
    int data = 3;
    int test_result;

    //test enqueuing address that is contained in data in q.
    q = queue_create();
    test_result = queue_enqueue(q, &data);
    TEST_ASSERT(test_result == 0);

    //test enqueuing address that is contained in data in NULL.
    test_result = queue_enqueue(NULL, &data);
    TEST_ASSERT(test_result == -1);

    //test enqueuing NULL address in q.
    test_result = queue_enqueue(q, NULL);
    TEST_ASSERT(test_result == -1);
}

void test_queue_dequeue(void)
{
    fprintf(stdout, "*** TEST queue_dequeue ***\n");
    queue_t q;
    int data[] = {1, 2, 3};
    int test_result;

    //test empty queue;
    q = queue_create();
    test_result = queue_dequeue(q, (void**)&data[0]);
    TEST_ASSERT(test_result == -1);

    //test dequeuing q by replacing the oldest item with "data".
    queue_enqueue(q, &data[0]);
    test_result = queue_dequeue(q, (void**)&data[1]);
    TEST_ASSERT(test_result == 0);

    //test dequeuing q by replacing the oldest item with "NULL".
    test_result = queue_dequeue(q, NULL);
    TEST_ASSERT(test_result == -1);

    //test dequeuing null.
    test_result = queue_dequeue(NULL, (void**)&data[2]);
    TEST_ASSERT(test_result == -1);
}

void test_queue_delete(void)
{
    fprintf(stdout, "*** TEST queue_delete ***");
    queue_t q;
    int data[] = {1, 2, 3};
    int test_result;

    //test deleting data in empty queue.
    q = queue_create();
    test_result = queue_delete(q, &data[0]);
    TEST_ASSERT( test_result == -1);

    //test deleting data in non_empty queue which contains that data.
    queue_enqueue(q, &data[1]);
    test_result = queue_delete(q, &data[1]);
    TEST_ASSERT(test_result == 0);

    //test deleting data in null.
    test_result = queue_delete(NULL, &data[2]);
    TEST_ASSERT(test_result == -1);

    //test deleting NULL in q.
    test_result = queue_delete(q, NULL);
    TEST_ASSERT(test_result == -1);
}
/*
void test_queue_iterate(void) {
    fprintf(stdout, "*** TEST queue_iterate ***\n");
    queue_t q;
    int data[] = {1, 2, 3};
    int test_result;
    queue_func_t func();
    q = queue_create();

    //test calling queue_iterate.
    test_result = queue_iterate(q, func(), NULL, (void**)&data[0]);
    TEST_ASSERT(test_result == 0);

    //test calling queue_iterate with NULL queue.
    test_result = queue_iterate(NULL, func(), NULL, (void**)&data[0]);
    TEST_ASSERT(test_result == -1);

    //test calling queue_iterate with NULL data.
    test_result = queue_iterate(q, func(), NULL, NULL);
    TEST_ASSERT(test_result == -1);
}*/

void test_queue_length(void) {
    fprintf(stdout, "*** TEST queue_length ***\n");
    queue_t q;
    q = queue_create();
    int test_result;
    for (int i = 0; i < 25; i++) {
        queue_enqueue(q, &i);
    }

    //test returning the length of queue.
    test_result = queue_length(q);
    TEST_ASSERT(test_result == 25);

    //test returning -1 if q = NULL.
    test_result = queue_length(NULL);
    TEST_ASSERT(test_result == -1);
}

int main(void)
{
	test_create();
	test_queue_simple();
    test_queue_destory();
    test_queue_enqueue();
    test_queue_dequeue();
    test_queue_delete();
    //test_queue_iterate();

	return 0;
}
