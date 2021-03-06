#include <stdio.h>
#include <stdarg.h>

int printf(const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vfprintf(stdout, fmt, ap);
	va_end(ap);
	return ret;
}
#ifdef __wasilibc_unmodified_upstream
#else
weak_alias(printf, iprintf);
weak_alias(printf, printf_no_Lf);
#endif
