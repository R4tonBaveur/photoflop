#ifndef QUEUE_H
#define QUEUE_H
struct queue{
	struct queue *next;
	void *data;
};
struct queue* enqueue(struct queue *Queue,void *value);
struct queue* dequeue(struct queue* Queue);
struct queue* new_queue();
void print_queue(struct queue *Queue,void (*print)(void*));
#endif
