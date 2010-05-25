#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

/**
 * Print a formated error message.
 */
static void print_error(int error, const char *file, int line,
	const char *func, const char *fmt, va_list va){
	
	fprintf(stderr, "%s:%i/%s(): ", file, line, func);

	if(fmt){
		vfprintf(stderr, fmt, va);
	}

	if(error){
		if(fmt){
			fputc(' ', stderr);
		}
		fputs(strerror(error), stderr);
	}
}

/**
 * Print a message and explanation of error code
 * to stderr and exit with EXIT_FAILURE.
 */
void error_internal(int e, const char *file, int line,
	const char *func, const char *fmt, ...){
	va_list va;

	va_start(va, fmt);
	print_error(e, file, line, func, fmt, va);
	va_end(va);

	exit(EXIT_FAILURE);
}

/**
 * Print a message and explanation of error code to stderr.
 */
void warning_internal(int e, const char *file, int line,
	const char *func, const char *fmt, ...){
	va_list va;

	va_start(va, fmt);
	print_error(e, file, line, func, fmt, va);
	va_end(va);
}

/**
 * Wrapper around malloc. If allocation fails
 * then calls error() and terminates program.
 */
void *checked_malloc(size_t size){
	void *p = malloc(size);
	if(!p)
		error(errno, NULL);
	return p;
}

/**
 * Wrapper around realloc. If allocation fails
 * then calls error() and terminates program.
 */
void *checked_realloc(void *ptr, size_t size){
	void *p = realloc(ptr, size);
	if(!p)
		error(errno, NULL);
	return p;
}

/**
 * Copy a null terminated string.
 * Call error() if allocation fails.
 */
char *checked_strdup(const char *s){
	if(s)
		return strcpy(checked_malloc(strlen(s) + 1), s);
	else
		return NULL;
}
