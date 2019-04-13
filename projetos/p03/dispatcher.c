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
            task_switch(next);
        }
        else
        {
            printf(WARNING"could not find next task.\n");
        }
    }

    task_exit(0);
}

task_t* scheduler()
{
    printf(DEBUG"scheduler: Entered.\n");
    sleep(1);

    // Removing and getting first element in queue for FCFS scheduling.
    queue_t* next_task_in_queue = queue_remove(&ready_task_queue, ready_task_queue);
    

    // if (current_task)
    // {
    //     do
    //     {
    //         if ((task_t*) current_task_in_queue == current_task)
    //         {
    //             printf(DEBUG"scheduler: Found task %d as current task.\n", ((task_t*)current_task_in_queue)->t_id);
    //             sleep(3);
    //             next = (task_t*) current_task_in_queue->next;
    //             break;
    //         }
    //     } while(current_task_in_queue != task_queue);
    //
    //     if (next == NULL)
    //     {
    //         printf(FAILED"scheduler: Failed to find next task.\n");
    //         sleep(1);
    //         printf(WARNING"scheduler: Assigning current_task to next\n");
    //         if (current_task == NULL)
    //         {
    //             printf(FAILED"scheduler: Current task has NULL pointer value.\n");
    //             sleep(1);
    //             exit(1);
    //         }
    //         else
    //         {
    //             printf(DEBUG"scheduler: Current task - %d.\n", current_task->t_id);
    //         }
    //     }
    // }
    // else
    // {
    //     printf(FAILED"scheduler: could not find current_task.\n");
    //     sleep(1);
    // }

    printf(DEBUG"scheduler: returning task %d.\n", next->t_id);
    sleep(3);

    return next;
}
