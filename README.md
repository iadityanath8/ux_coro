# Coroutine Library (ux_coro)

The `ux_coro` library provides a simple coroutine implementation using `ucontext.h` in C.

## Introduction

The `ux_coro` library provides a simple coroutine implementation in C using the `ucontext.h` library. Coroutines are useful for implementing cooperative multitasking and can be used to perform asynchronous tasks or execute multiple tasks concurrently within a single thread.

## Features

- Lightweight coroutine creation and management.
- Ability to yield and resume coroutine execution.
- Supports passing arguments to coroutines.

## Usage

## Installation

To use the `ux_coro` library in your project, follow these steps:

1. Clone or download the repository to your local machine.
2. Include the header file `ux_coro.h` in your source files.
3. Link against the library during compilation.


```c
#include "ux_coro.h"

int main(){
    CoroCallback my_coroutine;
    ux_coro* main_context;
    void* args;
    ux_coro_handle* coro_handle = ux_coro_init(my_coroutine, main_context, args);

    // Execute the next step of the coroutine
    ux_coro_next(coro_handle);

    // Yield the coroutine
    ux_coro_yield(coro_handle);

    // Exit the coroutine
    ux_coro_exit(coro_handle);
    ux_coro_destroy(coro_handle);
}
