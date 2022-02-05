#ifndef STACK_H
#define STACK_H
struct stack{
	struct stack *next;
	void *data;
};
struct stack* push(struct stack *Stack,void *value);
struct stack* pop(struct stack* Stack);
struct stack* new_stack();
void print_stack(struct stack *Stack,void (*print)(void*));
#endif
