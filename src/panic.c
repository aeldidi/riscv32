#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void
panic_impl(const char* filename, const int line, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "panic:%s:%d ", filename, line);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);

	exit(EXIT_FAILURE);
}
#define panic(...) panic_impl(__FILE__, __LINE__, __VA_ARGS__)
#define unimplemented()                                                       \
	panic_impl(__FILE__, __LINE__, "reached unimplemented code")
#define unreachable()                                                         \
	panic_impl(__FILE__, __LINE__, "reached unreachable code")
