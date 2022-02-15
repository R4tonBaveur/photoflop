#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
void print_int(void *val){
	int * p = val;
	printf("%d",*p);
}

int main(){
	struct queue* Q = new_queue();
	print_queue(Q,print_int);
	int *v;
	for(int i=0;i<10;i++){
		v = malloc(sizeof(int));
		*v = i;
		enqueue(Q,v);
	}
	print_queue(Q,print_int);
	struct queue* res = dequeue(Q);
	print_queue(Q,print_int);
	print_queue(res,print_int);
	return 0;
}
