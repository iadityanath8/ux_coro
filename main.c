#define UX_CORO_IMPL
#include "ux_coro.h"
#include <stdio.h>

void func(ux_coro_handle* h){
	for (int i = 0; i < 10; i++) {
		printf("Hello world in here woow %d times\n", i);
		ux_coro_yield(h);
	}
	ux_coro_exit(h);
}

int main() {
	int a = 90;
	ux_coro_handle* h = ux_coro_init(NULL, NULL, NULL);
	ux_coro_handle* m = ux_coro_init((CoroCallback)func, get_curr_ctx(h), &a);
	
	while (!ux_coro_finished(m)) {
		ux_coro_next(m);
	}

	return 0;
}