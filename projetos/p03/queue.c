#include <stdio.h>
#include "queue.h"
#include "debugging.h"

void create_queue(queue_t** queue, queue_t* elem);
int elem_in_another_queue(queue_t* elem);
int remove_checks_ok(queue_t* queue, queue_t* elem);
int element_in_queue(queue_t* queue, queue_t* elem);
void pretty_print_queue_state(queue_t* first, queue_t* elem, queue_t* last);

void queue_append(queue_t **queue, queue_t *elem)
{
  if (*queue == NULL)
  {
    create_queue(queue, elem);
  }

  if (elem == NULL)
  {
    printf(FAILED"queue_append: The element does not exist.\n");
    sleep(1);
    return;
  }

  if (elem_in_another_queue(elem))
    return;

  queue_t* first = *queue;
  queue_t* last = first->prev;

  printf("\n\tStructure before insertion:\n");
  pretty_print_queue_state(first, elem, last);


  last->next = elem;
  elem->next = first;
  elem->prev = last;
  first->prev = elem;

  printf("\n\n\tStructure after insertion:\n");
  pretty_print_queue_state(first, elem, last);

  printf(OK"queue_append: Successfully appended element.\n");

}

void create_queue(queue_t** queue, queue_t* elem)
{
  printf(WARNING"queue_append: The queue does not exist.\n");
  sleep(1);
  printf("\n\t\tCreating new one...\n");
  if (elem != NULL)
  {
    if (!elem_in_another_queue(elem))
    {
      *queue = elem;
      (*queue)->next = *queue;
      (*queue)->prev = *queue;
    }
  }
  else
  {
    printf(FAILED"queue_append: Inserted NULL elem on empty queue. Aborting...\n");
    sleep(1);
    return;
  }

  pretty_print_queue_state(*queue, *queue, *queue);
  return;
}

int elem_in_another_queue(queue_t* elem)
{
  if (elem->prev != NULL)
  {
    printf(WARNING"elem_in_another_queue: The element has prev address set.\n");
    sleep(1);
    printf("\n\t\tPossibly belongs to another queue. Aborting...\n");
    return 1;
  }

  if (elem->next != NULL)
  {
    printf(FAILED"elem_in_another_queue: The element has next address set.\n");
    sleep(1);
    printf("\n\t\tPossibly belongs to another queue. Aborting...\n");
    return 1;
  }

  return 0;
}


void pretty_print_queue_state(queue_t* prev, queue_t* elem, queue_t* next)
{
  printf("\n\tPrev:%p\tprev:%p\tnext:%p\n", (void*)prev, (void*)prev->prev, (void*)prev->next);
  printf("\n\tElem:%p\tprev:%p\tnext:%p\n", (void*)elem, (void*)elem->prev, (void*)elem->next);
  printf("\n\tNext:%p\tprev:%p\tnext:%p\n\n", (void*)next, (void*)next->prev, (void*)next->next);

}



queue_t *queue_remove (queue_t **queue, queue_t *elem)
{
  if (!remove_checks_ok(*queue, elem))
    return NULL;

  if (!element_in_queue(*queue, elem))
    return NULL;

  queue_t* prev = elem->prev;
  queue_t* next = elem->next;

  printf("\n\n\tStructure before remove:\n");
  printf("\n\tQueue: %p\n", (void*) *queue);
  pretty_print_queue_state(prev, elem, next);


  prev->next = elem->next;
  next->prev = elem->prev;


  if (*queue == elem)
  {
    *queue = elem->next;
    if (*queue == elem)
      *queue = NULL;
  }


  elem->prev = NULL;
  elem->next = NULL;

  printf("\n\n\tStructure after remove:\n");
  pretty_print_queue_state(prev, elem, next);

  printf("\n\tSuccessfully removed element.\n\n\n");
  return elem;
}




int remove_checks_ok(queue_t* queue, queue_t* elem)
{
  if (queue == NULL)
  {
    printf(FAILED"remove_checks_ok: The queue does not exist. Aborting...\n");
    sleep(1);
    return 0;
  }

  if (elem == NULL)
  {
    printf(FAILED"remove_checks_ok: The element does not exist. Aborting...\n");
    sleep(1);
    return 0;
  }

  if (elem->prev == NULL)
  {
    printf(FAILED"remove_checks_ok: The element has prev address not set.\n");
    sleep(1);
    printf("\n\t\tWill have NULL pointer issues if removed. Aborting...\n");
    return 0;
  }

  if (elem->next == NULL)
  {
    printf(FAILED"remove_checks_ok: The element has next address not set.\n");
    sleep(1);
    printf("\n\t\tWill have NULL pointer issues if removed. Aborting...\n");
    return 0;
  }

  return 1;
}




int element_in_queue(queue_t* queue, queue_t* elem)
{
  queue_t* iterator = queue;
  do
  {
    if (iterator == elem)
      return 1;

    iterator = iterator->next;
  } while(iterator != queue);

  return 0;
}





int queue_size (queue_t *queue)
{
  if (queue == NULL)
  {
    printf("\n\tWarning (queue_size): The queue does not exist.\n");
    printf("\n\t\tCounted 0 elements. Returning...\n");
    return 0;
  }

  queue_t *first = queue->next;

  if (first->next == NULL)
  {
    printf(FAILED"queue_size: Bad implementation.\n");
    sleep(1);
    printf("\n\t\tNULL pointer found. Queue not circular. Aborting...\n");
  }

  queue_t *iterator = first->next;
  int counter = 1;
  while (iterator != first)
  {
    counter++;
    iterator = iterator->next;
    if (iterator->next == NULL)
    {
      printf(FAILED"queue_size: Bad implementation.\n");
      sleep(1);
      printf("\n\t\tNULL pointer found. Queue not circular. Aborting...\n");
      return 0;
    }
  }

  printf(OK"queue_size: Successfully counted the queue's number of elements.\n");
  printf("\n\t\tCounted %d elements.\n\n\n", counter);

  return counter;
}




void queue_print (char *name, queue_t *queue, void print_elem (void*) )
{
  if (queue == NULL)
  {
    printf(FAILED"queue_print: The queue does not exist. Aborting...\n");
    sleep(1);
    return;
  }

  if (name == NULL)
  {
    printf(FAILED"queue_print: Name parameter does not exist. Aborting...\n");
    sleep(1);
    return;
  }

  queue_t *first = queue;
  queue_t *iterator = first;
  do
  {
    if (iterator->next == NULL)
    {
      printf(FAILED"queue_print: Bad implementation.\n");
      sleep(1);
      printf("\n\t\tNULL pointer found. Queue not circular. Aborting...\n");
      return;
    }

    print_elem(iterator);

    iterator = iterator->next;
  } while (iterator != first);
}
