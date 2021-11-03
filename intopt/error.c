#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

/* error can be called in the same way as printf. */
void (error)(const char* file, const char* func, int line, const char *fmt, ...)
{
	va_list		ap;
	static char	abuf[BUFSIZ];
	static char	bbuf[BUFSIZ];

	fflush(NULL);

	va_start(ap, fmt);

	snprintf(abuf, BUFSIZ, "%s: \"%s\" %s line %d: ", progname, file, func, line);
	snprintf(bbuf, BUFSIZ, "error: ");
	strncat(abuf, bbuf, BUFSIZ-100);

	vsnprintf(bbuf, BUFSIZ, fmt, ap);
	strncat(abuf, bbuf, BUFSIZ-100);
	
	if (errno != 0) {
		strncat(abuf, ": ", BUFSIZ-3);
		strncat(abuf, strerror(errno), BUFSIZ-100);
		errno = 0;
	}

	fprintf(stderr, "%s\n", abuf);

	va_end(ap);

	exit(1);
}
