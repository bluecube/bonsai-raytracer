#ifndef BONSAI_CLIENT_UTIL_H
#define BONSAI_CLIENT_UTIL_H

#include <stdlib.h>

void debug(const char *fmt, ...);

/**
 * Print a message and explanation of error code
 * to stderr and exit with EXIT_FAILURE.
 * \param e Errno or 0 if error description
 * should not be printed.
 * \param format Custom error message or NULL.
 */
#define error(e, format, ...) \
	error_internal((e), __FILE__, __LINE__, __func__, (format), ##__VA_ARGS__)

/**
 * Print a message and explanation of error code to stderr.
 * \param e Errno or 0 if error description
 * should not be printed.
 * \param format Custom error message or NULL.
 */
#define warning(e, format, ...) \
	warning_internal((e), __FILE__, __LINE__, __func__, (format), ##__VA_ARGS__)
		

void error_internal(int e, const char *file, int line,
	const char *func, const char *fmt, ...)
	__attribute__ ((noreturn));
void warning_internal(int e, const char *file, int line,
	const char *func, const char *fmt, ...);

void *checked_malloc(size_t size);
void *checked_realloc(void *ptr, size_t size);
char *checked_strdup(const char *s);

#endif
