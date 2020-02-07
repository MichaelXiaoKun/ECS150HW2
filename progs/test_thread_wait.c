//
// Created by Zhecheng Huang on 2/6/20.
//]
#include <stdio.h>
#include <uthread.h>
uthread_t tid[3];
int thread3(void *arg) {
  int ret;
  printf("thread3\n");
  return 3;
}

int thread2(void *arg) {
  int ret;
  printf("thread2\n");
  uthread_join(tid[0], &ret);
  printf("thread1 returned %d\n", ret);
  uthread_join(tid[2], &ret);
  printf("thread3 returned %d\n", ret);
  return 2;
}

int thread1(void *arg) {
  tid[1] = uthread_create(thread2, NULL);
  tid[2] = uthread_create(thread3,NULL);
  printf("thread1\n");
  return 1;
}

int main(void) {
  int ret;
  tid[0] = uthread_create(thread1, NULL);
  uthread_yield();
  uthread_join(tid[1], &ret);
  printf("thread2 returned %d\n", ret);
  return 0;
}
/*
int thread1(void){

  printf("hi, I am thread1");
  fflush(stdout);
  return 1;
}

int thread2(void){
  printf("hi, I am thread2");
  uthread_yield();
  fflush(stdout);
  return 2;
}


int main(void)
{
  int ret,ret2;
  uthread_t tid,tid2;
  tid2 = uthread_create(thread2, &ret2);
  tid = uthread_create(thread1, &ret);
  uthread_join(tid2,&ret2);
  uthread_join(tid, &ret);

  return 0;
}
 */