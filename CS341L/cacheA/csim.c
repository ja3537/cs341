/********************
 * Joseph Adams
 * jpadams
 *
 * cs341L
 * cache lab part A
 * due 11/11/20
 *
 * This program simulates a cache by creating an array of doubly linked lists.
 * Each linked list is used to represent a set, with each node representing a
 * line in that set.
 ********************/

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "cachelab.h"

int s;
unsigned long S; //number of sets
int E;
int b;
char* trace; //to accept from traceFile
FILE* traceFile; //user specified traceFile pointer

int hits = 0;
int misses = 0;
int evictions = 0;

char inst; //I, M, L, S
unsigned long address; //address from trace file
int size; //size given by trace file. Does nothing.
unsigned long tag; //tag of address
unsigned long setIndex; //set index of address


typedef struct node{
  unsigned long tag;
  struct node* next;
  struct node* previous;
} node;

node** set; //used to point to array of node pointers, each pointing to the head of a doubly linked list

unsigned long generateMask(int s){//helper function to return mask of 1's of length s
  unsigned long mask = 0;
  for(int i = 0; i < s; i++){
    mask <<= 1;
    mask += 1;
  }
  return mask;
}

node* findTail(node* head){//returns tail of linked list

  if(head == NULL) return head;
  node* current = head;

  while(current -> next != NULL) current = current -> next;

  return current;
}

char search(node* head){ //looks for tag in list (set)
  //If found, changes order so our tag is head
  //returns 0 if not found

  if(head == NULL){
    misses++;
    return 0;
  }

  node* current = head;
  while(current -> tag != tag){
    if(current -> next == NULL){//tail reached
      misses++;
      return 0;
    }
    current = current -> next;
  } //only exits if tag has been found

  hits++;

  if(current == head) return 1; //tag already first

  else if(current == findTail(head)){//node is tail
    node* previous = current -> previous;
    previous -> next = NULL;
    head -> previous = current;
    current -> next = head;
  }

  else{ //node is not head or tail
    node* previous = current -> previous;
    node* next = current -> next;
    previous -> next = next;
    next -> previous = previous;
    current -> previous = NULL;
    current -> next = head;
    head -> previous = current;
  }

  set[setIndex] = current; //node is new head
  return 1;
}

int listLength(node* head){//returns length of a list
  if(head == NULL) return 0;

  int count = 1;
  node* current = head;
  while(current -> next != NULL){
    count++;
    current = current -> next;
  }

  return count;
}

void createNode(node* head){//creates node and adds to front of list
  node* new = (node*) malloc(sizeof(node));
  if(new == NULL){
    printf("MALLOC FAILURE!\n");
    exit(0);
  }
  new -> tag = tag;

  if(head != NULL){
    new -> next = head;
    head -> previous = new;
  }
  set[setIndex] = new; //new node is now head
}

void evictNode(node* head){//will actually put tag in the tail and move to front of list

  node* tail = findTail(head);
  tail -> tag = tag;

  if(tail -> previous != NULL){ //tail is not head
    node* previous = tail -> previous;
    previous -> next = NULL;
    tail -> previous = NULL;
    tail -> next = head;
    head -> previous = tail;
  }
  set[setIndex] = tail;
  evictions++;
}

void updateCache(){//this is the function called by main(). It is called twice for 'M'

  node* head = set[setIndex];
  if(!search(head)){//tag not found in cache
    if(listLength(head) < E) createNode(head); //set not full
    else evictNode(head); //set full, so evict
  }
}

void freeList(node* head){ //helper function for freeNodes(). frees a doubly linked list.
  if(head == NULL) return;
  node* current = head -> next;
  while(current != NULL){
    node* next = current -> next;
    current = NULL;
    free(current);
    current = next;
  }
  free(head);
}

void freeNodes(){
  for(int i = 0; i <= S; i++){
    freeList(set[i]);
  }
  free(set);
}

int main(int argc, char** argv){
  char opt;
  char hFlag = 0; //-h option
  char sMissing = 1; //flags used in case a parameter is missing
  char EMissing = 1;
  char bMissing = 1;
  char tMissing = 1;

  while((opt = getopt(argc, argv, "h:s:E:b:t:")) != -1){
    switch(opt){
    case 'h':
      hFlag = 1;
    case 's':
      s = atoi(optarg);
      sMissing = 0;
      break;
    case 'E':
      E = atoi(optarg);
      EMissing = 0;
      break;
    case 'b':
      b = atoi(optarg);
      bMissing = 0;
      break;
    case 't':
      trace = optarg;
      tMissing = 0;
      break;
    }
  }

  if(hFlag){ //printed if -h given
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    return 0;
  }
  
  if(sMissing || EMissing || bMissing || tMissing){ //check for all the flags
    printf("A parameter is missing!\n");
    return 0;
  }

  S = pow(2, s); //number of sets
  set = malloc(S*sizeof(node*)); //allocate array where each entry is a pointer to linked list.
 
  traceFile = fopen(trace, "r"); //open trace file
  if(traceFile == NULL){
    printf("No trace file found!\n");
    return 0;
  }

  while(fscanf(traceFile, " %c %lx, %d", &inst, &address, &size) > 0){//parse trace file
    if(inst == 'I') continue;

    tag = address >> (s + b); //get tag for address
    setIndex = (address >> b) & generateMask(s); //get set index for address

    if(inst == 'M') updateCache(); //update cache twice for 'M'

    updateCache();
  }
         
    printSummary(hits, misses, evictions);
    freeNodes();
    fclose(traceFile);
    return 0;
}
