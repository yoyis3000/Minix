/* This file contains the main program of the SEMA semaphore
 * cartman
 */

/* The _MAIN def indicates that we want the sema____ structs to be created
 * here. Used from within sema____.h
 */
#define _MAIN

#include <minix/com.h>
#include "sema.h"
#include <stdio.h>
#include "dubStack.h"
//#include "semaproc.h"

/* Declare some local functions. */
/*static void reply(endpoint_t whom, message *m_ptr);
static void sef_local_startup(void);
*/

struct machine machine;	


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
  int i;
  for(i=0; i<10; i++){
    semaphores[i] = (Sem*) malloc(sizeof(Sem));
    semaphores[i]->q = makeQueue();
    semaphores[i]->verified = 0;
  }

	//if (OK != (s=sys_getmachine(&machine)))
	//	panic("couldn't get machine info: %d", s);

	/* This is SCHED's main loop - get work and do it, forever and forever. */
	while (TRUE) {
		int ipc_status;

		/* Wait for the next message and extract useful information from it. */
		if ((result= receive(ANY, &m_in, &ipc_status)) != OK)
			panic("SCHED sef_receive error");
		who_e = m_in.m_source;	/* who sent the message */
		call_nr = m_in.m_type;	/* system call number */

		

		switch(call_nr) {
		case SEM_DOWN:
			result = do_sem_down(&m_in);
			break;
		case SEM_UP:
			result = do_sem_up(&m_in);
			break;
		case SEM_RELEASE:
			result = do_sem_release(&m_in);
			break;
		case SEM_INIT:
			result = do_sem_init(&m_in);
			break;
		default:
			result = EINVAL;;
		}

		/* Send reply. */
   if(result != EDONTREPLY){
      message m_out;
      m_out.m_type = result;
      sendnb(who_e, &m_out);
    }
	}
	return(OK);
}




