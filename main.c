#include <stdio.h>
#include <stdlib.h>
#include "customStack.h"

#define MAX_EXPRESSION_LENGTH 50
#define MAX_EXPRESSION_LENGTH_S "50"
#define MAX_LITERAL_LENGTH 10

int getPrecedence(char c){
	if(c == '('){
		return 0;
	}
	else if(c == '+' || c == '-'){
		return 1;
	}
	else {
		return 2;
	}
}

void evaluateExpression(char* expressionString, int expressionStringLength, int maxLiteralLength){
	ExprStack* symbolStack = newExprStack(expressionStringLength, 1);
	ExprStack* outPutStack = newExprStack(expressionStringLength, maxLiteralLength);
	char* expressionIterator = expressionString;
	char* literal = malloc(sizeof(char) * MAX_LITERAL_LENGTH);
	int literalIndex = 0;
	while(*expressionIterator != '\0'){
		if(*expressionIterator == ' '){
			//skip spaces
		}
		else if(*expressionIterator == '('){
			*literal = *expressionIterator;
			pushExprStack(symbolStack, literal);
			literal = malloc(sizeof(char) * maxLiteralLength);
		}
		else if(*expressionIterator == ')'){
			pushExprStack(outPutStack, literal);
			literal = malloc(sizeof(char) );
			char* topChar = peekExprStack(symbolStack);
			while(exprStackIsEmpty(symbolStack) == false && *topChar != '('){
				pushExprStack(outPutStack, topChar);
				popExprStack(symbolStack);
				topChar = peekExprStack(symbolStack);
			}
			popExprStack(symbolStack);
		}
		else if(
			*expressionIterator == '*' ||
			*expressionIterator == '/' ||
			*expressionIterator == '+' ||
			*expressionIterator == '-'
		){
			//push anything that was in the literal to the output stack
			//must be numbers
			if(literalIndex > 0){
					pushExprStack(outPutStack, literal);
					literalIndex = 0;
					literal = malloc(sizeof(char));
			}
			*literal = *expressionIterator;
			if(exprStackIsEmpty(symbolStack) == true){
				pushExprStack(symbolStack, literal);
			}
			else{
				char* topChar = peekExprStack(symbolStack);
				if(getPrecedence(*topChar) >= getPrecedence(*literal)){
					popExprStack(symbolStack);
					pushExprStack(outPutStack, topChar);
				}
				pushExprStack(symbolStack, literal);
			}
			literal = malloc(sizeof(char) * maxLiteralLength);
		}
		else{
			literal[literalIndex++] = *expressionIterator;
		}
		expressionIterator++;
	}
	if(literalIndex > 0){
		pushExprStack(outPutStack, literal);
		literalIndex = 0;
	}
	while(exprStackIsEmpty(symbolStack) != true){
		pushExprStack(outPutStack, peekExprStack(symbolStack));
		popExprStack(symbolStack);
	}
	printf("Postfix: ");
	printExprStack(outPutStack);
}

int main(){
	char* evaluationString = (char*)malloc(sizeof(char) * MAX_EXPRESSION_LENGTH);
	printf("Enter expression: ");
	if(scanf("%" MAX_EXPRESSION_LENGTH_S "[^\n]", evaluationString) != 1){
		printf("Error while reading in expression\n");
		return 1;
	}
	evaluateExpression(evaluationString, MAX_EXPRESSION_LENGTH, MAX_LITERAL_LENGTH);
	return 0;
}
