#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pingpong.h"
#include "queue.h"
#include "debugging.h"
#include "dispatcher.h"

#define STACKSIZE 32000



int task_counter;

task_t* current_task;
task_t* dispatcher_task;
task_t* main_task;

queue_t* ready_task_queue;



int define_internal_task_attributes(task_t* task);
int create_task_stack(task_t* task);


#ifdef TEST
int test_task_start_routine();
#endif

int main (int argc, char *argv[]);

// Sleep time for time function
int TIME = 1;


void pingpong_init()
{
    printf("%10s pingpong_init: Beginning.\n", OK);
    setvbuf(stdout, 0, _IONBF, 0);


    main_task = malloc(sizeof(task_t));
    if (!main_task)
    {
        printf("%10s pingpong_init: could not allocate memory for main task.\n", FAILED);
        DEBUG_SLEEP(TIME);
        exit(1);
    }
    printf("%10s pingpong_init: successfully allocated memory for main task.\n", OK);
    current_task = main_task;

    int main_id = task_create(main_task, (void*) *main, "   main");
    // printf("Aqui\n");
    if (main_id < 0)
    {
        printf("%10s task_create: failed to create main task.\n", FAILED);
        DEBUG_SLEEP(TIME);
        exit(1);
    }

    dispatcher_task = malloc(sizeof(task_t));
    int dispatcher_id = task_create(dispatcher_task, (void*) *dispatcher_body, "    dispatcher");
    if (dispatcher_id < 0)
    {
        printf("%10s task_create: could not create dispatcher\n", FAILED);
        exit(1);
    }

    if (ready_task_queue == NULL)
    {
        printf("%10s pingpong_init: could not create task_queue.\n", FAILED);
        DEBUG_SLEEP(TIME);
    }

    printf("%10s pingpong_init: finished successfully.\n", OK);
}


int task_create(task_t* task, void (*start_routine) (void*), void* arg)
{
    int append = 1;
    if (task == main_task)
    {
        printf("%10s task_create: Received task main. Will not append to queue.\n", DEBUG);
        DEBUG_SLEEP(TIME);
        append = 0;
    }

    printf("%10s Entered task_create for %s.\n", OK, (char*) arg);

    int err = define_internal_task_attributes(task);
    if (err)
    {
        printf("%10s task_create: could not define internal attributes for %s\n", FAILED, (char*) arg);
        DEBUG_SLEEP(TIME);
        return -1;
    }

    makecontext(&(task->t_context), (void*) *start_routine, 1, arg);

    printf("%10s task_create: created task %d\n", OK, task->t_id);

    if (append)
        queue_append((queue_t**) &ready_task_queue, (queue_t*) task);

    // else
    //     return

    if (ready_task_queue && (task_t*) ready_task_queue->prev == task)
        printf("%10s task_create: successfully appended task %d to task_queue.\n", OK, task->t_id);
    else
    {
        printf("%10s task_create: failed to append task %d to task_queue.\n", FAILED, task->t_id);
        DEBUG_SLEEP(TIME);
    }

    printf("%10s task_create: exiting.\n", OK);
    return task->t_id;
}

int define_internal_task_attributes(task_t* task)
{
    task->t_id = task_counter;
    task_counter++;

    getcontext (&(task->t_context));

    if (create_task_stack(task))
    {
        printf("%10s define_internal_task_attributes: could not create stack for task %d.\n", FAILED,
        task->t_id);
        DEBUG_SLEEP(TIME);
        return 1;
    }

    printf("        task_id - %d\n", task->t_id);

    task->parent = current_task;

    return 0;
}


int create_task_stack(task_t* task)
{
    char* stack = malloc(STACKSIZE);
    if (stack)
    {
        task->t_context.uc_stack.ss_sp = stack;
        task->t_context.uc_stack.ss_size = STACKSIZE;
        task->t_context.uc_stack.ss_flags = 0;
    }

    else
    {
        printf("%10s   create_task_stack: could not create stack for task %d.\n", FAILED, task->t_id);
        DEBUG_SLEEP(TIME);
        return 1;
    }

    printf("%10s create_task_stack: successfully created stack for task %d.\n", OK, task->t_id);
    return 0;
}




int task_switch(task_t* task)
{
    printf("%10s task_switch: switching context %d -> %d.\n", OK, current_task->t_id, task->t_id);
    ucontext_t current_context;
    getcontext(&current_context);
    current_task->t_context = current_context;
    current_task = task;
    swapcontext(&current_context, &(task->t_context));

    return 0;
}


void task_exit(int exitCode)
{
    printf("%10s task_exit: received exitCode %d\n", OK, exitCode);
    if (exitCode)
        exit(exitCode);

    printf("%10s task_exit: task %d exited\n", OK, current_task->t_id);
    printf("    tasks remaining - %d.\n", task_counter);
    task_counter--;

    task_switch(dispatcher_task);
}

int task_id()
{
    return current_task->t_id;
}


void task_yield()
{
    printf("%10s task_yield: Entered.\n", DEBUG);
    DEBUG_SLEEP(TIME);
    task_t* next = NULL;
    next = scheduler();

    printf("%10s task_yield: next task received by scheduler - %d.\n", DEBUG, next->t_id);
    DEBUG_SLEEP(TIME);

    if (next == NULL)
    {
        printf("%10s task_yield: next task received address 0.\n", FAILED);
        DEBUG_SLEEP(TIME);
        task_switch(main_task);
        // exit(1);
    }

    task_switch(next);
}



#ifdef TEST



int test_task_start_routine()
{
    printf("%10s test_task_start_routine: started\n", OK);


    printf("  returning to init\n");

    task_switch(init_task);

    return 0;
}


int main(int argc, char* argv[])
{
    printf("main: Printing from main.\n");



    pingpong_init();
    printf("%10s pingpong_init: test_task returned.\n", OK);
    printf("  returning from main.\n");

    return 0;
}

#endif
