#include <stdio.h>
#include "queue.h"
#include "dispatcher.h"
#include "debugging.h"

extern int task_counter;
extern task_t* current_task;
extern queue_t* ready_task_queue;


void dispatcher_body()
{
    task_t* next;
    while(task_counter > 0)
    {
        next = scheduler();
        if (next)
        {
            printf("%10s dispatcher_body: Found next task with id %d.\n", DEBUG, next->t_id);
            task_switch(next);
        }
        else
        {
            printf("%10s could not find next task.\n", WARNING);
        }
    }

    printf(OK"dispatcher_body: Exiting dispatcher_body.\n");
    task_exit(0);
}

task_t* scheduler()
{
    // Sleep time for debugging.
    int TIME = 3;

    printf("%10s scheduler: Entered.\n", DEBUG);
    DEBUG_SLEEP(TIME);

    // Removing and getting first element in queue for FCFS scheduling.
    task_t* next_task_in_queue = (task_t*) queue_remove(&ready_task_queue, ready_task_queue);
    if (next_task_in_queue)
    {
        queue_append(&ready_task_queue, next_task_in_queue);
        printf("%10s scheduler: returning task %d.\n", DEBUG, next_task_in_queue->t_id);
        DEBUG_SLEEP(TIME);
    }
    else
    {
        printf("%10s scheduler: Next task found has address NULL.\n", WARNING);
    }

    return next_task_in_queue;
}
