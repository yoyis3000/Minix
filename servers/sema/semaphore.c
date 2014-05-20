/* This file contains the scheduling policy for SEMA
 *
 * The entry points are:
 *   do_sem_init:         Called to initialize semaphore
 *   do_sem_down          Request to decrement semaphore
 *   do_sem_up            Request to increment semaphore
 *   do_sem_release	  Request to release semaphore
 */



/* For easy reference 

1135  # define SEM_VALUE  m1_i1
1136  # define SEM_NUMBER m1_i2
1137  # define SEM_ID     m1_i3
1138  #define SEM_INIT (IPC_BASE+8)
1139  #define SEM_DOWN (IPC_BASE+9)
1140  #define SEM_UP (IPC_BASE+10)
1141  #define SEM_RELEASE (IPC_BASE+11)

End of easy reference */
//#include "sched.h"
//#include "schedproc.h"
#include <assert.h>
#include <minix/com.h>
#include "sema.h"
#include <machine/archtypes.h>
#include "kernel/proc.h" /* for queue constants */ // cartman
#include "dubStack.h"
#include <stdbool.h>
#include <stdio.h>
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

/* Insert Bryce Here */


/* createAndInitializeLinkedList
 *
 * This function allocates space for a linked list and sets the intial
 * value.  Then it returns the newly created and initialized struct to
 * the caller.  This struct will be passed into all of the other functions
 * to use the linked list
 */

linkedlist *createLinkedList()
{
  linkedlist *llist = (linkedlist *)malloc(sizeof(linkedlist));
  llist->head = NULL;
  return llist;
}

/* addHead
 *
 * This function takes two parameters - a linked list and a friend.
 * This creates a node for the linked list and connects the friend to the 
 * node.  Then it adds the node to the head of the linked list.
 */

void addHead(linkedlist *llist, int val)
{
  // create a node 
  node *n = (node *)malloc(sizeof(node));
  n->pid = val;
  n->next = NULL;

  // if the list is empty
  if (llist==NULL)
  {
    llist->head=n;
  }
  // if the list is not empty
  else
  {
    // make the new link's next pointer point to
    // the first link in the list
    n->next=llist->head;


    //  make the head pointer point to the new link
    llist->head=n;
  }
  //printf("current list:\n");
  //printList(llist);
}

/* addTail
 *
 * This function takes two parameters - a linked list and a friend.
 * This creates a node for the linked list and connects the friend to the 
 * node.  Then it adds the node to the tail (end) of the linked list.
 */

void addTail(linkedlist *qlist, int val)
{

  // create a node
  node *n = (node *)malloc(sizeof(node));
        n->pid=val;
  n->next = NULL;

  // if the list is empty
  if (qlist==NULL)
  {
    qlist->head=n;
  }
  // if the list has many items in it
  else
  {

    // advance to the last element in list
    node *tmp = qlist->head;
    while (tmp->next!=NULL)
    {
      tmp = tmp->next;
    }
  
    // make that last element point to the new link
    tmp->next=n;
  }
}

int popHead(linkedlist *list){
  if(list == NULL) return -1;

  node *n = list->head;
  int retVal = n->pid;
  if(n->next == NULL)
    list->head = NULL;
  else
    list->head = list->head->next;
  return retVal;
}

bool isEmpty(linkedlist *list){
  if(list->head==NULL) return true;
  else return false;
}

/*
 * printList
 *
 * This steps down through each of the nodes in a linked list and 
 * prints out the information stored in the friend to which the node points.
 * Instead of automatically printing to the screen, it prints to the 
 * file pointer passed in.  If the programmer wants to print to the screen,
 * he/she will pass in stdout.
 */

void printList(linkedlist *llist)
{

  node *n;
  int myNum=0;
  // for each node, print
  if(isEmpty(llist)) return;
  for(n = llist->head;n->next!=NULL;n=n->next)
  {
    myNum++;
  }
}

dsQueue* makeQueue(){
  dsQueue *myQueue = (dsQueue *)malloc(sizeof(dsQueue));
  myQueue->inbox = createLinkedList();
  myQueue->outbox = createLinkedList();
  return myQueue;
}

void enQueue(dsQueue *myq, int val){
  addHead(myq->inbox, val);
  myq->size++;
}

int deQueue(dsQueue *myq){
  if(isEmpty(myq->outbox)){
    while(!isEmpty(myq->inbox)){
      addHead(myq->outbox, popHead(myq->inbox));
    }
  }
  myq->size--;
  return popHead(myq->outbox);
}


/* End Bryce Here */


/***main start ***/
struct machine machine; 
int start = 1;
int next = 0;
Sem semaphores[11];
int aSize= 10;

/*===========================================================================*
 *        main               *
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
  //  panic("couldn't get machine info: %d", s);

  /* This is SCHED's main loop - get work and do it, forever and forever. */
  while (TRUE) {
    int ipc_status;
    result = OK;
    /* Wait for the next message and extract useful information from it. */
    if (sef_receive_status(ANY, &m_in, &ipc_status) != OK)
      panic("SCHED sef_receive error");
    who_e = m_in.m_source;  /* who sent the message */
    call_nr = m_in.m_type;  /* system call number */

    

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

/***main end ***/
/*===========================================================================*
 *				do_sem_init				     *
 *===========================================================================*/

int do_sem_init(message *m_ptr)
{ 
  int start_value = m_ptr->m1_i1;
  int j;
  int i;
  if(start_value < 0){
    return EPERM;
  }
  if(semaphores == NULL){ 
    return ENOMEM;
  }
  if(aSize >= next){
    int found = 0;
    j=0;
  printf("Immmm here\n ");
    while(found == 0 && j<aSize){
      if(semaphores[j]->verified == 0){
        found = 1;
      }
      else{
        j++;
      }
    }
    semaphores[j]->semval = start;
    semaphores[j]->verified = 1;
  }
  else{
    aSize = aSize + 11;
    *semaphores = (Sem*)realloc(semaphores, sizeof( Sem*) * aSize);
    int k;
    for(i=aSize-next; i<=aSize; i++){
      semaphores[i] = (Sem*) realloc(semaphores, sizeof(Sem) * aSize);
      semaphores[i]->q = makeQueue();
      semaphores[i]->verified = 0;
    }
  }
  if(j > next){
    next = j;
  }
  aSize++;
  return j;
}

/*===========================================================================*
 *				do_sem_down				     *
 *===========================================================================*/

int do_sem_down(message *m_ptr)
{
  int semnum = m_ptr->m1_i2;
  int source = m_ptr->m_source;
  if(semaphores[semnum]->verified == 0){
    return EINVAL;
  }
  else if(semaphores[semnum]->semval > 0){ 
    semaphores[semnum]->semval--;
   // debug("SEM_DOWN. seme#:%d,  old: %d, new: %d", semNumber, semaphores[semNumber]->value+1, semaphores[semNumber]->value);
    return OK;
  }
 // debug("about to add pid: %d to the queue", source);
  enQueue(semaphores[semnum]->q, source);
  return EDONTREPLY;
}

/*===========================================================================*
 *				do_sem_up				     *
 *===========================================================================*/

int do_sem_up(message *m_ptr)
{
  int semnum = m_ptr->m1_i2;
  if(semaphores[semnum]->verified == 0){
    return EINVAL;
  }

  // add resource to that specific semaphore number
  semaphores[semnum]->semval++; 
//  debug("SEM_UP, sem number: %d, new sem value: %d", semNumber, semaphores[semNumber]->value);

  if(semaphores[semnum]->q->size > 0){
    semaphores[semnum]->semval--;
    message m;
    m.m_type = OK;
    m.m_source = deQueue(semaphores[semnum]->q);
    sendnb(m.m_source, &m);
  }

  return OK;
}

/*===========================================================================*
 *				do_sem_release				     *
 *===========================================================================*/

int do_sem_release(message *m_ptr)
{
  int semnum = m_ptr->m1_i3;
  semaphores[semnum]->verified = 0;
  semaphores[semnum]->semval = 0;
  semaphores[semnum]->q->size = 0;
 // clear_queue(semaphores[semNumber]->q);

  return OK;
}
