/* This file contains the scheduling policy for SEMA
 *
 * The entry points are:
 *   do_sem_init:         Called to initialize semaphore
 *   do_sem_down          Request to decrement semaphore
 *   do_sem_up            Request to increment semaphore
 *   do_sem_release	  Request to release semaphore
 */

//#include "sched.h"
//#include "schedproc.h"
#include <assert.h>
#include <minix/com.h>
#include <machine/archtypes.h>
#include "kernel/proc.h" /* for queue constants */ // cartman

static timer_t sched_timer;
static unsigned balance_timeout;

#define BALANCE_TIMEOUT	5 /* how often to balance queues in seconds */

static int schedule_process(struct schedproc * rmp, unsigned flags);
static void balance_queues(struct timer *tp);

#define SCHEDULE_CHANGE_PRIO	0x1
#define SCHEDULE_CHANGE_QUANTUM	0x2
#define SCHEDULE_CHANGE_CPU	0x4

#define SCHEDULE_CHANGE_ALL	(	\
		SCHEDULE_CHANGE_PRIO	|	\
		SCHEDULE_CHANGE_QUANTUM	|	\
		SCHEDULE_CHANGE_CPU		\
		)

#define schedule_process_local(p)	\
	schedule_process(p, SCHEDULE_CHANGE_PRIO | SCHEDULE_CHANGE_QUANTUM)
#define schedule_process_migrate(p)	\
	schedule_process(p, SCHEDULE_CHANGE_CPU)

#define CPU_DEAD	-1

#define cpu_is_available(c)	(cpu_proc[c] >= 0)

#define DEFAULT_USER_TIME_SLICE 200

/* processes created by RS are sysytem processes */
#define is_system_proc(p)	((p)->parent == RS_PROC_NR)

#define IT_WORKS_MAYBE 1000
static unsigned cpu_proc[CONFIG_MAX_CPUS];

/*===========================================================================*
 *				do_sem_init				     *
 *===========================================================================*/

int do_sem_init(int start_value)
{
  printf("From do_sem_init\n");
  return IT_WORKS_MAYBE;
}

/*===========================================================================*
 *				do_sem_down				     *
 *===========================================================================*/

int do_sem_down(int semaphore_number)
{
  printf("From do_sem_down\n");
  return IT_WORKS_MAYBE;
}

/*===========================================================================*
 *				do_sem_up				     *
 *===========================================================================*/

int do_sem_up(int semaphore_number)
{
  printf("From do_sem_init\n");
  return IT_WORKS_MAYBE;
}

/*===========================================================================*
 *				do_sem_release				     *
 *===========================================================================*/

int do_sem_release(int semaphore_number)
{
  printf("From do_sem_release\n");
  return IT_WORKS_MAYBE;
}
