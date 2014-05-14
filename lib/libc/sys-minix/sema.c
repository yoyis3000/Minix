
#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <lib.h>
#include <minix/rs.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sema.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

/* saw this in sem.c, I might need it apperently */ // cartman

static int get_ipc_endpt(endpoint_t *pt){
  return minix_rs_lookup("ipc", pt);
}

int sem_init(int start_value){
  message m;
  m.SEM_VALUE = start_value;

  if (get_ipc_endpt(&SEMA_PROC_NR) != OK) {
    errno = ENOSYS;
    return -1;
  }
  printf("Making sure this works, this is sem_down\n");
  return(_syscall(SEMA_PROC_NR, SEM_INIT, &m)); 
}

int sem_down(int semaphore_number){
  message m;
  m.SEM_VALUE = semaphore_number;

  if (get_ipc_endpt(&SEMA_PROC_NR) != OK) {
    errno = ENOSYS;
    return -1;
  }
  printf("Making sure this works, this is sem_down\n");
  return(_syscall(SEMA_PROC_NR, SEM_DOWN, &m)); 
}

int sem_up(int semaphore_number){
  message m;
  m.SEM_VALUE = semaphore_number;
  if (get_ipc_endpt(&SEMA_PROC_NR) != OK) {
    errno = ENOSYS;
    return -1;
  }
  return(_syscall(SEMA_PROC_NR, SEM_UP, &m)); 
  printf("Making sure this works, this is sem_up\n");
}

int sem_release(int semaphore_number){
  message m;
  m.SEM_VALUE = semaphore_number;
  if (get_ipc_endpt(&SEMA_PROC_NR != OK) {
    errno = ENOSYS;
    return -1;
  }
  printf("Making sure this works, this is sem_release\n");
  return(_syscall(SEMA_PROC_NR, SEM_RELEASE, &m)); 
}
