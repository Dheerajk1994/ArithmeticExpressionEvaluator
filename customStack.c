//customStack.c
//implementation of customStack.h

#include "customStack.h"

struct  _ExprStack{
	char** stackArray;
	int stackSize;
	int exprLength;
	int iterator;
};

//create new stack
ExprStack* newExprStack(int stackSize, int maxExprLength){
	ExprStack* stack = (ExprStack*)malloc(sizeof(ExprStack));
	stack->stackArray = malloc(sizeof(char*) * stackSize);
	char** stackIterator = stack->stackArray;
	char** end = stackIterator + stackSize;
	for(end; stackIterator != end; ++stackIterator){
		*stackIterator = malloc(sizeof(char) * maxExprLength);
	}
	stack->stackSize = stackSize;
	stack->exprLength = maxExprLength;
	stack->iterator = -1;
	return stack;
}

//push into stack
bool pushExprStack(ExprStack* stack, char* val){
	if(stack->iterator + 1 < stack->stackSize){
		stack->iterator++;
		stack->stackArray[stack->iterator] = val;
		//printf("pushed %s\n", val);
		return true;
	}
	else{
		printf("Stack Overflow\n");
		return false;
	}
}

//pop stack
bool popExprStack(ExprStack* stack){
	if(stack->iterator >= 0){
		stack->iterator--;
		return true;
	}
	else{
		printf("Unable to pop. Stack is empty\n");
		return false;
	}
}

//peek stack
char* peekExprStack(ExprStack* stack){
	if(stack->iterator >= 0){
		stack->iterator;
		return stack->stackArray[stack->iterator];
	}
	else{
		printf("Stack empty\n");
		return NULL;
	}
}

//check if stack is empty
bool exprStackIsEmpty(ExprStack* stack){
	return ((stack->iterator) < 0);
}

//print stack
void printExprStack(ExprStack* stack){
	char** iterator = stack->stackArray;
	for(int i = 0; i < stack->iterator + 1; ++i){
		printf("%s ", *iterator);
		iterator++;
	}
	printf("\n");
}


//TODO
//free up allocted memeory
void freeStackMemory(ExprStack* stack){
	free(stack->stackArray);
}



