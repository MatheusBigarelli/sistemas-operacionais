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


void pingpong_init()
{
    printf(OK"pingpong_init: Beginning.\n");
    setvbuf(stdout, 0, _IONBF, 0);

    main_task = malloc(sizeof(task_t));
    if (!main_task)
    {
        printf(FAILED"pingpong_init: could not allocate memory for main task.\n");
        sleep(1);
        exit(1);
    }
    printf(OK"pingpong_init: successfully allocated memory for main task.\n");
    current_task = main_task;

    int main_id = task_create(main_task, (void*) *main, "   main");
    if (main_id < 0)
    {
        printf(FAILED"task_create: failed to create main task.\n");
        sleep(1);
        exit(1);
    }

    dispatcher_task = malloc(sizeof(task_t));
    int dispatcher_id = task_create(dispatcher_task, (void*) *dispatcher_body, "    dispatcher");
    if (dispatcher_id < 0)
    {
        printf(FAILED"task_create: could not create dispatcher\n");
        exit(1);
    }

    if (task_queue == NULL)
    {
        printf(FAILED"pingpong_init: could not create task_queue.\n");
        sleep(1);
    }

    printf(OK"pingpong_init: finished successfully.\n");
}


int task_create(task_t* task, void (*start_routine) (void*), void* arg)
{
    if (task == main_task)
    {
        printf(DEBUG"task_create: Received task main. Will not append to queue.\n");
        sleep(1);
        return 0;
    }

    printf(OK"Entered task_create for %s.\n", (char*) arg);

    int err = define_internal_task_attributes(task);
    if (err)
    {
        printf(FAILED"task_create: could not define internal attributes for %s\n", (char*) arg);
        sleep(1);
        return -1;
    }

    makecontext(&(task->t_context), (void*) *start_routine, 1, arg);

    printf(OK"task_create: created task %d\n", task->t_id);


    queue_append((queue_t**) &ready_task_queue, (queue_t*) task);


    if ((task_t*) task_queue->prev == task)
        printf(OK"task_create: successfully appended task %d to task_queue.\n", task->t_id);
    else
    {
        printf(FAILED"task_create: failed to append task %d to task_queue.\n", task->t_id);
        sleep(1);
    }

    return task->t_id;
}

int define_internal_task_attributes(task_t* task)
{
    task->t_id = task_counter;
    task_counter++;

    getcontext (&(task->t_context));

    if (create_task_stack(task))
    {
        printf(FAILED"define_internal_task_attributes: could not create stack for task %d.\n",
        task->t_id);
        sleep(1);
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
        printf(FAILED"  create_task_stack: could not create stack for task %d.\n", task->t_id);
        sleep(1);
        return 1;
    }

    printf(OK"create_task_stack: successfully created stack for task %d.\n", task->t_id);
    return 0;
}




int task_switch(task_t* task)
{
    printf(OK"task_switch: switching context %d -> %d.\n", current_task->t_id, task->t_id);
    ucontext_t current_context;
    getcontext(&current_context);
    current_task->t_context = current_context;
    current_task = task;
    swapcontext(&current_context, &(task->t_context));

    return 0;
}


void task_exit(int exitCode)
{
    printf(OK"task_exit: received exitCode %d\n", exitCode);
    if (exitCode)
        exit(exitCode);

    printf(OK"task_exit: task %d exited\n", current_task->t_id);
    printf("    tasks remaining - %d", task_counter);
    task_counter--;

    task_switch(dispatcher_task);
}

int task_id()
{
    return current_task->t_id;
}


void task_yield()
{
    printf(DEBUG"task_yield: Entered.\n");
    sleep(1);
    task_t* next = NULL;
    next = scheduler();

    printf(DEBUG"task_yield: next task received by scheduler - %d.\n", next->t_id);
    sleep(1);

    if (next == NULL)
    {
        printf(FAILED"task_yield: next task received address 0.\n");
        sleep(1);
        exit(1);
    }

    task_switch(next);
}



#ifdef TEST



int test_task_start_routine()
{
    printf(OK"test_task_start_routine: started\n");


    printf("  returning to init\n");

    task_switch(init_task);

    return 0;
}


int main(int argc, char* argv[])
{
    printf("main: Printing from main.\n");



    pingpong_init();
    printf(OK"pingpong_init: test_task returned.\n");
    printf("  returning from main.\n");

    return 0;
}

#endif
