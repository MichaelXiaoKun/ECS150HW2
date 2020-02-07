# Project-
### Project 2 Report
- The goal of this project is to simulate the user-level multi thread.
- The project has a series of important APIs which are stored in four different c files, and we include some tester files which are used to check if they are executing and working properly.
- 

#### Phase 1: Queue API and Testing
We've implemented Queue using Linkedlist structure. For each time of adding a new node to the head of linkedlist (i.e. each time of calling enqueue function), we increment the current length of linkedlist by 1. Also, for each time of deleting a new node from the linkedlist (i.e. each time of calling "dequeue" function or "delete" function), we decrease the length of linkedlist by 1.

The testing for Queue API involves checking if the return values of each API functions match what they're supposed to be. We created up to 4 difference cases to test whether those functions can handle all possibilities related to NULL address or empty queue.

#### Phase 2 & 3: uthread API
We've implemented uthread API functions via Queue and its API functions.

- The TCB struct and tid_
  - The TCB struct contains all info for context switch and threads scheduling.
  - tid_ plus one after each thread creation.


- uthread_create(uthread_func_t func, void *arg)
    - This function takes two arguments func and *arg, and it's implemented to create a new thread. The thread is created by constructing a variable of type "struct" called "TCB", which includes thread_state, context of type uthread_ctx_t, TID_KEY, TID, and retval (return value). 


- uthread_yield()
    - This function takes no argument, and it's implemented to stop the currently running thread and start running the next one that hasn't been run yet. The idea behind the implementation is that it keep tracks of each TCB from the head of queue to find the READY state that first appears. When such a state is found, we swap the state of the currently running thread with the thread we've just found, and we run into that thread immediately.

- uthread_join(uthread_t tid, int *retval)
    - This function takes two arguments "tid" and "*retval", the retval is the return value of the TCB that has just been finished executed. It consists a while loop that keep yield the TCB_queue until the tid dies. "Death" of tid happened in exit stage.The while loop structre guarantees the tid becomes zombie and it the function finally collect it.
    - 
- uthread_exit(int retval)
    - This function takes one argument "retval", which stands for return_value, and it's implemented to finish the execution of the currently running thread. The argument retval is used to collect the return value of the currently running thread for the purpose of being used in uthread_join(uthread_t tid, int *retval). The idea behind the implementation is that we change the running state of the current thread to ZOMBIE where the thread is no longer being called for regaining execution. Then, we unblock the target thread, iterate through the queue to find the TCB that has the READY state, and move that to the head of queue.

#### Phase 4: preemption
The preemption part is just the implementation of how the signal can be received periodically and triggers the activation of calling thread_yield() in order to prevent the threads from being unable to finish their executions and go to the next thread that has the READY state.
- preempt_start()
    - This function is called to initiate the timer for receiving the signal, and the idea behind the implementation is that it initializes the sigaction of type struct; meanwhile, we implement the function signal_handler(int signum), which is to be called immediately, once the alarm signal is received.
- preempt_enable()
    - This function is called to enable blocking the alarm signal. Once the signal is blocked, the program won't get notifed of calling uthread_yield(). The ideas behind the implementation is that we utilize sigprocmask function and some sigset_t variables. First we create a sigset_t-type variable, and call sigprocmask function with the argument SIG_UNBLOCK.
- preempt_disable()
    - This function is called to enable unblocking the alarm signal, which works in the opposite way of preempt_enable(), and the implementation is considerably similar to what is implemented in preempt_enable().

The testing for preemption is basically to check if one thread can automatically yield to another thread. If preemption works properly, it would periodically block or unblock the signal with the call of preempt_enable or preempt_disable, which calls uthread_yield function in a certain period of time. In my tester program, we remove all the uthread_yield in each thread functions. By doing that, the yield function has to be called from the signal_handler function, and we can see if the tester runs successfully and outputs the same results. Since the frequency is extremely large, we visualize the test results by repeatedly printing the statement for more than thousand times in each thread. When running the tester file, you can see that two statements are printed randomly.













    
