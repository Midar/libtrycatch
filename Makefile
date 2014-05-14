CC ?= clang
CFLAGS ?= -Weverything -std=c11

## For C11
#CFLAGS += -DTRYCATCH_THREAD_LOCAL=_Thread_local
## For GCC / Clang
#CFLAGS += -DTRYCATCH_THREAD_LOCAL=__thread

libtrycatch.a: trycatch.o
	ar cr $@ $<

example: example.o libtrycatch.a
	${CC} -L. -ltrycatch -o $@ $<
