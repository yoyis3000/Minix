/* Very first editing of minix, this is the header file for sema.h*/
#ifndef _SEMA_H_
#define _SEMA_H_

/* Including random files, got this from shed.h */
#include <time.h>
#include <sys/cdefs.h>
#include <sys/featuretest.h>
#include <sys/sched.h>

int sem_init(int start_value);

int sem_down(int semaphore_number);

int sem_up(int semaphore_number);

int sem_release(int samephore);



#endif /* _SEMA_H_ */
