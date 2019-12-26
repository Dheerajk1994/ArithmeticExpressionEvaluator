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

bool isOperand(char* expressionIterator){
  return(
	 *expressionIterator == '*' ||
	 *expressionIterator == '/' ||
	 *expressionIterator == '+' ||
	 *expressionIterator == '-'
	 );
}

int binaryOperateToInt(char* operator, int operand1, int operand2){
  //TODO: prone to error
  printf("operand2: %d operand1: %d\n", operand2, operand1);
  char* end;
  switch(*operator){
  case '+':
    return operand1 + operand2;
    break;
  case '-':
    return operand1 - operand2;
    break;
  case'*':
    return operand1 * operand2;
    break;
  case'/':
    return operand1 / operand2;
    break;
  }
}
void solveExpression(ExprStack* outPutStack,int expressionStringLength, int maxLiteralLength){
  ExprStack* postFixStack = newExprStack(expressionStringLength, maxLiteralLength);
  ExprStack* solutionStack = newExprStack(expressionStringLength, maxLiteralLength);

  char* top;
  while(exprStackIsEmpty(outPutStack) == false){
    top = peekExprStack(outPutStack);
    pushExprStack(postFixStack, top);
    popExprStack(outPutStack);
  }
  char* intString = NULL;
  while(exprStackIsEmpty(postFixStack) == false){
    top = peekExprStack(postFixStack);
    popExprStack(postFixStack);
    if(isOperand(top) == false){
      pushExprStack(solutionStack, top);
    }
    else{
      printExprStack(solutionStack);
      char* end;
      int operand2 = strtol(peekExprStack(solutionStack), &end, 10);
      popExprStack(solutionStack);
      int operand1 = strtol(peekExprStack(solutionStack), &end, 10);
      popExprStack(solutionStack);
      int result = binaryOperateToInt(top, operand1, operand2);
      //printf("result %d\n", result);
      intString = malloc(sizeof(char) * MAX_LITERAL_LENGTH);
      sprintf(intString, "%d", result);
      printf("result to char string %s\n", intString);
      pushExprStack(solutionStack, intString);
      //printf("top of solutions stack after pushing: %s\n", peekExprStack(solutionStack));
    }
  }
  printf("Solution: %s\n", peekExprStack(solutionStack));
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
    else if(isOperand(expressionIterator) == true){
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
  solveExpression(outPutStack, expressionStringLength, maxLiteralLength);
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
