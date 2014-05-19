#ifndef DUBSTACK_H
#define DUBSTACK_H

#include <stdbool.h>

typedef struct _node {
  int pid;
  struct _node *next;
} node;

typedef struct _linkedlist {
  node *head;
  bool valid;
} linkedlist;

typedef struct _dsQueue {
  linkedlist *inbox;
  linkedlist *outbox;
  int size; /*George Edit*/
} dsQueue;

typedef struct _Sem{
  int verified;
  int semval;
  dsQueue *q;  /* George change from dsQueue q to *q*/
} *Sem;


linkedlist *createLinkedList();
void addHead(linkedlist *, int);
void addTail(linkedlist *, int);
int popHead(linkedlist *);
bool isEmpty(linkedlist *);
void printList(linkedlist *);
dsQueue *makeQueue();
void enQueue(dsQueue *, int);
int deQueue(dsQueue *);

 int start = 1;
 int next = 0;
 Sem semaphores[11];
 int aSize = 10;

#endif
