/* This file contains the main program of the SEMA semaphore
 * cartman
 */

/* The _MAIN def indicates that we want the sema____ structs to be created
 * here. Used from within sema____.h
 */
#define _MAIN

#include "sema.h"
//#include "semaproc.h"

/* Declare some local functions. */
/*static void reply(endpoint_t whom, message *m_ptr);
static void sef_local_startup(void);

struct machine machine;		/* machine info */

/*===========================================================================*
 *				main					     *
 *===========================================================================*/
int main(void)
{
	/* Main routine of the scheduler. */
	message m_in; /* the incoming message itself is kept here. */
	int call_nr;  /* system call number */
	int who_e;    /* callers endpoint */
	int result;   /* result to system call */
	int rv;
	int s;

	/* SEF startup. */
	sef_startup();

	if (OK != (s=sys_getmachine(&machine)))
		panic("couldn't get machine info: %d", s);

	/* This is SCHED's main loop - get work and do it, forever and forever. */
	while (TRUE) {
		int ipc_status;

		/* Wait for the next message and extract useful information from it. */
		if (sef_receive_status(ANY, &m_in, &ipc_status) != OK)
			panic("SCHED sef_receive error");
		who_e = m_in.m_source;	/* who sent the message */
		call_nr = m_in.m_type;	/* system call number */

		/* Check for system notifications first. Special cases. */
		if (is_ipc_notify(ipc_status)) {
			switch(who_e) {
				case CLOCK:
					expire_timers(m_in.NOTIFY_TIMESTAMP);
					continue;	/* don't reply */
				default :
					result = ENOSYS;
			}

			goto sendreply;
		}

		switch(call_nr) {
		case SEMA_INHERIT:
		case SEMA_DOWN:
			result = do_sem_down(&m_in);
			break;
		case SEMA_UP:
			result = do_sem_up(&m_in);
			break;
		case SEMA_RELEASE:
			result = do_sem_release(&m_in);
			break;
		case SEMA_INIT:
			result = do_sem_init(&m_in);
			break;
		default:
			result = no_sys(who_e, call_nr);
		}

sendreply:
		/* Send reply. */
		if (result != SUSPEND) {
			m_in.m_type = result;  		/* build reply message */
			reply(who_e, &m_in);		/* send it away */
		}
 	}
	return(OK);
}

/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
static void reply(endpoint_t who_e, message *m_ptr)
{
	int s = send(who_e, m_ptr);    /* send the message */
	if (OK != s)
		printf("SEMA: unable to send reply to %d: %d\n", who_e, s);
}




