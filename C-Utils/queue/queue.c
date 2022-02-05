#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
struct queue* enqueue(struct queue *Queue,void *value){
	/*
	 * enqueues value to the Queue and returns it
	 * Param: Queue->Queue we want to add new element to
	 * Param: value->value to add
	 * */
	struct queue* tmp = Queue;
	while(Queue->next!=NULL){
		Queue = Queue->next;
	};
	Queue->next = malloc(sizeof(struct queue));
	Queue->next->data = value;
	Queue = tmp;
	return Queue;
};
struct queue* dequeue(struct queue* Queue){
	/*
	 * dequeues the first value of the Queue and returns the dequeued element
	 * Param: Queue->Queue we want to dequeue fro;
	 * */
	struct queue* res = new_queue();
	res->next = Queue->next;
	if(Queue->next != NULL && Queue->next->next != NULL){
		Queue->next = Queue->next->next;
	}
	return res;

}
struct queue* new_queue(){
	/*
	 * Returns a pointer to a new empty queue
	 * */
	struct queue *Queue = malloc(sizeof(struct queue));
	Queue->data = NULL;
	return Queue;
};
void print_queue(struct queue *Queue,void (*print)(void*)){
	/*
	 * Prints the queue
	 * Param: Queue-> Queue to print
	 * Param: print-> function to print an element of the type contained by the queue
	 * */
	printf("Queue: [");
	struct queue* tmp = Queue;
	Queue = Queue->next;
	while(Queue!=NULL && Queue->next!=NULL){
		print(Queue->data);
		printf(", ");
		Queue = Queue->next;
	}
	if(Queue!=NULL){
		print(Queue->data);
	}
	printf("]\n");
	Queue = tmp;
}
/*void print_int(int val){
	printf("%d",val);
}

int main(){
	struct queue* Q = new_queue();
	print_queue(Q,print_int);
	for(int i=0;i<10;i++){
		enqueue(Q,i);
	}
	print_queue(Q,print_int);
	return 0;
}*/
