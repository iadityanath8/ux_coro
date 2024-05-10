#ifndef UX_CORO
#define UX_CORO

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <stdbool.h>


#define TEST_ON 1
#define STACK_SIZE 8192
#define get_curr_ctx(x) x->c1
#define get_other_ctx(x) x->c2


typedef void (*CoroCallback)(void);

typedef enum{
  CORO_READY,
  CORO_PENDING,
  CORO_FINISHED
}ux_coro_state;

typedef struct{
  ucontext_t context;
  char* stack;
  bool finished;
  ux_coro_state st;
}ux_coro;

typedef struct{
  ux_coro* c1;
  ux_coro* c2;
  void* args;
}ux_coro_handle;

ux_coro_handle* ux_coro_init(CoroCallback cb,ux_coro* main_context,void* args);
void ux_coro_exit(ux_coro_handle* h);
void ux_coro_yield(ux_coro_handle* h);
void ux_coro_next(ux_coro_handle* h);
void ux_coro_destroy(ux_coro_handle* handle);
bool ux_coro_finished(ux_coro_handle* hanlde);
#endif // UX_CORO

#ifdef UX_CORO_IMPL
ux_coro_handle* ux_coro_init(CoroCallback cb,ux_coro* main_context,void* args){
  ux_coro* u = malloc(sizeof(ux_coro));
  getcontext(&u->context);
  
  u->stack = malloc(sizeof(char)*STACK_SIZE);
  u->context.uc_stack.ss_sp = u->stack;
  u->context.uc_stack.ss_size = sizeof(char)*STACK_SIZE; 
  u->context.uc_link = &main_context->context;
  u->finished = false;
  u->st = CORO_READY;

  ux_coro_handle* h = malloc(sizeof(ux_coro_handle));
  h->c1 = u;
  h->c2 = main_context;
  h->args = args;

  makecontext(&u->context,(CoroCallback)cb,1,h);
  return h;
}

void ux_coro_exit(ux_coro_handle* h){
  get_curr_ctx(h)->st = CORO_FINISHED;
  get_curr_ctx(h)->finished = true;
}

void ux_coro_next(ux_coro_handle* h){
  get_curr_ctx(h)->st = CORO_PENDING;
  swapcontext(&get_other_ctx(h)->context,&get_curr_ctx(h)->context); 
}

void ux_coro_yield(ux_coro_handle* h){
  get_curr_ctx(h)->st = CORO_PENDING;
  swapcontext(&get_curr_ctx(h)->context,&get_other_ctx(h)->context);
}

bool ux_coro_finished(ux_coro_handle* h){
  return get_curr_ctx(h)->finished;
}

/**
 *  
 *  ux_coro_handle h = {.c1 = coro_init(),.c2 = coro_init()}
 *  register(m,n);l
 */
void ux_coro_destroy(ux_coro_handle* handle){
  if(handle){
    if(handle->c1){
      free(handle->c1->stack);
      free(handle->c1);
    }
    free(handle);
  }
}

static void destroy(ux_coro_handle* h){
  free(h->c1->stack);
  free(h->c1);
  free(h);
}

#endif
