/******************************************************************************
                                                        Han Zhang
                                                        CSE 12, Winter 2020
                                                        January 26, 2020
                                                        cs12wi20kj
                                Assignment Four
File Name:      calc.c
Description:    a program that will simulate a top-of-the-line calculator that 
		will perform such advanced commands as addition, subtractions, 
		multiplication, division, exponentiation, and factorial! The 
		program will accept mathematical expressions in "infix" 
		notation and convert it to its "postfix" equivalent, and then 
		evaluate this expression. All operations are integer operations
******************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"
#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define SIGN_BIT (1L << ((sizeof (long) << 3) - 1))
#define BYTE 8
static char operators[] = "()+-*/^ !";
static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);
static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };
/*-----------------------------------------------------------------------------
Function Name:		eval
Purpose:		calculate stack1
Description:		uses two stacks, reverse stack1 to stack2 first,
                        calculate nums and save result on stack1
Input:			stack1: stack to save the result
Result:			calculate successfully
			return the result
-----------------------------------------------------------------------------*/
long eval (Stack * stack1) {
/* [remove after use]
ALGORITHM FOR POST-FIX EVALUATION:
You will need 2 stacks for this algorithm.  Your function "eval" will
take a pointer to a stack (stack1) as a parameter.  It will use this stack
as one of its needed stacks.  The other stack will be local.  After the
evaluation, the parameter stack will be empty.
Reverse stack1 onto stack2, then begin the evaluation:
While stack2 is not empty
     pop numbers from stack2, pushing all digits popped to stack1.  Once a
     non-numbers is encountered from stack2, the appropriate number of
     operands will be popped from stack1 and evaluated using the operator
     just popped from stack2.  The result of this computation will be
     pushed on stack1.
The final result that remains on stack1 is the final result of the
expression.  This result is then used as the return value for this
function.  During the evaluation process, stack2 holds positive integers
and operators, but stack1 only holds signed integers.
*/
	long op1;		/*for first operand*/
	long op2;		/*for second operand*/
	long num;		/*for the operator*/
	long result = 0;	/*ton save the result*/
	Stack * stack2;		/*to store values*/
	stack2 = new_Stack(CALCSTACKSIZE);	/*create new stack*/
	/*reverse stack 1 to stack 2*/
	while(!isempty_Stack(stack1)) {				
		pop(stack1, &num);
		push(stack2, num); 
	} 
	
	/*get operand and operator from stack 2*/
	while(!isempty_Stack(stack2)) {
		
		/*get operator*/
		pop(stack2, &num); 
		/*if non-number, push to stack1*/
		if (num >= 0){
			push(stack1, num);
		/*else, calculate*/
		} else {
			/*if num not !, pop 2 nums*/
			if (CHARACTER(num) != '!') {
				pop(stack1, &op1);
				pop(stack1, &op2);
			} 
			/*if num is !, pop only 1 num*/
			else {
				pop(stack1, &op1);
				op2 = 0;
			}
			/*call functions to calculate*/
			result = functions[INDEX(num)](op1, op2);
			
			/*push result to stack1*/	
			push(stack1, result);
		}
	}
	
	/*delete stack after use*/
	delete_Stack(&stack2);
	/*pop result on stack 1*/
	pop(stack1, &result);	
	return result;
}
/*-----------------------------------------------------------------------------
Function Name:		intopost
Purpose:		convert infix to postfix
Description:		use two stacks to distinguish input
                        and use if else statement to push and pop
Input:			stack1: stack will change be edited by this function
Result:			convert succesfully
			return positive number
-----------------------------------------------------------------------------*/
long intopost (Stack * stack1) {
/* [remove after use]
ALGORITHM FOR INFIX-TO-POSTFIX:
You will need 2 stacks for this algorithm.  Your function "intopost" will
take a pointer to a stack (stack1) as a parameter.  It will use this stack
as one of its needed stacks.  The other stack will be local.  The
parameter stack will also serve as the place to store the resultant
post-fix expression.  The return value from intopost is either EOF
when ^D is entered or a non-zero value indicating that the function
succeeded.
Process each character of the input in turn
     if character is EOF, return EOF
     if character is blank, then ignore it
     if character is a digit, then continue to read digits until you read
          a non-digit, converting this number to decimal as you go.  Store
          this decimal number on stack1
     else if character is '(' then push it to stack2
     else if character is ')' then repeatedly pop stack2, pushing all
          symbols popped from stack2 onto stack1 until the first '('
          encountered is popped from stack2.  Discard '(' and ')'
     else repeatedly push to stack1 what is popped from stack2
          until stack2 is empty or stack2's top symbol has a lower
          priority than the character entered.  Then push the character
          onto stack2.
After processing all characters, pop anything remaining on stack2, pushing
all symbols popped from stack2 to stack1.  Stack1 now contains the
post-fix expression, in reverse order.
*/
	
	int input = fgetc(stdin);		/*keep user input*/
	long popped;				/*store popped thing*/
	long topped;				/*store top value*/
	Stack * stack2;				/*to store operators*/
	long digit;				/*to store numbers*/
	stack2 = new_Stack(CALCSTACKSIZE);		/*create new stack*/
	/*process each character of the input*/
	while (input != '\n') {
		/*return EOF if EOF*/
		if (input == EOF) {
			/*delete before quit*/
			delete_Stack(&stack2);
			return EOF;
		}
		/*ignore blank*/
		if (input == ' ') {
		/*if is digit, put back to input and decin whole number*/
		} else if (isdigit(input)) {
			ungetc(input, stdin);
			digit = decin();
			push(stack1, digit);
		
		/*if (, push to stack2*/	
		} else if (input == (int)'(') {
			push(stack2, setupword(input));
		/*if ), pop until find the nearest (*/
		} else if (input == (int)')') {
			pop(stack2, &popped);
			/*pop until (*/
			while(popped != setupword('(')) {
				push(stack1, popped);
				pop(stack2, &popped);
			}
		} else {
			/*stop loop if empty stack */
			while (!isempty_Stack(stack2)){
				/*check the top value*/
				top(stack2, &topped);
				
				/*compare the prority*/
				/*if not less than input*/
				/*pop top and push to stack1*/
				if (PRIORITY(topped) >= 
						PRIORITY(setupword(input))) {
					pop(stack2, &popped);
					push(stack1, popped);
				/*else finish the loop*/
				} else {
					break;
				}
			}
			/*push the input to stack2*/
			push(stack2, setupword(input));	
		}
		
		/*make the loop run*/
		input = fgetc(stdin);
	}
	
	/*push everything from stack2 to stack1*/
	while (!isempty_Stack(stack2)) {
		pop(stack2, &popped);
		push(stack1, popped);
	}
	
	/*delete local stack*/	
	delete_Stack(&stack2);
	return 1;
}
/*-----------------------------------------------------------------------------
Function Name:		add
Purpose:		add numbers
Description:		use two parameter to add numbers
Input:			augend: one number to be added
			addend: another number to be added
Result:			add successfully
			return result
-----------------------------------------------------------------------------*/
static long add (long augend, long addend) {
	return augend + addend;
}
/*-----------------------------------------------------------------------------
Function Name:		divide
Purpose:		divide numbers
Description:		use two parameter to divide numbers
Input:			divisor: one number to be divide
			dividend: another number to be divide
Result:			divide successfully
			return result
-----------------------------------------------------------------------------*/
static long divide (long divisor, long dividend) {
	return dividend / divisor;
}
/*-----------------------------------------------------------------------------
Function Name:		exponent
Purpose:		get the exponent of a number
Description:		use recursive loop to get the result for 
                        the exponent of the number
Input:			power: number's power
			base: number's base
Result:			get exponent successfully
			return the number
-----------------------------------------------------------------------------*/
static long exponent (long power, long base) {
	/*if power=0, return 1*/
	if (power == 0) {
		return 1;
	/*else return power-1*/
	} else {
		return exponent(power-1, base)*base;
	}
}
/*-----------------------------------------------------------------------------
Function Name:		fact
Purpose:		factorial the number
Description:		use recursive loop to get the factorial of 
                        the number
Input:			xxx: number need to be facted
			ignored: ignore this number
Result:			fact successfully
			return the result
-----------------------------------------------------------------------------*/
static long fact (long xxx, long ignored) {
	/*if xxx=0, return 1*/
	if (xxx == 0) {
		return 1;
	/*if xxx=1, return 1*/
	} else if (xxx == 1) {
		return 1;
	/*else return xxx-1*/
	} else {
		return fact(xxx-1, ignored)*xxx;
	}
}
/*-----------------------------------------------------------------------------
Function Name:		mult
Purpose:		multiply numbers
Description:		use two parameter to multiply numbers
Input:			factorx: one number to be multiply
			factory: another number to be multiply
Result:			multiply successfully
			return result
-----------------------------------------------------------------------------*/
static long mult (long factorx, long factory) {
	return factorx * factory;
}
/*-----------------------------------------------------------------------------
Function Name:		setupword
Purpose:		convert word to unique numbers
Description:		convert word to negative numbers to distinguish
                        between words and nums
Input:			character: character need to be converted
Result:			convert successfully
			return the number
-----------------------------------------------------------------------------*/
static long setupword (int character) {
	long index = 0;	/*to track progress in array*/
	/*convert characters into negative numbers*/	
	while (operators[index] != NULL) {
		if (character == operators[index]) {
			break;
		}
	     index++;
	}
	return SIGN_BIT | (index << BYTE) | character;
}
/*-----------------------------------------------------------------------------
Function Name:		sub
Purpose:		substract numbers
Description:		use two parameter to substract numbers
Input:			subtrahend: one number to be subtracted
			minuend: another number to be subtracted
Result:			subtract successfully
			return result
-----------------------------------------------------------------------------*/
static long sub (long subtrahend, long minuend) {
	return minuend - subtrahend;
}