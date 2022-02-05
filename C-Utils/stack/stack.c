#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

struct stack* push(struct stack *Stack,void *value){
    /*
     * pushes a new element on top of the stack and returns it
     */
    struct stack* new = new_stack();
    new->next = malloc(sizeof(struct stack));
    new->next = Stack->next;
    Stack->next = new;
    new->data = value;
    return Stack;
};
struct stack* pop(struct stack* Stack){
    struct stack* res = Stack->next;
    if(Stack->next!=NULL && Stack->next->next != NULL){
    	Stack->next = Stack->next->next;
    }
    return res;
};
struct stack* new_stack(){
    /*
     * Returns a pointer to a new empty stack
     */
    struct stack *Stack = malloc(sizeof(struct stack));
    Stack->data = NULL;
    return Stack;
};
void print_stack(struct stack *Stack,void (*print)(void*)){
    /*
     * Prints the stack
     * Param: Stack-> Stack to print
     * Param: print-> function to print an element of the type contained by the queue
     * */
    printf("Stack: [\n");
    struct stack* tmp = Stack;
    Stack = Stack->next;
    while(Stack!=NULL && Stack->next!=NULL){
        printf("  ");
        print(Stack->data);
        printf(", \n");
        Stack = Stack->next;
    }
    if(Stack!=NULL){
        print(Stack->data);
        printf("\n");
    }
    printf("]\n");
    Stack = tmp;
};
