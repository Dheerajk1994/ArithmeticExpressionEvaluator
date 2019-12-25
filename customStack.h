//customStack.h
//header file for custom stack
//custom array based stack

#ifndef CUSTOM_STACK_H
#define CUSTOM_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _ExprStack ExprStack;
ExprStack* newExprStack(int size, int maxExprLength);
bool pushExprStack(ExprStack* stack, char* val);
char* peekExprStack(ExprStack* stack);
bool popExprStack(ExprStack* stack);
bool exprStackIsEmpty(ExprStack* stack);
void printExprStack(ExprStack* stack);
void freeStackMemory(ExprStack* stack);

#endif
