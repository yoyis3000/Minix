#ifndef DUBSTACK_H
#define DUBSTACK_H

#include <stdbool.h>

#include "sema.h"
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


#endif
