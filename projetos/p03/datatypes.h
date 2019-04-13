// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Estruturas de dados internas do sistema operacional

#ifndef __DATATYPES__
#define __DATATYPES__

#include <ucontext.h>


typedef enum status_t
{
    READY,
    EXECUTING,
    SUSPENDED,
    FINISHED
} status_t;

// Estrutura que define uma tarefa
typedef struct task_t
{
    struct task_t *prev, *next;
    int t_id;
    ucontext_t t_context;
    void *stack;
    struct task_t *parent;
    enum status_t status;
  // preencher quando necessário
} task_t ;

// estrutura que define um semáforo
typedef struct
{
  // preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif
