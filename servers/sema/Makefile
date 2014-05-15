# Makefile for Scheduler (SCHED)
PROG= sema
SRCS= main.c semaphore.c

DPADD+= ${LIBSYS} ${LIBTIMERS}
LDADD+= -lsys -ltimers

MAN=

BINDIR?= /usr/sbin

CPPFLAGS.main.c+= -I${NETBSDSRCDIR}
CPPFLAGS.semaphore.c+= -I${NETBSDSRCDIR}

.include <minix.bootprog.mk>
