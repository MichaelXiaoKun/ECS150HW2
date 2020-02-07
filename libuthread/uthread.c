#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"
#define  USHRT_MAX 10000//enough?

/* TODO Phase 2 */
queue_t q = NULL;

uthread_t tid_ = 1;

typedef enum {
  READY,
  RUNNING,
  BLOCK,
  ZOMBIE
} STATE;

typedef struct TCB {
  STATE thread_state;
  uthread_ctx_t *context;
  int TID_KEY;
  int TID;
  int retval;
} TCB;

TCB *running;

int find_tid_ready(void *data, void *args) {
  TCB *ptr = data;
  if (ptr->TID == *((int *) args)&& ptr->thread_state == READY) {
    return 1;
  }
  return 0;
}


int find_tid(void *data, void *args) {
  TCB *ptr = data;
  if (ptr->TID == *((int *) args)) {
    return 1;
  }
  return 0;
}

int find_tid_to_unblocked(void *data, void *args) {
  TCB *ptr = data;
  if (ptr->TID == *((int *) args) && ptr->thread_state == BLOCK) {
    return 1;
  }
  return 0;
}

int find_tid_zombie(void *data, void *args) {
  TCB *ptr = data;
  if (ptr->TID == *((int *) args) && ptr->thread_state == ZOMBIE) {
    return 1;
  }
  return 0;
}

static int find_next_ready(void *data, void *arg) {
  arg++;
  TCB *ptr = data;
  if (ptr->thread_state == READY) {
    return 1;
  }
  return 0;
}

void unblock() {
  //unblock target thread
  //does not do context switch, do it outside
  TCB *to_unblock = NULL;
  queue_iterate(q, find_tid_to_unblocked, &running->TID_KEY, (void**)&to_unblock);
  //do not always works so check before excutes
  if (to_unblock) {
    to_unblock->thread_state = READY;
    //place at the rear of q
    queue_delete(q, to_unblock);
    queue_enqueue(q, to_unblock);
  }
}

void destroy_thread(TCB* target){
  //delete from q
  queue_delete(q,target);
  //delete it self
  free(target->context->uc_stack.ss_sp);
  free(target->context);
  free(target);
}

void block() {
  //block running thread and find next to run



}

void uthread_yield(void) {
  /* TODO Phase 2 */
  TCB *next_ready = NULL;
  TCB *copy = NULL;

  queue_iterate(q, find_next_ready, NULL, (void**)&next_ready);

  if (next_ready) {
    queue_delete(q, next_ready);
  }

  next_ready->thread_state = RUNNING;

  running->thread_state = READY;

  queue_enqueue(q, next_ready);

  copy = running;

  running = next_ready;

  uthread_ctx_switch(copy->context, running->context);
}

uthread_t uthread_self(void) {
  /* TODO Phase 2 */
  return running->TID;
}

int set_up(void) {
  q = queue_create();
  running = malloc(sizeof(TCB));
  running->TID = 0;
  running->TID_KEY = -1;
  running->thread_state = RUNNING;
  running->context = malloc(sizeof(uthread_ctx_t));
  queue_enqueue(q, running);
  if (!running || !running->context || !q) {
    return -1;
  }
  preempt_start();
  return 0;
}

int uthread_create(uthread_func_t func, void *arg) {
  /* TODO Phase 2 */
  if (tid_ >= USHRT_MAX){
    fprintf(stderr, "%s", "threads overflow\n ");
    fflush(stderr);
    return -1;
  }

    if (!q) {
      //create queue if q does not exist
      if (set_up() == -1) {
        fprintf(stderr, "%s", "mem allocation error\n ");
        fflush(stderr);
        return -1;
      }
    }

  uthread_ctx_t *uctx = malloc(sizeof(uthread_ctx_t));
  if (uctx == NULL) {
    fprintf(stderr, "%s", "mem allocation error\n ");
    fflush(stderr);
    return -1;
  }
  void *vaid_stack_top = uthread_ctx_alloc_stack();

  if (vaid_stack_top == NULL) {
    fprintf(stderr, "%s", "can not allocate thread stack\n ");
    fflush(stderr);
    return -1;
  }

  uthread_ctx_init(uctx, vaid_stack_top, func, arg);
  TCB *thread_TCB = malloc(sizeof(TCB));
  thread_TCB->context = uctx;
  thread_TCB->TID = tid_;
  thread_TCB->TID_KEY = -1;
  tid_++;
  thread_TCB->thread_state = READY;
  queue_enqueue(q, thread_TCB);

  /*
  TCB* next_ready;
  TCB* copy;

  queue_dequeue(q,&next_ready);

  next_ready->thread_state = RUNNING;

  running->thread_state = READY;

  queue_enqueue(q,running);

  copy = running;

  running = next_ready;


  uthread_ctx_switch(copy->context,running->context);
  */
  return thread_TCB->TID;
}

void uthread_exit(int retval) {

  TCB *next_ready;
  TCB *copy;

  running->thread_state = ZOMBIE;
  running->retval = retval;

  //unblock target thread
  unblock();
  queue_iterate(q, find_next_ready, NULL, (void**)&next_ready);
  copy = running;
  running = next_ready;
  queue_delete(q, copy);
  queue_enqueue(q, copy);
  uthread_ctx_switch(copy->context, running->context);
}

int uthread_join(uthread_t tid, int *retval) {
  TCB *to_join;

  if (tid == 0) {
    fprintf(stderr, "%s", "Join main thread\n");
    fflush(stderr);
    return -1;
  }

  if (tid == running->TID) {
    fprintf(stderr, "%s", "Join it self\n");
    fflush(stderr);
    return -1;
  }

  queue_iterate(q, find_tid, &tid, (void**)&to_join);

  if (!to_join) {
    fprintf(stderr, "%s", "Tid does not exist\n");
    fflush(stderr);
    return -1;
  }

  if (to_join->TID_KEY != -1) {
    fprintf(stderr, "%s", "Tid has been joined\n");
    fflush(stderr);
    return -1;
  }

  //is t2 ready
  while (1){

    if(to_join->thread_state == ZOMBIE){
      if(retval){
        *retval = to_join->retval;
      }
      queue_delete(q, running);
      queue_enqueue(q, running);
      destroy_thread(to_join);
      break;
    } else if (to_join->thread_state == BLOCK){
      to_join->TID_KEY = running->TID;
      running->thread_state = BLOCK;
      queue_delete(q, running);
      queue_enqueue(q,running);
      uthread_yield();
    } else{
      to_join->TID_KEY = running->TID;
      running->thread_state = BLOCK;
      queue_delete(q, running);
      queue_enqueue(q,running);
      uthread_yield();
    }
  }
  return 0;
}

