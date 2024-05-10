#include <stdio.h>
#define UX_CORO_IMPL
#include "../ux_coro.h"


void print_10_times(ux_coro_handle* h){
  
  for(int i = 0;i < 10;i++){
    printf("Hello world %d times\n",i);
    ux_coro_yield(h);
  }
  ux_coro_exit(h);
}

int main(){
  ux_coro_handle* mm = ux_coro_init(NULL,NULL,NULL);
  ux_coro_handle* h = ux_coro_init((CoroCallback)print_10_times,get_curr_ctx(mm),NULL); 
  
  ux_coro_next(h);
  ux_coro_next(h);
  
  printf("MEOW \n");
  return 0;
}

