#ifndef error_h
#define error_h

#undef error

#define error(...) (error(__FILE__, __func__, __LINE__, __VA_ARGS__))

extern char*	progname;

_Noreturn void (error)(const char*, const char*, int, const char*, ...);

#endif
