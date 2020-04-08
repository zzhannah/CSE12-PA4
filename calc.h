/*=============================================================================
 calc.h
 @author Gary Gillespie
 @version 01/17/2020
=============================================================================*/
#ifndef CALC_H
#define CALC_H
#include "stack.h"
#define CALCSTACKSIZE 100
long eval (Stack *);
long intopost (Stack *);
#endif