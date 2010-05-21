#ifndef BONSAI_CLIENT_UTIL_H
#define BONSAI_CLIENT_UTIL_H

#include <stdlib.h>

void debug(const char *fmt, ...);

void error(int error, const char *fmt, ...);
void warning(int error, const char *fmt, ...);

void *checked_malloc(size_t size);
void *checked_realloc(void *ptr, size_t size);
char *checked_strdup(const char *s);

#endif
