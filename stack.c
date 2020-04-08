/******************************************************************************
                                                        Han Zhang
                                                        CSE 12, Winter 2020
                                                        January 17, 2020
                                                        cs12wi20kj
                                Assignment Three
File Name:      stack.c
Description:    This array implementation of stack is an array of longs (words)
		, the stack count is the first element in the array,the stack 
		size is the second element in the array, and the stack pointer 
		is the third element in the array.  The stack pointer has the 
		value of an index into the array to denote the next available 
		space in the stack.							
******************************************************************************/
#include <stdio.h>
#include "mylib.h"
#include "stack.h"
#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */
/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";
/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";
/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */
/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}
void debug_on (void) {
        debug = TRUE;
}
/*-----------------------------------------------------------------------------
Function Name:		delete_Stack
Purpose:		delete the stack passed in
Description:		deallocates memory allocated in new_Stack. 
                        Assigns incoming pointer to NULL.
Input:			** spp: pointer to pointer to current stack
Result:			return truth value
-----------------------------------------------------------------------------*/
long delete_Stack (Stack ** spp) {
	/*validity check*/
	if (spp == NULL || *spp ==NULL) {
                fprintf (stderr, DELETE_NONEXIST);
                return 0;
	/*else deallocate stack and set to null*/
        } else {
		free(* spp - STACK_OFFSET);
		* spp = NULL;
		/*print debug msg if debug on*/
		if (debug == TRUE) {
			fprintf(stderr, DEALLOCATE, (long int)stack_counter);
		}
		
		--stack_counter;
		return 1;
	}
}
/*-----------------------------------------------------------------------------
Function Name:		empty_Stack
Purpose:		empty current stack
Description:		empty the stack
Input:			this_Stack: the stack need to be empty
Result:			empty the stack
			no return value
-----------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {	
	/*validity check*/
	if (this_Stack == NULL) {
                fprintf (stderr, EMPTY_NONEXIST);
	/*else overwrite next avaliable space*/
        } else {
		this_Stack[STACK_POINTER_INDEX] = 0;	
	}
}
/*-----------------------------------------------------------------------------
Function Name:		isempty_Stack
Purpose:		check if empty
Description:		check if the stack is empty
Input:			this_Stack: the Stack need to be checked
Result:			check the stack
			return truth value
-----------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {
	/*validity check*/
	if (this_Stack == NULL) {
                fprintf (stderr, ISEMPTY_NONEXIST);
                return 0;
	
	/*return true if empty*/
	} else {
		/*if size = 0, empty*/
		if (this_Stack[STACK_POINTER_INDEX] == 0) {
			return 1;
		} else {
			return 0;
		}
	}
}
/*-----------------------------------------------------------------------------
Function Name:		isfull_Stack
Purpose:		check if full
Description:		check if the stack is full
Input:			this_Stack: the Stack need to be checked
Result:			check the stack
			return truth value
-----------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
	long spi;	/*keep track stack pointer index*/
	long ssi;	/*keep track stack size index*/
	/*validity check*/
	if (this_Stack == NULL) {
                fprintf (stderr, ISFULL_NONEXIST);
                return 0;
	
	/*return true if full*/
	} else {
		
		spi = this_Stack[STACK_POINTER_INDEX];
		ssi = this_Stack[STACK_SIZE_INDEX];
		/*if SPI = SSI, full*/
		if (spi == ssi) {
			return 1;
		} else {
			return 0;
		}
	}
}
/*-----------------------------------------------------------------------------
Function Name:		new_Stack
Purpose:		create new stack
Description:		allocates stack array, and initializes 
                        stack pointer.  Result is a pointer in the 
                        array where user data allotment begins
Input:			stacksize: the size the stack need to be
Result:			create new stack
			return new stack
-----------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {
	/*allocate memory for stack*/
	void * memory = malloc((stacksize+STACK_OFFSET) * sizeof(long));
	Stack * this_Stack = (Stack *)memory + STACK_OFFSET;
	this_Stack[STACK_POINTER_INDEX] = 0;
	this_Stack[STACK_SIZE_INDEX] = stacksize;
	this_Stack[STACK_COUNT_INDEX] = ++stack_counter;
	if (debug == TRUE) {
		fprintf(stderr,ALLOCATED, (long int)stack_counter);
	}
	return this_Stack;
}
/*-----------------------------------------------------------------------------
Function Name:		get_occupancy
Purpose:		get how many stack is using
Description:		returns the number of elements stored on the stack.
Input:			this_Stack: the Stack need to be checked
Result:			get the number
			return the number
-----------------------------------------------------------------------------*/
long get_occupancy (Stack * this_Stack) {
	
	/*validity check*/
	if (this_Stack == NULL) {
                fprintf (stderr, NUM_NONEXIST);
                return 0;
	/*get number of elements*/
	} else {
		return this_Stack[STACK_POINTER_INDEX];
	}
	
}
/*-----------------------------------------------------------------------------
Function Name:		pop
Purpose:		send back and remove the last number
Description:		removes and sends back the top element of the stack.  
			Result is 0 or non-0, indicating failure or success, 
                        respectively .
Input:			this_Stack: the Stack need to be checked
			item: place to store poped value
Result:			pop the number
			return truth value
-----------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {
	
	long pop;	/*to hold the index of pop*/
	/*validity check*/
	if (this_Stack == NULL) {
                fprintf (stderr, POP_NONEXIST);
                return 0;
	/*if is empty*/
	} else if (isempty_Stack(this_Stack) == 1){
		fprintf (stderr, POP_EMPTY);
		return 0;
	/*return the num*/
	} else {
		pop = --this_Stack[STACK_POINTER_INDEX];
		*item = this_Stack[pop];
		/*if debug on, print debug msg*/
		if (debug == TRUE) {
			if (*item >= 0) {
				fprintf(stderr, POP, 
				(long)this_Stack[STACK_COUNT_INDEX], *item);
			}
			else {fprintf(stderr, HEXPOP, 
				(long)this_Stack[STACK_COUNT_INDEX], *item);
			}
		}
		return 1;
	}
}
/*-----------------------------------------------------------------------------
Function Name:		push
Purpose:		add new number to the stack
Description:		laces one value on the specified stack. Result is 0 
			or non-0 indicating failure or success, respectively
Input:			this_Stack: the Stack need to be checked
			item: place to store pushed value
Result:			push the number
			return truth value
-----------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {
	long sp;	/*to hold working value of the stack pointer*/
	/*validity check*/
	if (this_Stack == NULL) {
                fprintf (stderr, PUSH_NONEXIST);
                return 0;
	/*check if stack is full*/
	} else if (isfull_Stack(this_Stack) == 1){
		fprintf (stderr, ISFULL_NONEXIST);
		return 0;
	/*place item onto the stack*/
	} else {
		sp = this_Stack[STACK_POINTER_INDEX];
		this_Stack[sp] = item;
		this_Stack[STACK_POINTER_INDEX]++;
		/*debug msg*/
		if (debug == TRUE) {
			if (item >= 0) {
				fprintf(stderr, PUSH, 
				(long)this_Stack[STACK_COUNT_INDEX], item);
			}
			else {fprintf(stderr, HEXPUSH, 
				(long)this_Stack[STACK_COUNT_INDEX], item);
			}
		}
		return 1;
	}
}
/*-----------------------------------------------------------------------------
Function Name:		top
Purpose:		send back the last number
Description:		sends back the top element of the stack. Stack 
			is left unaffected. Result is 0 or non-0 indicating 
			failure or success, respectively.
Input:			this_Stack: the Stack need to be checked
			item: place to store top value
Result:			send back the number
			return truth value
-----------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {
	long top;	/*to keep track top index*/
	/*validity check*/
	if (this_Stack == NULL) {
                fprintf (stderr, TOP_NONEXIST);
                return 0;
	/*check if stack is empty*/
	} else if (isempty_Stack(this_Stack) == 1){
		fprintf(stderr, TOP_EMPTY);
		return 0;
	/*return the num*/
	} else {
		top = --this_Stack[STACK_POINTER_INDEX];
		*item = this_Stack[top];
		this_Stack[STACK_POINTER_INDEX]++;
		if (debug == TRUE) {
			if (*item >= 0) {
				fprintf(stderr, TOP, 
				(long)this_Stack[STACK_COUNT_INDEX], *item);
			}
			else {fprintf(stderr, HEXTOP, 
				(long)this_Stack[STACK_COUNT_INDEX], *item);
			}
		}
		return 1;
	}
}
/*-----------------------------------------------------------------------------
Function Name:		write_Stack
Purpose:		write stack to file
Description:		prints out the contents of the stack to the parameter 
			specified FILE
Input:			this_Stack: the stack need to write
			stream: place to write this_Stack
Result:			write to the file
			return stream
-----------------------------------------------------------------------------*/
FILE * write_Stack (Stack * this_Stack, FILE * stream) {
        long index = 0;         /* index into the stack */
        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }
        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));
        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {
                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }
        return stream;
}