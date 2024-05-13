#ifndef UX_CORO
#define UX_CORO

#include <stdio.h>
#include <stdlib.h>

# if _WIN32
#include <Windows.h>
#elif __linux__
#include <ucontext.h>
#endif

#include <stdbool.h>


#define TEST_ON 1
#define STACK_SIZE 8192
#define get_curr_ctx(x) x->c1
#define get_other_ctx(x) x->c2

#ifdef _WIN32
typedef LPFIBER_START_ROUTINE CoroCallback;
#elif __linux__
typedef void (*CoroCallback)(void);
#endif

typedef enum{
  CORO_READY,
  CORO_PENDING,
  CORO_FINISHED
}ux_coro_state;

typedef struct{
#if _WIN32
    LPVOID context;
# elif __linux__
  ucontext_t context;
  char* stack;
#endif
  bool finished;
  ux_coro_state st;
}ux_coro;

typedef struct{
  ux_coro* c1;     // current function's coroutine context
  ux_coro* c2;     // other function's coroutine context where we want to switch to
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
  
#if _WIN32
  if (cb == NULL && main_context == NULL && args == NULL) {
      u->context = ConvertThreadToFiber(NULL);
      u->finished = false;
      u->st = CORO_READY;
      
      ux_coro_handle* h = malloc(sizeof(ux_coro_handle));
      h->c1 = u;
      h->c2 = main_context;
      h->args = args;
      return h;
  }
  ux_coro* w = malloc(sizeof(ux_coro));
  w->finished = false;
  w->st = CORO_READY;

  ux_coro_handle* win_handle = malloc(sizeof(ux_coro_handle));
  win_handle->c1 = w;
  win_handle->c2 = main_context;
  win_handle->args = args;

  w->context = CreateFiber(0, cb, win_handle);
  return win_handle;

#elif __linux__
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
#endif
}

void ux_coro_exit(ux_coro_handle* h){
  get_curr_ctx(h)->st = CORO_FINISHED;
  get_curr_ctx(h)->finished = true;
}

void ux_coro_next(ux_coro_handle* h){
  get_curr_ctx(h)->st = CORO_PENDING;
#if _WIN32
  SwitchToFiber(get_curr_ctx(h)->context);
#elif __linux__
  swapcontext(&get_other_ctx(h)->context,&get_curr_ctx(h)->context); 
#endif
}

void ux_coro_yield(ux_coro_handle* h){
    get_curr_ctx(h)->st = CORO_PENDING;
#if _WIN32
    SwitchToFiber(get_other_ctx(h)->context);
#elif __linux__
  swapcontext(&get_curr_ctx(h)->context,&get_other_ctx(h)->context);
#endif
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
#if __linux__
   if(handle){
    if(handle->c1){
      free(handle->c1->stack);
      free(handle->c1);
    }
    free(handle);
  }
#elif _WIN32
    if (handle) {
        if (handle->c1) {
            DeleteFiber(handle->c1->context);
            free(handle->c1);
        }
        free(handle);
    }
#endif
}

#endif
