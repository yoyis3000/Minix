/* Function prototypes. */
//cartman

struct semaproc;
#include <timers.h>
/* main.c */
int main(void);

/* sema.c */
static int get_ipc_endpt(endpoint_t *pt);
int do_sem_init(message *m_ptr);
int do_sem_down(message *m_ptr);
int do_sem_up(message *m_ptr );
int do_sem_release(message *m_ptr);

//void setreply(int proc_nr, int result);

 
/* utility.c /
int no_sys(int who_e, int call_nr);
int sched_isokendpt(int ep, int *proc);
int sched_isemtyendpt(int ep, int *proc);
int accept_message(message *m_ptr);*/
