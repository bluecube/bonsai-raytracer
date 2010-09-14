#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include <malloc.h>

/**
 * Print a formated error message.
 * \param error Errno, or 0 if no error description should be printed.
 * \param file File name of the error source, or NULL if
 * file name should not be printed
 * \param line Line number of the error source.
 * Only used if #file is not NULL.
 * \param func Function namer of the error source.
 * Only used if #file is not NULL.
 * \param fmt Printf-like format of the error message or NULL.
 * \param va Varargs parameters to the message format.
 * Only used if #fmt is not null.
 */
static void print_error(int error, const char *file, int line,
	const char *func, const char *fmt, va_list va){
	
	if(file){
		fprintf(stderr, "%s:%i/%s(): ", file, line, func);
	}

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
	//void *p = malloc(size);
	void *p = memalign(16, size);
	if(!p)
		error(errno, NULL);
	return p;
}

/**
 * Reallocate a block of memory using checked_alloc rather than realloc.
 * It's not as efficient as realloc, but the memory is aligned.
 */
void *checked_realloc_x(void *ptr, size_t newSize, size_t oldSize){
	void *p = checked_malloc(newSize);

	memcpy(p, ptr, newSize < oldSize ? newSize : oldSize);

	free(ptr);

	return p;
}

/**
 * Wrapper around realloc. If allocation fails
 * then calls error() and terminates program.
 */
void *checked_realloc(void *ptr, size_t size){
	void *p = realloc(ptr, size);
	if(!p && size != 0)
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

