#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

/**
 * Print a formated error message.
 */
static void print_error(int error, const char *fmt, va_list va){
	flockfile(stderr);

	if(fmt){
		vfprintf(stderr, fmt, va);
	}

	if(error){
		if(fmt){
			fputc(' ', stderr);
		}
		fputs(strerror(error), stderr);
	}

	fputc('\n', stderr);
	
	funlockfile(stderr);
}

/**
 * Print a message and explanation of error code
 * to stderr and exit with EXIT_FAILURE.
 * \param error Errno or 0 if error description
 * should not be printed.
 * \param fmt Custom error message or NULL.
 */
void error(int error, const char *fmt, ...){
	va_list va;

	va_start(va, fmt);
	print_error(error, fmt, va);
	va_end(va);

	exit(EXIT_FAILURE);
}

/**
 * Print a message and explanation of error code to stderr.
 * \param error Errno or 0 if error description
 * should not be printed.
 * \param fmt Custom error message or NULL.
 */
void warning(int error, const char *fmt, ...){
	va_list va;

	va_start(va, fmt);
	print_error(error, fmt, va);
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

