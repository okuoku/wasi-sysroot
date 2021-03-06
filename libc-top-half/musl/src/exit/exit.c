#include <stdlib.h>
#include <stdint.h>
#include "libc.h"

static void dummy()
{
}

/* atexit.c and __stdio_exit.c override these. the latter is linked
 * as a consequence of linking either __toread.c or __towrite.c. */
weak_alias(dummy, __funcs_on_exit);
weak_alias(dummy, __stdio_exit);
weak_alias(dummy, _fini);

#ifdef __wasilibc_unmodified_upstream // fini
extern weak hidden void (*const __fini_array_start)(void), (*const __fini_array_end)(void);

static void libc_exit_fini(void)
{
	uintptr_t a = (uintptr_t)&__fini_array_end;
	for (; a>(uintptr_t)&__fini_array_start; a-=sizeof(void(*)()))
		(*(void (**)())(a-sizeof(void(*)())))();
	_fini();
}

weak_alias(libc_exit_fini, __libc_exit_fini);
#endif

#ifdef __wasilibc_unmodified_upstream
_Noreturn void exit(int code)
{
	__funcs_on_exit();
	__libc_exit_fini();
	__stdio_exit();
	_Exit(code);
}
#else
// Split out the cleanup functions so that we can call them without calling
// _Exit if we don't need to. This allows _start to just return if main
// returns 0.
void __prepare_for_exit(void)
{
	__funcs_on_exit();
	__stdio_exit();
}

_Noreturn void exit(int code)
{
	__prepare_for_exit();
	_Exit(code);
}
#endif
