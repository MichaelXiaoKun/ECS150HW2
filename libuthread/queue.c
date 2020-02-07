#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node{
  void* data;
  struct node* next;
  struct node* before;
} node;

struct queue {
	/* TODO Phase 1 */
	size_t length;
	struct node* head;
	struct node* rear;
	//may be more
};

node* create_node(void* data)
{
  node* result = (node*)malloc(sizeof(node));
  result->data = data;
  return result;
}

queue_t queue_create(void)
{
	/* TODO Phase 1 */
    queue_t queue =  malloc(sizeof(struct queue));
    if(queue){
      queue->length = 0;
      return queue;
    }else{
      return NULL;
    }

}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue == NULL || queue->length != 0){
      return -1;
	}

	free(queue);
  return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */

	 // error process
	 if (queue == NULL || data == NULL)
       return -1;

	 if(queue->length == 0){
       node* new_node = create_node(data);
       if(!new_node){
         return -1;
       }
       new_node->next = NULL;
       new_node->before = NULL;
	   queue->head = new_node;
	   queue->rear = new_node;
	   queue->length++;
	 } else{
       node* new_node = create_node(data);
       if(!new_node){
         return -1;
       }
       new_node->before = queue->rear;
       new_node->next = NULL;
       queue->rear->next = new_node;
       queue->rear = new_node;
       queue->length++;
	 }
  return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
  if (queue == NULL || data == NULL || queue->length == 0)
    return -1;

  node* next_head =  queue->head->next;
  *data =  queue->head->data;
  free(queue->head);
  queue->head = next_head;
  if(queue->length == 1){
    queue->rear = NULL;
  } else{
    queue->head->before = NULL;
  }

  queue->length--;
  return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue == NULL){
      return -1;
	}
    node* ptr =  queue->head;
	if(ptr == NULL){
      return -1;
	}

	/*
	 * go to the next until the target or the last
	 */
    while(ptr->data != data && ptr->next != NULL ){
      ptr = ptr->next;
    }
    // until the last one and the last one is not the
    if(ptr->data == data){
      //find
      if(ptr->before == NULL && ptr->next == NULL ){
        //only one find
        free(ptr);
        queue->head = NULL;
        queue->rear = NULL;
        queue->length --;
      } else if(ptr->before == NULL){
        // first one find
        queue->head = queue->head->next;
        queue->head->before = NULL;
        free(ptr);
        queue->length--;
      } else if (ptr->next == NULL){
        // last one find
        queue->rear = queue->rear->before;
        queue->rear->next = NULL;
        free(ptr);
        queue->length--;
      } else{
        // in the middle
        ptr->before->next = ptr->next;
        ptr->next->before = ptr->before;
        free(ptr);
        queue->length--;
      }
      return 0;
    } else{
      //not find
      return -1;
    }

}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
	if(queue == NULL || func == NULL){
      return -1;
	}
	node* ptr = queue->head;
  while (ptr){
    if((*func)(ptr->data,arg)==1){
      if(data != NULL){
        *data = ptr->data;
      }
      break;
    }
    ptr = ptr->next;
  }
  return 0;
}

int queue_length(queue_t queue)
{
  if(!queue){
    return -1;
  }
  return queue->length;
}

