
#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <minix/rs.h>

#include <unistd.h>
#include <minix/callnr.h>
#include <minix/ipc.h>
#include <minix/com.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sema.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <minix/com.h>

/* saw this in sem.c, I might need it apperently */ // cartman

static int get_ipc_endpt(endpoint_t *pt){
  return minix_rs_lookup("ipc", pt);
}

int sem_init(int start_value){
  message m;
  m.m1_i1 = start_value;
  m.m_type = SEM_INIT;
  /*if (get_ipc_endpt(&SEMA_PROC_NR) != OK) {
    errno = ENOSYS;
    return -1;
  }*/
  printf("Making sure this works, this is seminit\n");
  return(_syscall(SEMA_PROC_NR, SEM_INIT, &m)); 
}

int sem_down(int semaphore_number){
  message m;
  m.m1_i2 = semaphore_number;
  m.m_type = SEM_DOWN;

  printf("Making sure this works, this is sem_down\n");
  return(_syscall(SEMA_PROC_NR, SEM_DOWN, &m)); 
}

int sem_up(int semaphore_number){
  message m;
  m.m_type = SEM_UP;
  m.m1_i2 = semaphore_number;
  printf("Making sure this works, this is sem_up\n");
  return(_syscall(SEMA_PROC_NR, SEM_UP, &m)); 
}

int sem_release(int semaphore_number){
  message m;
  m.m1_i3 = semaphore_number;
  m.m_type = SEM_RELEASE;
  printf("Making sure this works, this is sem_release\n");
  return(_syscall(SEMA_PROC_NR, SEM_RELEASE, &m)); 
}
