#pragma once

#include <ucontext.h>

/* Function type for user-level thread entry points.
 * @args is a pointer to user-specified arguments.
 */
typedef void (*coro_entry)(void *args);

/* Create a new user-level thread that is marked as runnable.
 * The thread's entry point is @entry with the given @args.
 * The thread will not start executing until the next call to coro_yield().
 * Returns a unique thread identifier on success, or -1 on failure.
 */
int coro_create(coro_entry entry, void *args);

/* Yield execution to the next available user-level thread.
 * The scheduler may choose to run a different thread or resume the current one.
 */
void coro_yield(void);

/* Terminate the currently running user-level thread.
 * This call does not return.
 */
void coro_exit(void);

/* Terminate the user-level thread identified by @id.
 * The target thread is forcefully stopped and cleaned up.
 */
void coro_destroy(int id);

/* Retrieve the identifier of the currently running user-level thread.
 * Returns the unique identifier of the calling thread.
 */
int coro_getid(void);

/* Receive a value sent from another user-level thread.
 * On return, the object pointed by @who is set to the identifier of the sender.
 * Returns the value sent.
 */
int coro_recv(int *who);

/* Send an integer value @val to the user-level thread identified by @dest.
 */
void coro_send(int dest, int val);