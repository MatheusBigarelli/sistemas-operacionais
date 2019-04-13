#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"


#define STACKSIZE 32000


#define OK "[ \e[0;32m \bOK \e[0;37m \b] "
#define WARNING "[ \e[0;33m \bWARNING \e[0;37m \b] "
#define FAILED "[ \e[0;31m \bFAILED \e[0;37m \b] "


int task_counter = 0;
task_t* main_task;
task_t* init_task;
task_t* current_task;



void define_internal_task_attributes(task_t* task);
int test_task_start_routine();


int main (int argc, char *argv[]);


void pingpong_init()
{
    printf(OK"pingpong_init: Beginning.\n");
    setvbuf(stdout, 0, _IONBF, 0);


    main_task = malloc(sizeof(task_t));
    int error = task_create(main_task, (void*) *main, "  main");
    if (error)
        printf(FAILED"task_create: error creating main_task.\n");

    getcontext(&(main_task->t_context));

    current_task = main_task;
}


int task_create(task_t* task, void (*start_routine) (void*), void* arg)
{
    printf(OK"Entered task_create from %s.\n", (char*) arg);

    define_internal_task_attributes(task);
    makecontext(&(task->t_context), (void*) *start_routine, 1, arg);

    printf(OK"task_create: created task %d\n", task->t_id);

    return 0;
}

void define_internal_task_attributes(task_t* task)
{
    getcontext (&(task->t_context));

    char* stack = malloc(STACKSIZE);
    if (stack)
    {
        task->t_context.uc_stack.ss_sp = stack;
        task->t_context.uc_stack.ss_size = STACKSIZE;
        task->t_context.uc_stack.ss_flags = 0;
        if (task->prev)
            task->t_context.uc_link = &(task->prev->t_context);
        else
        {
            printf(WARNING"Probably main uc_link = %p\n", (void*) task->t_context.uc_link);
            task->t_context.uc_link = 0;
        }

        task->t_id = task_counter;
        task_counter++;
    }
    else
    {
       printf(FAILED"task_create:\n");
       printf("  define_internal_task_attributes: could not allocate stack.\n");
       exit(1);
    }
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


int test_task_start_routine()
{
    printf(OK"test_task_start_routine: started\n");


    printf("  returning to init\n");

    task_switch(init_task);

    return 0;
}


void task_exit(int exitCode)
{
    printf(OK"task_exit: received exitCode %d\n", exitCode);
    if (exitCode)
        exit(exitCode);

    printf(OK"task_exit: task %d exited\n", current_task->t_id);

    task_switch(main_task);
}

int task_id()
{
    return current_task->t_id;
}



#ifdef TEST

int main(int argc, char* argv[])
{
    printf("main: Printing from main.\n");



    pingpong_init();
    printf(OK"pingpong_init: test_task returned.\n");
    printf("  returning from main.\n");

    return 0;
}

#endif
