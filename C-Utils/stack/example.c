#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
void print_int(void *val){
	int * p = val;
	printf("%d",*p);
}
int main(){
	struct stack* S = new_stack();
	print_stack(S,print_int);
        int* v;
        for(int i=0;i<10;i++){
            v = malloc(sizeof(int));
            *v = i;
            push(S,v);
        }
        print_stack(S,print_int);
        print_stack(pop(S),print_int);
        return 0;
};
